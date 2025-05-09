#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

const char* string[]={"Bem vindo!","Vamos iniciar!"}; // Lista[0,...]

void setup(){
lcd.init();
lcd.backlight();
}

void Texto(int a, int b, int c, int d, int x, int y){ // [A-(Linha de cima)| B-(Sempre 0)|C-(Linha de baixo)| D-(Sempre 1)| X-(Texto de cima na lista)| Y-(Texto de baixo na lista)]
lcd.setCursor(a, b); 
lcd.print(string[x]);
lcd.setCursor(c, d); 
lcd.print(string[y]);
}
void loop()
{
  Texto(3,0,1,1,0,1);
}