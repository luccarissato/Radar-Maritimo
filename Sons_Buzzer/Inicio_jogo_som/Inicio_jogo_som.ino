int buzzer = 7;
int tempo = 120;

int melody[] = {
  262, 16,
  330, 16,
  392, 16,
  523, 8,
  659, 8
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;

void inicio_jogo_som() {
  int divider = 0, noteDuration = 0;

  for (int thisNote = 0; thisNote < notes * 2; thisNote += 2) {
    divider = melody[thisNote + 1];

    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration + 40);
    noTone(buzzer);
  }
}

void setup() {
}

void loop() {
}
