void postIt() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;
    String aspas = "\"";
    String serial = "tomada01";
    String data_hora = updateHoraAtual();
    String postMessage;
  
    http.begin("http://10.7.227.121:3001/v1/device/history/logs");
    http.addHeader("Content-Type", "application/json");
    if(loadPower <= 0.0095){
      loadPower = 0;
    }

    postMessage = "[{\"data\": [{ " + aspas + "serial" + aspas + ": " + aspas + serial + aspas + ", ";
    postMessage += aspas + "POT" + aspas + ": " + loadPower + "}], \"dateTime\": " + aspas + data_hora + aspas + " }]";
    
    int httpCode = http.POST(postMessage);
    http.end();
  
  } else {
    Serial.print("Error in Wifi connection");
  }
}