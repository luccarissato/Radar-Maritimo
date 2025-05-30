#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h> 
#include <Adafruit_NeoPixel.h>   
// --- BTN ---
//Pins dos botões do jogador 1 - B(botão)C/L(coluna/linha)1J(jogador)1
#define BLAJ1 38
#define BLBJ1 39
#define BLCJ1 40
#define BLDJ1 41
#define BLEJ1 42
#define BLFJ1 43

#define BC1J1 44
#define BC2J1 45
#define BC3J1 46
#define BC4J1 47
#define BC5J1 48
#define BC6J1 49

// Pins dos botões do jogador 2 - B(botão)C/L(coluna/linha)1J(jogador)2
#define BLAJ2 26
#define BLBJ2 27
#define BLCJ2 28
#define BLDJ2 29
#define BLEJ2 30
#define BLFJ2 31

#define BC1J2 32
#define BC2J2 33
#define BC3J2 34
#define BC4J2 35
#define BC5J2 36
#define BC6J2 37
// --- LED ---
#define LEDPINJ1 22
#define LEDPINJ2 24
#define NUMPIXELS 36

Adafruit_NeoPixel pixelsJ1 = Adafruit_NeoPixel(NUMPIXELS, LEDPINJ1, NEO_RGB + NEO_KHZ800);
  
Adafruit_NeoPixel pixelsJ2 = Adafruit_NeoPixel(NUMPIXELS, LEDPINJ2, NEO_RGB + NEO_KHZ800);
// --- LCDs ---
LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x26, 16, 2);

const char* string[] = {
  "", "Hora de navegar!", "Todos a bordo?",
  "Esconda navios", "na zona rival", "Posicione o",
  "barco de 3 casas", "Toque uma casa", "amarela inicial!",
  "Toque na segunda", "casa amarela", "Escolha o fim",
  "do barco 3", "barco de 2 casas", "do barco 2", "do barco de 1 casa!",
  "Selecione uma", "casa amarela", "Duas escolhas", "te aguardam:",
  "Clique no barco", "e mude de lugar", "ou aperte duas",
  "vezes para jogar", "Barco cancelado!", "Vamos de novo",
  "Hora de iniciar", "a pontaria!", "Sua vez de jogar", "marujo!",
  "Respire fundo...", "vez do oponente", "Sem sorte...",
  "Agua!", "Acertou o trecho", "do navio inimigo", "Bom tiro, marujo",
  "acertou o barco!", "Acertaram parte", "do seu navio!", "O inimigo achou",
  "o seu barco!", "Jogue de novo!", "Boa jogada!", "Essa casa ta",
  "ocupada, marujo!", "Bom jogo! Vamos", "jogar de novo?", "Vitoria, marujo!",
  "Bom trabalho", "Tente novamente:", "Casa ocupada", "Um clique de",
  "cada vez marujo!"
};

void Texto(LiquidCrystal_I2C &lcd, int a, int b, const char* texto1, int c, int d, const char* texto2) {
  lcd.setCursor(a, b);
  lcd.print(texto1);
  lcd.setCursor(c, d);
  lcd.print(texto2);
}

// --- Sons ---

const int buzzer = 7; // pino do buzzer

// Variáveis globais comuns para melodia
int tempo = 120;
int wholenote = (60000 * 4) / tempo;

// Função para tocar uma melodia
void tocaMelodia(int melody[], int length, int tempoLocal) {
  int wholenoteLocal = (60000 * 4) / tempoLocal;

  for (int i = 0; i < length * 2; i += 2) {
    int divider = melody[i + 1];
    int noteDuration = 0;

    if (divider > 0) {
      noteDuration = wholenoteLocal / divider;
    } else {
      noteDuration = (wholenoteLocal / abs(divider)) * 1.5;
    }

    tone(buzzer, melody[i], noteDuration * 0.9);
    delay(noteDuration + 40);
    noTone(buzzer);
  }
}

// Funções específicas de sons:

void inicio_jogo_som() {
  int melody[] = {
    262, 16,   // C4 - início firme
    330, 16,   // E4 - subida
    392, 16,   // G4 - brilho
    523, 8,    // C5 - auge
    659, 8     // E5 - final épico
  };
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  int tempoLocal = 120;
  tocaMelodia(melody, notes, tempoLocal);
}

void casa_errada_som() {
  int melody[] = {
    330, 16,   // E4
    294, 16,   // D4
    277, 16,   // C#4
    262, 8     // C4
  };
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  int tempoLocal = 100;
  int wholenoteLocal = (60000 * 4) / tempoLocal;

  for (int thisNote = 0; thisNote < notes * 2; thisNote += 2) {
    int divider = melody[thisNote + 1];
    int noteDuration = 0;

    if (divider > 0) {
      noteDuration = wholenoteLocal / divider;
    } else {
      noteDuration = (wholenoteLocal / abs(divider)) * 1.5;
    }

    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }

  tone(buzzer, 740, 200);  // F#5 som final de dúvida
  delay(200);
  noTone(buzzer);
}

void fim_rodada_som() {
  int melody[] = {
    659, 16,   // E5 – começa alto
    523, 16,   // C5 – descida
    392, 8     // G4 – fim firme
  };
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  int tempoLocal = 100;
  tocaMelodia(melody, notes, tempoLocal);
}

void acertou_agua_som() {
  int melody[] = {
    262, 16,  // C4 - base (toque principal)
    349, 32,  // F4 - toque rápido agudo, como splash
    330, 16   // E4 - ressonância rápida
  };
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  int tempoLocal = 120;

  // Queda prolongada com descida suave de frequência
  for (int freq = 2500; freq >= 400; freq -= 30) {
    tone(buzzer, freq, 25);
    delay(30);
  }

  delay(500);

  // Melodia curta final
  tocaMelodia(melody, notes, tempoLocal);
}

void acertou_barco_som() {
  // Queda prolongada com descida suave de frequência
  for (int freq = 2500; freq >= 400; freq -= 30) {
    tone(buzzer, freq, 25);
    delay(30);
  }

  delay(500);

  // Subida rápida e agressiva de frequência
  for (int freq = 400; freq <= 800; freq += 80) {
    tone(buzzer, freq, 40);
    delay(40);
  }

  // Explosão repentina: tom grave e longo
  tone(buzzer, 70, 700);
  delay(700);

  // Eco grave e pesado contínuo
  int ecoFreqs[] = {90, 60, 45, 30};
  int ecoDur[] = {300, 250, 200, 150};
  for (int i = 0; i < 4; i++) {
    tone(buzzer, ecoFreqs[i], ecoDur[i]);
    delay(ecoDur[i]);
  }

  noTone(buzzer);
}

void fim_de_jogo_som() {
  // Sequência de tons ascendentes curtos, seguido de um tom longo final
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

void setup() {
  pinMode(buzzer, OUTPUT);
  lcd1.init();
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();
  
  inicio_jogo_som();
  Texto(lcd1, 0, 0, string[1], 1, 1, string[2]); //Hora de navegar! Todos a bordo?
  Texto(lcd2, 0, 0, string[1], 1, 1, string[2]); 
}

void loop() {
}
