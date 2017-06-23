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
StaticJsonBuffer<100> jsonBuffer; //Parâmetros da biblioteca ArduinoJson
SocketIOClient socket; //Instância da biblioteca SocketIOClient
DS1307 rtc(4, 5); //Instância da biblioteca do rtc
Time temp; //Instância da biblioteca Time 
JsonObject& root = jsonBuffer.createObject();

//===================================================================================
//VARIABLES
//===================================================================================

char host[] = "api.saiot.ect.ufrn.br"; //Host de envio
int port = 80; //porta para envio
extern String RID; //Constantes necessárias requisitadas pela biblioteca socket
extern String Rname;
extern String Rcontent;
//String JSON;
int pos = 0; //Posição
int pinSensor = A0; //Porta onde os sinais estão sendo enviados
int sensorValue_aux = 0; //Valor auxiliar do sensor
double valueSensor = 0; //Valor do sensor
float valueCurrent = 0; //Valor da corrente
float voltsperBit = 0.00329; // 3.3/1023 proporção de uma unidade do adc pelo aumento de tensão do sinal
float sensibility = 0.185; //mv/A proporção do sensor saída/entrada
int power = 220; //Tensão que o circuito é submetido
int nData; //Número de medições feitas
int start, theEnd; //Medições de tempo
char dateBuffer[30]; //
String ipStr; //
int ano, mes, dia, hora, minuto, seg; //variáveis 
bool stopGettingData = false; //flag para se parar de pegar dados
String dataAtual; //String com a data atual
String aspas = "\""; //separadores de string
String espaco = " "; 

//===================================================================================
//SETUP
//=================================================================================

void setup() {

  Serial.begin(115200); //inicia o serial
  pinMode(pinSensor, INPUT_PULLUP); //seta a porta de leitura
  sending.attach(timeToPost, flagPost); //interrupção
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
