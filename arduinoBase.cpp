// Arduíno 1

#include <LiquidCrystal_I2C.h>

//Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,16,2);

int estadoBotao = LOW;
const String botaoPressionado = "BOTAO_PRESSIONADO";

void setup() {
  lcd.init();
  Serial.begin(9600);
  lcd.setBacklight(HIGH);
}

void loop() {
  //Serial.print('A');

          
  if (Serial.available()) {
  	String mensagem = Serial.readStringUntil('\n');
    mensagem.trim();
    
    if (mensagem.equals(botaoPressionado)) {
    	Serial.println("Recebido: botao foi pressionado");
    	Serial.println("A");
      lcd.setCursor(0,0);
      lcd.print("BOTAO ON");
      estadoBotao = HIGH;
      delay(1000);
      lcd.clear();
    } else if (mensagem == "BOTAO_SOLTO") {
      Serial.println("Recebido: Botão foi solto");
      estadoBotao = LOW;
    }
  }
  //delay(500);
}