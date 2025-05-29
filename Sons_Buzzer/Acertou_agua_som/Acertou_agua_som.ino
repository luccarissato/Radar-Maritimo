const int buzzer = 7;
int tempo = 120;

// Melodia final (C4, F4, E4)
int melody[] = {
  262, 16,  // C4 - base (toque principal)
  349, 32,  // F4 - toque rápido agudo, como splash
  330, 16   // E4 - ressonância rápida
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
}

void acertou_agua_som() {
  // 1. Queda prolongada com descida suave de frequência
  for (int freq = 2500; freq >= 400; freq -= 30) {
    tone(buzzer, freq, 25);
    delay(30);
  }

  // 2. Pausa tensa antes da explosão
  delay(500);

  // 3. Melodia curta no final (como toque eletrônico ou ressonância)
  for (int i = 0; i < notes * 2; i += 2) {
    int divider = melody[i + 1];
    int noteDuration = (divider > 0) ? (wholenote / divider) : (wholenote / abs(divider)) * 1.5;

    tone(buzzer, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }

  noTone(buzzer);
}
