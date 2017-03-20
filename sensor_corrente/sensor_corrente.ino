/*
   uuid, value, data_hora

*/
//===================================================================================
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <dummy.h>
LiquidCrystal_I2C lcd(0x3f, 18, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
//===================================================================================

#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>//#include <SocketIOsocket.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
//===================================================================================

#define evento "/post/dispositivo/"
#define uuid_dispositivo "lampada002"
#define debugar true
#define luz D2 //D2
//===================================================================================
StaticJsonBuffer<100> jsonBuffer;

SocketIOClient socket;

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

  root["uuid"] = uuid_dispositivo;
  root.printTo(JSON);

  if (!socket.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  if (socket.connected())
  {
    Serial.println("\n\n\n\n\nConectou no servidor\n\n\n");
    socket.emit("/post/dispositivo/", JSON);
  }

  /*
    socket.on("action", light);
    socket.on("action2", funcionaDois);
  */
}


int pos = 0;
void loop() {
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
}
