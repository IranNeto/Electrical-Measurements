#include <Time.h>
#include <TimeLib.h>

/*
   uuid, value, data_hora

   [
  {
    "serial": "nomedodispositivo",
    "data_hora": "2017-01-10 22:03:03",
    "pulso": 10
  },
  {
    "serial": "nomedodispositivo",
    "data_hora": "2017-01-10 22:05:03",
    "pulso": 20
  },
  outros mais
  ]
*/
//===================================================================================
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <dummy.h>
LiquidCrystal_I2C lcd(0x3f, 18, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
//===================================================================================
#include <DS1307.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>//#include <SocketIOsocket.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

//===================================================================================

#define evento "/post/log/tomada/"
#define serial "sensorCorrente"


//===================================================================================
StaticJsonBuffer<100> jsonBuffer;
SocketIOClient socket;
DS1307 rtc(4, 5);
Time temp;
//===================================================================================

char host[] = "saiot.ect.ufrn.br";
int port = 80;

extern String RID;
extern String Rname;
extern String Rcontent;

String JSON;
JsonObject& root = jsonBuffer.createObject();
//===================================================================================

uint16_t pinSensor = A0;
int sensorValue_aux = 0;
double valueSensor = 0;
float valueCurrent = 0;
float voltsperBit = 0.00329; // 3.3/1023
float sensibility = 0.185; //mv/A
int power = 220;
int i;
int loopTime = 5000;
char dateBuffer[19];

unsigned long previousMillis = 0;
long interval = 5000;
unsigned long lastreply = 0;
unsigned long lastsend = 0;
//=================================================================================

void setup() {
  //Incia a Serial
  Serial.begin(115200);
  pinMode(pinSensor, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  delay(10);

  WiFiManager wifis;
  wifis.autoConnect();

  if (!socket.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }

  rtc.halt(false);
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  rtc.enableSQW(true);
  socket.on("debug", funfou);
  /*
    socket.on("action2", funcionaDois);
  */
}
void funfou(String f) {
  Serial.println(f);
}

int pos = 0;

void loop() {
  socket.monitor();
  time_t t = now();
  Serial.println("\n\n\n\n\nLOOP\n");
  socket.emit(evento, JSON);
  sprintf(dateBuffer,"%04u-%02u-%02u %02u:%02u:%02u", temp.year, temp.date, t.mon, temp.hour, temp.min, temp.sec);
  root["serial"] = serial;
  root["data_hora"] = String(dateBuffer);
  root["pulso"] = rand();
  root.printTo(JSON);
  String BigJson = "[" + JSON + "]";
  Serial.println(JSON);
  Serial.println(BigJson);
  socket.emit(evento, BigJson);
  JSON = "";
  BigJson = "";
  
  delay(2000);
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
