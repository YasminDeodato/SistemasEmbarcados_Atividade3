const int potPin = A0; // Pino para o potenciômetro
const int button1 = 2; // Pino para o botão de incrementar
const int button2 = 3; // Pino para o botão de decrementar
const int baudRate = 9600; // Taxa de transmissão
int counterValue = 0; // Contador
volatile bool button1Pressed = false; // Flag para o botão 1
volatile bool button2Pressed = false; // Flag para o botão 2
volatile bool readPotFlag = false;
volatile int potValue = 0;
volatile int dutyCycle = 0;

void setup() {
  // Configura o Timer1 para gerar uma interrupção a cada 2 segundos
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;   // Para 2 s com um prescaler de 256 (com base em 16MHz)
  TCCR1B = TCCR1B | 0b00001000;   // Configura o prescaler para 256
  TCCR1B = TCCR1B | 0b00000101;   // Configura o modo CTC
  TIMSK1 = TIMSK1 | 0b00000010;   // Ativa a interrupção para o Timer1
 
  Serial.begin(baudRate);
  Serial1.begin(baudRate); // Assumindo que Serial1 é o canal de comunicação com a Placa 1

  Serial.println("Iniciando arduino");
 
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  
  interrupts();
 
  // Configura interrupções para os botões
  attachInterrupt(digitalPinToInterrupt(button1), button1ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(button2), button2ISR, FALLING);
}

void loop() {
  if (readPotFlag) { 
    sendPotData(); 
    readPotFlag = false; 
  }
  if (button1Pressed) {
    Serial.println("INCREMENTA");
    Serial1.write("I");
    button1Pressed = false;
  }
  if (button2Pressed) {
    Serial.println("DECREMENTA");
    Serial1.write("D");
    button2Pressed = false;
  }
}

void sendPotData() {
  Serial.print("DUTY_");
  Serial.println(dutyCycle);
  Serial1.write("P");
  Serial1.write(dutyCycle);
}

// ISR do Timer1 - Potenciometro
ISR(TIMER1_COMPA_vect) {
  readPotFlag = true; // Ativa a flag para ler a potenciometro
  potValue = analogRead(potPin);
  dutyCycle = map(potValue, 0, 1023, 0, 255);
}

// ISR para o botão 1
void button1ISR() {
  button1Pressed = true;
  counterValue++;
}

// ISR para o botão 2
void button2ISR() {
  button2Pressed = true;
  counterValue--;
}
