//********************************************************************************
//
//     PROGRAMA EXEMPLO: Comunicação USART entre PIC 18F e o Arduino.
// OBJETIVO: Aprender a utilizar a comunicação serial assíncrona do PIC18F4520.
//
//********************************************************************************

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

char ucRead;        // Variavel para armazenar o dado lido.
unsigned int Control = 1;  // +++++++ VARIAVEL DE CONTROLE DA COMUNICAÇÃO ++++++++
                           // LEMBRE DE ALTERAR ESSA VARIAVE TAMBEM NO ARDUINO
int contador = 0;
char buffer[50];

// Mapeamento dos segmentos para os números de 0 a 9
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
int unidade = 0;
int dezena = 0;
int centena = 0;
int milhar = 0;

void display7Seg(int valor) {
    unidade = valor % 10;
    dezena = (valor / 10) % 10;
    centena = (valor / 100) % 10;
    milhar = (valor / 1000) % 10;

    //do {
        // Display 1 (Milhar)
        PORTA.RA2 = 1; // Liga primeiro display
        PORTD = segmentos[milhar]; // Mostra o digito correspondente
        Delay_ms(1); // Delay de 1ms
        PORTA.RA2 = 0; // Desliga o display

        // Display 2 (Centena)
        PORTA.RA3 = 1; // Liga segundo display
        PORTD = segmentos[centena]; // Mostra o digito correspondente
        Delay_ms(1); // Delay de 1ms
        PORTA.RA3 = 0; // Desliga o display

        // Display 3 (Dezena)
        PORTA.RA4 = 1; // Liga terceiro display
        PORTD = segmentos[dezena]; // Mostra o digito correspondente
        Delay_ms(1); // Delay de 1ms
        PORTA.RA4 = 0; // Desliga o display

        // Display 4 (Unidade)
        PORTA.RA5 = 1; // Liga quarto display
        PORTD = segmentos[unidade]; // Mostra o digito correspondente
        Delay_ms(1); // Delay de 1ms
        PORTA.RA5 = 0; // Desliga o display
    //} while (1);
    //Delay_ms(500);
}

char numero_str[12];

void main(){
   //ADCON1  = 0x0E;                           //Configura os pinos do PORTB como digitais, e RA0 (PORTA) como analógico
   ADCON1 = 0x0f; //configura todos os pinos como I/O
   TRISA = 0; //define porta como saida
   PORTA = 0; //resseta todos os pinos do porta
   TRISD = 0; //define portd como saida
   PORTD = 0; //seta todos os pinos do portd

   Lcd_Init();                               //Inicializa módulo LCD
   Lcd_Cmd(_LCD_CURSOR_OFF);                 //Apaga cursor
   Lcd_Cmd(_LCD_CLEAR);                      //Limpa display
   
   PWM1_Init(5000);      // Inicializa módulo PWM com 5Khz
   PWM1_Start();         // Start PWM

   UART1_Init(9600);  // Utiliza bibliotecas do compilador para configuração o Baud rate.

   while(1){  // SELECIONE A VARIAVEL DE CONTROLE (CONTROL) DECLARADA ACIMA.
     display7Seg(contador);
     /*if (Control == 0){   // O PIC (Control = 0) envia um caracter e o Arduino responde com outro caracter.
       UART1_Write('T'); // Transmite o caracter para o Arduino
       lcd_out(1,1,"PIC Send/Receive");
       lcd_out(2,1,"Send = T");
       Delay_ms(50);
       if(UART1_Data_Ready()){  // Verifica se um dado foi recebido no buffer
          UART1_Read_String(received_string, BUFFER_SIZE);
         ucRead = UART1_Read(); // Lê o dado recebido do buffer.
         Delay_ms(50);   // Pausa de 50ms.
         if (ucRead == 'S'){
          lcd_out(2,10,"Rec.= ");
          lcd_chr_cp (ucRead);
          }
       }
     }   */

     if (Control == 1){   // O PIC (Control = 1) recebe um caracter do Arduino e responde com outro caracter.
       if(UART1_Data_Ready()){  // Verifica se um dado foi recebido no buffer
         ucRead = UART1_Read(); // Lê o dado recebido do buffer.
         Delay_ms(50);       // Pausa de 50ms.
         if (ucRead == 'D'){ //decrementa contador
            if (contador > 0) {
               contador--;
            }
            lcd_out(1,1,"PIC Receive/Send");
            lcd_out(2, 1, "DECREM.");
            sprintf(numero_str, "%d", contador);
            lcd_out(2, 10, numero_str);
            //display7Seg(contador);
         }
         if (ucRead == 'I'){ //incrementa contador
            contador++;
            lcd_out(1,1,"PIC Receive/Send");
            lcd_out(2, 1, "INCREM.");
            sprintf(numero_str, "%d", contador);
            lcd_out(2, 10, numero_str);
            //display7Seg(contador);
         }
         if (ucRead == 'P'){ //recebe valor potenciometro
             int duty = UART1_Read();
             lcd_out(1,1,"DUTY");
             sprintf(numero_str, "%d", duty);
             lcd_out(2, 10, numero_str);
             PWM1_Set_Duty(duty);
         }
       UART1_Write('P');
        // Envia o valor lido de "uiValorAD" para o módulo CCP1 pwm
       //lcd_out(2,9,"Send = P");
       //Delay_ms(50);
       }
     }
   }
}
