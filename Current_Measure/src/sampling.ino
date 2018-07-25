//LIBRARIES ======================
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <stdlib.h>
#include <stdio.h>

//DEFINES AND FOWARDS DECLARATIONS ======================
#define POSTDISPOSITIVO "/device/manager/device" // v 1.7
//#define serial "sensorCorrente2"
#define timeToPost 2
#define power 220 //220 V
void flagPost();
double start = millis();

//VARIABLES ======================
String key;

String jsconf = "{\"name\": \"tomada01\",\"serial\": \"tomada01\",\"protocol\":\"http\",\"sensors\":[{\"key\": \"POT\",\"unit\": \"Wh\",\"type\":\"number\",\"tag\": \"Medidor Consumo\"}]}";

bool gotFp = false;
int pinSensor = A0; //Sensor's pin at ESP8266
int sensorValueI = 0; //Sersor's value data i of n
double sensorValueAcc = 0; //Sensor's value accumulated
float sensibility = 0.185; //mv/A in/out ratio of the sensor
float voltsPerBit = 0.00329; //Minimal fluctuation of voltage to add a unity in the ADC
int nData; //Number of data sampled
long double timeBegin, timeEnd = 0.0; //store the time that begun and finished the data sampling.
int year, month, day, hour, minute, second; //store the time request (see time.ino)
bool stopGettingData = false; //flag that ends the sampling (ver post.ino)
double loadPower; //power consumed by the load in the sampling time
float valueCurrent;
int i;
long double interval = 1000.0;
char buffer[40];
//SETUP ======================
void setup() {
    WiFi.begin("ESP", "12345678");
    // key = getFingerPrint();
    Serial.begin(9600);
    pinMode(pinSensor, INPUT_PULLUP);
    delay(10);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.println("Waiting for connection");
        }
    HTTPClient http_conf;
    http_conf.begin("http://10.7.227.121:3001/v1/device/manager/device");
    http_conf.addHeader("Content-Type", "application/json");
    Serial.println(http_conf.POST(jsconf));
}


//LOOP ======================

//
//1810 amostras em 2seg
void loop() {
    delay(10000);
    i = 0;
    sensorValueAcc = 0;
    timeBegin = micros();
    while(timeEnd - timeBegin < 1500000){
        sensorValueI = 0;
        sensorValueI = map(analogRead(A0), 0, 775, 0, 512) - 512;
        Serial.printf("%i \n", sensorValueI);
        sensorValueI *= sensorValueI;
        sensorValueAcc += sensorValueI;
        i++;
        timeEnd = micros();
        while(micros() - timeEnd < interval){}
        // delayMicroseconds(10);
    }
    valueCurrent = ((sqrt(sensorValueAcc/i)*voltsPerBit)/sensibility);
    loadPower = valueCurrent * 220.0 * 1500.0/3600000.0;
    Serial.print("> ");
    Serial.printf("%.3f, %.3f \n", loadPower, valueCurrent);
    postIt();
}
