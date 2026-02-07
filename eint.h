#ifndef __EINT_H__ 
#define __EINT_H__
#include"types.h"
extern volatile u32 menu_flag;
void eint0_isr(void) __irq;
void EINT0_Init(void);
#endif
