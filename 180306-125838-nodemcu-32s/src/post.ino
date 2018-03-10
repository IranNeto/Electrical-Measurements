void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      digitalWrite(D4, LOW);
      Serial.println("Connected!");
    } else {
      digitalWrite(D4, HIGH);
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*
void postIt(float* harmonics){

  for(int i = 0; i < nharmonics; i++){
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(harmonics[i], 6);
  }

}

/*
//Definir como vou enviar isso
void postIt(float* harmonics) {

  String qm = "\""; //separator of quotation marks ("")

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

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
*/