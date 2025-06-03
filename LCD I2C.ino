#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd1(0x26, 16, 2); 
LiquidCrystal_I2C lcd2(0x27, 16, 2);  

const char* string[] = {"", "Hora de navegar!", "Todos a bordo?", 
"Esconda navios", "na zona rival","Posicione o","barco de 3 casas",
"Toque uma casa", "amarela inicial!","Toque na segunda","casa amarela",
"Escolha o fim","do barco 3","barco de 2 casas","do barco 2","barco de 1 casa!",
"Selecione uma","casa amarela","Duas escolhas","te aguardam:","Clique no barco",
"e mude de lugar","ou aperte duas","vezes para jogar","Barco cancelado!","Vamos de novo",
"Hora de iniciar","a pontaria!","Sua vez de jogar","marujo!","Respire fundo...","vez do oponente",
"Sem sorte...","Agua!","Acertou o trecho","do navio inimigo","Bom tiro, marujo","acertou o barco!",
"Acertaram parte","do seu navio!","O inimigo achou","o seu barco!","Jogue de novo!","Boa jogada!",
"Essa casa ta","ocupada, marujo!","Bom jogo! Vamos","jogar de novo?","Vitoria, marujo!","Bom trabalho",
"Tente novamente:","Casa ocupada","Um clique de","cada vez marujo!"};

void setup() {
  lcd1.init();
  lcd1.backlight();
  
  lcd2.init();
  lcd2.backlight();
}

void Texto(LiquidCrystal_I2C &lcd, int a, int b, const char* texto1, int c, int d, const char* texto2) {
  lcd.setCursor(a, b);
  lcd.print(texto1);
  lcd.setCursor(c, d);
  lcd.print(texto2);
}

void loop() {
  Texto(lcd1, 0, 0, string[1], 1, 1, string[2]); // "Hora de navegar!" e "Todos a bordo?"
  
  Texto(lcd1, 1, 0, string[3], 1, 1, string[4]); // "Esconda navios" e "na zona rival"
  
  Texto(lcd1, 2, 0, string[5], 0, 1, string[6]); // "Posicione o" e "barco de 3 casas"
  
  Texto(lcd1, 1, 0, string[7], 0, 1, string[8]); // "Toque uma casa" e "amarela inicial!"
  
  Texto(lcd1, 0, 0, string[9], 2, 1, string[10]); // "Toque na segunda" e "casa amarela"
  
  Texto(lcd1, 1, 0, string[11], 3, 1, string[12]); // "Escolha o fim" e "do barco 3"
  
  Texto(lcd1, 2, 0, string[5], 0, 1, string[13]); // "Posicione o" e "barco de 2 casas"
  
  Texto(lcd1, 1, 0, string[7], 0, 1, string[8]); // "Toque uma casa" e "amarela inicial!"
  
  Texto(lcd1, 1, 0, string[11], 3, 1, string[14]); // "Escolha o fim" e "do barco 2"
  
  Texto(lcd1, 2, 0, string[5], 0, 1, string[15]); // "Posicione o" e "barco de 1 casa!"
  
  Texto(lcd1, 1, 0, string[16], 2, 1, string[17]); // "Selecione uma" e "casa amarela"
  
  Texto(lcd1, 1, 0, string[18], 2, 1, string[19]); // "Duas escolhas" e "te aguardam:"
  
  Texto(lcd1, 0, 0, string[20], 0, 1, string[21]); // "Clique no barco" e "e mude de lugar"
  
  Texto(lcd1, 0, 0, string[22], 0, 1, string[23]); // "ou aperte duas" e "vezes para jogar"
  
  Texto(lcd1, 0, 0, string[24], 1, 1, string[25]); // "Barco cancelado!" e "Vamos de novo"
  
  Texto(lcd1, 0, 0, string[26], 2, 1, string[27]); // "Hora de iniciar" e "a pontaria!"
  
  Texto(lcd1, 0, 0, string[28], 4, 1, string[29]); // "Sua vez de jogar" e "marujo!"
  
  Texto(lcd1, 0, 0, string[30], 0, 1, string[31]); // "Respire fundo..." e "vez do oponente"
  
  Texto(lcd1, 2, 0, string[32], 5, 1, string[33]); // "Sem sorte..." e "Agua!"
  
  Texto(lcd1, 0, 0, string[34], 0, 1, string[35]); // "Acertou o trecho" e "do navio inimigo"
  
  Texto(lcd1, 0, 0, string[36], 0, 1, string[37]); // "Bom tiro, marujo" e "acertou o barco!"
  
  Texto(lcd1, 0, 0, string[38], 1, 1, string[39]); // "Acertaram parte" e "do seu navio!"
  
  Texto(lcd1, 0, 0, string[40], 2, 1, string[41]); // "O inimigo achou" e "o seu barco!"
  
  Texto(lcd1, 1, 0, string[42], 2, 1, string[43]); // "Jogue de novo!" e "Boa jogada!"
  
  Texto(lcd1, 2, 0, string[44], 0, 1, string[45]); // "Essa casa ta" e "ocupada, marujo!"
  
  Texto(lcd1, 0, 0, string[46], 0, 1, string[47]); // "Bom jogo! Vamos" e "jogar de novo?"
  
  Texto(lcd1, 0, 0, string[48], 2, 1, string[49]); // "Vitoria, marujo!" e "Bom trabalho"
  
  Texto(lcd1, 0, 0, string[50], 2, 1, string[51]); // "Tente novamente:" e "Casa ocupada"
  
  Texto(lcd1, 2, 0, string[52], 0, 1, string[53]); // "Um clique de" e "cada vez marujo!"
}
