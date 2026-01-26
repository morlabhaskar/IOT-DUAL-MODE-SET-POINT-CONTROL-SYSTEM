#include "types.h"
void UART0_Init(void);
void UART0_TxChar(u8);
u8 UART0_RxChar(void);
void UART0_TxInt(int);
void UART0_TxString(const char *str);
void UART0_TxFloat(f32 f,u32 ndp);
void UART0_TxHex(u32 n);
void UART0_TxOct(u32 n);







