String updateHoraAtual() {
  String currentDate; //store the string of the current date requested

  HTTPClient httpHora;
  httpHora.begin("https://api.saiot.ect.ufrn.br/api/log/data-hora", "BB 3E 7F 45 F4 0D C8 88 EC 9C F2 40 39 47 F6 93 D2 07 37 C1‎"); //route to request
  int httpCode = httpHora.GET(); //return the http code anserw
  currentDate = httpHora.getString();
  return (currentDate.substring(1, 20));
}
