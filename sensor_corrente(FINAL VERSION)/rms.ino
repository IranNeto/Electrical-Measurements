float rms(double valorSensor, double dataInterval){
  // finaliza o calculo da mÃ©dia quadratica e ajusta o valor lido para volts
  valueSensor = sqrt(valueSensor/nData)*voltsperBit;
  // calcula a corrente considerando a sensibilidade do sernsor (185 mV por amper)
  valueCurrent = (valueSensor/sensibility);

  if (valueCurrent <= 0.095) {
    valueCurrent = 0;
  }
  valueSensor = 0;
  double pot = (valueCurrent*220)*(dataInterval/3600000); 
  return pot;
}

