// Arduíno 1

#include <LiquidCrystal_I2C.h>
//Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,16,2);
int incomingByte, x, y;

void setup() {
  lcd.init();
  Serial.begin(9600);
  lcd.setBacklight(HIGH);
}

void loop() {
  Serial.print('A');
  delay(500);
}