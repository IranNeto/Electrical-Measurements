//=======================================================================================================
//=====================================LIBRARIES ========================================================
//=======================================================================================================

//#include <Time.h>
//#include <TimeLib.h>
#include <Ticker.h>
#include <Wire.h>
//#include <SocketIOClient.h>
#include <ESP8266HTTPClient.h>
//#include <DS1307.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "arduinoFFT.h"

//=======================================================================================================
//===================================DEFINES AND FOWARDS DECLARATIONS ===================================
//=======================================================================================================

/* FFT */
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02

const uint16_t samples = 1024; //This value MUST ALWAYS be a power of 2
double signalFrequency = 60;
double samplingFrequency;
uint8_t amplitude = 512;
double vReal[samples];
double vImag[samples];

/* GENERAL */
void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType);
void flagPost();
Ticker sending;
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
char dateBuffer[40]; //by default of some library
String ipStr; //by default of some library
int pinSensor = A0; //Sensor's pin at ESP8266
//float sensibility = 0.185; //mv/A in/out ratio of the sensor
//float voltsPerBit = 0.00329; //Minimal fluctuation of voltage to add a unity in the ADC
//int nData; //Number of data sampled
int year, month, day, hour, minute, second; //store the time request (see time.ino)
bool stopGettingData = false; //flag that ends the sampling (ver post.ino)
double sumData;
float sensibility = 0.185; //mv/A in/out ratio of the sensor
float voltsPerBit = 0.00329; //Minimal fluctuation of voltage to add a unity in the ADC
int nHarmonic = 0;
double magnitudeHarmonic[20] ={};
//=======================================================================================================
//========================================= SETUP =======================================================
//=======================================================================================================


void setup() {
	Serial.begin(115200);
	pinMode(pinSensor, INPUT);
	Serial.println("OK");
	//sending.attach_ms(timeToPost, flagPost);
	/*
	WiFi.begin("RSBEZERRA2", "");
	Serial.begin(9600);
	pinMode(pinSensor, INPUT); //<---------------------
	sending.attach_ms(timeToPost, flagPost); //interruption: each timeToPost seconds the function flagToPost is called
	delay(10);
	WiFiManager wifis;
	wifis.autoConnect();
	IPAddress ip = WiFi.localIP();
	ipStr = String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);
	*/
}




//=======================================================================================================
//========================================== LOOP =======================================================
//=======================================================================================================


double tempo_final, tempo_inicial = millis();
int i = 0;
int data[samples] = {};

void loop() {
	if (i < samples){
		data[i] = analogRead(A0);
		data[i] = map(data[i], 1, 765, 1, 512);
		data[i] -= 511;
		i++;
		Serial.println(i);
	} else {
		tempo_final = millis();
		calculaNamostras(tempo_final - tempo_inicial, data);
	}
}
