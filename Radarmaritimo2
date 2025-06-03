#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pino do LED e numéro de LEDs totais na matriz
#define PIN 50
#define NUM_LEDS 36

// declarando a fita LED
Adafruit_NeoPixel fita(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// botões das colunas
int btn1 = 32;
int btn2 = 33;
int btn3 = 34;
int btn4 = 35;
int btn5 = 36;
int btn6 = 37;

// botões das linhas
int btnA = 26;
int btnB = 27;
int btnC = 28;
int btnD = 29;
int btnE = 30;
int btnF = 31;

// armazena os estados dos botões das linhas e colunas
bool btnStatesLinha[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool btnStatesColuna[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

// armazenam a ultima linha/coluna pressionada (rastream a posição pressionada na matriz física)
int linhaAtiva = -1;
int colunaAtiva = -1;

// indica se uma linha precisa ser selecionada ainda, quanto tempo a linha foi pressionada e se uma linha está sendo exibida
bool linhaHabilitada = false;
unsigned long tempoLinhaLigada = 0;
bool mostrandoLinha = false;

// estrutura para armazenar a posição dos barcos
struct Intersecao {
  int linha;
  int coluna;
};

// Barcos do primeiro jogador
Intersecao grupo1[3];
int totalGrupo1 = 0;

Intersecao grupo2[2];
int totalGrupo2 = 0;

Intersecao grupo3[1];
int totalGrupo3 = 0;

// Barcos do segundo jogador
Intersecao novoGrupo1[3];
int totalNovoGrupo1 = 0;

Intersecao novoGrupo2[2];
int totalNovoGrupo2 = 0;

Intersecao novoGrupo3[1];
int totalNovoGrupo3 = 0;

// fases de posicionamento do barco 3, 2 e 1
bool modoGrupo1 = true;
bool modoGrupo2 = false;
bool modoGrupo3 = false;

// checa se é vez do segundo jogador, serve pra mudar entre a fase de seleção de barcos do primeiro e do segundo jogador e
// indica o tempo em que o jogador termina de posicionar seus barcos
bool segundaRodada = false;
bool aguardandoReinicio = false;
unsigned long tempoFinalizacao = 0;

// ativa a fase de ataque, qual jogador está atacando e quando ocorreu o ultimo ataque
bool modoAtaque = false;
bool jogador1Atacando = true;
bool jogador2Atacando = false;
unsigned long tempoUltimoAtaque = 0;

// ativa o fim de jogo, indica quem venceu e o momento em que o jogador venceu 
bool jogoFinalizado = false;
int vencedor = 0;
unsigned long tempoVitoria = 0;

// quais casa os jogadores acertaram
bool acertosJogador1[NUM_LEDS] = {false};
bool acertosJogador2[NUM_LEDS] = {false};

// quais casas os jogadores erraram
bool errosJogador1[NUM_LEDS] = {false};
bool errosJogador2[NUM_LEDS] = {false};
LiquidCrystal_I2C lcd1(0x26, 16, 2);
LiquidCrystal_I2C lcd2(0x27, 16, 2);

int buzzer = 52;
int tempo = 120;

// as notas a serem tocadas e sua duração
int melody[] = {
  262, 16,
  330, 16,
  392, 16,
  523, 8,
  659, 8
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;

// som do buzzer pro fim da rodada
int fim_rodada_melody[] = {
  523, 8,
  392, 16,
  330, 16,
  262, 8
};
int fim_rodada_notes = sizeof(fim_rodada_melody) / sizeof(fim_rodada_melody[0]) / 2;

// som do buzzer pro fim do jogo
int fim_jogo_melody[] = {
  523, 4,
  659, 4,
  784, 4,
  1047, 2
};
int fim_jogo_notes = sizeof(fim_jogo_melody) / sizeof(fim_jogo_melody[0]) / 2;

// som do buzzer pra quando um barco é acertado
int acertou_barco_melody[] = {
  784, 16,
  1047, 8
};
int acertou_barco_notes = sizeof(acertou_barco_melody) / sizeof(acertou_barco_melody[0]) / 2;

// som do buzzer pra quando erram um barco
int acertou_agua_melody[] = {
  200, 16,
  180, 8
};
int acertou_agua_notes = sizeof(acertou_agua_melody) / sizeof(acertou_agua_melody[0]) / 2;

// função para tocar o som de início de jogo
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

// função para tocar o fim de rodada
void fim_rodada_som() {
  int divider = 0, noteDuration = 0;

  for (int thisNote = 0; thisNote < fim_rodada_notes * 2; thisNote += 2) {
    divider = fim_rodada_melody[thisNote + 1];

    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, fim_rodada_melody[thisNote], noteDuration * 0.9);
    delay(noteDuration + 40);
    noTone(buzzer);
  }
}

// função para tocar o fim de jogo
void fim_jogo_som() {
  int divider = 0, noteDuration = 0;

  for (int thisNote = 0; thisNote < fim_jogo_notes * 2; thisNote += 2) {
    divider = fim_jogo_melody[thisNote + 1];

    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, fim_jogo_melody[thisNote], noteDuration * 0.9);
    delay(noteDuration + 40);
    noTone(buzzer);
  }
}

// função para tocar o som de quando um barco é alvejado
void acertou_barco_som() {
  int divider = 0, noteDuration = 0;

  for (int thisNote = 0; thisNote < acertou_barco_notes * 2; thisNote += 2) {
    divider = acertou_barco_melody[thisNote + 1];

    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, acertou_barco_melody[thisNote], noteDuration * 0.9);
    delay(noteDuration + 10);
    noTone(buzzer);
  }
}

// função para tocar o som de quando um jogador erra um barco
void acertou_agua_som() {
  int divider = 0, noteDuration = 0;

  for (int thisNote = 0; thisNote < acertou_agua_notes * 2; thisNote += 2) {
    divider = acertou_agua_melody[thisNote + 1];

    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, acertou_agua_melody[thisNote], noteDuration * 0.9);
    delay(noteDuration + 20);
    noTone(buzzer);
  }
}

// função para mostrar as frases no display LCD
void Texto(LiquidCrystal_I2C &lcd, int a, int b, const char* texto1, int c, int d, const char* texto2) {
  lcd.clear();
  lcd.setCursor(a, b);
  lcd.print(texto1);
  lcd.setCursor(c, d);
  lcd.print(texto2);
}

// quando um jogo chega ao fim, redefine todas as variáveis alteradas para o estado original, apaga os LEDs
// e mostra a mensagem para o jogador selecionar o barco de 3 casas
void resetGame() {
  totalGrupo1 = 0;
  totalGrupo2 = 0;
  totalGrupo3 = 0;
  totalNovoGrupo1 = 0;
  totalNovoGrupo2 = 0;
  totalNovoGrupo3 = 0;
  segundaRodada = false;
  modoGrupo1 = true;
  modoGrupo2 = false;
  modoGrupo3 = false;
  jogoFinalizado = false;
  modoAtaque = false;
  linhaAtiva = -1;
  colunaAtiva = -1;
  linhaHabilitada = false;
  mostrandoLinha = false;
  vencedor = 0;

  for (int i = 0; i < NUM_LEDS; i++) {
    fita.setPixelColor(i, 0);
    acertosJogador1[i] = false;
    acertosJogador2[i] = false;
    errosJogador1[i] = false;
    errosJogador2[i] = false;
  }
  fita.show();

  Texto(lcd1, 0, 0, "1o Jogador", 0, 1, "Selecione barco3");
  inicio_jogo_som();
}

// funciona pra transicionar entre a fase de seleção de barcos do jogador 1 e 2 (!segundaRodada)
// e pra transicionar entre a fase de seleção de barcos do jogador 2 e a fase de ataques (else)
void reiniciarSistema() {
  if (!segundaRodada) {
    segundaRodada = true;
    totalNovoGrupo1 = 0;
    totalNovoGrupo2 = 0;
    totalNovoGrupo3 = 0;
    modoGrupo1 = true;
    modoGrupo2 = false;
    modoGrupo3 = false;
    aguardandoReinicio = false;

    for (int i = 0; i < NUM_LEDS; i++) {
      fita.setPixelColor(i, 0);
      acertosJogador1[i] = false;
      acertosJogador2[i] = false;
      errosJogador1[i] = false;
      errosJogador2[i] = false;
    }
    fita.show();

    Texto(lcd1, 0, 0, "2o Jogador", 0, 1, "Selecione barco3");
  } else {
    iniciarModoAtaque();
  }
}

// checa se alguma casa ja foi selecionada (para evitar que o jogador pressione uma casa novamente)
bool posicaoOcupada(int lin, int col) {
  if (!segundaRodada) {
    for (int i = 0; i < totalGrupo1; i++) {
      if (grupo1[i].linha == lin && grupo1[i].coluna == col) return true;
    }
    for (int i = 0; i < totalGrupo2; i++) {
      if (grupo2[i].linha == lin && grupo2[i].coluna == col) return true;
    }
    for (int i = 0; i < totalGrupo3; i++) {
      if (grupo3[i].linha == lin && grupo3[i].coluna == col) return true;
    }
  } else {
    for (int i = 0; i < totalNovoGrupo1; i++) {
      if (novoGrupo1[i].linha == lin && novoGrupo1[i].coluna == col) return true;
    }
    for (int i = 0; i < totalNovoGrupo2; i++) {
      if (novoGrupo2[i].linha == lin && novoGrupo2[i].coluna == col) return true;
    }
    for (int i = 0; i < totalNovoGrupo3; i++) {
      if (novoGrupo3[i].linha == lin && novoGrupo3[i].coluna == col) return true;
    }
  }
  return false;
}

// essa função garante que as casas do primeiro barco (que ocupa 3 espaços) sejam colocadas
// uma ao lado da outra (diretamente adjacente de maneira horizontal/vertical) e que formam uma linha/coluna contínua
bool ehAdjacenteGrupo1(int lin, int col) {
  if (!segundaRodada) {
    if (totalGrupo1 == 0) return true;

    for (int i = 0; i < totalGrupo1; i++) {
      int diffLin = abs(grupo1[i].linha - lin);
      int diffCol = abs(grupo1[i].coluna - col);

      if ((diffLin == 0 && diffCol == 1) || (diffCol == 0 && diffLin == 1)) {
        return true;
      }
    }
  } else {
    if (totalNovoGrupo1 == 0) return true;

    for (int i = 0; i < totalNovoGrupo1; i++) {
      int diffLin = abs(novoGrupo1[i].linha - lin);
      int diffCol = abs(novoGrupo1[i].coluna - col);

      if ((diffLin == 0 && diffCol == 1) || (diffCol == 0 && diffLin == 1)) {
        return true;
      }
    }
  }
  return false;
}

// essa função garante que as casas do segundo barco (que ocupa 2 espaços) sejam colocadas
// uma ao lado da outra (diretamente adjacente de maneira horizontal/vertical) e que formam uma linha/coluna contínua
bool ehAdjacenteGrupo2(int lin, int col) {
  if (!segundaRodada) {
    if (totalGrupo2 == 0) return true;

    int diffLin = abs(grupo2[0].linha - lin);
    int diffCol = abs(grupo2[0].coluna - col);

    return (diffLin == 0 && diffCol == 1) || (diffCol == 0 && diffLin == 1);
  } else {
    if (totalNovoGrupo2 == 0) return true;

    int diffLin = abs(novoGrupo2[0].linha - lin);
    int diffCol = abs(novoGrupo2[0].coluna - col);

    return (diffLin == 0 && diffCol == 1) || (diffCol == 0 && diffLin == 1);
  }
}

// essa função adiciona a posição do barco inserida pelo jogador 
// por meio da intersecção linha/coluna ao seu respectivo grupo (que tipo de barco é)
void adicionarIntersecao(int lin, int col) {
  if (posicaoOcupada(lin, col)) {
    return;
  }
  
  if (modoGrupo1) {
    if (!segundaRodada) {
      if (totalGrupo1 < 3) {
        if (totalGrupo1 > 0 && !ehAdjacenteGrupo1(lin, col)) {
          return;
        }

        grupo1[totalGrupo1].linha = lin;
        grupo1[totalGrupo1].coluna = col;
        totalGrupo1++;

        if (totalGrupo1 == 3) {
          modoGrupo1 = false;
          modoGrupo2 = true;
          Texto(lcd1, 3, 0, "Barco 3 OK", 0, 1, "Selecione barco2");
        }
      }
    } else {
      if (totalNovoGrupo1 < 3) {
        if (totalNovoGrupo1 > 0 && !ehAdjacenteGrupo1(lin, col)) {
          return;
        }

        novoGrupo1[totalNovoGrupo1].linha = lin;
        novoGrupo1[totalNovoGrupo1].coluna = col;
        totalNovoGrupo1++;

        if (totalNovoGrupo1 == 3) {
          modoGrupo1 = false;
          modoGrupo2 = true;
          Texto(lcd1, 3, 0, "Barco 3 OK", 0, 1, "Selecione barco2");
        }
      }
    }
  } else if (modoGrupo2) {
    if (!segundaRodada) {
      if (totalGrupo2 < 2) {
        if (totalGrupo2 == 1 && !ehAdjacenteGrupo2(lin, col)) {
          return;
        }

        grupo2[totalGrupo2].linha = lin;
        grupo2[totalGrupo2].coluna = col;
        totalGrupo2++;

        if (totalGrupo2 == 2) {
          modoGrupo2 = false;
          modoGrupo3 = true;
          Texto(lcd1, 3, 0, "Barco 2 OK", 0, 1, "Selecione barco1");
        }
      }
    } else {
      if (totalNovoGrupo2 < 2) {
        if (totalNovoGrupo2 == 1 && !ehAdjacenteGrupo2(lin, col)) {
          return;
        }

        novoGrupo2[totalNovoGrupo2].linha = lin;
        novoGrupo2[totalNovoGrupo2].coluna = col;
        totalNovoGrupo2++;

        if (totalNovoGrupo2 == 2) {
          modoGrupo2 = false;
          modoGrupo3 = true;
          Texto(lcd1, 3, 0, "Barco 2 OK", 0, 1, "Selecione barco1");
        }
      }
    }
  } else if (modoGrupo3) {
    if (!segundaRodada) {
      if (totalGrupo3 < 1) {
        grupo3[totalGrupo3].linha = lin;
        grupo3[totalGrupo3].coluna = col;
        totalGrupo3++;

        if (totalGrupo3 == 1) {
          modoGrupo3 = false;
          Texto(lcd1, 0, 0, "Concluido!", 0, 1, "Aguardando...");
          aguardandoReinicio = true;
          tempoFinalizacao = millis();
          fim_rodada_som();
        }
      }
    } else {
      if (totalNovoGrupo3 < 1) {
        novoGrupo3[totalNovoGrupo3].linha = lin;
        novoGrupo3[totalNovoGrupo3].coluna = col;
        totalNovoGrupo3++;

        if (totalNovoGrupo3 == 1) {
          modoGrupo3 = false;
          Texto(lcd1, 0, 0, "Concluido!", 0, 1, "Aguardando...");
          aguardandoReinicio = true;
          tempoFinalizacao = millis();
          fim_rodada_som();
        }
      }
    }
  }
}

// acende as colunas
void mostrarColuna(int col) {
  for (int i = 0; i < 6; i++) {
    int index = col + i * 6;
    fita.setPixelColor(index, fita.Color(255, 50, 0));
  }
}

// acende as linhas
void mostrarLinha(int lin) {
  for (int i = 0; i < 6; i++) {
    int index = lin * 6 + i;
    fita.setPixelColor(index, fita.Color(0, 0, 139));
  }
}

// mostra o LED correspondente ao cruzamento da linha e da coluna (casa selecionada pelo jogador)
void mostrarIntersecao(int lin, int col, uint32_t cor) {
  int index = col + lin * 6;
  fita.setPixelColor(index, cor);
}

// dependendo de qual parte do barco e de qual barco esta sendo posicionado mostra as possíveis casas que podem ser selecionadas
void mostrarPossiveisIntersecoes() {
  if (modoGrupo1) {
    if (!segundaRodada) {
      if (totalGrupo1 == 0) return;
      else if (totalGrupo1 == 1) {
        int lin = grupo1[0].linha;
        int col = grupo1[0].coluna;

        int possiveis[4][2] = {{lin - 1, col}, {lin + 1, col}, {lin, col - 1}, {lin, col + 1}};

        for (int i = 0; i < 4; i++) {
          int l = possiveis[i][0];
          int c = possiveis[i][1];
          if (l >= 0 && l < 6 && c >= 0 && c < 6 && !posicaoOcupada(l, c)) {
            int idx = c + l * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
        }
      }
      else if (totalGrupo1 == 2) {
        int lin0 = grupo1[0].linha;
        int col0 = grupo1[0].coluna;
        int lin1 = grupo1[1].linha;
        int col1 = grupo1[1].coluna;

        if (lin0 == lin1) {
          int minCol = min(col0, col1);
          int maxCol = max(col0, col1);

          if (minCol - 1 >= 0 && !posicaoOcupada(lin0, minCol - 1)) {
            int idx = (minCol - 1) + lin0 * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
          if (maxCol + 1 < 6 && !posicaoOcupada(lin0, maxCol + 1)) {
            int idx = (maxCol + 1) + lin0 * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
        }
        else if (col0 == col1) {
          int minLin = min(lin0, lin1);
          int maxLin = max(lin0, lin1);

          if (minLin - 1 >= 0 && !posicaoOcupada(minLin - 1, col0)) {
            int idx = col0 + (minLin - 1) * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
          if (maxLin + 1 < 6 && !posicaoOcupada(maxLin + 1, col0)) {
            int idx = col0 + (maxLin + 1) * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
        }
      }
    } else {
      if (totalNovoGrupo1 == 0) return;
      else if (totalNovoGrupo1 == 1) {
        int lin = novoGrupo1[0].linha;
        int col = novoGrupo1[0].coluna;

        int possiveis[4][2] = {{lin - 1, col}, {lin + 1, col}, {lin, col - 1}, {lin, col + 1}};

        for (int i = 0; i < 4; i++) {
          int l = possiveis[i][0];
          int c = possiveis[i][1];
          if (l >= 0 && l < 6 && c >= 0 && c < 6 && !posicaoOcupada(l, c)) {
            int idx = c + l * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
        }
      }
      else if (totalNovoGrupo1 == 2) {
        int lin0 = novoGrupo1[0].linha;
        int col0 = novoGrupo1[0].coluna;
        int lin1 = novoGrupo1[1].linha;
        int col1 = novoGrupo1[1].coluna;

        if (lin0 == lin1) {
          int minCol = min(col0, col1);
          int maxCol = max(col0, col1);

          if (minCol - 1 >= 0 && !posicaoOcupada(lin0, minCol - 1)) {
            int idx = (minCol - 1) + lin0 * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
          if (maxCol + 1 < 6 && !posicaoOcupada(lin0, maxCol + 1)) {
            int idx = (maxCol + 1) + lin0 * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
        }
        else if (col0 == col1) {
          int minLin = min(lin0, lin1);
          int maxLin = max(lin0, lin1);

          if (minLin - 1 >= 0 && !posicaoOcupada(minLin - 1, col0)) {
            int idx = col0 + (minLin - 1) * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
          if (maxLin + 1 < 6 && !posicaoOcupada(maxLin + 1, col0)) {
            int idx = col0 + (maxLin + 1) * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
        }
      }
    }
  }
  else if (modoGrupo2) {
    if (!segundaRodada) {
      if (totalGrupo2 == 0) return;
      else if (totalGrupo2 == 1) {
        int lin = grupo2[0].linha;
        int col = grupo2[0].coluna;

        int possiveis[4][2] = {{lin - 1, col}, {lin + 1, col}, {lin, col - 1}, {lin, col + 1}};

        for (int i = 0; i < 4; i++) {
          int l = possiveis[i][0];
          int c = possiveis[i][1];
          if (l >= 0 && l < 6 && c >= 0 && c < 6 && !posicaoOcupada(l, c)) {
            int idx = c + l * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
        }
      }
    } else {
      if (totalNovoGrupo2 == 0) return;
      else if (totalNovoGrupo2 == 1) {
        int lin = novoGrupo2[0].linha;
        int col = novoGrupo2[0].coluna;

        int possiveis[4][2] = {{lin - 1, col}, {lin + 1, col}, {lin, col - 1}, {lin, col + 1}};

        for (int i = 0; i < 4; i++) {
          int l = possiveis[i][0];
          int c = possiveis[i][1];
          if (l >= 0 && l < 6 && c >= 0 && c < 6 && !posicaoOcupada(l, c)) {
            int idx = c + l * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
        }
      }
    }
  }
}

// checa se as casas selecionadas pelo jogador contém um barco do oponente na fase de ataques
bool pertenceAosGruposDoOponente(int lin, int col) {
  if (jogador1Atacando) {
    for (int i = 0; i < totalNovoGrupo1; i++) {
      if (novoGrupo1[i].linha == lin && novoGrupo1[i].coluna == col) return true;
    }
    for (int i = 0; i < totalNovoGrupo2; i++) {
      if (novoGrupo2[i].linha == lin && novoGrupo2[i].coluna == col) return true;
    }
    for (int i = 0; i < totalNovoGrupo3; i++) {
      if (novoGrupo3[i].linha == lin && novoGrupo3[i].coluna == col) return true;
    }
  } else {
    for (int i = 0; i < totalGrupo1; i++) {
      if (grupo1[i].linha == lin && grupo1[i].coluna == col) return true;
    }
    for (int i = 0; i < totalGrupo2; i++) {
      if (grupo2[i].linha == lin && grupo2[i].coluna == col) return true;
    }
    for (int i = 0; i < totalGrupo3; i++) {
      if (grupo3[i].linha == lin && grupo3[i].coluna == col) return true;
    }
  }
  return false;
}

// verifica se o jogador atualmente atacando venceu o jogo
void verificarVitoria() {
  int totalLedsOponente = 0;
  int acertosDoJogador = 0;

  if (jogador1Atacando) {
    totalLedsOponente = totalNovoGrupo1 + totalNovoGrupo2 + totalNovoGrupo3;
    for (int i = 0; i < NUM_LEDS; i++) {
      int row = i / 6;
      int col = i % 6;
      if (acertosJogador1[i] && pertenceAosGruposDoOponente(row, col)) {
        acertosDoJogador++;
      }
    }
    if (totalLedsOponente > 0 && acertosDoJogador >= totalLedsOponente) {
      vencedor = 1;
      jogoFinalizado = true;
      tempoVitoria = millis();
      Texto(lcd1, 0, 0, "Vitoria do", 0, 1, "Jogador 1!");
      for (int i = 0; i < NUM_LEDS; i++) {
        fita.setPixelColor(i, fita.Color(0, 255, 0));
      }
      fita.show();
      fim_jogo_som();
    }
  } else {
    totalLedsOponente = totalGrupo1 + totalGrupo2 + totalGrupo3;
    for (int i = 0; i < NUM_LEDS; i++) {
      int row = i / 6;
      int col = i % 6;
      if (acertosJogador2[i] && pertenceAosGruposDoOponente(row, col)) {
        acertosDoJogador++;
      }
    }
    if (totalLedsOponente > 0 && acertosDoJogador >= totalLedsOponente) {
      vencedor = 2;
      jogoFinalizado = true;
      tempoVitoria = millis();
      Texto(lcd1, 0, 0, "Vitoria do", 0, 1, "Jogador 2!");
      for (int i = 0; i < NUM_LEDS; i++) {
        fita.setPixelColor(i, fita.Color(0, 255, 0));
      }
      fita.show();
      fim_jogo_som();
    }
  }
}

// função responsável por iniciar a fase de ataque de barcos
void iniciarModoAtaque() {
  modoAtaque = true;
  jogador1Atacando = true;
  jogador2Atacando = false;
  jogoFinalizado = false;
  vencedor = 0;

  for (int i = 0; i < NUM_LEDS; i++) {
    fita.setPixelColor(i, 0);
    acertosJogador1[i] = false;
    acertosJogador2[i] = false;
    errosJogador1[i] = false;
    errosJogador2[i] = false;
  }
  fita.show();

  Texto(lcd1, 0, 0, "Modo Ataque:", 0, 1, "Jogador 1 ataca");
}

// checa se o jogador acertou seu palpite e permite que ele continue jogando até errar
void processarAtaque(int lin, int col) {
  int index = col + lin * 6;

  if ((jogador1Atacando && (acertosJogador1[index] || errosJogador1[index])) ||
      (jogador2Atacando && (acertosJogador2[index] || errosJogador2[index]))) {
    return;
  }

  if (jogador1Atacando) {
    if (pertenceAosGruposDoOponente(lin, col)) {
      acertosJogador1[index] = true;
      acertou_barco_som();
      Texto(lcd1, 0, 0, "Acerto!", 0, 1, "Jogador 1 continua");
    } else {
      errosJogador1[index] = true;
      acertou_agua_som();
      Texto(lcd1, 0, 0, "Errou!", 0, 1, "Vez do Jogador 2");
      fita.show();
      delay(1000);
      jogador1Atacando = false;
      jogador2Atacando = true;
    }
  } else {
    if (pertenceAosGruposDoOponente(lin, col)) {
      acertosJogador2[index] = true;
      acertou_barco_som();
      Texto(lcd1, 0, 0, "Acerto!", 0, 1, "Jogador 2 continua");
    } else {
      errosJogador2[index] = true;
      acertou_agua_som();
      Texto(lcd1, 0, 0, "Errou!", 0, 1, "Vez do Jogador 1");
      fita.show();
      delay(1000);
      jogador1Atacando = true;
      jogador2Atacando = false;
    }
  }
  fita.show();
  tempoUltimoAtaque = millis();
  verificarVitoria();
}

// iniciando os botões e o display LCD
void setup() {
  fita.begin();
  fita.show();

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
  pinMode(btn5, INPUT_PULLUP);
  pinMode(btn6, INPUT_PULLUP);

  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(btnC, INPUT_PULLUP);
  pinMode(btnD, INPUT_PULLUP);
  pinMode(btnE, INPUT_PULLUP);
  pinMode(btnF, INPUT_PULLUP);

  lcd1.init();
  lcd1.backlight();

  lcd2.init();
  lcd2.backlight();
  lcd2.clear();

  Texto(lcd1, 2, 0, "1o Jogador:", 0, 1, "Selecione barco3");
  inicio_jogo_som();
}


void loop() {
  // checa se o jogo terminou e aguarda 10 segundos antes de reiniciar
  if (jogoFinalizado) {
    if (millis() - tempoVitoria >= 10000) {
      resetGame();
    }
    return;
  }

  // verifica se está aguardando reinício entre fases do jogo
  if (aguardandoReinicio) {
    if (millis() - tempoFinalizacao >= 2000) {
      if (segundaRodada) {
        // limpa a matriz LED e prepara para o modo de ataque
        for (int i = 0; i < NUM_LEDS; i++) {
          fita.setPixelColor(i, 0);
          acertosJogador1[i] = false;
          acertosJogador2[i] = false;
          errosJogador1[i] = false;
          errosJogador2[i] = false;
        }
        fita.show();
        iniciarModoAtaque();
        aguardandoReinicio = false;
      } else {
        // reinicia as condições para que o segundo jogador possa selecionar seus barcos
        reiniciarSistema();
      }
    }
    return;
  }

  // mapeia os botões das colunas e linhas
  int botoesLinha[] = {btn1, btn2, btn3, btn4, btn5, btn6};
  int botoesColuna[] = {btnA, btnB, btnC, btnD, btnE, btnF};

  // checa o estado dos botões das colunas
  for (int i = 0; i < 6; i++) {
    bool estadoAtual = digitalRead(botoesColuna[i]);
    if (btnStatesColuna[i] == HIGH && estadoAtual == LOW) {
      colunaAtiva = i;
      linhaAtiva = -1;
      linhaHabilitada = true;
      mostrandoLinha = false;
    }
    btnStatesColuna[i] = estadoAtual;
  }

  // delimita o tempo que a linha selecionada fica acesa 
  if (mostrandoLinha) {
    if (millis() - tempoLinhaLigada >= 300) {
      mostrandoLinha = false;
      colunaAtiva = -1;
      linhaAtiva = -1;
    }
  }
  // verifica os botões das linhas quando uma coluna ja foi selecionada
  else if (linhaHabilitada) {
    for (int i = 0; i < 6; i++) {
      bool estadoAtual = digitalRead(botoesLinha[i]);
      if (btnStatesLinha[i] == HIGH && estadoAtual == LOW) {
        linhaAtiva = i;
        linhaHabilitada = false;
        mostrandoLinha = true;
        tempoLinhaLigada = millis();

        // calcula a intersecção
        if (colunaAtiva != -1) {
          if (modoAtaque) {
            processarAtaque(linhaAtiva, colunaAtiva);
          } else {
            adicionarIntersecao(linhaAtiva, colunaAtiva);
          }
        }
      }
      btnStatesLinha[i] = estadoAtual;
    }
  } else {
    // atualiza o estado dos botões das linhas
    for (int i = 0; i < 6; i++) {
      btnStatesLinha[i] = digitalRead(botoesLinha[i]);
    }
  }

  // caso o jogo não tenha finalizado limpa a matriz LED
  if (!jogoFinalizado) {
    for (int i = 0; i < NUM_LEDS; i++) {
      fita.setPixelColor(i, 0);
    }
  }

  // mostra a coluna ativa se não estiver mostrando linha
  if (colunaAtiva != -1 && !mostrandoLinha && !jogoFinalizado) {
    mostrarColuna(colunaAtiva);
  }

  // mostra a linha ativa e a intersecção
  if (mostrandoLinha && linhaAtiva != -1 && !jogoFinalizado) {
    mostrarLinha(linhaAtiva);
    if (colunaAtiva != -1) {
      if (modoAtaque) {
        // diferentes cores para acertos de cada jogador
        if (pertenceAosGruposDoOponente(linhaAtiva, colunaAtiva)) {
            if (jogador1Atacando) mostrarIntersecao(linhaAtiva, colunaAtiva, fita.Color(0, 255, 0));
            else mostrarIntersecao(linhaAtiva, colunaAtiva, fita.Color(0, 0, 255));
        } else {
            mostrarIntersecao(linhaAtiva, colunaAtiva, fita.Color(255, 0, 0));
        }
      } else {
        // cores diferentes para cada jogador durante o posicionamento
        if (!segundaRodada) {
          mostrarIntersecao(linhaAtiva, colunaAtiva, fita.Color(0, 255, 0));
        } else {
          mostrarIntersecao(linhaAtiva, colunaAtiva, fita.Color(255, 20, 147));
        }
      }
    }
  }

  // atualiza a exibição dos barcos posicionados ou dos ataques
  if (!jogoFinalizado) {
    if (!modoAtaque) {
      // mostra barcos posicionados pelo jogador atual
      if (!segundaRodada) {
        for (int i = 0; i < totalGrupo1; i++) {
          mostrarIntersecao(grupo1[i].linha, grupo1[i].coluna, fita.Color(0, 255, 0));
        }
        for (int i = 0; i < totalGrupo2; i++) {
          mostrarIntersecao(grupo2[i].linha, grupo2[i].coluna, fita.Color(0, 255, 0));
        }
        for (int i = 0; i < totalGrupo3; i++) {
          mostrarIntersecao(grupo3[i].linha, grupo3[i].coluna, fita.Color(0, 255, 0));
        }
      } else {
        for (int i = 0; i < totalNovoGrupo1; i++) {
          mostrarIntersecao(novoGrupo1[i].linha, novoGrupo1[i].coluna, fita.Color(255, 20, 147));
        }
        for (int i = 0; i < totalNovoGrupo2; i++) {
          mostrarIntersecao(novoGrupo2[i].linha, novoGrupo2[i].coluna, fita.Color(255, 20, 147));
        }
        for (int i = 0; i < totalNovoGrupo3; i++) {
          mostrarIntersecao(novoGrupo3[i].linha, novoGrupo3[i].coluna, fita.Color(255, 20, 147));
        }
      }
    } else {
      // mostra acertos e erros dos jogadores
      for (int i = 0; i < NUM_LEDS; i++) {
        if (jogador1Atacando) {
          if (acertosJogador1[i]) {
            fita.setPixelColor(i, fita.Color(0, 255, 0));
          } else if (errosJogador1[i]) {
            fita.setPixelColor(i, fita.Color(13,33,79));
          }
        } else {
          if (acertosJogador2[i]) {
            fita.setPixelColor(i, fita.Color(255, 16, 240));
          } else if (errosJogador2[i]) {
            fita.setPixelColor(i, fita.Color(13,33,79));
          }
        }
      }
    }
  }

  // mostra possíveis posições para colocar barcos durante o posicionamento
  if (!modoAtaque && (modoGrupo1 || modoGrupo2)) {
    mostrarPossiveisIntersecoes();
  }

  // atualiza a exibição da matriz LED
  fita.show();
  delay(30);
}
