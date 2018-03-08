//LIBRARIES ======================
//#include <Time.h>
//#include <TimeLib.h>
#include <Ticker.h>
#include <Wire.h>
//#include <SocketIOClient.h>
#include <ESP8266HTTPClient.h>
//#include <DS1307.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

//DEFINES AND FOWARDS DECLARATIONS ======================

#define routeToPost "/post/log/tomada/"
//#define serial "sensorCorrente2"
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
//SETUP ======================

void setup() {
    WiFi.begin("ESP", "12345678");
    Serial.begin(9600);
    pinMode(pinSensor, INPUT_PULLUP);
    delay(10);
        while (WiFi.status() != WL_CONNECTED) {
            
            delay(500);
            Serial.println("Waiting for connection");
        }
    }

int teste = 0;
double s = millis();
double end;

//LOOP ======================

void loop() {
    nData = 0; //resetting number of samples
    loadPower = 2.0;
    postIt(loadPower);
    delay(2000);
}