#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <dummy.h>
LiquidCrystal_I2C lcd(0x3f, 18, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//Porta analógica de leitura
uint16_t pinoSensor = 0;

//Variável auxiliar que pega o valor de leitura
int sensorValue_aux = 0;

double valorSensor = 0;
float valorCorrente = 0;

//float voltsporUnidade = 0.004887586;// 5/1023
float voltsporUnidade = 0.00322265625; // 3.3/1024
//float voltsporUnidade = 0.0009;// 1/1023
// Para ACS712 de  5 Amperes use 0.185
// Para ACS712 de 10 Amperes use 0.100

float sensibilidade = 0.185; //mv/A

int i = 0;

//Tensao da rede AC 110 Volts e na verdade (127 volts)
int tensao = 220;

/*Declaracao de Constates e Objetos*/
void setup() {
  //Incia a Serial
  Serial.begin(9600);
  pinMode(pinoSensor, INPUT);
  lcd.init();
  lcd.backlight();
}

void loop() {

  for (int i = 5000; i > 0; i--) {
    int val = analogRead(pinoSensor);
    sensorValue_aux = map(val, 10, 690, 0, 1023);
    // le o sensor na pino analogico A0 e ajusta o valor lido ja que a saída do sensor é (1023)vcc/2 para corrente =0
    // somam os quadrados das leituras.
    sensorValue_aux -= 511;
    valorSensor += sensorValue_aux * sensorValue_aux;
    delay(1);
  }
  //delay(10000000);
  //for modificado
  /*
    for (int i = 1000; i > 0; i--) {
    sensorValue_aux = (511 * sin(2*PI*i));
    valorSensor += sensorValue_aux*sensorValue_aux;
    //Serial.println(sensorValue_aux);
    delay(10);
    }
  */
  // finaliza o calculo da média quadratica e ajusta o valor lido para volts
  valorSensor = sqrt(valorSensor/5000)* voltsporUnidade;
  // calcula a corrente considerando a sensibilidade do sernsor (185 mV por amper)
  valorCorrente = (valorSensor/sensibilidade);

  if (valorCorrente <= 0.095) {
    valorCorrente = 0;
  }
  valorSensor = 0;

  //Escreve
  //Mostra o valor da corrente
  
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("C(a): ");
  lcd.setCursor(8, 0);
  lcd.print(valorCorrente);
  lcd.setCursor(0, 1);
  lcd.print("P(w): ");
  lcd.setCursor(8, 1);
  
  float pot = valorCorrente * tensao;
  lcd.print(pot);
  
  Serial.print(valorCorrente);
  Serial.print("  ");
  Serial.println(pot);

  delay(1);
} 
