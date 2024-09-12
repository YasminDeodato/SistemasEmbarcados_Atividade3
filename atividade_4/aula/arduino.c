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
int temperatura = 0;
int luminosidade = 0;
int umidade = 0;
//int dutyAnterior = 0;

void main(){
    //ADCON1  = 0x0E;               // configura os pinos do PORTB como digitais, e RA0 (PORTA) como analógico
    ADCON1 = 0x0f;                  // configura todos os pinos como I/O
    TRISA = 0;                      // define porta como saida
    PORTA = 0;                      // reseta todos os pinos do porta
    TRISD = 0;                      // define portd como saida
    PORTD = 0;                      // seta todos os pinos do portd
    TRISC.RC1 = 0;                  // PORT C configurado como saída - Buzzer
    TRISB.RB0 = 0;
    Lcd_Init();                     // inicializa módulo LCD
    Lcd_Cmd(_LCD_CURSOR_OFF);       // apaga cursor
    Lcd_Cmd(_LCD_CLEAR);            // limpa display

    PWM1_Init(5000);                // inicializa módulo PWM com 5Khz
    PWM1_Start();                   // start PWM

    UART1_Init(9600);               // comunicacao UART
    lcd_out(1, 1, "oi");
    while(1) {
        //PWM1_Set_Duty(duty);          // envia valor duty para ventoinha
        PORTB.RB0 = ~PORTB.RB0;

        if (Control == 1){           // O PIC (Control = 1) recebe um caracter do Arduino e responde com outro caracter.
            if(UART1_Data_Ready()){   // verifica se um dado foi recebido no buffer
                ucRead = UART1_Read();   // lê o dado recebido do buffer.
                PORTB.RB0 = ~PORTB.RB0;
                if (ucRead == 'T'){      // decrementa contador
                    //temperatura = UART1_Read();
                    Lcd_Cmd(_LCD_CLEAR);
                    lcd_out(1, 1, "TEMPERATURA");
                    sprintf(numero_str, "%f", temperatura);
                    lcd_out(2, 1, numero_str);

                }
                if (ucRead == 'L'){      // incrementa contador
                    //luminosidade = UART1_Read();
                    Lcd_Cmd(_LCD_CLEAR);
                    lcd_out(1, 1, "LUMINOSIDADE");
                    sprintf(numero_str, "%d", luminosidade);
                    lcd_out(2, 1, numero_str);
                }
                if (ucRead == 'U'){      // recebe valor potenciometro
                    //umidade = UART1_Read();
                    Lcd_Cmd(_LCD_CLEAR);
                    lcd_out(1, 1, "UMIDADE");
                    sprintf(numero_str, "%d", umidade);
                    lcd_out(2, 1, numero_str);
                }
            }
        }
        Delay_ms(1000);
   }
}
