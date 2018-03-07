
String updateHoraAtual() {
  String currentDate; //store the string of the current date requested

  HTTPClient httpHora;
  httpHora.begin("https://api.saiot.ect.ufrn.br/v1/history/data-hora/", "3C 5E 30 6D 94 49 86 6E CF F4 3E 1F 89 DE 40 7A 80 55 F5 77‎"); //route to request
  int httpCode = httpHora.GET(); //return the http code anserw
  currentDate = httpHora.getString();
  Serial.println(currentDate);
  return (currentDate.substring(0, 19));
}