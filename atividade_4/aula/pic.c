/* pinos */
int pinoSensorLM35 = A0;
int pinoSensorLDR = A4;
int pinoSensorUmidade = A2;
int pinoBotaoA = 2;
int pinoBotaoB = 3;

// ------------------- BOTOES ---------------- 
// estados dos botoes
int botaoA;
int botaoB;

int botaoAApertado = 0;
int botaoBApertado = 0;

int ultimoBotaoA = LOW;
int ultimoBotaoB = LOW;

// timers
unsigned long ultimoDebounceTempoBotaoA = 0;
unsigned long ultimoDebounceTempoBotaoB = 0;
unsigned long debounceDelay = 100;

/* variaveis para dados dos sensores*/
volatile float temperaturaLida = 0;
volatile int temperaturaLidaInt = 0;
volatile int luminosidadeLida = 0;
volatile int umidadeLida = 0;

float temperaturaSoma = 0;
int luminosidadeSoma = 0;
int umidadeSoma = 0;

float temperaturaMedia = 0;
int luminosidadeMedia = 0;
int umidadeMedia = 0;
int contadorLeituraSensores = 0;

/* botoes e menu */
bool botaoAPressionado = false;
bool botaoBPressionado = false;
bool botaoCPressionado = false;
int estadoInterface = -1;


void setup() {
    Serial.begin(9600);       // monitor serial
    Serial1.begin(9600);      // comunicacao com PIC
    pinMode(pinoBotaoA, INPUT);
    pinMode(pinoBotaoB, INPUT);
    Serial.println("Arduino");

    // configura o Timer1 para gerar uma interrupcao a cada 1 segundo
    // noInterrupts();
    // TCCR1A = 0;
    // TCCR1B = 0;
    // TCNT1 = 0;
    // OCR1A = 15624;
    // TCCR1B = TCCR1B | 0b00001000;
    // TCCR1B = TCCR1B | 0b00000101;
    // TIMSK1 = TIMSK1 | 0b00000010;
    // interrupts();

    // // configura interrupcoes para os botoes
    // attachInterrupt(digitalPinToInterrupt(pinoBotaoA), botaoAISR, FALLING);
    // attachInterrupt(digitalPinToInterrupt(pinoBotaoB), botaoBISR, FALLING);
}

void menuInicial() {
    estadoInterface = 0;
    Serial.println("---------------------------------------------");
    Serial.println("Escolha a opcao desejada: ");
    Serial.println(" (1) Media da Luminosidade ");
    Serial.println(" (2) Media da Umidade ");
    Serial.println(" (3) Media da Temperatura ");
    Serial.println("---------------------------------------------");
}

// ISR para o botão A
void botaoAISR() {
  botaoAPressionado = true;
}

// ISR para o botão B
void botaoBISR() {
  botaoBPressionado = true;
}

// ISR para o botão C
void botaoCISR() {
  botaoCPressionado = true;
}

// ISR do Timer1
ISR(TIMER1_COMPA_vect) {
  temperaturaLida = analogRead(pinoSensorLM35)  * 0.1075268817204301;
  luminosidadeLida = analogRead(pinoSensorLDR);
  umidadeLida = analogRead(pinoSensorUmidade);
  //somaMetricasAtuais();
  contadorLeituraSensores++;
}

void somaMetricasAtuais() {
    temperaturaSoma = temperaturaSoma + temperaturaLida;
    luminosidadeSoma = luminosidadeSoma + luminosidadeLida;
    umidadeSoma = umidadeSoma + umidadeLida;
}

void calculaMediaMetricas() {
    temperaturaMedia = temperaturaSoma/contadorLeituraSensores;
    luminosidadeMedia = luminosidadeSoma/contadorLeituraSensores;
    umidadeMedia = umidadeSoma/contadorLeituraSensores;
}

void resetaSomasMetricas() {
    temperaturaSoma = 0;
    luminosidadeSoma = 0;
    umidadeSoma = 0;
}

void printaMediaMetricas() {
    Serial.print("Media Temperatura: ");
    Serial.println(temperaturaMedia);
    Serial.print("Media Luminosidade: ");
    Serial.println(luminosidadeMedia);
    Serial.print("Media Umidade: ");
    Serial.println(umidadeMedia);
}

void printaMetricasLidas() {
    Serial.print("Temperatura atual: ");
    Serial.println(temperaturaLida);
    temperaturaLidaInt = (int) temperaturaLida;
    Serial1.write("T");
    //Serial1.write(temperaturaLidaInt);

    Serial.print("Luminosidade atual: ");
    Serial.println(luminosidadeLida);
    Serial1.write("L");
    //Serial1.write(luminosidadeLida);

    Serial.print("Umidade atual: ");
    Serial.println(umidadeLida);
    Serial1.write("U");
    //Serial1.write(umidadeLida);
}

void loop() {
    // if (botaoAPressionado) {
    //     Serial.println("botaoA pressionado");
    //     botaoAPressionado = false;
    // }
    // if (botaoBPressionado) {
    //     Serial.println("botaoB pressionado");
    //     botaoBPressionado = false;
    // }
    Serial.println("Enviando dados");
    Serial1.write("T");
    delay(1000);
    // a cada 10 segundos envia as metricas para PIC
    //if (contadorLeituraSensores == 10) {
        //calculaMediaMetricas();
        //resetaSomasMetricas();
        //printaMediaMetricas();
        //printaMetricasLidas();
        //contadorLeituraSensores = 0;
    //}
}
