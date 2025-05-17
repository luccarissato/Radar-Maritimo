#include <Adafruit_NeoPixel.h>

// Pin do led do jogador 1
#define LEDPINJ1 1
// Pin do led do jogador 2
#define LEDPINJ2 2

#define NUMPIXELS 36

// Declarando os LEDs do jogador 1
Adafruit_NeoPixel pixelsJ1 = Adafruit_NeoPixel(NUMPIXELS, LEDPINJ1, NEO_RGB + NEO_KHZ800);
// Declarando os LEDs do jogador 2 
Adafruit_NeoPixel pixelsJ2 = Adafruit_NeoPixel(NUMPIXELS, LEDPINJ2, NEO_RGB + NEO_KHZ800);

//Pins dos botões do jogador 1 - B(botão)C/L(coluna/linha)1J(jogador)1
#define BL1J1 3 
#define BL2J1 4
#define BL3J1 5
#define BL4J1 6
#define BL5J1 7
#define BL6J1 8
#define BC1J1 9
#define BC2J1 10
#define BC3J1 11
#define BC4J1 12
#define BC5J1 13
#define BC6J1 14

//Pins dos botões do jogador 2 - B(botão)C/L(coluna/linha)1J(jogador)2
#define BL1J2 15 
#define BL2J2 16
#define BL3J2 17
#define BL4J2 18
#define BL5J2 19
#define BL6J2 20
#define BC1J2 21
#define BC2J2 22
#define BC3J2 23
#define BC4J2 24
#define BC5J2 25
#define BC6J2 26

//Cores do jogador 1
byte azulClaro [60];
byte verdeJ1 [12];
byte laranjaJ1 [12];

//Cores do jogador 2
byte azulEscuro [60];
byte verdeJ2 [12];
byte laranjaJ2 [12];

//Matriz de tradução das coordenadas para o LED específico
byte matrizLeds[6][6] = {
  {  0,  1,  2,  3,  4,  5 },
  {  6,  7,  8,  9, 10, 11 },
  { 12, 13, 14, 15, 16, 17 },
  { 18, 19, 20, 21, 22, 23 },
  { 24, 25, 26, 27, 28, 29 },
  { 30, 31, 32, 33, 34, 35 }
};

//Posição dos barcos do jogador 1
bool barcosJ1 [6][6] = {};
//Posição dos barcos do jogador 2
bool barcosJ2 [6][6] = {};

//Número de barcos do jogador 1
byte contJ1 = 6;
//Número de barcos do jogador 2
byte contJ2 = 6;

byte coordLinha;
byte coordColuna;

byte contAcertosJ1;
byte contErrosJ1;

byte contAcertosJ2;
byte contErrosJ2;

uint32_t corAzulEscuro = pixelsJ1.Color(0, 0, 139);
uint32_t corAzulClaro = pixelsJ2.Color(173, 216, 230);
uint32_t corVerde = Adafruit_NeoPixel::Color(0, 255, 0);
uint32_t corLaranja = Adafruit_NeoPixel::Color(255, 165, 0);
uint32_t corAmarelo = Adafruit_NeoPixel::Color(255, 255, 0);

void setup() {
  pixelsJ1.begin();
  pixelsJ2.begin();

}

// Apaga todos os LEDs do objeto recebido
void clearPixels(Adafruit_NeoPixel &strip) {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

// Acende um LED da diagonal principal
void lightDiagonal(Adafruit_NeoPixel &strip, int pos, uint32_t color) {
  int index = pos * 6 + pos;
  if (index < NUMPIXELS) {
    strip.setPixelColor(index, color);
    strip.show();
  }
}

void loop() {
  // Animação para J1
  for (int i = 0; i < 6; i++) {
    clearPixels(pixelsJ1);
    lightDiagonal(pixelsJ1, i, corAzulEscuro);
    delay(100);
  }
  for (int i = 4; i >= 0; i--) {
    clearPixels(pixelsJ1);
    lightDiagonal(pixelsJ1, i, corLaranja);
    delay(100);
  }

  // Animação para J2
  for (int i = 0; i < 6; i++) {
    clearPixels(pixelsJ2);
    lightDiagonal(pixelsJ2, i, corAzulClaro);
    delay(100);
  }
  for (int i = 4; i >= 0; i--) {
    clearPixels(pixelsJ2);
    lightDiagonal(pixelsJ2, i, corLaranja);
    delay(100);
  }
}