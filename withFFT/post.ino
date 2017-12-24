void flagPost() {
  //stopGettingData = true;
  Serial.println(analogRead(pinSensor));
}

void postIt(double loadPower) {
  String qm = "\""; //separator of quotation marks ("")

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;
    http.begin("https://api.saiot.ect.ufrn.br/v1/history/tomada" , "57 F8 1A E8 70 DA 66 0B 7A A7 65 89 17 C9 F1 0E BB 1A DA E8");
    http.addHeader("Content-Type", "application/json");
    String aspas = "\"";
    String serial = "tomadinha";
    String data_hora = updateHoraAtual();
    String postMessage = "[{ \"dados\": [{" + aspas + "serial" + aspas + ": " + aspas + serial + aspas + ", ";
    postMessage += aspas + "pulso" + aspas + ": " + loadPower + " }], \"data_hora\": " + aspas + data_hora + aspas + "}]";
    Serial.println(postMessage);
    int httpCode = http.POST(postMessage);
    Serial.print("http result:");
    Serial.println(httpCode);
    http.writeToStream(&Serial);
    String payload = http.getString();
    http.end();

  } else {

    Serial.print("Error in Wifi connection");

  }

  Serial.println("\n Posted \n");
  stopGettingData = false; //resetting to begin a new sampling loop
}
