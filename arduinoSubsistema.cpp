const int ledPin = 7;
const int buzzerPin = 8;
const int buttonPin = 2;
const int lm35Pin = A1;

volatile bool buttonPressed = false;
volatile bool readTemperatureFlag = false;
float temperatureReadings[10];
int tempIndex = 0;
bool LED = LOW;
int measurementCount = 0;
unsigned long currentMillis; 

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  analogReference(INTERNAL1V1);
 
  // Configura o Timer1 para gerar uma interrupção a cada segundo
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 31249;   // Para 1 Hz com um prescaler de 256
  TCCR1B = TCCR1B | 0b00001000;   // Configura o prescaler para 256
  TCCR1B = TCCR1B | 0b00000101;   // Configura o modo CTC
  TIMSK1 = TIMSK1 | 0b00000010;   // Ativa a interrupção para o Timer1
 
  interrupts();
 
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);
  
  Serial.begin(9600);   
  Serial.println("Modulo-B");
  Serial3.begin(9600); // Comunicação com o modulo A - leitura
  Serial2.begin(9600); // Comunicação com o modulo A - escrita
}

void loop() {
  if (Serial3.available()) {
    String caractere = Serial3.readStringUntil('\n');
    Serial.print("Recebi da base ");
    caractere.trim();
    Serial.println(caractere);
    
    if (caractere == "A") {
      changeLedToHigh();
    }
    if (caractere == "B") { 
      changeLedToLow();
    }
    if (caractere == "C") {
      changeBuzzerToHigh();
    }
    if (caractere == "D") { 
      changeBuzzerToLow();
    }
  }
   
   if (buttonPressed) { sendButtonStatus(); }
   readTemperature();
}

// ISR do Timer1
ISR(TIMER1_COMPA_vect) {
  int reading = analogRead(lm35Pin);
  float temperature = reading * 0.1075268817;
  temperatureReadings[tempIndex] = temperature;
  tempIndex = (tempIndex + 1) % 10; // Atualiza o índice do array circular
  measurementCount++; // Incrementa a contagem de medições
}


void readTemperature() {
  float sum = 0;
  // Calcula a média das últimas 10 leituras, se completou 10 medições
  if (measurementCount == 10) {
    for (int i = 0; i < 10; i++) {
      sum += temperatureReadings[i];
    }
    measurementCount = 0;
    sendTemperature(sum / 10); // Retorna a média das leituras
  }
}

void sendTemperature(float temperature) {
  Serial.print("TEMP_");
  Serial.println(temperature);
  Serial2.print("TEMP_");
  Serial2.println(temperature);
}

void buttonISR() {
    buttonPressed = true;
}

void sendButtonStatus() {
    Serial.println("BOTAO_PRESSIONADO");
    Serial2.println("BOTAO_PRESSIONADO");
    buttonPressed = false;
}

void changeBuzzerToHigh() {
    Serial.println("BUZZER_ON");
    digitalWrite(buzzerPin, HIGH);
}

void changeBuzzerToLow() {
    Serial.println("BUZZER_OFF");
    digitalWrite(buzzerPin, LOW);
}

void changeLedToHigh() {
    Serial.println("LED_ON");
    digitalWrite(ledPin, HIGH);
}

void changeLedToLow() {
    Serial.println("LED_ON");
    digitalWrite(ledPin, LOW);
}
