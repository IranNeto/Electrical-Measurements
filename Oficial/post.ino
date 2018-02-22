void flagPost() {
  stopGettingData = true;
}

void postIt(double loadPower) {

  String qm = "\""; //separator of quotation marks ("")

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;
    http.begin("https://api.saiot.ect.ufrn.br/v1/history/tomada" , "3C 5E 30 6D 94 49 86 6E CF F4 3E 1F 89 DE 40 7A 80 55 F5 77‎");
    http.addHeader("Content-Type", "application/json");
    String aspas = "\"";
    String serial = "sensorCorrente2";
    String data_hora = updateHoraAtual();
    String postMessage = "[{ \"dados\": [{ " + aspas + "serial" + aspas + ": " + aspas + serial + aspas + ", ";
    postMessage += aspas + "data_hora" + aspas + ": " + aspas + data_hora + aspas + ", ";
    postMessage += aspas + "pulso" + aspas + ": " + loadPower + "}], \"data_hora\": " + aspas + data_hora + aspas + " }]";
    int httpCode = http.POST(postMessage);
    Serial.print("http result:");
    Serial.println(httpCode);
    http.writeToStream(&Serial);
    String payload = http.getString();
    http.end();

  } else {

    Serial.print("Error in Wifi connection");

  }
  stopGettingData = false; //resetting to begin a new sampling loop
}
