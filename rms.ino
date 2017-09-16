float rms(double sensorValueAcc, double samplingTime){
  

  // calculating rms value (see README.md)
  sensorValueAcc = (sqrt(sensorValueAcc/nData)*voltsperBit)/sensibility;

  //reducing smallers currents values
  if (sensorValueAcc <= 0.095) {
    sensorValueAcc = 0;
  }

  //resetting sensor's value
  sensorValueAcc = 0;

  /*
    valueCurrent*220 = Get the total load power
    samplingTime/3600000 = transform the sampling time to hour
    the loadPower must be Wh 
  */
  loadPower = (valueCurrent*220)*(samplingTime/3600000); 
  return loadPower;
}

