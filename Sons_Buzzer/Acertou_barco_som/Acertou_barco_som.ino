const int buzzer = 7;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
  acertoubarco();
  delay(6000); // intervalo entre eventos
}

void acertou_barco_som() {
  // 1. Queda prolongada com descida suave de frequência
  for (int freq = 2500; freq >= 400; freq -= 30) {
    tone(buzzer, freq, 25);
    delay(30);
  }

  // 2. Pausa tensa antes da explosão
  delay(500);

// Clímax: subida rápida e agressiva de frequência (sem ritmo musical)
for (int freq = 400; freq <= 800; freq += 80) {
  tone(buzzer, freq, 40);
  delay(40);
}

// Explosão repentina: tom grave, forte e longo (BOOM)
tone(buzzer, 70, 700);
delay(700);

// Eco grave e pesado, sem pausas, som contínuo
int ecoFreqs[] = {90, 60, 45, 30};
int ecoDur[] = {300, 250, 200, 150};

for (int i = 0; i < 4; i++) {
  tone(buzzer, ecoFreqs[i], ecoDur[i]);
  delay(ecoDur[i]); // sem pausa extra para manter continuidade
}

noTone(buzzer);

}
