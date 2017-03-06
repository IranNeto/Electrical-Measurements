// Daremos um nome ao pino que ficara o LED:
int led = LED_BUILTIN;
int pin = 11;
bool state = true;
void interrupcao(){
  state = !state;
  digitalWrite(led, state); // Liga o LED (HIGH = nível lógico alto)
}
 
// Esta função "setup" roda uma vez quando a placa e ligada ou resetada
void setup() {
  pinMode(pin, INPUT);
  pinMode(led, OUTPUT); // Configura o pino do led (digital) como saída
  attachInterrupt(0,interrupcao,CHANGE); //Configurando a interrupção
}
 
// Função que se repete infinitamente quando a placa é ligada
void loop() {
}
