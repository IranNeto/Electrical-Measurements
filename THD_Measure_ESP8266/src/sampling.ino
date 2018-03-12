#include <Arduino.h>
#include <math.h>
#include "arduinoFFT.h"
#include <PubSubClient.h>

arduinoFFT FFT = arduinoFFT();
int N = 1024;

//WiFiMulti wifiMulti;
//HTTPClient http;

int PORT = 1883;

const char* ssid = "Robotica-IMD";
const char* password = "roboticawifi";
const char* mqtt_server = "10.7.220.211";
long lastMsg = 0;
char buff[8];

//WiFiClient espClient;
//PubSubClient client(espClient);



//numero atribuir(float real, float imag);
int SAMPLING_FREQUENCY = 1024; //Hz, must be less than 10000 due to ADC
const int SAMPLES = N; //Number of samples
unsigned int sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
unsigned long microseconds;
double vReal[1024] = {};
double vImag[1024] = {};
double timeBegin, timeEnd; //store the time that begun and finished the data sampling.
double sensorValueAcc = 0; //Sensor's value accumulated
float Ifundamental;

//Valores para o sensor de 20A
float sensibility = 0.185; //mv/A in/out ratio of the sensor
float voltsPerBit = 0.00329;; //Minimal fluctuation of voltage to add a unity in the ADC
float harmonics[10] = {};
int i = 0;
int nharmonics = 10;
float offset;
float THD;
int k = 0;
//SETUP ======================


void setup() {
    Serial.begin(9600);
    Serial.println("INCIANDO O ESP32");
    
    /*
    setup_wifi();
    client.subscribe("esp01");
    client.setServer(mqtt_server, PORT);
    client.setCallback(callback);
    */

    /*
    wifiMulti.addAP("ESP12614711", "");
    http.setReuse(true);
    
    while (wifiMulti.run() != WL_CONNECTED) {
        delay(50);
        Serial.println("Waiting for connection");
    }
    */

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
    timeBegin = micros();
    while (i < SAMPLES){
        microseconds = micros(); 
        vReal[i] = map(analogRead(A0), 1, offset, 1, 512); //map in 3v3 adc scale
        vReal[i] -= 512; //offset
        //Serial.println(vReal[i]);
        sensorValueAcc += (vReal[i] * vReal[i]); //sum square
        vImag[i] = 0.0; //Imaginary part to proceed the fft algorithm
        i++; //counting number of samples,
        
        //keep a constant sampling frequency
        while(micros() < (microseconds + sampling_period_us)){} 
    }
    timeEnd = micros();
    i = 0;
    //FFT procediment
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    
    //Get the major peak
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    
    //filter the peak
    if(peak > 120 || peak < 50)
        peak = 0.0; 
    
    //sums peak and harmonic contributions in a interval of +/- 5
    for(int n = 0; n < 10; n++){
        for(int j = -5; j < 6; j++){
            if(peak + j < SAMPLES/2 && peak + j >= 0){ //avoid vector's boundaries
                if(peak){
                    harmonics[n] += (vReal[((n+1)*(int)peak)+j]);                                                                      
                }else{
                    harmonics[n] += vReal[0]; //if peak == 0
                    break;
                }
            }
        }
        if(!peak) break; //i
    }
    //Gets the Current consumed in the aquire data time interval
    Ifundamental = ((sqrt(sensorValueAcc/SAMPLES)*voltsPerBit)/sensibility);

    //Gets the THD index
    THD = getTHD(harmonics, Ifundamental);

    //filter these values
    if(harmonics[0] < 200){
        peak = 0.0;
        THD = 0.0;
        Ifundamental = 0.0;
    }

    //To Debug
    Serial.print("PEAK: ");
    Serial.println(peak, 6);
    Serial.print("I: ");
    Serial.println(Ifundamental, 6);
    Serial.print("THD: ");
    Serial.println(THD,6);

    //Restarting var values
    sensorValueAcc = 0;
    for(int i = 0; i < SAMPLES; i++){
        vReal[i] = 0.0;
        vImag[i] = 0.0;
        if(i < 10)
            harmonics[i] = 0.0;
    }
    
    //Loop Index
    Serial.print("=============K = ");
    Serial.print(k);
    Serial.println("========================");
    delay(2000);
    k++;
    /*
    if (!client.connected()) {
        reconnect();
    }
    */
    
}