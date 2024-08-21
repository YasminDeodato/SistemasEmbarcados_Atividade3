// Arduíno 1

#include <LiquidCrystal_I2C.h>


// Crie uma instância do display LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Endereço I2C do display, 16 colunas e 2 linhas
int estadoBotao = LOW;
const String botaoPressionado = "BOTAO_PRESSIONADO";

void setup() {
  lcd.init();      // Inicializa o display
  lcd.backlight(); // Ativa a luz de fundo do display
  Serial.begin(9600); // Inicializa a comunicação serial com o módulo-B
  Serial.println("Módulo-A pronto.");
}

void loop() {
  if (Serial.available()) { // Verifica se há dados recebidos
    String message = Serial.readStringUntil('\n'); // Lê a mensagem até a quebra de linha
    message.trim();
    handleSerialMessage(message); // Processa a mensagem recebida
  }
}

void handleSerialMessage(String message) {
  Serial.println(message);
   if (message.equals(botaoPressionado)) {
    Serial.println("Recebido: botao foi pressionado");
      lcd.setCursor(0,0);
      lcd.print("BOTAO ON");
      estadoBotao = HIGH;
      delay(1000);
      lcd.clear();
    } else if (message == "BOTAO_SOLTO") {
      Serial.println("Recebido: Botão foi solto");
      estadoBotao = LOW;
    }
}
