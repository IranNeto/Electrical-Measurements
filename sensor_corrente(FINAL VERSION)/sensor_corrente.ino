//Atualizar o tempo, Pulso Nan

//LIBRARIES
//===================================================================================
#include <Time.h>
#include <Ticker.h>
#include <TimeLib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <dummy.h>
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
String JSON;
int pos = 0;
uint16_t pinSensor = A0;
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
//===================================================================================
//SETUP
//=================================================================================

void setup() {

  Serial.begin(115200);
  pinMode(pinSensor, INPUT_PULLUP);
  //lcd.init();
  //lcd.backlight();
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

  rtc.halt(false);
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  rtc.enableSQW(true);

  
}
//===================================================================================
//LOOP
//===================================================================================

void loop() {
  socket.monitor();
  nData = 0;
  while(!stopGettingData){
    sensorValue_aux = analogRead(pinSensor);
    sensorValue_aux = map(sensorValue_aux, 1, 490, 1, 512);
    sensorValue_aux -= 476; //METADE
    //Serial.println(sensorValue_aux);
    valueSensor += sensorValue_aux * sensorValue_aux;
    delay(3);
    nData++;
  }
  postar(rms(valueSensor));
  
}

/*
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("C(a): ");
  lcd.setCursor(8, 0);
  lcd.print(valorCorrente);
  lcd.setCursor(0, 1);
  lcd.print("P(w): ");
  lcd.setCursor(8, 1);
*/
//float pot = rms(takeData(loopTime)) * power;
//pos++;
//Serial.println(pos);
//Serial.print("  ");
//Serial.println(pot);
//if (pos > 9) {
//for (int i = 0; i < pos; i++) {
//Serial.println(allDatas[i]);
//}
//delay(10000);
//}


