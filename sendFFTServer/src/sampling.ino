#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <math.h>
#include "arduinoFFT.h"
#include <PubSubClient.h>

/* Declarações iniciais */
arduinoFFT FFT = arduinoFFT();
const int N = 1024;
int PORT = 8088;
const char* ssid = "LII";
const char* password = "wifiLI2Rn";
const char* mqtt_server = "192.168.0.115";
long lastMsg = 0;
char buff[10000];
double timeEnd, timeBegin;

WiFiClient espClient;
PubSubClient client(espClient);

//numero atribuir(float real, float imag);
int SAMPLING_FREQUENCY = 1024; //Hz, must be less than 10000 due to ADC
unsigned int sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
unsigned long microseconds;
//double vReal[N] = {};
int vReal;
int i = 0;
int k = 0;
//SETUP ======================


void setup() {
    Serial.begin(9600);
    Serial.println("INCIANDO O ESP8266");

    setup_wifi();
    client.setServer(mqtt_server, PORT);
    client.setCallback(callback);
    
}

String json;
//LOOP ======================

void loop() {
    //Inicío da amostragem
    timeBegin = millis();

    /* 
       buff é um vetor de char que guardará todos os dados que vai ser enviado ao servidor. Pelo que foi testado, formatando
       os dados como tipo String a um consumo muito grande de memória, daí tratamos caractere a caractere pra armazenar grande quantidade de dados.
       Na primeira posição do vetor tem um colchete e os demais são os dados.
       sprintf formata o dado do ADC como um inteiro de 4 casas (ou seja 955 == 0955, por exemplo) e adiciona um espaço. Tudo isso (numero e espaço)
       é colocado na posição logo depois do colchete na variavel buff. Como tratamos caractere por caractere, 4 numéros e um espaço são 5 caracteres,
       por isso o i é somado de 5 em 5. O while no final do for é uma maneira de assegurar a frequencia de amostragem que queremos.
       E assim é adicionado no vetor e é mandado para o servidor. O último serial.println é mostrando quando tempo tudo isso demora.   
    */
    buff[0] = '[';
    for(int i = 1, j = 0; j < N; i+=5){
        microseconds = micros(); 
        //vReal = map(analogRead(A0), 1, 774, 1, 512) - 512; //map in 3v3 adc scale
        sprintf(buff + i,"%.4i ", analogRead(A0));
        delayMicroseconds(10); //opcional
        while(micros() < (microseconds + sampling_period_us)){}
    }

    if (!client.connected()) {
        reconnect();
    }

    //I*THD = Corrente total distorcida;
    //I*(1-THD) = Corrente da frequencia fundamental
    client.loop();
    //json.toCharArray(buff, json.length()+1);
    //Serial.print("Publish message: ");
    client.publish("/temperature", buff);
    Serial.println(millis() - timeBegin);
    delay(1000);
    k++;
    
}