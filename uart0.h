#ifndef __UART0_H__
#define __UART0_H__

#include "types.h"
void uart0_init();
void uart0_txchar(u8 txbyte);
u8 uart0_rxchar();
void uart0_str(u8 *s);
void uart0_float(f32 f);
void uart0_int(u32 n);

#endif