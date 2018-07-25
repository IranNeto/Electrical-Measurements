void postIt() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;
    String aspas = "\"";
    String serial = "tomada01";
    String data_hora = updateHoraAtual();
    String postMessage;
  
    http.begin("http://10.7.227.121:3001/v1/device/history/logs");
    http.addHeader("Content-Type", "application/json");
    //loadPower = sprintf(buffer, "%.4f", loadPower);
    if(loadPower <= 0.0095){
      loadPower = 0;
    }

    postMessage = "[{\"data\": [{ " + aspas + "serial" + aspas + ": " + aspas + serial + aspas + ", ";
    postMessage += aspas + "POT" + aspas + ": " + loadPower + "}], \"dateTime\": " + aspas + data_hora + aspas + " }]";
    
    int httpCode = http.POST(postMessage);
    // Serial.println(httpCode);
    
    while(httpCode != 200){
      // Serial.println("postando de novo");
      // Serial.println("Atualizando fingerPrint");
      // updateFingerPrint();
      httpCode = http.POST(postMessage);
      delay(3000);
    }
    http.end();
  
  } else {
    Serial.print("Error in Wifi connection");
  }
  stopGettingData = false; //resetting to begin a new sampling loop
}

// void updateFingerPrint(){
//   String currentFp; //store the string of the current date requested
//   HTTPClient checkFp;
//   checkFp.begin("http://auth.saiot.ect.ufrn.br");
//   int httpCode = checkFp.GET(); //return the http code anserw
//   currentFp = checkFp.getString();
//   if(key != currentFp){
//     key = currentFp;
//   }
// }

String getFingerPrint(){
  String currentFp; //store the string of the current date requested
  HTTPClient getFp;
  getFp.begin("http://auth.saiot.ect.ufrn.br");
  int httpCode = getFp.GET(); //return the http code anserw
  if(httpCode == 200){
    gotFp = true;
    return getFp.getString();
  } else {
    delay(2000);
  }
}

String getToken(){
  if (WiFi.status() == WL_CONNECTED) {
  HTTPClient http;
  http.begin("http://10.7.227.122:3001/v1/device/auth/login");
  http.addHeader("Content-Type", "application/json");
  String a = "\"";
  String msg = "{" + a + "email" + a + ": " + a + "guga@e.c" + a + ", " + a + "password" + a + ": ";
  msg += a + "gugagugaguga" + a + ", " + a + "serial" + a + ": " + a + "tomadinha" + a + "}";
  int code = http.POST(msg);
  Serial.println(code);
  Serial.println(http.getString());
  delay(1000000);
  } else {
    Serial.println("NAAAAO");
  }
}