#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIN 50
#define NUM_LEDS 36

Adafruit_NeoPixel fita(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Botões para linhas (1 a 6)
int btn1 = 32;
int btn2 = 33;
int btn3 = 34;
int btn4 = 35;
int btn5 = 36;
int btn6 = 37;

// Botões para colunas (A a F)
int btnA = 26;
int btnB = 27;
int btnC = 28;
int btnD = 29;
int btnE = 30;
int btnF = 31;

// Estados anteriores dos botões
bool btnStatesLinha[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool btnStatesColuna[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

int linhaAtiva = -1;
int colunaAtiva = -1;

bool linhaHabilitada = false;
unsigned long tempoLinhaLigada = 0;
bool mostrandoLinha = false;

struct Intersecao {
  int linha;
  int coluna;
};

// Grupos iniciais
Intersecao grupo1[3];
int totalGrupo1 = 0;

Intersecao grupo2[2];
int totalGrupo2 = 0;

Intersecao grupo3[1];
int totalGrupo3 = 0;

// Novos grupos para a segunda rodada
Intersecao novoGrupo1[3];
int totalNovoGrupo1 = 0;

Intersecao novoGrupo2[2];
int totalNovoGrupo2 = 0;

Intersecao novoGrupo3[1];
int totalNovoGrupo3 = 0;

bool modoGrupo1 = true;
bool modoGrupo2 = false;
bool modoGrupo3 = false;

bool segundaRodada = false;
bool aguardandoReinicio = false;
unsigned long tempoFinalizacao = 0;

// Variáveis para controle do modo de ataque
bool modoAtaque = false;
bool jogador1Atacando = true;
bool jogador2Atacando = false;
unsigned long tempoUltimoAtaque = 0;


bool jogoFinalizado = false;
int vencedor = 0; // 0 = nenhum, 1 = jogador1, 2 = jogador2
unsigned long tempoVitoria = 0;


bool acertosJogador1[NUM_LEDS] = {false};
bool acertosJogador2[NUM_LEDS] = {false};

bool errosJogador1[NUM_LEDS] = {false};
bool errosJogador2[NUM_LEDS] = {false};
LiquidCrystal_I2C lcd1(0x26, 16, 2);
LiquidCrystal_I2C lcd2(0x27, 16, 2);

// --- Buzzer variables and function ---
int buzzer = 52;
int tempo = 120; // Tempo musical (BPM)

int melody[] = {
  262, 16,        // C4 - início firme
  330, 16,        // E4 - subida
  392, 16,        // G4 - brilho
  523, 8,         // C5 - auge
  659, 8        // E5 - final épico
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;

// New melody for end of round
int fim_rodada_melody[] = {
  523, 8,        // C5
  392, 16,       // G4
  330, 16,       // E4
  262, 8        // C4
};
int fim_rodada_notes = sizeof(fim_rodada_melody) / sizeof(fim_rodada_melody[0]) / 2;

// New melody for end of game (victory)
int fim_jogo_melody[] = {
  523, 4, // C5
  659, 4, // E5
  784, 4, // G5
  1047, 2 // C6 - high triumph!
};
int fim_jogo_notes = sizeof(fim_jogo_melody) / sizeof(fim_jogo_melody[0]) / 2;


// Melody for a hit (acerto)
int acertou_barco_melody[] = {
  784, 16, // G5 - sharp, quick tone
  1047, 8  // C6 - higher, distinct
};
int acertou_barco_notes = sizeof(acertou_barco_melody) / sizeof(acertou_barco_melody[0]) / 2;

// NEW: Melody for a miss (acertou_agua)
int acertou_agua_melody[] = {
  200, 16, // Low C - splash
  180, 8   // Even lower A - deeper splash
};
int acertou_agua_notes = sizeof(acertou_agua_melody) / sizeof(acertou_agua_melody[0]) / 2;


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
    delay(noteDuration + 40);   // ⏱️ Pausa extra entre as notas
    noTone(buzzer);
  }
}

// New function for end of round sound
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

// New function for end of game sound
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

// Function for a hit sound
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
    delay(noteDuration + 10); // Shorter delay for a quicker sound
    noTone(buzzer);
  }
}

// NEW: Function for a miss sound
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
    delay(noteDuration + 20); // Small delay for splash effect
    noTone(buzzer);
  }
}
// --- End of Buzzer section ---


void Texto(LiquidCrystal_I2C &lcd, int a, int b, const char* texto1, int c, int d, const char* texto2) {
  lcd.clear();
  lcd.setCursor(a, b);
  lcd.print(texto1);
  lcd.setCursor(c, d);
  lcd.print(texto2);
}

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
  vencedor = 0; // Reset winner

  for (int i = 0; i < NUM_LEDS; i++) {
    fita.setPixelColor(i, 0); // Turn off all LEDs
    acertosJogador1[i] = false;
    acertosJogador2[i] = false;
    errosJogador1[i] = false;
    errosJogador2[i] = false;
  }
  fita.show();

  Texto(lcd1, 0, 0, "1o Jogador", 0, 1, "Selecione barco3");
  inicio_jogo_som(); // Adicionado aqui para tocar ao reiniciar o jogo
}


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
      fita.setPixelColor(i, 0); // Turn off all LEDs
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

// Checks if a position is already occupied by the current player's own groups
bool posicaoOcupada(int lin, int col) {
  if (!segundaRodada) { // Player 1 is placing
    for (int i = 0; i < totalGrupo1; i++) {
      if (grupo1[i].linha == lin && grupo1[i].coluna == col) return true;
    }
    for (int i = 0; i < totalGrupo2; i++) {
      if (grupo2[i].linha == lin && grupo2[i].coluna == col) return true;
    }
    for (int i = 0; i < totalGrupo3; i++) {
      if (grupo3[i].linha == lin && grupo3[i].coluna == col) return true;
    }
  } else { // Player 2 is placing
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


bool ehAdjacenteGrupo1(int lin, int col) {
  if (!segundaRodada) { // Player 1's groups
    if (totalGrupo1 == 0) return true; // First position can be anywhere

    for (int i = 0; i < totalGrupo1; i++) {
      int diffLin = abs(grupo1[i].linha - lin);
      int diffCol = abs(grupo1[i].coluna - col);

      if ((diffLin == 0 && diffCol == 1) || (diffCol == 0 && diffLin == 1)) {
        return true; // Adjacent horizontally or vertically
      }
    }
  } else { // Player 2's new groups
    if (totalNovoGrupo1 == 0) return true; // First position can be anywhere

    for (int i = 0; i < totalNovoGrupo1; i++) {
      int diffLin = abs(novoGrupo1[i].linha - lin);
      int diffCol = abs(novoGrupo1[i].coluna - col);

      if ((diffLin == 0 && diffCol == 1) || (diffCol == 0 && diffLin == 1)) {
        return true; // Adjacent horizontally or vertically
      }
    }
  }
  return false;
}

bool ehAdjacenteGrupo2(int lin, int col) {
  if (!segundaRodada) { // Player 1's groups
    if (totalGrupo2 == 0) return true; // First position can be anywhere

    // Only one position for Group 2 to be adjacent to
    int diffLin = abs(grupo2[0].linha - lin);
    int diffCol = abs(grupo2[0].coluna - col);

    return (diffLin == 0 && diffCol == 1) || (diffCol == 0 && diffLin == 1);
  } else { // Player 2's new groups
    if (totalNovoGrupo2 == 0) return true; // First position can be anywhere

    // Only one position for Novo Grupo 2 to be adjacent to
    int diffLin = abs(novoGrupo2[0].linha - lin);
    int diffCol = abs(novoGrupo2[0].coluna - col);

    return (diffLin == 0 && diffCol == 1) || (diffCol == 0 && diffLin == 1);
  }
}

void adicionarIntersecao(int lin, int col) {
  if (posicaoOcupada(lin, col)) {
    // If the position is already occupied by the current player's own ships,
    // don't add it.
    return;
  }
  
  if (modoGrupo1) {
    if (!segundaRodada) { // Player 1's groups
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
    } else { // Player 2's new groups
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
    if (!segundaRodada) { // Player 1's groups
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
    } else { // Player 2's new groups
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
    if (!segundaRodada) { // Player 1's groups
      if (totalGrupo3 < 1) {
        grupo3[totalGrupo3].linha = lin;
        grupo3[totalGrupo3].coluna = col;
        totalGrupo3++;

        if (totalGrupo3 == 1) {
          modoGrupo3 = false;
          Texto(lcd1, 0, 0, "Concluido!", 0, 1, "Aguardando...");
          aguardandoReinicio = true;
          tempoFinalizacao = millis();
          // Call the end of round sound for Player 1
          fim_rodada_som();
        }
      }
    } else { // Player 2's new groups
      if (totalNovoGrupo3 < 1) {
        novoGrupo3[totalNovoGrupo3].linha = lin;
        novoGrupo3[totalNovoGrupo3].coluna = col;
        totalNovoGrupo3++;

        if (totalNovoGrupo3 == 1) {
          modoGrupo3 = false;
          Texto(lcd1, 0, 0, "Concluido!", 0, 1, "Aguardando...");
          aguardandoReinicio = true;
          tempoFinalizacao = millis();
          // Call the end of round sound for Player 2
          fim_rodada_som();
        }
      }
    }
  }
}

void mostrarColuna(int col) {
  for (int i = 0; i < 6; i++) {
    int index = col + i * 6;
    fita.setPixelColor(index, fita.Color(255, 50, 0)); // Orange for active column
  }
}

void mostrarLinha(int lin) {
  for (int i = 0; i < 6; i++) {
    int index = lin * 6 + i;
    fita.setPixelColor(index, fita.Color(0, 0, 139)); // Dark Blue for active row
  }
}

void mostrarIntersecao(int lin, int col, uint32_t cor) {
  int index = col + lin * 6;
  fita.setPixelColor(index, cor);
}

void mostrarPossiveisIntersecoes() {
  if (modoGrupo1) {
    if (!segundaRodada) { // Player 1's groups
      if (totalGrupo1 == 0) return; // No points yet, no possible intersections
      else if (totalGrupo1 == 1) {
        int lin = grupo1[0].linha;
        int col = grupo1[0].coluna;

        int possiveis[4][2] = {{lin - 1, col}, {lin + 1, col}, {lin, col - 1}, {lin, col + 1}};

        for (int i = 0; i < 4; i++) {
          int l = possiveis[i][0];
          int c = possiveis[i][1];
          // Check if position is within bounds and not occupied by player 1's own ships
          if (l >= 0 && l < 6 && c >= 0 && c < 6 && !posicaoOcupada(l, c)) {
            int idx = c + l * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0)); // Light orange for possible adjacent
          }
        }
      }
      else if (totalGrupo1 == 2) {
        int lin0 = grupo1[0].linha;
        int col0 = grupo1[0].coluna;
        int lin1 = grupo1[1].linha;
        int col1 = grupo1[1].coluna;

        if (lin0 == lin1) { // Horizontal line
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
        else if (col0 == col1) { // Vertical line
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
    } else { // Player 2's new groups
      if (totalNovoGrupo1 == 0) return;
      else if (totalNovoGrupo1 == 1) {
        int lin = novoGrupo1[0].linha;
        int col = novoGrupo1[0].coluna;

        int possiveis[4][2] = {{lin - 1, col}, {lin + 1, col}, {lin, col - 1}, {lin, col + 1}};

        for (int i = 0; i < 4; i++) {
          int l = possiveis[i][0];
          int c = possiveis[i][1];
          // Check if position is within bounds and not occupied by player 2's own ships
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
    if (!segundaRodada) { // Player 1's groups
      if (totalGrupo2 == 0) return;
      else if (totalGrupo2 == 1) {
        int lin = grupo2[0].linha;
        int col = grupo2[0].coluna;

        int possiveis[4][2] = {{lin - 1, col}, {lin + 1, col}, {lin, col - 1}, {lin, col + 1}};

        for (int i = 0; i < 4; i++) {
          int l = possiveis[i][0];
          int c = possiveis[i][1];
          // Check if position is within bounds and not occupied by player 1's own ships
          if (l >= 0 && l < 6 && c >= 0 && c < 6 && !posicaoOcupada(l, c)) {
            int idx = c + l * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
        }
      }
    } else { // Player 2's new groups
      if (totalNovoGrupo2 == 0) return;
      else if (totalNovoGrupo2 == 1) {
        int lin = novoGrupo2[0].linha;
        int col = novoGrupo2[0].coluna;

        int possiveis[4][2] = {{lin - 1, col}, {lin + 1, col}, {lin, col - 1}, {lin, col + 1}};

        for (int i = 0; i < 4; i++) {
          int l = possiveis[i][0];
          int c = possiveis[i][1];
          // Check if position is within bounds and not occupied by player 2's own ships
          if (l >= 0 && l < 6 && c >= 0 && c < 6 && !posicaoOcupada(l, c)) {
            int idx = c + l * 6;
            fita.setPixelColor(idx, fita.Color(255, 150, 0));
          }
        }
      }
    }
  }
}

// Determines if a coordinate belongs to the current player's opponent's groups
bool pertenceAosGruposDoOponente(int lin, int col) {
  if (jogador1Atacando) { // Player 1 is attacking, so check Player 2's groups
    for (int i = 0; i < totalNovoGrupo1; i++) {
      if (novoGrupo1[i].linha == lin && novoGrupo1[i].coluna == col) return true;
    }
    for (int i = 0; i < totalNovoGrupo2; i++) {
      if (novoGrupo2[i].linha == lin && novoGrupo2[i].coluna == col) return true;
    }
    for (int i = 0; i < totalNovoGrupo3; i++) {
      if (novoGrupo3[i].linha == lin && novoGrupo3[i].coluna == col) return true;
    }
  } else { // Player 2 is attacking, so check Player 1's groups
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

void verificarVitoria() {
  int totalLedsOponente = 0;
  int acertosDoJogador = 0;

  if (jogador1Atacando) { // Player 1 is attacking, so check Player 2's groups
    totalLedsOponente = totalNovoGrupo1 + totalNovoGrupo2 + totalNovoGrupo3;
    for (int i = 0; i < NUM_LEDS; i++) {
      // Convert LED index back to row and column
      int row = i / 6;
      int col = i % 6;
      if (acertosJogador1[i] && pertenceAosGruposDoOponente(row, col)) {
        acertosDoJogador++;
      }
    }
    // Check if player 1 has hit all of player 2's groups
    if (totalLedsOponente > 0 && acertosDoJogador >= totalLedsOponente) {
      vencedor = 1;
      jogoFinalizado = true;
      tempoVitoria = millis();
      Texto(lcd1, 0, 0, "Vitoria do", 0, 1, "Jogador 1!");
      // Turn all LEDs green for victory
      for (int i = 0; i < NUM_LEDS; i++) {
        fita.setPixelColor(i, fita.Color(0, 255, 0)); // Green
      }
      fita.show();
      fim_jogo_som(); // Play victory sound
    }
  } else { // jogador2Atacando
    totalLedsOponente = totalGrupo1 + totalGrupo2 + totalGrupo3;
    for (int i = 0; i < NUM_LEDS; i++) {
      // Convert LED index back to row and column
      int row = i / 6;
      int col = i % 6;
      if (acertosJogador2[i] && pertenceAosGruposDoOponente(row, col)) {
        acertosDoJogador++;
      }
    }
    // Check if player 2 has hit all of player 1's groups
    if (totalLedsOponente > 0 && acertosDoJogador >= totalLedsOponente) {
      vencedor = 2;
      jogoFinalizado = true;
      tempoVitoria = millis();
      Texto(lcd1, 0, 0, "Vitoria do", 0, 1, "Jogador 2!");
      // Turn all LEDs green for victory
      for (int i = 0; i < NUM_LEDS; i++) {
        fita.setPixelColor(i, fita.Color(0, 255, 0)); // Green
      }
      fita.show();
      fim_jogo_som(); // Play victory sound
    }
  }
}

void iniciarModoAtaque() {
  modoAtaque = true;
  jogador1Atacando = true;
  jogador2Atacando = false;
  jogoFinalizado = false;
  vencedor = 0;

  // Clear all LEDs when entering attack mode
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

void processarAtaque(int lin, int col) {
  int index = col + lin * 6;

  // Check if the current player has already hit or missed this spot
  // This check is important to prevent re-processing the same hit/miss
  if ((jogador1Atacando && (acertosJogador1[index] || errosJogador1[index])) ||
      (jogador2Atacando && (acertosJogador2[index] || errosJogador2[index]))) {
    return; // Already processed this intersection for the current player
  }

  if (jogador1Atacando) {
    if (pertenceAosGruposDoOponente(lin, col)) { // Player 1 is attacking Player 2's groups
      acertosJogador1[index] = true;
      acertou_barco_som(); // Play hit sound!
      // Do NOT show the LED here. It will be handled in the loop based on the acertos arrays.
      Texto(lcd1, 0, 0, "Acerto!", 0, 1, "Jogador 1 continua");
    } else {
      errosJogador1[index] = true;
      acertou_agua_som(); // NEW: Play miss sound!
      // Do NOT show the LED here. It will be handled in the loop based on the erros arrays.
      Texto(lcd1, 0, 0, "Errou!", 0, 1, "Vez do Jogador 2");
      fita.show(); // Show the miss immediately
      delay(1000); // 1 SECOND DELAY FOR MISS
      jogador1Atacando = false;
      jogador2Atacando = true;
    }
  } else { // jogador2Atacando
    if (pertenceAosGruposDoOponente(lin, col)) { // Player 2 is attacking Player 1's groups
      acertosJogador2[index] = true;
      acertou_barco_som(); // Play hit sound!
      // Do NOT show the LED here. It will be handled in the loop based on the acertos arrays.
      Texto(lcd1, 0, 0, "Acerto!", 0, 1, "Jogador 2 continua");
    } else {
      errosJogador2[index] = true;
      acertou_agua_som(); // NEW: Play miss sound!
      // Do NOT show the LED here. It will be handled in the loop based on the erros arrays.
      Texto(lcd1, 0, 0, "Errou!", 0, 1, "Vez do Jogador 1");
      fita.show(); // Show the miss immediately
      delay(1000); // 1 SECOND DELAY FOR MISS
      jogador1Atacando = true;
      jogador2Atacando = false;
    }
  }
  // Immediately refresh the LEDs to show the result of the attack
  fita.show(); // This line is crucial for the immediate visual feedback
  tempoUltimoAtaque = millis();
  verificarVitoria();
}


void setup() {
  Serial.begin(9600);

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
  lcd2.clear(); // Clear LCD2 on startup

  Texto(lcd1, 2, 0, "1o Jogador:", 0, 1, "Selecione barco3");
  inicio_jogo_som();
  // Play the start-game melody
}

void loop() {
  if (jogoFinalizado) {
    // Keep victory message on LCD and LEDs for 10 seconds, then reset
    if (millis() - tempoVitoria >= 10000) { // 10 seconds
      resetGame();
    }
    return; // Don't process any other game logic while finalized
  }

  if (aguardandoReinicio) {
    if (millis() - tempoFinalizacao >= 2000) { // 2 seconds delay
      if (segundaRodada) {
        // Clear all LEDs and attack/miss flags before starting attack mode
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
        reiniciarSistema();
      }
    }
    return; // Don't process input while awaiting restart
  }

  int botoesLinha[] = {btn1, btn2, btn3, btn4, btn5, btn6};
  int botoesColuna[] = {btnA, btnB, btnC, btnD, btnE, btnF};

  // Process column buttons
  for (int i = 0; i < 6; i++) {
    bool estadoAtual = digitalRead(botoesColuna[i]);
    if (btnStatesColuna[i] == HIGH && estadoAtual == LOW) { // Button pressed
      colunaAtiva = i;
      linhaAtiva = -1; // Reset active row
      linhaHabilitada = true;
      mostrandoLinha = false;
    }
    btnStatesColuna[i] = estadoAtual;
  }

  // Handle temporary display of selected line/column
  if (mostrandoLinha) {
    if (millis() - tempoLinhaLigada >= 300) { // Display intersection for 300ms
      mostrandoLinha = false;
      colunaAtiva = -1; // Clear active column after timeout
      linhaAtiva = -1; // Clear active row as well
    }
  }
  // Process row buttons only if a column is active
  else if (linhaHabilitada) {
    for (int i = 0; i < 6; i++) {
      bool estadoAtual = digitalRead(botoesLinha[i]);
      if (btnStatesLinha[i] == HIGH && estadoAtual == LOW) { // Button pressed
        linhaAtiva = i;
        linhaHabilitada = false;
        mostrandoLinha = true;
        tempoLinhaLigada = millis();

        if (colunaAtiva != -1) { // Both row and column selected
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
    // Keep reading line button states to ensure they are up-to-date even when not actively selecting
    for (int i = 0; i < 6; i++) {
      btnStatesLinha[i] = digitalRead(botoesLinha[i]);
    }
  }

  // Clear all LEDs before drawing current state, unless game is finalized (victory screen)
  if (!jogoFinalizado) {
    for (int i = 0; i < NUM_LEDS; i++) {
      fita.setPixelColor(i, 0); // Turn off all LEDs
    }
  }

  // Display active column
  if (colunaAtiva != -1 && !mostrandoLinha && !jogoFinalizado) { // Only show active column if not showing intersection
    mostrarColuna(colunaAtiva);
  }

  // Display active row if `mostrandoLinha` is true and game is not finalized
  if (mostrandoLinha && linhaAtiva != -1 && !jogoFinalizado) {
    mostrarLinha(linhaAtiva);
    // Also show the potential intersection color when both are active
    if (colunaAtiva != -1) {
      if (modoAtaque) {
        // During attack, briefly show potential hit/miss color
        if (pertenceAosGruposDoOponente(linhaAtiva, colunaAtiva)) {
            if (jogador1Atacando) mostrarIntersecao(linhaAtiva, colunaAtiva, fita.Color(0, 255, 0)); // Green for potential hit by P1
            else mostrarIntersecao(linhaAtiva, colunaAtiva, fita.Color(0, 0, 255)); // Blue for potential hit by P2
        } else {
            mostrarIntersecao(linhaAtiva, colunaAtiva, fita.Color(255, 0, 0)); // Red for potential miss
        }
      } else {
        // During group placement, show the selected intersection
        if (!segundaRodada) {
          mostrarIntersecao(linhaAtiva, colunaAtiva, fita.Color(0, 255, 0)); // Green for Player 1's selection
        } else {
          mostrarIntersecao(linhaAtiva, colunaAtiva, fita.Color(255, 20, 147)); // Pink for Player 2's selection
        }
      }
    }
  }

  // Display placed groups or attack marks (only if not in victory screen)
  if (!jogoFinalizado) {
    if (!modoAtaque) {
      // During group placement, show the placed groups
      if (!segundaRodada) { // Player 1's groups (initial setup)
        for (int i = 0; i < totalGrupo1; i++) {
          mostrarIntersecao(grupo1[i].linha, grupo1[i].coluna, fita.Color(0, 255, 0)); // Green for Player 1's groups
        }
        for (int i = 0; i < totalGrupo2; i++) {
          mostrarIntersecao(grupo2[i].linha, grupo2[i].coluna, fita.Color(0, 255, 0));
        }
        for (int i = 0; i < totalGrupo3; i++) {
          mostrarIntersecao(grupo3[i].linha, grupo3[i].coluna, fita.Color(0, 255, 0));
        }
      } else { // Player 2's groups (second round setup)
        for (int i = 0; i < totalNovoGrupo1; i++) {
          mostrarIntersecao(novoGrupo1[i].linha, novoGrupo1[i].coluna, fita.Color(255, 20, 147)); // Pink for Player 2's groups
        }
        for (int i = 0; i < totalNovoGrupo2; i++) {
          mostrarIntersecao(novoGrupo2[i].linha, novoGrupo2[i].coluna, fita.Color(255, 20, 147));
        }
        for (int i = 0; i < totalNovoGrupo3; i++) {
          mostrarIntersecao(novoGrupo3[i].linha, novoGrupo3[i].coluna, fita.Color(255, 20, 147));
        }
      }
    } else { // In attack mode, show only hits/misses for the current player
      for (int i = 0; i < NUM_LEDS; i++) {
        if (jogador1Atacando) {
          if (acertosJogador1[i]) {
            fita.setPixelColor(i, fita.Color(0, 255, 0)); // Green for P1's hits
          } else if (errosJogador1[i]) {
            fita.setPixelColor(i, fita.Color(13,33,79)); // Dark Blue for P1's misses
          }
        } else { // jogador2Atacando
          if (acertosJogador2[i]) {
            fita.setPixelColor(i, fita.Color(255, 16, 240)); // Light Pink for P2's hits
          } else if (errosJogador2[i]) {
            fita.setPixelColor(i, fita.Color(13,33,79)); // Dark Blue for P2's misses
          }
        }
      }
    }
  }

  // Display possible intersections only during group selection phase
  if (!modoAtaque && (modoGrupo1 || modoGrupo2)) {
    mostrarPossiveisIntersecoes();
  }

  fita.show();
  delay(30);
}
