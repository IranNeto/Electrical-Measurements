float rms(double sensorValueAcc, double samplingTime){
  valueCurrent = (sqrt(sensorValueAcc/nData)*voltsPerBit)/sensibility;
  loadPower = (valueCurrent*220)*(samplingTime/3600000);
  valueCurrent = 0; 
  return loadPower;
}

