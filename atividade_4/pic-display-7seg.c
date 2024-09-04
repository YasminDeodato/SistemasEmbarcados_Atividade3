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

void Display7Seg(int valor) {
    ADCON1 = 0x0f;  //configura todos os pinos como I/O
    TRISA = 0;      //define porta como saida
    PORTA = 0;      //resseta todos os pinos do porta
    TRISD = 0;      //define portd como saida
    PORTD = 0;      //seta todos os pinos do portd
    int unidade = valor % 10;
    int dezena = (valor / 10) % 10;
    int centena = (valor / 100) % 10;
    int milhar = (valor / 1000) % 10;
    
    do {
        // Display 1 (Milhar)
        PORTA.RA2 = 1; // Liga primeiro display
        PORTD = segmentos[milhar]; // Mostra o digito correspondente
        __delay_ms(1); // Delay de 1ms
        PORTA.RA2 = 0; // Desliga o display
        
        // Display 2 (Centena)
        PORTA.RA3 = 1; // Liga segundo display
        PORTD = segmentos[centena]; // Mostra o digito correspondente
        __delay_ms(1); // Delay de 1ms
        PORTA.RA3 = 0; // Desliga o display
        
        // Display 3 (Dezena)
        PORTA.RA4 = 1; // Liga terceiro display
        PORTD = segmentos[dezena]; // Mostra o digito correspondente
        __delay_ms(1); // Delay de 1ms
        PORTA.RA4 = 0; // Desliga o display
        
        // Display 4 (Unidade)
        PORTA.RA5 = 1; // Liga quarto display
        PORTD = segmentos[unidade]; // Mostra o digito correspondente
        __delay_ms(1); // Delay de 1ms
        PORTA.RA5 = 0; // Desliga o display
    } while (1);
}

char ucRead;        // Variavel para armazenar o dado lido.
unsigned int Control = 1;  // +++++++ VARIAVEL DE CONTROLE DA COMUNICAÇÃO ++++++++
                           // LEMBRE DE ALTERAR ESSA VARIAVE TAMBEM NO ARDUINO

void main(void) {
    ADCON1  = 0x0E;      //Configura os pinos do PORTB como digitais, e RA0 (PORTA) como analógico
    
    UART1_Init(9600);   // Utiliza bibliotecas do compilador para configuração o Baud rate.
    
    while (1) {
        if (Control == 0){   // O PIC (Control = 0) envia um caracter e o Arduino responde com outro caracter.
            /*UART1_Write('T'); // Transmite o caracter para o Arduino
            lcd_out(1,1,"PIC Send/Receive");
            lcd_out(2,1,"Send = T");
            Delay_ms(50);
            if(UART1_Data_Ready()){  // Verifica se um dado foi recebido no buffer
                ucRead = UART1_Read(); // Lê o dado recebido do buffer.
                Delay_ms(50);   // Pausa de 50ms.
                if (ucRead == 'S'){
                lcd_out(2,10,"Rec.= ");
                lcd_chr_cp (ucRead);
                }
            }*/
        }

        if (Control == 1){   // O PIC (Control = 1) recebe um caracter do Arduino e responde com outro caracter.
            if(UART1_Data_Ready()){  // Verifica se um dado foi recebido no buffer
                ucRead = UART1_Read(); // Lê o dado recebido do buffer.
                Display7Seg(contador); // envia numero para ser mostrado no display 7seg
            }
        }
    }
}
