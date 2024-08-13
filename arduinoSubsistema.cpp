// Arduíno 2

int caractere = 0;
int pinoLed = 12;
int pinoBotao = 3;

volatile bool estadoBotao = false;
int botaoApertado = 0;
int ultimoBotao = LOW;

unsigned long ultimoDebounceTempoBotao = 0;
unsigned long debounceDelay = 100;

bool LED = LOW;
void setup() {
  Serial.begin(9600);
  pinMode(pinoLed, OUTPUT);
  pinMode(pinoBotao,INPUT_PULLUP);
  
  // interrupcao
  attachInterrupt(digitalPinToInterrupt(pinoBotao), leituraBotaoISR, FALLING);
}

// ISR BOTAO
void leituraBotaoISR() {
  estadoBotao = true;
}

void loop() {  
  if (Serial.available()) {
      caractere = Serial.read();
      if(caractere == 65){
        LED = !LED;
        digitalWrite(pinoLed, LED);
      }
  }
  if (estadoBotao) {
  	Serial.println("BOTAO_PRESSIONADO");
    //Serial.println("");
    estadoBotao = false;
  }
}