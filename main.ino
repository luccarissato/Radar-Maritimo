#include <Adafruit_NeoPixel.h>

// Coisas a testar:
// Se o código das diagonais está funcionando e se suas cores estão mostrando.

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

// Pins dos botões do jogador 2 - B(botão)C/L(coluna/linha)1J(jogador)2
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

// Cores do jogador 1
byte azulClaro [60];
byte verdeJ1 [12];
byte laranjaJ1 [12];

// Cores do jogador 2
byte azulEscuro [60];
byte verdeJ2 [12];
byte laranjaJ2 [12];

// Matriz de tradução das coordenadas para o LED específico
byte matrizLeds[6][6] = {
  {  0,  1,  2,  3,  4,  5 },
  {  6,  7,  8,  9, 10, 11 },
  { 12, 13, 14, 15, 16, 17 },
  { 18, 19, 20, 21, 22, 23 },
  { 24, 25, 26, 27, 28, 29 },
  { 30, 31, 32, 33, 34, 35 }
};

// Posição dos barcos do jogador 1
bool barcosJ1 [6][6] = {};
// Posição dos barcos do jogador 2
bool barcosJ2 [6][6] = {};

// Número de barcos do jogador 1
byte contJ1 = 6;
// Número de barcos do jogador 2
byte contJ2 = 6;

// Coordenadas antes de serem assinaladas uma cor
byte coordLinha;
byte coordColuna;
byte apagar;
int diferenca;

// Variáveis de auxílio no acionamento das luzes na posição de barcos
byte posicaoVelha;
byte posicaoAtual;

byte contAcertosJ1;
byte contErrosJ1;

byte contAcertosJ2;
byte contErrosJ2;

uint32_t corAzulEscuro = pixelsJ1.Color(0, 0, 139);
uint32_t corAzulClaro = pixelsJ2.Color(173, 216, 230);
uint32_t corVerde = Adafruit_NeoPixel::Color(0, 255, 0);
uint32_t corLaranja = Adafruit_NeoPixel::Color(255, 165, 0);
uint32_t corAmarelo = Adafruit_NeoPixel::Color(255, 255, 0);
uint32_t apagado = Adafruit_NeoPixel::Color(0, 0, 0);

bool buttonState;

void setup() {
  pixelsJ1.begin();
  pixelsJ2.begin();

// Iniciado os botões do jogador 1
  pinMode(BL1J1, INPUT);
  pinMode(BL2J1, INPUT);
  pinMode(BL3J1, INPUT);
  pinMode(BL4J1, INPUT);
  pinMode(BL5J1, INPUT);
  pinMode(BL6J1, INPUT);
  pinMode(BC1J1, INPUT);
  pinMode(BC2J1, INPUT);
  pinMode(BC3J1, INPUT);
  pinMode(BC4J1, INPUT);
  pinMode(BC5J1, INPUT);
  pinMode(BC6J1, INPUT);

// Iniciando os botões do jogador 2
  pinMode(BL1J2, INPUT);
  pinMode(BL2J2, INPUT);
  pinMode(BL3J2, INPUT);
  pinMode(BL4J2, INPUT);
  pinMode(BL5J2, INPUT);
  pinMode(BL6J2, INPUT);
  pinMode(BC1J2, INPUT);
  pinMode(BC2J2, INPUT);
  pinMode(BC3J2, INPUT);
  pinMode(BC4J2, INPUT);
  pinMode(BC5J2, INPUT);
  pinMode(BC6J2, INPUT);

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

// Traduz as coordenadas na posição do LED na fita
int posicao() {
  int linha = coordLinha;
  int coluna = coordColuna;
  return matrizLeds[linha][coluna];
}

// Detecta qual botão das linhas está sendo pressionado, detectará as entradas do jogador cujo numero for atribuído a função
int detectarCoordLinha(int jogador) {
  int inicio, fim;
  uint32_t corPiscar;

  if (jogador == 1) {
    inicio = 3;
    fim = 9;
    corPiscar = azulClaro;
  } else if (jogador == 2) {
    inicio = 15;
    fim = 21;
    corPiscar = azulEscuro;
  } else {
    return -1;  // jogador inválido
  }

  while (true) {
    // Verifica se algum botão foi pressionado
    for (int c = inicio; c < fim; c++) {
      int buttonState = digitalRead(c);
      if (buttonState == HIGH) {
        coordLinha = c - inicio;
        return coordLinha;
      }
    }

    // Padrão piscante: linha por linha com cor específica
    for (int c = 0; c < 36; c += 6) {
      pixelsJ1.setPixelColor(0 + c, corPiscar); 
      pixelsJ1.setPixelColor(1 + c, corPiscar); 
      pixelsJ1.setPixelColor(2 + c, corPiscar); 
      pixelsJ1.setPixelColor(3 + c, corPiscar); 
      pixelsJ1.setPixelColor(4 + c, corPiscar); 
      pixelsJ1.setPixelColor(5 + c, corPiscar);
    }
    pixelsJ1.show();
    delay(500);

    for (int c = 0; c < 36; c += 6) {
      pixelsJ1.setPixelColor(0 + c, apagado); 
      pixelsJ1.setPixelColor(1 + c, apagado); 
      pixelsJ1.setPixelColor(2 + c, apagado); 
      pixelsJ1.setPixelColor(3 + c, apagado); 
      pixelsJ1.setPixelColor(4 + c, apagado); 
      pixelsJ1.setPixelColor(5 + c, apagado);
    }
    pixelsJ1.show();
    delay(500);
  }
}


// Detecta qual botão das colunas está sendo pressionado, detectará as entradas do jogador cujo numero for atribuído a função, além de acender os LEDs no padrão auxiliador
int detectarCoordColuna(int jogador) {
  int inicio, fim;
  uint32_t corPiscar;

  if (jogador == 1) {
    inicio = 9;
    fim = 15;  // pinos 9 a 14
    corPiscar = azulClaro;
  } else if (jogador == 2) {
    inicio = 21;
    fim = 27;  // pinos 21 a 26
    corPiscar = azulEscuro;
  } else {
    return -1;  // jogador inválido
  }

  while (true) {
    // Verifica se algum botão foi pressionado
    for (int c = inicio; c < fim; c++) {
      int buttonState = digitalRead(c);
      if (buttonState == HIGH) {
        coordColuna = c - inicio;
        return coordColuna;
      }
    }

    // Padrão de piscar LEDs (piscando uma coluna) com cor variável
    for (int c = 0; c < 5; c++) {
      pixelsJ1.setPixelColor(0 + c, corPiscar); 
      pixelsJ1.setPixelColor(6 + c, corPiscar); 
      pixelsJ1.setPixelColor(12 + c, corPiscar); 
      pixelsJ1.setPixelColor(18 + c, corPiscar); 
      pixelsJ1.setPixelColor(24 + c, corPiscar); 
      pixelsJ1.setPixelColor(30 + c, corPiscar);
    }
    pixelsJ1.show();
    delay(500);

    for (int c = 0; c < 5; c++) {
      pixelsJ1.setPixelColor(0 + c, apagado); 
      pixelsJ1.setPixelColor(6 + c, apagado); 
      pixelsJ1.setPixelColor(12 + c, apagado); 
      pixelsJ1.setPixelColor(18 + c, apagado); 
      pixelsJ1.setPixelColor(24 + c, apagado); 
      pixelsJ1.setPixelColor(30 + c, apagado);
    }
    pixelsJ1.show();
    delay(500);
  }
}


void segundaCasaAcender(int atual, int velha) {
  diferenca = atual - velha;
  apagar = atual;

  if (diferenca == 1) {
    pixelsJ1.setPixelColor(atual + 1, corAmarelo);
    pixelsJ1.setPixelColor(atual - 2, corAmarelo);

  } else if (diferenca == -1) {
    pixelsJ1.setPixelColor(atual - 1, corAmarelo);
    pixelsJ1.setPixelColor(atual + 2, corAmarelo);

  } else if (diferenca == -6) {
    pixelsJ1.setPixelColor(atual - 6, corAmarelo);
    pixelsJ1.setPixelColor(atual + 12, corAmarelo);

  } else if (diferenca == 6) {
    pixelsJ1.setPixelColor(atual - 12, corAmarelo);
    pixelsJ1.setPixelColor(atual + 6, corAmarelo);
  }
}

void segundaCasaApagar(int diferenca) {

  if (diferenca == 1) {
    pixelsJ1.setPixelColor(apagar + 1, apagado);
    pixelsJ1.setPixelColor(apagar - 2, apagado);

  } else if (diferenca == -1) {
    pixelsJ1.setPixelColor(apagar - 1, apagado);
    pixelsJ1.setPixelColor(apagar + 2, apagado);

  } else if (diferenca == -6) {
    pixelsJ1.setPixelColor(apagar - 6, apagado);
    pixelsJ1.setPixelColor(apagar + 12, apagado);

  } else if (diferenca == 6) {
    pixelsJ1.setPixelColor(apagar - 12, apagado);
    pixelsJ1.setPixelColor(apagar + 6, apagado);
  }
}

void loop() {
  // Animação diagonal para J1
  for (int i = 0; i < 6; i++) {
    lightDiagonal(pixelsJ1, i, corAzulEscuro);
    clearPixels(pixelsJ1);
    delay(100);
  }
  for (int i = 4; i >= 0; i--) {
    lightDiagonal(pixelsJ1, i, corLaranja);
    clearPixels(pixelsJ1);
    delay(100);
  }

  // Animação diagonal para J2
  for (int i = 0; i < 6; i++) {
    lightDiagonal(pixelsJ2, i, corAzulClaro);
    clearPixels(pixelsJ2);
    delay(100);
  }
  for (int i = 4; i >= 0; i--) {
    lightDiagonal(pixelsJ2, i, corLaranja);
    clearPixels(pixelsJ2);
    delay(100);
  }

  delay(1000);

  //Fase de seleção de barcos do J1
  for (int z = 0; z < 6; z++){ // São selecionados 6 casas, por isso repete 6 vezes
    if (z == 0) { // O padrão de cores muda de casa pra casa, então deve-se checar qual está sendo selecionada
      // Novas coordenadas
      detectarCoordLinha(1);
      detectarCoordColuna(1);

      // Endereçando o barco novo
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[0] = coordLinha;
      verdeJ1[1] = coordColuna;
      posicaoAtual = posicao();

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);
      pixelsJ1.setPixelColor(posicaoAtual-1, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual+1, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual-6, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual+6, corAmarelo);
      pixelsJ1.show();

  } else if (z == 1) {
      posicaoVelha = posicaoAtual;
      // Novas coordenadas
      detectarCoordLinha(1);
      detectarCoordColuna(1);

      // Apagando as casas amarelas antigas
      pixelsJ1.setPixelColor(posicaoVelha-1, apagado);
      pixelsJ1.setPixelColor(posicaoVelha+1, apagado);
      pixelsJ1.setPixelColor(posicaoVelha-6, apagado);
      pixelsJ1.setPixelColor(posicaoVelha+6, apagado);
      pixelsJ1.show();

      // Endereçando o barco novo
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[2] = coordLinha;
      verdeJ1[3] = coordColuna;
      posicaoAtual = posicao();

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);
      segundaCasaAcender(posicaoAtual, posicaoVelha);
      pixelsJ1.show();


  } else if (z == 2) {
      // Novas coordenadas
      detectarCoordLinha(1);
      detectarCoordColuna(1);

      // Apagando as casas amarelas antigas
      segundaCasaApagar(diferenca);

      //Endereçando o novo barco
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[4] = coordLinha;
      verdeJ1[5] = coordColuna;
      posicaoAtual = posicao();

      //Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);
    
  } else if (z == 3) {
      // Novas coordenadas
      detectarCoordLinha(1);
      detectarCoordColuna(1);

      // Endereçando o barco novo
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[6] = coordLinha;
      verdeJ1[7] = coordColuna;
      posicaoAtual = posicao();

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);
      pixelsJ1.setPixelColor(posicaoAtual-1, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual+1, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual-6, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual+6, corAmarelo);
      pixelsJ1.show();
    
  } else if (z == 4) {
      posicaoVelha = posicaoAtual;
      // Novas coordenadas
      detectarCoordLinha(1);
      detectarCoordColuna(1);

      // Apagando as casas amarelas antigas
      pixelsJ1.setPixelColor(posicaoVelha-1, apagado);
      pixelsJ1.setPixelColor(posicaoVelha+1, apagado);
      pixelsJ1.setPixelColor(posicaoVelha-6, apagado);
      pixelsJ1.setPixelColor(posicaoVelha+6, apagado);
      pixelsJ1.show();

      // Endereçando o barco novo
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[8] = coordLinha;
      verdeJ1[9] = coordColuna;
      posicaoAtual = posicao();

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);
      pixelsJ1.show();

    
  } else if (z == 5) {
      // Novas coordenadas
      detectarCoordLinha(1);
      detectarCoordColuna(1);

      //Endereçando o novo barco
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[10] = coordLinha;
      verdeJ1[11] = coordColuna;
      posicaoAtual = posicao();

      //Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);
  }

  //Fase de seleção de barcos do J2
  for (int z = 0; z < 6; z++){ // São selecionados 6 casas, por isso repete 6 vezes
    if (z == 0) { // O padrão de cores muda de casa pra casa, então deve-se checar qual está sendo selecionada
      // Novas coordenadas
      detectarCoordLinha(2);
      detectarCoordColuna(2);

      // Endereçando o barco novo
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[0] = coordLinha;
      verdeJ2[1] = coordColuna;
      posicaoAtual = posicao();

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);
      pixelsJ2.setPixelColor(posicaoAtual-1, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual+1, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual-6, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual+6, corAmarelo);
      pixelsJ2.show();

  } else if (z == 1) {
      posicaoVelha = posicaoAtual;
      // Novas coordenadas
      detectarCoordLinha(2);
      detectarCoordColuna(2);

      // Apagando as casas amarelas antigas
      pixelsJ2.setPixelColor(posicaoVelha-1, apagado);
      pixelsJ2.setPixelColor(posicaoVelha+1, apagado);
      pixelsJ2.setPixelColor(posicaoVelha-6, apagado);
      pixelsJ2.setPixelColor(posicaoVelha+6, apagado);
      pixelsJ2.show();

      // Endereçando o barco novo
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[2] = coordLinha;
      verdeJ2[3] = coordColuna;
      posicaoAtual = posicao();

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);
      segundaCasaAcender(posicaoAtual, posicaoVelha);
      pixelsJ2.show();


  } else if (z == 2) {
      // Novas coordenadas
      detectarCoordLinha(2);
      detectarCoordColuna(2);

      // Apagando as casas amarelas antigas
      segundaCasaApagar(diferenca);

      //Endereçando o novo barco
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[4] = coordLinha;
      verdeJ2[5] = coordColuna;
      posicaoAtual = posicao();

      //Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);
    
  } else if (z == 3) {
      // Novas coordenadas
      detectarCoordLinha(2);
      detectarCoordColuna(2);

      // Endereçando o barco novo
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[6] = coordLinha;
      verdeJ2[7] = coordColuna;
      posicaoAtual = posicao();

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);
      pixelsJ2.setPixelColor(posicaoAtual-1, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual+1, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual-6, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual+6, corAmarelo);
      pixelsJ2.show();
    
  } else if (z == 4) {
      posicaoVelha = posicaoAtual;
      // Novas coordenadas
      detectarCoordLinha(2);
      detectarCoordColuna(2);

      // Apagando as casas amarelas antigas
      pixelsJ2.setPixelColor(posicaoVelha-1, apagado);
      pixelsJ2.setPixelColor(posicaoVelha+1, apagado);
      pixelsJ2.setPixelColor(posicaoVelha-6, apagado);
      pixelsJ2.setPixelColor(posicaoVelha+6, apagado);
      pixelsJ2.show();

      // Endereçando o barco novo
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[8] = coordLinha;
      verdeJ2[9] = coordColuna;
      posicaoAtual = posicao();

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);
      pixelsJ2.show();

    
  } else if (z == 5) {
      // Novas coordenadas
      detectarCoordLinha(2);
      detectarCoordColuna(2);

      //Endereçando o novo barco
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[10] = coordLinha;
      verdeJ2[11] = coordColuna;
      posicaoAtual = posicao();

      //Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);
  }
}
}
}
