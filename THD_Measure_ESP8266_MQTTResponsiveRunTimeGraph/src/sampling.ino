#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <math.h>
#include "arduinoFFT.h"
#include <PubSubClient.h>

arduinoFFT FFT = arduinoFFT();
int N = 1024;

int PORT = 8080;

const char* ssid = "ESP";
const char* password = "12345678";
const char* mqtt_server = "10.0.0.103";
long lastMsg = 0;
char buff[100];

WiFiClient espClient;
PubSubClient client(espClient);

//numero atribuir(float real, float imag);
int SAMPLING_FREQUENCY = 1024; //Hz, must be less than 10000 due to ADC
unsigned int sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
unsigned long microseconds;
double vReal[1024] = {};
double vImag[1024] = {};
//double timeBegin, timeEnd; //store the time that begun and finished the data sampling.
double sensorValueAcc = 0; //Sensor's value accumulated
float I;

//Valores para o sensor de 20A
float sensibility = 0.185; //mv/A in/out ratio of the sensor //Para 5A
float voltsPerBit = 0.00329; //Minimal fluctuation of voltage to add a unity in the ADC //3,3/1024
float harmonics[10] = {};
int i = 0;
int nharmonics = 10;
float offset;
float THD;
int k = 0;
//SETUP ======================
float ih, ifd;

void setup() {
    Serial.begin(9600);
    Serial.println("INCIANDO O ESP8266");

    setup_wifi();
    client.setServer(mqtt_server, PORT);
    client.setCallback(callback);
    
    //ESP.wdtDisable();
    /*
    http.begin("10.0.0.101", 8080, "/");
    int httpCode = http.GET();
    Serial.println(httpCode);
    http.writeToStream(&Serial);
    http.end();
    Serial.println("DEBUG");
    delay(10000);
    */
    
}

//LOOP ======================
void loop() {
    delay(100); //Inicial delay
    offset = getOffset();
    while (i < N){
        microseconds = micros(); 
        vReal[i] = map(analogRead(A0), 1, offset, 1, 512); //map in 3v3 adc scale
        vReal[i] -= 512; //offset
        //Serial.println(vReal[i]);
        sensorValueAcc += (vReal[i] * vReal[i]); //sum square
        i++; //counting number of samples,
        //keep a constant sampling frequency
        delayMicroseconds(10);
        while(micros() < (microseconds + sampling_period_us)){} 
    }
    i = 0;
    //FFT procediment
    FFT.Windowing(vReal, N, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, N, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, N);
    
    //Get the major peak
    double peak = FFT.MajorPeak(vReal, N, SAMPLING_FREQUENCY);
    
    //filter the peak
    if(peak > 120 || peak < 50)
        peak = 0.0; 
    
    //sums peak and harmonic contributions in a interval of +/- 5
    for(int n = 0; n < 10; n++){
        for(int j = -5; j < 6; j++){
            if(peak + j < N/2 && peak + j >= 0){ //avoid vector's boundaries
                if(peak){
                    harmonics[n] += (vReal[((n+1)*(int)peak)+j]);                                                                      
                }else{
                    harmonics[n] += vReal[0]; //if peak == 0
                    break;
                }
            } else {
                harmonics[n] = 0.0;
            }
        }
        if(!peak) break; //i
    }
    //Gets the Current consumed in the aquire data time interval
    I = ((sqrt(sensorValueAcc/N)*voltsPerBit)/sensibility);

    //Gets the THD index
    THD = getTHD(harmonics, I);

    //filter these values
    if(harmonics[0] < 500 | !peak){
        peak = 0.0;
        THD = 0.0;
        I = 0.0;
    }

    //To Debug
    Serial.print("PEAK: ");
    Serial.println(peak, 6);
    Serial.print("I: ");
    Serial.println(I, 6);
    Serial.print("THD: ");
    Serial.println(THD,6);

    //Restarting var values
    sensorValueAcc = 0;
    for(int i = 0; i < N; i++){
        vReal[i] = 0.0;
        vImag[i] = 0.0;
        if(i < 10)
            harmonics[i] = 0.0;
    }
    
    //Loop Index
    Serial.print("=============K = ");
    Serial.print(k);
    Serial.println("========================");

    if (!client.connected()) {
        reconnect();
    }

    //I*THD = Corrente total distorcida;
    //I*(1-THD) = Corrente da frequencia fundamental
    ih = I*THD/100;
    ifd = I*(1-ih);
    String aspas = "\"";
    String json = "{ " + aspas + "I" + aspas + ":" + I + ",";
        json += aspas + "ih" + aspas + ":" + ih + ",";
        json += aspas + "ifd" + aspas + ":" + ifd + ",";
        json += aspas + "thd" + aspas + ":" + THD+ ",";
        json += aspas + "freq" + aspas + ":" + peak;
        json += "}";
    json.toCharArray(buff, json.length()+1);
    Serial.print("Publish message: ");
    Serial.println(buff);
    client.publish("/temperature", buff);
    delay(500);
    k++;
    
}