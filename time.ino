String updateHoraAtual() {
  String currentDate; //store the string of the current date requested

  HTTPClient http;
  http.begin("http://api.saiot.ect.ufrn.br/api/log/data-hora"); //route to request
  int httpCode = http.GET(); //return the http code anserw
  
  currentDate = http.getString();

  http.end();
  ano = currentDate.substring(0, 4).toInt();
  mes = currentDate.substring(5, 7).toInt();
  dia = currentDate.substring(8, 10).toInt();
  hora = currentDate.substring(11, 13).toInt();
  minuto = currentDate.substring(14, 16).toInt();
  seg = currentDate.substring(17, 19).toInt();
  //reset the current hour format
  sprintf(dateBuffer, "%04u-%02u-%02u %02u:%02u:%02u",  year, month, day, hour, minute, second);
  return (String(dateBuffer));
}
