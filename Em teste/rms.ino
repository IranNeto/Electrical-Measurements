bool isOn(float* data, double sumData){
  float media = sumData/100;
  float squareSum = 0;
  for(int i = 0; i < 100; i++){
    squareSum += (data[i] - media) * (data[i] - media);
  }
  float dp = sqrt(squareSum/100);

  if(dp < 15){
    return false;
  } else {
    return true;
  }
}


float rms(double sensorValueAcc, double samplingTime){
  

  // calculating rms value (see README.md)
  valueCurrent = (sqrt(sensorValueAcc/nData)*voltsPerBit)/sensibility;

  // reducing smallers currents values
  if (valueCurrent <= 0.01) {
    valueCurrent = 0;
  }

  loadPower = (valueCurrent*power)*(samplingTime/3600000);
  valueCurrent = 0; 
  return loadPower;
}

