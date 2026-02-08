#ifndef __UART0_DEFINES_H__
#define __UART0_DEFINES_H__

#define FUNC2         1
#define TXDO_PIN_NO   0
#define RXD0_PIN_NO   1
#define TXDO_PIN_0_0  FUNC2
#define RXD0_PIN_0_1  FUNC2

//divisor baud rate
#define FOSC    12000000
#define CCLK    (FOSC*5)
#define PCLK    (CCLK/4)
#define BAUD    9600
#define DIVISOR (PCLK/(BAUD*16))

//U0LCR register bits
#define WORD_LEN  3
#define DLAB      7

//U0LSR register bits
#define RDR_BIT   0
#define THRE_BIT  5
#define TEMT_BIT  6

#endif