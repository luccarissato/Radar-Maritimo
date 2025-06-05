const int buzzer = 7;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
  acertoubarco();
  delay(6000); 
}

void acertou_barco_som() {
  
  for (int freq = 2500; freq >= 400; freq -= 30) {
    tone(buzzer, freq, 25);
    delay(30);
  }


  delay(500);


for (int freq = 400; freq <= 800; freq += 80) {
  tone(buzzer, freq, 40);
  delay(40);
}


tone(buzzer, 70, 700);
delay(700);


int ecoFreqs[] = {90, 60, 45, 30};
int ecoDur[] = {300, 250, 200, 150};

for (int i = 0; i < 4; i++) {
  tone(buzzer, ecoFreqs[i], ecoDur[i]);
  delay(ecoDur[i]); 
}

noTone(buzzer);

}
