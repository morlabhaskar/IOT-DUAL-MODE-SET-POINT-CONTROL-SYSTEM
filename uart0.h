#ifndef __UART0_H__
#define __UART0_H__

#include <LPC214x.h>

extern volatile char buff[200];
extern volatile unsigned int rx_index;
extern volatile unsigned char r_flag;
extern volatile unsigned char i;

void InitUART0(void);
void UART0_Tx(char ch);
void UART0_Str(char *s);
void UART0_Int(unsigned int n);

#endif

/*#include "types.h"
void UART0_Init(void);
void UART0_TxChar(u8);
u8 UART0_RxChar(void);
void UART0_TxInt(int);
void UART0_TxString(const char *str);
void UART0_TxFloat(f32 f,u32 ndp);
void UART0_TxHex(u32 n);
void UART0_TxOct(u32 n);*/


