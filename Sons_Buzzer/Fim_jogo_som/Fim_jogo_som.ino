const int buzzer = 7;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
}

void fim_de_jogo_som() {

  tone(buzzer, 500, 150);
  delay(170);
  tone(buzzer, 600, 150);
  delay(170);
  tone(buzzer, 750, 200);
  delay(220);
  tone(buzzer, 900, 400); 
  delay(420);

  noTone(buzzer);
}
