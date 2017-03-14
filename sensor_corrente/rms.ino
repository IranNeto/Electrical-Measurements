double takeData(int loopTime){
  i = 0;
  int start = millis();
  int theEnd = millis();
  while((theEnd - start) < loopTime){
    sensorValue_aux = analogRead(pinSensor);
    sensorValue_aux = map(sensorValue_aux, 1, 490, 1, 512);
    sensorValue_aux -= 476; //METADE
    //Serial.println(sensorValue_aux);
    valueSensor += sensorValue_aux * sensorValue_aux;
    delay(3);
    i++;
    theEnd = millis();
  }
  return valueSensor;
}

float rms(double valorSensor){
  // finaliza o calculo da mÃ©dia quadratica e ajusta o valor lido para volts
  valueSensor = sqrt(valueSensor/i)*voltsperBit;
  // calcula a corrente considerando a sensibilidade do sernsor (185 mV por amper)
  valueCurrent = (valueSensor/sensibility);

  if (valueCurrent <= 0.095) {
    valueCurrent = 0;
  }
  valueSensor = 0;

  return valueCurrent;
}

