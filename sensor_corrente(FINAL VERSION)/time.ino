String updateHoraAtual() {
  HTTPClient http;
  http.begin("http://api.saiot.ect.ufrn.br/api/log/data-hora");
  int httpCode = http.GET(); //Retorna o código http, caso não conecte irá retornar -1
  dataAtual = http.getString();
  http.end();
  ano = dataAtual.substring(0, 4).toInt();
  mes = dataAtual.substring(5, 7).toInt();
  dia = dataAtual.substring(8, 10).toInt();
  hora = dataAtual.substring(11, 13).toInt();
  minuto = dataAtual.substring(14, 16).toInt();
  seg = dataAtual.substring(17, 19).toInt();
  sprintf(dateBuffer, "%04u-%02u-%02u %02u:%02u:%02u",  ano, mes, dia, hora, minuto, seg);
  return (String(dateBuffer));
}
