#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <math.h>
#include "arduinoFFT.h"
#include <PubSubClient.h>

arduinoFFT FFT = arduinoFFT();
int N = 1024;

WiFiMulti wifiMulti;
HTTPClient http;

int PORT = 1883;

const char* ssid = "Robotica-IMD";
const char* password = "roboticawifi";
const char* mqtt_server = "10.7.220.211";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char buff[8];

//numero atribuir(float real, float imag);
int SAMPLING_FREQUENCY = 1024; //Hz, must be less than 10000 due to ADC
const int SAMPLES = N;
unsigned int sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
unsigned long microseconds;
double vReal[1024];
double vImag[1024] = {};
double timeBegin, timeEnd; //store the time that begun and finished the data sampling.
double sensorValueAcc = 0; //Sensor's value accumulated
float Ifundamental;

//Valores para o sensor de 20A
float sensibility = 0.1; //mv/A in/out ratio of the sensor
float voltsPerBit = 0.0008056641; //Minimal fluctuation of voltage to add a unity in the ADC
float harmonics[10] = {};
int i = 0;
int nharmonics = 10;
float offset;

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
    delay(100);
    offset = getOffset();
    timeBegin = micros();
    while (i < N){
        microseconds = micros();
        vReal[i] = map(analogRead(36), 1, offset, 1, 2048);
        vReal[i] -= 2048;
        sensorValueAcc += (vReal[i] * vReal[i]);
        vImag[i] = 0;
        i++; //counting number of samples,
        
        while(micros() < (microseconds + sampling_period_us)){}
    }

    timeEnd = micros();
    i = 0;
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    if(peak > 120) 
        peak = 0.0;
    Serial.print("PEAK: ");
    Serial.println(peak, 6); 
    
    for(int n = 0; n < 10; n++){
        for(int j = -5; j < 6; j++){
            if(peak + j < N/2 && peak + j >= 0){ //evita procuras fora do limite do vetor
                if(peak){
                    harmonics[n] += (vReal[((n+1)*(int)peak)+j]);                                                                      
                }else{
                    harmonics[n] += vReal[0];
                    break;
                }
            }
        }
        Serial.print("n: ");
        Serial.println(harmonics[n]);
        if(!peak) break;
    }
    Serial.print("ACC: ");
    Serial.println(sensorValueAcc, 6);
    Ifundamental = ((sqrt(sensorValueAcc/N)*voltsPerBit)/sensibility);
    
    Serial.println(Ifundamental, 6);
    Serial.print("THD: ");
    Serial.println(getTHD(harmonics, Ifundamental),6);
    sensorValueAcc = 0;
    Serial.println("==========================|========================");
    delay(2000);
    /*
    if (!client.connected()) {
        reconnect();
    }
    */
    
}
    //harmonics = getSpectrum(timeEnd - timeBegin, input);//getSpectrum(timeEnd - timeBegin, input);
    //postIt(getTHD(harmonics, Irms));
    //delay(8000);
/*


    for(int i = 0; i < N; i++){
        Serial.println(data[i]);
        input[i] = atribuir(data[i], 0);
    }
     sensorValueI = analogRead(36);
        sensorValueI = map(sensorValueI, 1, offset, 1, 2048); //manual conversion (see README.md)
        sensorValueI -= 2048; //offset (see README)
        sensorValueAcc += sensorValueI * sensorValueI; //sum of the square data
    //o calculo é a Irms
    Irms = ((sqrt(sensorValueAcc/N)*voltsPerBit)/sensibility);
    
    sensorValueAcc = 0;
    i = 0;
    
    //recebe a contribuição de cada harmonico através do espectro
    //soma das magnitudes
    
    Serial.print("Irms: ");
    Serial.println(Irms);
    
    delay(10000);
}
    */