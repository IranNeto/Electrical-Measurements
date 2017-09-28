float rms(double sensorValueAcc, double samplingTime){
  

  // calculating rms value (see README.md)
  valueCurrent = (sqrt(sensorValueAcc/nData)*voltsPerBit)/sensibility;

  //reducing smallers currents values
  //if (valueCurrent <= 0.095) {
  //  valueCurrent = 0;
  //}

  loadPower = (valueCurrent*220)*(samplingTime/3600000);
  valueCurrent = 0; 
  return loadPower;
}

