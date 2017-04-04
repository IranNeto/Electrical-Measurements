void flagPost(){
  stopGettingData = true;
}

void postar(double pot){
  String dateBuffer = updateHoraAtual();
  String JSON = "[{" + aspas + "serial" + aspas + ": " + aspas + serial + aspas + ", " + aspas + "data_hora" + aspas + ": " + aspas + dateBuffer + aspas + ", " + aspas + "pulso" + aspas + ": " + pot + "}]"; 
  socket.emit(evento, JSON);
  JSON = "";
  Serial.println("\n Postou \n");
  stopGettingData = false;
}
