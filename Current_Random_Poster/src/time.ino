
String updateHoraAtual() {
  String currentDate; //store the string of the current date requested

  HTTPClient httpHora;
  httpHora.begin("http://10.7.227.121:3001/v1/device/history/datetime"); //route to request
  // httpHora.begin("http://auth.saiot.ect.ufrn.br");
   //return the http code anserw
  int httpCode = httpHora.GET();
  // while(httpCode != 200){
  //   updateFingerPrint();
  //   httpCode = httpHora.GET();
  //   delay(3000);
  // }
  currentDate = httpHora.getString();
  // const char * headerNames[] = {"ETag"};
  // httpHora.collectHeaders(headerNames, sizeof(headerNames)/sizeof(headerNames[0]));
  
  // Serial.printf("Header count: %d\r\n", http.headers());
  
  // for (int i=0; i < httpHora.headers(); i++){
  //   Serial.println("E");
  //   if(httpHora.hasHeader("ETag")){
  //     String location = httpHora.header("ETag");
  //     Serial.println(location);
  //   }
  // }

  Serial.println(currentDate);
  return (currentDate.substring(0, 19));
}