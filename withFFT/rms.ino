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

