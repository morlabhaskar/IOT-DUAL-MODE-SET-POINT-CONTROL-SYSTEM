#ifndef __UART_DEFINES_H__ 
#define __UART_DEFINES_H__

//UART pin config defines
#define PIN_FUNC1 0
#define PIN_FUNC2 1
#define PIN_FUNC3 2
#define PIN_FUNC4 3

#define TXDO_PIN_0_1 PIN_FUNC2
#define RXDO_PIN_0_1 PIN_FUNC2

//clock and Baud Rate defines
#define FOSC 12000000
#define CCLK (FOSC*5)   //600 00 000  60MHz
#define PCLK (CCLK/4)   //150 00 000  15MHz
#define BAUD 9600
#define DIVISOR (PCLK/(16*BAUD))   //97.656 25
// #define 8BIT 3
//definition or determine the word length
/*
00  →  5 bit data
01  →  6 bit data
10  →  7 bit data
11  →  8 bit data
*/
//so 3(binary 11) selects 8-bit data mode
#define WORD_LEN 3
#define DLAB_BIT 7
#define RDR_BIT 0
#define THRE_BIT 5
#define TEMT_BIT 6

#endif


