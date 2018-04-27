//LIBRARIES ======================
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <stdlib.h>

//DEFINES AND FOWARDS DECLARATIONS ======================

#define routeToPost "/post/log/tomada/"
//#define serial "sensorCorrente2"
#define timeToPost 2
#define power 220 //220 V
void flagPost();
double start = millis();

//VARIABLES ======================
String key;
bool gotFp = false;
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
    key = getFingerPrint();
    WiFi.begin("ESP", "12345678");
    Serial.begin(9600);
    pinMode(pinSensor, INPUT_PULLUP);
    delay(10);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.println("Waiting for connection");
        }
        
    }

//LOOP ======================
void loop() {
    loadPower = random(3) % 2 + 0.5 + 1.1 * random(2);
    Serial.println(loadPower);
    postIt(loadPower);
    delay(2000);
}