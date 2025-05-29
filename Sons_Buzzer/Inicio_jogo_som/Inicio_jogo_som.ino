int buzzer = 7;
int tempo = 120;

int melody[] = {
  262, 16,   // C4 - início firme
  330, 16,   // E4 - subida
  392, 16,   // G4 - brilho
  523, 8,    // C5 - auge
  659, 8     // E5 - final épico
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
    delay(noteDuration + 40);  // ⏱️ Pausa extra entre as notas
    noTone(buzzer);
  }
}

void setup() {
}

void loop() {
}
