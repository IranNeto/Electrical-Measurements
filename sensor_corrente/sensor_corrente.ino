/*
 * uuid, value, data_hora
 * 
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <dummy.h>
LiquidCrystal_I2C lcd(0x3f, 18, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//Porta analÃ³gica de leitura
uint16_t pinSensor = A0;

//VariÃ¡vel auxiliar que pega o valor de leitura instantaneo
int sensorValue_aux = 0;
double valueSensor = 0;
float valueCurrent = 0;
float allDatas[10];
//float voltsporUnidade = 0.004887586;// 5/1023

//Isso significa que a cada variação de voltsporUnidade 1 bit será movido
float voltsperBit = 0.00329; // 3.3/1023
float sensibility = 0.185; //mv/A

//Tensao da rede AC 110 Volts e na verdade (127 volts)
int power = 220;
int i;
int loopTime = 10000;
/*Declaracao de Constates e Objetos*/
void setup() {
  //Incia a Serial
  Serial.begin(9600);
  pinMode(pinSensor, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
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
  float pot = rms(takeData(loopTime)) * power;
  //lcd.print(pot);
  
  //Serial.print(valueCurrent);
  allDatas[pos] = valueCurrent;
  pos++;
  Serial.println(pos);
  //Serial.print("  ");
  //Serial.println(pot);
  if(pos > 9){
    for(int i = 0; i < pos; i++){
      Serial.println(allDatas[i]);  
    }
    delay(10000);
  }
}
