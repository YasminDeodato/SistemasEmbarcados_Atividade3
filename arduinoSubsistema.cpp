// Arduíno 2
const int ledPin = 12;
const int buzzerPin = 8;
const int buttonPin = 3;
const int lm35Pin = A0;

int caractere = 0;

volatile bool buttonPressed = false;
volatile bool readTemperatureFlag = false;
float temperatureReadings[10];
int tempIndex = 0;
bool LED = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
 
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 31249;
  TCCR1B = TCCR1B | 0b00001000;
  TCCR1B = TCCR1B | 0b00000101;
  TIMSK1 = TIMSK1 | 0b00000010;
 
  interrupts();
 
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);
 
  Serial.begin(9600);  // Comunicação com o módulo-A
}

void loop() {
  unsigned long currentMillis = millis();
 
  changeLedStatus();
 
  sendButtonStatus();
 
    if (readTemperatureFlag) {
      readTemperatureFlag = false;
      float temperature = readTemperature(); // Lê a temperatura
      sendTemperature(temperature); // Envia a temperatura para o módulo-A
  }
   
}

// ISR do Timer1
ISR(TIMER1_COMPA_vect) {
  readTemperatureFlag = true; // Ativa a flag para ler a temperatura
}

void buttonISR() {
  buttonPressed = true;
}

float readTemperature() {
  float sum = 0;
  for (int i = 0; i < 10; i++) {
    int reading = analogRead(lm35Pin);
    float voltage = reading * 5.0 / 1024.0;
    float temperature = voltage * 100;
    temperatureReadings[tempIndex] = temperature;
    tempIndex = (tempIndex + 1) % 10;
    sum += temperature;
  }
  return sum / 10; // Retorna a média das leituras
}

void sendTemperature(float temperature) {
  Serial.print("T:");
  Serial.println(temperature);
}

void changeLedStatus(){
if (Serial.available()) {
      caractere = Serial.read();
      Serial.println(caractere);


      if(caractere == 65){
        LED = !LED;
        digitalWrite(ledPin, LED);
      }
  }
}

void sendButtonStatus() {
  if (buttonPressed) {
  Serial.println("BOTAO_PRESSIONADO");
    //Serial.println("");
    buttonPressed = false;
  }
}
