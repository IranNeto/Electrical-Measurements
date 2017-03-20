void light(String state) {
  int intensidade = state.toInt(); //caso chegue uma string usar .toInt()
  if (intensidade == 0) {
    Serial.println("DESLIGOU");
    digitalWrite(luz, LOW);
    analogWrite(luz, intensidade);
  }
  else if (intensidade > 0 && intensidade < 256) {
    Serial.println("LIGOUUUUU");
    analogWrite(luz, intensidade);
    Serial.println(intensidade);
  }
  else {
    Serial.println("TAH UMA PORRA");
  }
}
