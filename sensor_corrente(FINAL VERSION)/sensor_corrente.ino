//LIBRARIES
//===================================================================================

#include <Time.h>
#include <TimeLib.h>
#include <Ticker.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <DS1307.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>//#include <SocketIOsocket.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

//===================================================================================
//DEFINES AND FOWARDS DECLARATIONS
//===================================================================================

#define evento "/post/log/tomada/"
#define serial "sensorCorrente"
#define timeToPost 20
void flagPost();

//===================================================================================
//OBJECTS
//===================================================================================

LiquidCrystal_I2C lcd(0x3f, 18, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
StaticJsonBuffer<100> jsonBuffer;
SocketIOClient socket;
DS1307 rtc(4, 5);
Time temp;
JsonObject& root = jsonBuffer.createObject();
Ticker sending;

//===================================================================================
//VARIABLES
//===================================================================================

char host[] = "api.saiot.ect.ufrn.br";
int port = 80;
extern String RID;
extern String Rname;
extern String Rcontent;
//String JSON;
int pos = 0;
int pinSensor = A0;
int sensorValue_aux = 0;
double valueSensor = 0;
float valueCurrent = 0;
float voltsperBit = 0.00329; // 3.3/1023
float sensibility = 0.185; //mv/A
int power = 220;
int nData;
int start, theEnd;
char dateBuffer[30];
String ipStr;
unsigned long previousMillis = 0;
long interval = 5000;
unsigned long lastreply = 0;
unsigned long lastsend = 0;
int ano, mes, dia, hora, minuto, seg;
bool stopGettingData = false;
String dataAtual;
String aspas = "\"";
String espaco = " ";
//===================================================================================
//SETUP
//=================================================================================

void setup() {

  Serial.begin(115200);
  pinMode(pinSensor, INPUT_PULLUP);
  sending.attach(timeToPost, flagPost);
  delay(10);
  WiFiManager wifis;
  wifis.autoConnect();
  IPAddress ip = WiFi.localIP();
  ipStr = String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);
  if (!socket.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
}
//===================================================================================
//LOOP
//===================================================================================

void loop() {
  socket.monitor();
  nData = 0;
  //while(nData < 1000){
  double start = millis();
  while(!stopGettingData){
    sensorValue_aux = analogRead(A0);
    //Serial.println(sensorValue_aux);
    sensorValue_aux = map(sensorValue_aux, 1, 722, 1, 512);
    sensorValue_aux -= 511; //METADE
    valueSensor += sensorValue_aux * sensorValue_aux;
    delay(10);
    nData++;
  }
  double fim = millis();
                                                                                                                                                                                                                                                                                                        
  //Serial.print("Valores quadráticos = "); Serial.println(valueSensor);
  double pot = rms(valueSensor, fim-inicio);
  //Serial.print("Valor rms = "); Serial.println(pot);
  //Serial.print("nData = "); Serial.println(nData);
  //delay(100000000000000000);
  postar(pot);
}
