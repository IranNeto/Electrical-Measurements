void flagPost(){
  stopGettingData = true;
}

void postIt(double loadPower){

  String dateBuffer = updateHoraAtual(); //get the server's current hour
  String qm = "\""; //separator of quotation marks ("")
  
  //Manually assembling the json's fields
  String JSON = "[{" + qm + "serial" + qm + ": " + qm + serial + qm + ", " + qm + "data_hora" + qm + ": " + qm + dateBuffer + qm + ", " + qm + "pulso" + qm + ": " + loadPower + "}]"; 
  socket.sendJSON(routeToPost, JSON);
  JSON = ""; //resetting JSON
  Serial.println("\n Posted \n");
  stopGettingData = false; //resetting to begin a new sampling loop
}
