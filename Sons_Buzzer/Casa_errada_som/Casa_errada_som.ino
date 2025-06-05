int buzzer = 7;
int tempo = 100;
int melody[] = {
  330, 16,
  294, 16,
  277, 16,
  262, 8
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;

void casa_errada_som() {
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
    delay(noteDuration);
    noTone(buzzer);
  }

  tone(buzzer, 740, 200);
  delay(200);
  noTone(buzzer);
}

void setup() { }

void loop() { }
