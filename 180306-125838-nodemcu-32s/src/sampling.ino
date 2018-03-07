#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include "numero.h"
WiFiMulti wifiMulti;
HTTPClient http;
/*
//DEFINES AND FOWARDS DECLARATIONS ======================
#define timeToPost 2
#define power 220 //220 V
void flagPost();
Ticker sending;
double start = millis();

//OBJECTS ======================

//StaticJsonBuffer<100> jsonBuffer; //parameters library ArduinoJson
//SocketIOClient socket; //Instance library SocketIOClient
//DS1307 rtc(4, 5); //Instance library rtc
//Time temp; //Instance library Time
//JsonObject& root = jsonBuffer.createObject();
char dateBuffer[40]; //by default of some library
String ipStr; //by default of some library

//VARIABLES ======================

char host[] = "api.saiot.ect.ufrn.br"; //send to host
int port = 80; //send to port
extern String RID; //Constants of SocketIOClient
extern String Rname;
extern String Rcontent;
int pinSensor = A0; //Sensor's pin at ESP8266
int sensorValueI = 0; //Sersor's value data i of n
double sensorValueAcc = 0; //Sensor's value accumulated
float sensibility = 0.185; //mv/A in/out ratio of the sensor
float voltsPerBit = 0.00329; //Minimal fluctuation of voltage to add a unity in the ADC
int nData; //Number of data sampled
double timeBegin, timeEnd; //store the time that begun and finished the data sampling.
int year, month, day, hour, minute, second; //store the time request (see time.ino)
bool stopGettingData = false; //flag that ends the sampling (ver post.ino)
double loadPower; //power consumed by the load in the sampling time
float valueCurrent;
*/
//SETUP ======================


void setup() {
    
    Serial.begin(9600);
    Serial.println("AQUI");
    wifiMulti.addAP("Robotica-IMD", "roboticawifi");
    //http.setReuse(true);
    
    while (wifiMulti.run() != WL_CONNECTED) {
        delay(50);
        Serial.println("Waiting for connection");
    }
    int vector[5];
    getSpectrum(20, vector);
}

//LOOP ======================
double timeBegin = millis();

void loop() {
    /*
    http.begin("10.0.0.101", 8080, "/");
    int httpCode = http.GET();
    Serial.println(httpCode);
    http.writeToStream(&Serial);
    http.end();
    Serial.println("DEBUG");
    delay(10000);
    
    
    //nData = 0; //resetting number of samples
    
    while (i < 128) {
        sensorValueI = analogRead(pinSensor); //read value in the analogic pin
        sensorValueI = map(sensorValueI, 1, 775, 1, 512); //manual conversion (see README.md)
        sensorValueI -= 511; //offset (see README)
        sensorValueAcc += sensorValueI * sensorValueI; //sum of the data' squares
        delay(10);
        nData++; //counting number of samples
    }
    double timeEnd = millis();
    
    loadPower = rms(sensorValueAcc, timeEnd - timeBegin);
    postIt(loadPower);
    */

}