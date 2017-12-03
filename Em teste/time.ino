String updateHoraAtual() {
  String currentDate; //store the string of the current date requested

  HTTPClient httpHora;
  httpHora.begin("https://api.saiot.ect.ufrn.br/v1/history/data-hora", "57 F8 1A E8 70 DA 66 0B 7A A7 65 89 17 C9 F1 0E BB 1A DA E8"); //route to request
  int httpCode = httpHora.GET(); //return the http code anserw
  currentDate = httpHora.getString();
  return (currentDate.substring(0, 19));
}
