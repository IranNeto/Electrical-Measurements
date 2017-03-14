#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <dummy.h>
LiquidCrystal_I2C lcd(0x3f, 18, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//Porta analÃ³gica de leitura
uint16_t pinoSensor = A0;

//VariÃ¡vel auxiliar que pega o valor de leitura instantaneo
int sensorValue_aux = 0;

double valorSensor = 0;
float valorCorrente = 0;

//float voltsporUnidade = 0.004887586;// 5/1023

//Isso significa que a cada variação de voltsporUnidade 1 bit será movido
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
  pinMode(pinoSensor, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  /*
   * 5A = 930
   * 2.5A = 464
   * 0A = 1
   */
}

void loop() {

  for (int i = 3000; i > 0; i--) {
    //sensorValue_aux = map(val, 10, 690, 0, 1023);
    // le o sensor na pino analogico A0 e ajusta o valor lido ja que a saÃ­da do sensor Ã© (1023)vcc/2 para corrente =0
    sensorValue_aux = analogRead(pinoSensor);
    sensorValue_aux -= 475; //METADE
    sensorValue_aux = map(sensorValue_aux,1,950,1,1023);
    // somam os quadrados das leituras.
    Serial.println(sensorValue_aux);
    //
    valorSensor += sensorValue_aux * sensorValue_aux;
    delay(5);
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
  // finaliza o calculo da mÃ©dia quadratica e ajusta o valor lido para volts
  valorSensor = sqrt(valorSensor/3000)*voltsporUnidade;
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

  delay(10000000000000);
} 
