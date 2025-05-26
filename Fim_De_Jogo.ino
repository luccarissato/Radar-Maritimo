const int buzzer = 7;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
  playGameOverSound();
  delay(5000); // aguarda antes de repetir
}

void playGameOverSound() {
  // Sequência de tons ascendentes curtos, seguido de um tom longo final
  tone(buzzer, 500, 150);
  delay(170);
  tone(buzzer, 600, 150);
  delay(170);
  tone(buzzer, 750, 200);
  delay(220);
  tone(buzzer, 900, 400); // tom final mais longo
  delay(420);

  noTone(buzzer);
}
