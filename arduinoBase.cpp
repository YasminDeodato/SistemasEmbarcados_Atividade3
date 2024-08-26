
// Arduino Base

#include <LiquidCrystal_I2C.h>


// Crie uma instância do display LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Endereço I2C do display, 16 colunas e 2 linhas
int estadoBotao = LOW;
int estadoLed = LOW;
int estadoBuzzer = LOW;

int estadoInterface = -1;
//int opcaoSelecionada = 0;
const String botaoPressionado = "BOTAO_PRESSIONADO";

void setup() {
  lcd.init();      // Inicializa o display
  lcd.backlight(); // Ativa a luz de fundo do display
  Serial.begin(9600); // Inicializa a comunicação serial com o módulo-B
  Serial1.begin(9600); // Inicializa a comunicação serial com o módulo-B
  Serial.println("Modulo-A pronto.");
}

void menuInicial() {
  estadoInterface = 0;
  Serial.println("---------------------------------------------");
  Serial.println("Escolha a opcao desejada");
  Serial.println(" (1) Ligar LED");
  Serial.println(" (2) Desligar LED");
  Serial.println(" (3) Ligar Buzzer");
  Serial.println(" (4) Desligar Buzzer");
  Serial.println("---------------------------------------------");
}

void loop() {
  if (Serial.available()) {
    String opcao = Serial.readStringUntil('\n');
    enviarSelecaoParaSubsistema(opcao);

    Serial.print("BUZZER ");
    Serial.print(estadoBuzzer);
    
    Serial.print("| LED ");
    Serial.print(estadoLed);
    
  }
  
  if (Serial1.available()) { // Verifica se há dados recebidos
    String message = Serial1.readStringUntil('\n'); // Lê a mensagem até a quebra de linha
    message.trim();
    handleSerialMessage(message); // Processa a mensagem recebida
  }
  if (estadoInterface == -1) menuInicial();
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

void enviarSelecaoParaSubsistema(String opcao) {
  if (opcao == "1") {
    Serial1.println('A');
    estadoLed = HIGH;
  }

   if (opcao == "2") {
    Serial1.println("B");
    estadoLed = LOW;
  }

  if (opcao == "3") {
    Serial1.println("C");
    estadoBuzzer = HIGH;
  }

   if (opcao == "4") {
    Serial1.println("D");
    estadoBuzzer = LOW;
  }
}
