void flagPost(){
  stopGettingData = true;
}

void postar(float pot){
  updateHoraAtual();
  sprintf(dateBuffer, "%04u-%02u-%02u %02u:%02u:%02u",  year(), month(), day(), hour(), minute(), second());
  root["serial"] = serial;
  root["data_hora"] = String(dateBuffer);
  root["pulso"] = pot;
  root.printTo(JSON);
  String BigJson = "[" + JSON + "]";
  Serial.println(JSON);
  Serial.println(BigJson);
  socket.emit(evento, BigJson);
  JSON = "";
  BigJson = "";
  Serial.println("\n Postou \n");

  stopGettingData = false;
}
