#include <Arduino.h>
#include <ESP32Ticker.h>

Ticker test;
volatile int data[200], i = 0;

void flag(){
  Serial.println(analogRead(A0));
}

void setup(){
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  test.attach_ms(2,flag);
}

void loop(){
}