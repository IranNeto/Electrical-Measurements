#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <math.h>
#include "arduinoFFT.h"
#include <PubSubClient.h>

/* Declarações iniciais */

//LEMBRAR DE MODIFICAR LIMITE DE TAMANHO NA BIBLIOTECA DO MQTT
arduinoFFT FFT = arduinoFFT();
const int N = 1024;
int PORT = 8088;
const char* ssid = "LII";
const char* password = "wifiLI2Rn";
const char* mqtt_server = "192.168.0.102";
long lastMsg = 0;
String json;
double timeEnd, timeBegin;
char buff[10000];
WiFiClient espClient;
PubSubClient client(espClient);

int SAMPLING_FREQUENCY = 1024; //Hz, must be less than 10000 due to ADC
unsigned int sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
unsigned long microseconds;

int data;
int i = 0;
int k = 0;
int j = 0;
//SETUP ======================


void setup() {
    Serial.begin(9600);
    Serial.println("INCIANDO O ESP8266");

    setup_wifi();
    client.setServer(mqtt_server, PORT);
    client.setCallback(callback);
    reconnect();

}

//LOOP ======================

void loop() {
    //Inicío da amostragem
    timeBegin = micros();
    json += "{ dados:[";
    j = 0;

    while(j < N){
        microseconds = micros(); 
        data = analogRead(A0);
        json += data;
        
        if(j == N-1){
            json += "]}";
        } else{
            json += ", ";
        }
        //delayMicroseconds(10); //opcional
        while(micros() < (microseconds + sampling_period_us)){}
        j++;
    }

    j = 0;
    Serial.print("Tempo de aquisição: ");
    Serial.println(micros() - timeBegin);

    if (!client.connected()) {
        reconnect();
    }
    
    //I*THD = Corrente total distorcida;
    //I*(1-THD) = Corrente da frequencia fundamental
    client.loop();
    json.toCharArray(buff, json.length()+1);
    Serial.println(sizeof(json));
    //Serial.print("Publish message: ");
    client.publish("/temperature", buff);
    Serial.print("Tempo de envio: ");
    Serial.println(micros() - timeBegin);
    delay(1000);
    //k++;
    json = "";
    // Serial.println(sizeof(buff));
    // delay(10000000);
}
