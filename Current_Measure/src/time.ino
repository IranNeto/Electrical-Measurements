
String updateHoraAtual() {
  String currentDate; //store the string of the current date requested
  HTTPClient httpHora;
  httpHora.begin("http://10.7.227.121:3001/v1/device/history/datetime"); //route to request
  int httpCode = httpHora.GET();
  currentDate = httpHora.getString();
  Serial.println(currentDate);
  return (currentDate.substring(0, 19));
}