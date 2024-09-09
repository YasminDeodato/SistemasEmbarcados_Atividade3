// --- Ligações entre PIC e LCD ---
sbit LCD_RS at RE2_bit;   // PINO 2 DO PORTD INTERLIGADO AO RS DO DISPLAY
sbit LCD_EN at RE1_bit;   // PINO 3 DO PORTD INTERLIGADO AO EN DO DISPLAY
sbit LCD_D7 at RD7_bit;  // PINO 7 DO PORTD INTERLIGADO AO D7 DO DISPLAY
sbit LCD_D6 at RD6_bit;  // PINO 6 DO PORTD INTERLIGADO AO D6 DO DISPLAY
sbit LCD_D5 at RD5_bit;  // PINO 5 DO PORTD INTERLIGADO AO D5 DO DISPLAY
sbit LCD_D4 at RD4_bit;  // PINO 4 DO PORTD INTERLIGADO AO D4 DO DISPLAY

// Selecionando direção de fluxo de dados dos pinos utilizados para a comunicação com display LCD
sbit LCD_RS_Direction at TRISE2_bit;  // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 2 DO PORTD
sbit LCD_EN_Direction at TRISE1_bit;  // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 3 DO PORTD
sbit LCD_D7_Direction at TRISD7_bit;  // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 7 DO PORTD
sbit LCD_D6_Direction at TRISD6_bit;  // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 6 DO PORTD
sbit LCD_D5_Direction at TRISD5_bit;  // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 5 DO PORTD
sbit LCD_D4_Direction at TRISD4_bit;  // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 4 DO PORTD

char ucRead;               // Variavel para armazenar o dado lido.
unsigned int Control = 1;  // +++++++ VARIAVEL DE CONTROLE DA COMUNICAÇÃO ++++++++
                           // LEMBRE DE ALTERAR ESSA VARIAVE TAMBEM NO ARDUINO
int contador = 0;
char numero_str[12];
int unidade = 0;
int dezena = 0;
int centena = 0;
int milhar = 0;
int duty = 0;
//int dutyAnterior = 0;

// mapeamento dos segmentos para os números de 0 a 9
const char segmentos[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

void display7Seg(int valor) {
    unidade = valor % 10;
    dezena = (valor / 10) % 10;
    centena = (valor / 100) % 10;
    milhar = (valor / 1000) % 10;

    // display 1 - milhar
    PORTA.RA2 = 1;
    PORTD = segmentos[milhar];
    Delay_ms(1);
    PORTA.RA2 = 0;

    // display 2 - centena
    PORTA.RA3 = 1;
    PORTD = segmentos[centena];
    Delay_ms(1);
    PORTA.RA3 = 0;

    // display 3 - dezena
    PORTA.RA4 = 1;
    PORTD = segmentos[dezena];
    Delay_ms(1);
    PORTA.RA4 = 0;

    // display 4 - unidade
    PORTA.RA5 = 1;
    PORTD = segmentos[unidade];
    Delay_ms(1);
    PORTA.RA5 = 0;
}

void main(){
    //ADCON1  = 0x0E;               // configura os pinos do PORTB como digitais, e RA0 (PORTA) como analógico
    ADCON1 = 0x0f;                  // configura todos os pinos como I/O
    TRISA = 0;                      // define porta como saida
    PORTA = 0;                      // reseta todos os pinos do porta
    TRISD = 0;                      // define portd como saida
    PORTD = 0;                      // seta todos os pinos do portd
    TRISC.RC1 = 0;                  // PORT C configurado como saída - Buzzer

    Lcd_Init();                     // inicializa módulo LCD
    Lcd_Cmd(_LCD_CURSOR_OFF);       // apaga cursor
    Lcd_Cmd(_LCD_CLEAR);            // limpa display

    PWM1_Init(5000);                // inicializa módulo PWM com 5Khz
    PWM1_Start();                   // start PWM

    UART1_Init(9600);               // comunicacao UART

    while(1) {
        display7Seg(contador);        // mostra valor atual do contador no display 7seg
        PWM1_Set_Duty(duty);          // envia valor duty para ventoinha

        if (Control == 1){           // O PIC (Control = 1) recebe um caracter do Arduino e responde com outro caracter.
            if(UART1_Data_Ready()){   // verifica se um dado foi recebido no buffer
                ucRead = UART1_Read();   // lê o dado recebido do buffer.
                if (ucRead == 'D'){      // decrementa contador
                    if (contador > 0) {
                       contador--;
                    }
                    Lcd_Cmd(_LCD_CLEAR);
                    lcd_out(1, 1, "PIC Receive");
                    lcd_out(2, 1, "DECREM.");
                    sprintf(numero_str, "%d", contador);
                    lcd_out(2, 10, numero_str);
                }
                if (ucRead == 'I'){      // incrementa contador
                    contador++;
                    Lcd_Cmd(_LCD_CLEAR);
                    lcd_out(1, 1, "PIC Receive");
                    lcd_out(2, 1, "INCREM.");
                    sprintf(numero_str, "%d", contador);
                    lcd_out(2, 10, numero_str);
                }
                if (ucRead == 'P'){      // recebe valor potenciometro
                    duty = UART1_Read();
                    if (duty > 200) {
                        PORTC.RC1 = 0;  // liga buzzer
                    } else {
                        PORTC.RC1 = 1;  // desliga buzzer
                    }
                    //if (duty != dutyAnterior) {
                    Lcd_Cmd(_LCD_CLEAR);
                    lcd_out(1, 1, "DUTY");
                    sprintf(numero_str, "%d", duty);
                    lcd_out(2, 1, numero_str);
                    //dutyAnterior = duty;
                    //}
                }
            }
        }
   }
}
