const int buzzer = 7;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
  playBombOnShipLonger();
  delay(6000); // intervalo entre eventos
}

void playBombOnShipLonger() {

  // 3. Explosão mais intensa e profunda
  tone(buzzer, 200, 200);
  delay(200);
  tone(buzzer, 130, 180);
  delay(180);
  tone(buzzer, 100, 150);
  delay(200);

  // 4. Eco metálico mais longo e marcado
  tone(buzzer, 400, 80);
  delay(100);
  tone(buzzer, 300, 70);
  delay(90);
  tone(buzzer, 250, 60);
  delay(80);
  tone(buzzer, 200, 50);
  delay(70);

  noTone(buzzer);
}