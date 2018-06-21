#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <math.h>
#include "arduinoFFT.h"
#include <PubSubClient.h>

arduinoFFT FFT = arduinoFFT();
const int N = 1024;

int PORT = 8088;

const char* ssid = "LII";
const char* password = "wifiLI2Rn";
const char* mqtt_server = "192.168.0.115";
long lastMsg = 0;
char buff[10000];
double timeEnd, timeBegin;
WiFiClient espClient;
PubSubClient client(espClient);

//numero atribuir(float real, float imag);
int SAMPLING_FREQUENCY = N; //Hz, must be less than 10000 due to ADC
unsigned int sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
unsigned long microseconds;
//double vReal[N] = {};
int vReal;
float teste = 9.2223;
int i = 0;
int k = 0;
//SETUP ======================


void setup() {
    Serial.begin(9600);
    Serial.println("INCIANDO O ESP8266");

    setup_wifi();
    client.setServer(mqtt_server, PORT);
    client.setCallback(callback);
    
}

String json;
//LOOP ======================
void loop() {
    timeBegin = millis();
    buff[0] = '[';
    for(int i = 1, j = 0; j < N; i+=5){
        microseconds = micros(); 
        //vReal = map(analogRead(A0), 1, 774, 1, 512) - 512; //map in 3v3 adc scale
        sprintf(buff + i,"%.4i ", analogRead(A0));
        j++;
        delayMicroseconds(10);
        while(micros() < (microseconds + sampling_period_us)){}
    }

    if (!client.connected()) {
        reconnect();
    }

    //I*THD = Corrente total distorcida;
    //I*(1-THD) = Corrente da frequencia fundamental
    client.loop();
    //json.toCharArray(buff, json.length()+1);
    //Serial.print("Publish message: ");
    client.publish("/temperature", buff);
    Serial.println(millis() - timeBegin);
    delay(1000);
    k++;
    
}