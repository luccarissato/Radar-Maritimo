#include <Adafruit_NeoPixel.h>

// Pin do led do jogador 1
#define LEDPINJ1 A1
// Pin do led do jogador 2
#define LEDPINJ2 A2

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
int azulClaro[60];
int verdeJ1[12];
int laranjaJ1[12];

// Cores do jogador 2
int azulEscuro[60];
int verdeJ2[12];
int laranjaJ2[12];

// Matriz de tradução das coordenadas para o LED específico
int matrizLeds[6][6] = {
  { 0, 6, 12, 18, 24, 30 },
  { 1, 7, 13, 19, 25, 31 },
  { 2, 8, 14, 20, 26, 32 },
  { 3, 9, 15, 21, 27, 33 },
  { 4, 10, 16, 22, 28, 34 },
  { 5, 11, 17, 23, 29, 35 }
};

// Posição dos barcos do jogador 1
bool barcosJ1[6][6] = {};
// Posição dos barcos do jogador 2
bool barcosJ2[6][6] = {};

// Número de barcos do jogador 1
int barcosRestantesJ1 = 6;
// Número de barcos do jogador 2
int barcosRestantesJ2 = 6;

// Coordenadas antes de serem assinaladas uma cor
int coordLinha;
int coordColuna;

// Variáveis de auxílio no acionamento das luzes na posição de barcos
int posicaoVelha;
int posicaoAtual;
int apagar;
int diferenca;

int contAcertosJ1;
int contErrosJ1;

int contAcertosJ2;
int contErrosJ2;

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
  pinMode(BL1J1, INPUT_PULLUP);
  pinMode(BL2J1, INPUT_PULLUP);
  pinMode(BL3J1, INPUT_PULLUP);
  pinMode(BL4J1, INPUT_PULLUP);
  pinMode(BL5J1, INPUT_PULLUP);
  pinMode(BL6J1, INPUT_PULLUP);
  pinMode(BC1J1, INPUT_PULLUP);
  pinMode(BC2J1, INPUT_PULLUP);
  pinMode(BC3J1, INPUT_PULLUP);
  pinMode(BC4J1, INPUT_PULLUP);
  pinMode(BC5J1, INPUT_PULLUP);
  pinMode(BC6J1, INPUT_PULLUP);

  // Iniciando os botões do jogador 2
  pinMode(BL1J2, INPUT_PULLUP);
  pinMode(BL2J2, INPUT_PULLUP);
  pinMode(BL3J2, INPUT_PULLUP);
  pinMode(BL4J2, INPUT_PULLUP);
  pinMode(BL5J2, INPUT_PULLUP);
  pinMode(BL6J2, INPUT_PULLUP);
  pinMode(BC1J2, INPUT_PULLUP);
  pinMode(BC2J2, INPUT_PULLUP);
  pinMode(BC3J2, INPUT_PULLUP);
  pinMode(BC4J2, INPUT_PULLUP);
  pinMode(BC5J2, INPUT_PULLUP);
  pinMode(BC6J2, INPUT_PULLUP);
}

// Apaga todos os LEDs do objeto recebido
void clearPixels(Adafruit_NeoPixel& strip) {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

// Acende um LED da diagonal principal
void lightDiagonal(Adafruit_NeoPixel& strip, int pos, uint32_t color) {
  int index = pos * 6 + pos;
  if (index < NUMPIXELS) {
    strip.setPixelColor(index, color);
    strip.show();
  }
}

// Traduz as coordenadas na posição do LED na fita
int posicao(int linha, int coluna) {
  return matrizLeds[linha][coluna];
}

// Detecta qual botão das colunas está sendo pressionado, detectará as entradas do jogador cujo numero for atribuído a função, além de acender os LEDs no padrão auxiliador
int detectarCoordLinha(int jogador, bool piscar) {
  int inicio, fim;
  uint32_t corPiscar;
  Adafruit_NeoPixel* pixels;
  bool (*barcos)[6]; // Ponteiro para a matriz do jogador

  if (jogador == 1) {
    inicio = 3;
    fim = 9;
    corPiscar = corAzulClaro;
    pixels = &pixelsJ1;
    barcos = barcosJ1;
  } else if (jogador == 2) {
    inicio = 15;
    fim = 21;
    corPiscar = corAzulEscuro;
    pixels = &pixelsJ2;
    barcos = barcosJ2;
  } else {
    return -1;
  }

  while (true) {
    // Verifica se algum botão foi pressionado com debounce
    for (int c = inicio; c < fim; c++) {
      if (digitalRead(c) == LOW) {
        delay(20); // debounce simples
        if (digitalRead(c) == LOW) {
          while (digitalRead(c) == LOW); // espera soltar
          return c - inicio;
        }
      }
    }

    // Pisca as linhas se solicitado
    if (piscar) {
      for (int linha = 0; linha < 6; linha++) {
        for (int coluna = 0; coluna < 6; coluna++) {
          if (!barcos[linha][coluna]) {
            int pixelIndex = posicao(linha, coluna);
            pixels->setPixelColor(pixelIndex, corPiscar);
          }
        }
        pixels->show();
        delay(500);

        for (int coluna = 0; coluna < 6; coluna++) {
          if (!barcos[linha][coluna]) {
            int pixelIndex = posicao(linha, coluna);
            pixels->setPixelColor(pixelIndex, 0);
          }
        }
        pixels->show();
        delay(500);
      }
    } else {
      delay(10);
    }
  }
}


// Detecta qual botão das colunas está sendo pressionado, detectará as entradas do jogador cujo numero for atribuído a função, além de acender o LED no padrão auxiliador
int detectarCoordColuna(int jogador, bool piscar) {
  int inicio, fim;
  uint32_t corPiscar;
  Adafruit_NeoPixel* pixels;
  bool (*barcos)[6]; // Ponteiro para a matriz do jogador

  if (jogador == 1) {
    inicio = 9;
    fim = 15;
    corPiscar = corAzulClaro;
    pixels = &pixelsJ1;
    barcos = barcosJ1;
  } else if (jogador == 2) {
    inicio = 21;
    fim = 27;
    corPiscar = corAzulEscuro;
    pixels = &pixelsJ2;
    barcos = barcosJ2;
  } else {
    return -1;
  }

  while (true) {
    // Verifica se algum botão foi pressionado com debounce
    for (int c = inicio; c < fim; c++) {
      if (digitalRead(c) == LOW) {
        delay(20); // debounce simples
        if (digitalRead(c) == LOW) {
          while (digitalRead(c) == LOW); // espera soltar
          return c - inicio;
        }
      }
    }

    // Pisca as colunas se solicitado
    if (piscar) {
      for (int coluna = 0; coluna < 6; coluna++) {
        for (int linha = 0; linha < 6; linha++) {
          if (!barcos[linha][coluna]) {
            int pixelIndex = posicao(linha, coluna);
            pixels->setPixelColor(pixelIndex, corPiscar);
          }
        }
        pixels->show();
        delay(500);

        for (int linha = 0; linha < 6; linha++) {
          if (!barcos[linha][coluna]) {
            int pixelIndex = posicao(linha, coluna);
            pixels->setPixelColor(pixelIndex, 0);
          }
        }
        pixels->show();
        delay(500);
      }
    } else {
      delay(10);
    }
  }
}

// Padrões que podem acender dependendo de qual casa for selecionado na segunda posição do barco de 3 casas
void segundaCasaAcender(int atual, int velha, int jogador) {
  diferenca = atual - velha;
  apagar = atual;
  Adafruit_NeoPixel* pixels;

  // Define o objeto de LED com base no jogador
  if (jogador == 1) {
    pixels = &pixelsJ1;
  } else if (jogador == 2) {
    pixels = &pixelsJ2;
  } else {
    return;  // jogador inválido
  }

  // Acende LEDs nas posições adjacentes com base na diferença
  if (diferenca == 6) {
    pixels->setPixelColor(atual + 6, corAmarelo);
    pixels->setPixelColor(atual - 12, corAmarelo);

  } else if (diferenca == -6) {
    pixels->setPixelColor(atual - 6, corAmarelo);
    pixels->setPixelColor(atual + 12, corAmarelo);

  } else if (diferenca == -1) {
    pixels->setPixelColor(atual - 1, corAmarelo);
    pixels->setPixelColor(atual + 2, corAmarelo);

  } else if (diferenca == 1) {
    pixels->setPixelColor(atual - 2, corAmarelo);
    pixels->setPixelColor(atual + 1, corAmarelo);
  }
  pixels->show();
}

// Apagar a função segundaCasaAcender
void segundaCasaApagar(int diferenca, int jogador) {
  Adafruit_NeoPixel* pixels;

  // Seleciona o objeto de LED conforme o jogador
  if (jogador == 1) {
    pixels = &pixelsJ1;
  } else if (jogador == 2) {
    pixels = &pixelsJ2;
  } else {
    return;  // jogador inválido
  }

  // Apaga os LEDs nas posições correspondentes
  if (diferenca == 6) {
    pixels->setPixelColor(apagar + 6, apagado);
    pixels->setPixelColor(apagar - 12, apagado);

  } else if (diferenca == -6) {
    pixels->setPixelColor(apagar - 6, apagado);
    pixels->setPixelColor(apagar + 12, apagado);

  } else if (diferenca == -1) {
    pixels->setPixelColor(apagar - 1, apagado);
    pixels->setPixelColor(apagar + 2, apagado);

  } else if (diferenca == 1) {
    pixels->setPixelColor(apagar - 2, apagado);
    pixels->setPixelColor(apagar + 1, apagado);
  }

  pixels->show();
}

// Adiciona valores às listas
void appendCoordenadas(int linha, int coluna, int lista[], int* totalPares, int tamanhoMaximo) {
  int index = (*totalPares) * 2;  // cada par ocupa 2 posições
  if (index + 1 < tamanhoMaximo) {
    lista[index] = linha;
    lista[index + 1] = coluna;
    (*totalPares)++;
  } else {
    // Lista cheia, não adiciona nada
    // Pode acionar alerta aqui, se necessário
  }
}

// Mostrar a cor dos LEDs
void coresAcender(int jogador, bool palpites) {
  if (jogador == 1 && palpites == true) {
    // Turno de palpites jogador 1
    for (int p = 0; p < contErrosJ1; p++) {
      pixelsJ1.setPixelColor(posicao(azulClaro[p * 2], azulClaro[p * 2 + 1]), corAzulClaro);
    }

    for (int p = 0; p < contAcertosJ1; p++) {
      pixelsJ1.setPixelColor(posicao(laranjaJ2[p * 2], laranjaJ2[p * 2 + 1]), corLaranja);
    }

    pixelsJ1.show();

  } else if (jogador == 1 && palpites == false) {
    // Turno de defesa jogador 1
    for (int p = 0; p < 6; p++) {
      pixelsJ1.setPixelColor(posicao(verdeJ1[p * 2], verdeJ1[p * 2 + 1]), corVerde);
    }

    for (int p = 0; p < contErrosJ2; p++) {
      pixelsJ1.setPixelColor(posicao(azulEscuro[p * 2], azulEscuro[p * 2 + 1]), corAzulClaro);
    }

    for (int p = 0; p < contAcertosJ2; p++) {
      pixelsJ1.setPixelColor(posicao(laranjaJ2[p * 2], laranjaJ2[p * 2 + 1]), corLaranja);
    }

    pixelsJ1.show();

  } else if (jogador == 2 && palpites == true) {
    // Turno de palpites jogador 2
    for (int p = 0; p < contErrosJ2; p++) {
      pixelsJ2.setPixelColor(posicao(azulEscuro[p * 2], azulEscuro[p * 2 + 1]), corAzulEscuro);
    }

    for (int p = 0; p < contAcertosJ2; p++) {
      pixelsJ2.setPixelColor(posicao(laranjaJ1[p * 2], laranjaJ1[p * 2 + 1]), corLaranja);
    }

    pixelsJ2.show();

  } else if (jogador == 2 && palpites == false) {
    // Turno de defesa jogador 2
    for (int p = 0; p < 6; p++) {
      pixelsJ2.setPixelColor(posicao(verdeJ2[p * 2], verdeJ2[p * 2 + 1]), corVerde);
    }

    for (int p = 0; p < contErrosJ1; p++) {
      pixelsJ2.setPixelColor(posicao(azulClaro[p * 2], azulClaro[p * 2 + 1]), corAzulEscuro);
    }

    for (int p = 0; p < contAcertosJ1; p++) {
      pixelsJ2.setPixelColor(posicao(laranjaJ2[p * 2], laranjaJ2[p * 2 + 1]), corLaranja);
    }

    pixelsJ2.show();
  }
}


void loop() {
  // Animação diagonal para J1
  for (int i = 0; i < 6; i++) {
    lightDiagonal(pixelsJ1, i, corAzulEscuro);
    clearPixels(pixelsJ1);
    delay(300);
  }
  for (int i = 5; i >= 0; i--) {
    lightDiagonal(pixelsJ1, i, corLaranja);
    clearPixels(pixelsJ1);
    delay(300);
  }

  // Animação diagonal para J2
  for (int i = 0; i < 6; i++) {
    lightDiagonal(pixelsJ2, i, corAzulClaro);
    clearPixels(pixelsJ2);
    delay(300);
  }
  for (int i = 5; i >= 0; i--) {
    lightDiagonal(pixelsJ2, i, corLaranja);
    clearPixels(pixelsJ2);
    delay(300);
  }

  delay(1000);

  //Fase de seleção de barcos
  for (int z = 0; z < 6; z++) {  // São selecionados 6 casas, por isso repete 6 vezes
    if (z == 0) {                // O padrão de cores muda de casa pra casa, então deve-se checar qual está sendo selecionada
      // Novas coordenadas
      coordLinha = detectarCoordLinha(1, true);
      coordColuna = detectarCoordColuna(1, true);

      // Endereçando o barco novo
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[0] = coordLinha;
      verdeJ1[1] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);
      pixelsJ1.setPixelColor(posicaoAtual - 1, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual + 1, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual - 6, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual + 6, corAmarelo);
      pixelsJ1.show();

    } else if (z == 1) {
      posicaoVelha = posicaoAtual;
      // Novas coordenadas
      coordLinha = detectarCoordLinha(1, true);
      coordColuna = detectarCoordColuna(1, true);

      // Apagando as casas amarelas antigas
      pixelsJ1.setPixelColor(posicaoVelha - 1, apagado);
      pixelsJ1.setPixelColor(posicaoVelha + 1, apagado);
      pixelsJ1.setPixelColor(posicaoVelha - 6, apagado);
      pixelsJ1.setPixelColor(posicaoVelha + 6, apagado);
      pixelsJ1.show();

      // Endereçando o barco novo
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[2] = coordLinha;
      verdeJ1[3] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);
      segundaCasaAcender(posicaoAtual, posicaoVelha, 1);
      pixelsJ1.show();


    } else if (z == 2) {
      // Novas coordenadas
      coordLinha = detectarCoordLinha(1, true);
      coordColuna = detectarCoordColuna(1, true);

      // Apagando as casas amarelas antigas
      segundaCasaApagar(diferenca, 1);

      //Endereçando o novo barco
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[4] = coordLinha;
      verdeJ1[5] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      //Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);

    } else if (z == 3) {
      // Novas coordenadas
      coordLinha = detectarCoordLinha(1, true);
      coordColuna = detectarCoordColuna(1, true);

      // Endereçando o barco novo
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[6] = coordLinha;
      verdeJ1[7] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);
      pixelsJ1.setPixelColor(posicaoAtual - 1, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual + 1, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual - 6, corAmarelo);
      pixelsJ1.setPixelColor(posicaoAtual + 6, corAmarelo);
      pixelsJ1.show();

    } else if (z == 4) {
      posicaoVelha = posicaoAtual;
      // Novas coordenadas
      coordLinha = detectarCoordLinha(1, true);
      coordColuna = detectarCoordColuna(1, true);

      // Apagando as casas amarelas antigas
      pixelsJ1.setPixelColor(posicaoVelha - 1, apagado);
      pixelsJ1.setPixelColor(posicaoVelha + 1, apagado);
      pixelsJ1.setPixelColor(posicaoVelha - 6, apagado);
      pixelsJ1.setPixelColor(posicaoVelha + 6, apagado);
      pixelsJ1.show();

      // Endereçando o barco novo
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[8] = coordLinha;
      verdeJ1[9] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);
      pixelsJ1.show();


    } else if (z == 5) {
      // Novas coordenadas
      coordLinha = detectarCoordLinha(1, true);
      coordColuna = detectarCoordColuna(1, true);

      //Endereçando o novo barco
      barcosJ1[coordLinha][coordColuna] = true;
      verdeJ1[10] = coordLinha;
      verdeJ1[11] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      //Acendendo os LEDs atrelados ao barco novo
      pixelsJ1.setPixelColor(posicaoAtual, corVerde);
      pixelsJ1.show();
    }
  }

  //Fase de seleção de barcos do J2
  for (int z = 0; z < 6; z++) {  // São selecionados 6 casas, por isso repete 6 vezes
    if (z == 0) {                // O padrão de cores muda de casa pra casa, então deve-se checar qual está sendo selecionada
      // Novas coordenadas
      coordLinha = detectarCoordLinha(2, true);
      coordColuna = detectarCoordColuna(2, true);

      // Endereçando o barco novo
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[0] = coordLinha;
      verdeJ2[1] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);
      pixelsJ2.setPixelColor(posicaoAtual - 1, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual + 1, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual - 6, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual + 6, corAmarelo);
      pixelsJ2.show();

    } else if (z == 1) {
      posicaoVelha = posicaoAtual;
      // Novas coordenadas
      coordLinha = detectarCoordLinha(2, true);
      coordColuna = detectarCoordColuna(2, true);

      // Apagando as casas amarelas antigas
      pixelsJ2.setPixelColor(posicaoVelha - 1, apagado);
      pixelsJ2.setPixelColor(posicaoVelha + 1, apagado);
      pixelsJ2.setPixelColor(posicaoVelha - 6, apagado);
      pixelsJ2.setPixelColor(posicaoVelha + 6, apagado);
      pixelsJ2.show();

      // Endereçando o barco novo
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[2] = coordLinha;
      verdeJ2[3] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);
      segundaCasaAcender(posicaoAtual, posicaoVelha, 2);
      pixelsJ2.show();


    } else if (z == 2) {
      // Novas coordenadas
      coordLinha = detectarCoordLinha(2, true);
      coordColuna = detectarCoordColuna(2, true);

      // Apagando as casas amarelas antigas
      segundaCasaApagar(diferenca, 2);

      //Endereçando o novo barco
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[4] = coordLinha;
      verdeJ2[5] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      //Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);

    } else if (z == 3) {
      // Novas coordenadas
      coordLinha = detectarCoordLinha(2, true);
      coordColuna = detectarCoordColuna(2, true);

      // Endereçando o barco novo
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[6] = coordLinha;
      verdeJ2[7] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);
      pixelsJ2.setPixelColor(posicaoAtual - 1, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual + 1, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual - 6, corAmarelo);
      pixelsJ2.setPixelColor(posicaoAtual + 6, corAmarelo);
      pixelsJ2.show();

    } else if (z == 4) {
      posicaoVelha = posicaoAtual;
      // Novas coordenadas
      coordLinha = detectarCoordLinha(2, true);
      coordColuna = detectarCoordColuna(2, true);

      // Apagando as casas amarelas antigas
      pixelsJ2.setPixelColor(posicaoVelha - 1, apagado);
      pixelsJ2.setPixelColor(posicaoVelha + 1, apagado);
      pixelsJ2.setPixelColor(posicaoVelha - 6, apagado);
      pixelsJ2.setPixelColor(posicaoVelha + 6, apagado);
      pixelsJ2.show();

      // Endereçando o barco novo
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[8] = coordLinha;
      verdeJ2[9] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      // Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);
      pixelsJ2.show();


    } else if (z == 5) {
      // Novas coordenadas
      coordLinha = detectarCoordLinha(2, true);
      coordColuna = detectarCoordColuna(2, true);

      //Endereçando o novo barco
      barcosJ2[coordLinha][coordColuna] = true;
      verdeJ2[10] = coordLinha;
      verdeJ2[11] = coordColuna;
      posicaoAtual = posicao(coordLinha, coordColuna);

      //Acendendo os LEDs atrelados ao barco novo
      pixelsJ2.setPixelColor(posicaoAtual, corVerde);
      pixelsJ2.show();
    }
  }

  delay(2000);
  pixelsJ1.fill(apagado);
  pixelsJ2.fill(apagado);
  pixelsJ1.show();
  pixelsJ2.show();

  // Fase de palpites
  while (barcosRestantesJ1 != 0 && barcosRestantesJ2 != 0) {
    // Tabuleiro de defesa jogador 2
    coresAcender(2, false);
    // Apagando os LEDs da ultima fase do jogador 1
    pixelsJ1.fill(apagado);
    pixelsJ1.show();

    // Jogador 1 seleciona sua casa
    coordLinha = detectarCoordLinha(1, false);
    coordColuna = detectarCoordColuna(1, false);
    posicaoAtual = posicao(coordLinha, coordColuna);

    while (barcosJ2[coordLinha][coordColuna] == true) {
      coresAcender(1, true);
      if (barcosRestantesJ2 == 0) {
        coresAcender(1, true);
        break;
      }
      barcosRestantesJ2--;
      appendCoordenadas(coordLinha, coordColuna, laranjaJ2, &contAcertosJ1, 12);
      barcosJ2[coordLinha][coordColuna] = false;
      coordLinha = detectarCoordLinha(1, false);
      coordColuna = detectarCoordColuna(1, false);
      posicaoAtual = posicao(coordLinha, coordColuna);
    }

    appendCoordenadas(coordLinha, coordColuna, azulClaro, &contErrosJ1, 60);
    coresAcender(1, true);
    delay(2000);
    pixelsJ1.fill(apagado);
    delay(1000);
    // Tabuleiro de defesa jogador 1
    coresAcender(1, false);

    // Apagando os LEDs da ultima fase do jogador 2
    pixelsJ2.fill(apagado);
    pixelsJ2.show();

    // Jogador 2 seleciona sua casa
    coordLinha = detectarCoordLinha(2, false);
    coordColuna = detectarCoordColuna(2, false);

    while (barcosJ1[coordLinha][coordColuna] == true) {
      coresAcender(1, true);
      if (barcosRestantesJ1 == 0) {
        coresAcender(1, true);
        break;
      }
      barcosRestantesJ2--;
      appendCoordenadas(coordLinha, coordColuna, laranjaJ1, &contAcertosJ2, 12);
      barcosJ1[coordLinha][coordColuna] = false;
      coordLinha = detectarCoordLinha(2, false);
      coordColuna = detectarCoordColuna(2, false);
    }

    appendCoordenadas(coordLinha, coordColuna, azulEscuro, &contErrosJ2, 60);
    coresAcender(2, true);
    delay(2000);
    pixelsJ2.fill(apagado);
    delay(1000);
  }

  pixelsJ1.fill(apagado);
  pixelsJ2.fill(apagado);
  pixelsJ2.show();
  pixelsJ1.show();
}
