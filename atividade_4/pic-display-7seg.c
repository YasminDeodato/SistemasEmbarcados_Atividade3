#include <xc.h>

// Configurações do PIC
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config MCLRE = ON

#define _XTAL_FREQ 8000000

void UART_Init() {
    TRISC6 = 0; // TX como saída
    TRISC7 = 1; // RX como entrada
    SPBRG = 51; // Baud rate 9600 para 8MHz
    BRGH = 1;   // Alta velocidade
    SYNC = 0;   // Modo assíncrono
    SPEN = 1;   // Habilita pinos RX e TX
    TXEN = 1;   // Habilita transmissão
    CREN = 1;   // Habilita recepção
}

char UART_Read() {
    while (!RCIF); // Aguarda até que os dados sejam recebidos
    return RCREG;  // Retorna o dado recebido
}

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
    // Quebra o valor em dígitos individuais
    int unidade = valor % 10;
    int dezena = (valor / 10) % 10;
    int centena = (valor / 100) % 10;
    int milhar = (valor / 1000) % 10;
    
    // Rotina de multiplexação
    do {
        // Display 1 (Milhar)
        PORTA.RA2 = 1; // Liga primeiro display
        PORTD = segmentos[milhar]; // Mostra o dígito correspondente
        __delay_ms(1); // Delay de 1ms
        PORTA.RA2 = 0; // Desliga o display
        
        // Display 2 (Centena)
        PORTA.RA3 = 1; // Liga segundo display
        PORTD = segmentos[centena]; // Mostra o dígito correspondente
        __delay_ms(1); // Delay de 1ms
        PORTA.RA3 = 0; // Desliga o display
        
        // Display 3 (Dezena)
        PORTA.RA4 = 1; // Liga terceiro display
        PORTD = segmentos[dezena]; // Mostra o dígito correspondente
        __delay_ms(1); // Delay de 1ms
        PORTA.RA4 = 0; // Desliga o display
        
        // Display 4 (Unidade)
        PORTA.RA5 = 1; // Liga quarto display
        PORTD = segmentos[unidade]; // Mostra o dígito correspondente
        __delay_ms(1); // Delay de 1ms
        PORTA.RA5 = 0; // Desliga o display
    } while (1);
}

char ucRead;        // Variavel para armazenar o dado lido.

void main(void) {
    ADCON1 = 0x0f; // Configura todos os pinos como I/O
    TRISA = 0; // Define porta A como saída
    PORTA = 0; // Reseta todos os pinos do PORTA
    TRISD = 0; // Define PORTD como saída
    PORTD = 0; // Reseta todos os pinos do PORTD
    
    UART1_Init(9600);  // Inicializa a UART
    
    while (1) {
        if(UART1_Data_Ready()){  // Verifica se um dado foi recebido no buffer
            ucRead = UART1_Read(); // Lê o dado recebido do buffer.
            Display7Seg(contador); 
        }
    }
}
