// Arduino Base

#include <LiquidCrystal_I2C.h>


// Crie uma instância do display LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Endereço I2C do display, 16 colunas e 2 linhas
int estadoBotao = LOW;
int estadoLed = LOW;
int estadoBuzzer = LOW;
unsigned long inicioMed = 0;

int estadoInterface = -1;
int notificacoes = true;
//int opcaoSelecionada = 0;
const String botaoPressionado = "BOTAO_PRESSIONADO";

void setup() {
  lcd.init();      // Inicializa o display
  lcd.backlight(); // Ativa a luz de fundo do display
  Serial.begin(9600); // Inicializa a comunicação Serial1 com o módulo-B
  Serial2.begin(9600); // Inicializa a comunicação Serial1 com o módulo-B
  Serial3.begin(9600); // Inicializa a comunicação Serial1 com o módulo-B
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
  Serial.println(" (5) Desligar Notificacoes");
  Serial.println(" (6) Ligar Notificacoes");
  Serial.println("---------------------------------------------");
}

void loop() {
  if(millis() - inicioMed == 2000) {
    lcd.clear();
  }
  if (Serial.available()) {
    String opcao = Serial.readStringUntil('\n');
    enviarSelecaoParaSubsistema(opcao);
  }
 
  if (Serial2.available()) { // Verifica se há dados recebidos
    String message = Serial2.readStringUntil('\n'); // Lê a mensagem até a quebra de linha
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
      lcd.print("BOTAO ON ");
      estadoBotao = HIGH;
      inicioMed = millis();
    } else if (message == "BOTAO_SOLTO") {
      Serial.println("Recebido: Botão foi solto");
      estadoBotao = LOW;
    } else {
      Serial.println(message);
      if (notificacoes) {
        lcd.print(message);
        lcd.print(" ");
        inicioMed = millis();
      }
    }
}

void enviarSelecaoParaSubsistema(String opcao) {
  if (opcao == "1") {
    lcd.print("LED ON ");
    inicioMed = millis();
    Serial3.println("A");
    estadoLed = HIGH;
  }

   if (opcao == "2") {
    lcd.print("LED OFF ");
    inicioMed = millis();
    Serial3.println("B");
    estadoLed = LOW;
  }

  if (opcao == "3") {
    lcd.print("BUZZER ON ");
    inicioMed = millis();
    Serial3.println("C");
    estadoBuzzer = HIGH;
  }

   if (opcao == "4") {
    lcd.print("BUZZER OFF ");
    inicioMed = millis();
    Serial3.println("D");
    estadoBuzzer = LOW;
  }

  if (opcao == "5") {
    notificacoes = false;
  }
 
  if (opcao == "6") {
    notificacoes = true;
  }
}
