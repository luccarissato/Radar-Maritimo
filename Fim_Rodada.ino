int buzzer = 7;
int tempo = 100;

int melody[] = {
  659, 16,   // E5 – começa alto
  523, 16,   // C5 – descida
  392, 8     // G4 – fim firme
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void setup() {
  for (int thisNote = 0; thisNote < notes * 2; thisNote += 2) {
    divider = melody[thisNote + 1];
    
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration + 40);  // Pausa adicional entre as notas
    noTone(buzzer);
  }
}

void loop() {
  // não repete
}
