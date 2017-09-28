String updateHoraAtual() {
  String currentDate; //store the string of the current date requested

  HTTPClient http;
  http.begin("http://api.saiot.ect.ufrn.br/api/log/data-hora"); //route to request
  int httpCode = http.GET(); //return the http code anserw
  
  currentDate = http.getString();

  http.end();
  year = currentDate.substring(0, 4).toInt();
  month = currentDate.substring(5, 7).toInt();
  day = currentDate.substring(8, 10).toInt();
  hour = currentDate.substring(11, 13).toInt();
  minute = currentDate.substring(14, 16).toInt();
  second = currentDate.substring(17, 19).toInt();
  //reset the current hour format
  sprintf(dateBuffer, "%04u-%02u-%02u %02u:%02u:%02u",  year, month, day, hour, minute, second);
  return (String(dateBuffer));
}
