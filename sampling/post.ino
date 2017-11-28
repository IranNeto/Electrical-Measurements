void flagPost() {
  stopGettingData = true;  
  teste++;
  Serial.println(teste);
  if(teste > 1000){
    end = millis();
    Serial.println(end - s);
  }
}

void postIt(double loadPower) {

  String dateBuffer = updateHoraAtual(); //get the server's current hour
  String qm = "\""; //separator of quotation marks ("")

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;
    http.begin("https://api.saiot.ect.ufrn.br/api/log/tomada" , "BB 3E 7F 45 F4 0D C8 88 EC 9C F2 40 39 47 F6 93 D2 07 37 C1‎");
    http.addHeader("Content-Type", "application/json");
    String aspas = "\"";
    String serial = "sensorCorrente2";
    String data_hora = updateHoraAtual();
    String postMessage = "[{" + aspas + "serial" + aspas + ": " + aspas + serial + aspas + ", ";
    postMessage += aspas + "data_hora" + aspas + ": " + aspas + data_hora + aspas + ", ";
    postMessage += aspas + "pulso" + aspas + ": " + loadPower + "}]";
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
