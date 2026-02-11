#include <lpc21xx.h>
#include"types.h"
#include"eint.h"
#include "pin_connect_block.h"

#define EINT0_LED 16
#define EINT0_VIC_CHNO 15

extern u32 menu_flag;

void eint0_isr(void) __irq{
    menu_flag = 1;
    EXTINT = 1<<1;
    VICVectAddr = 0;
}

void EINT0_Init(void){
	//CFGPIN(PINSEL0,1,FUNC4);
// 	CFGPIN(PINSEL1,0,FUNC2);
// 	SETBIT(IODIR1,EINT0_LED);
// 	SSETBIT(VICIntEnable,14);
// 	VICVectCntl0=0x20|14;
// 	VICVectAddr0=(unsigned)eint0_isr;
	
// 	//Enable EINT0 
// 	//SSETBIT(EXTINT,0) //default
	
// 	//EINT0 as EDGE_TRIG
//   SETBIT(EXTMODE,0);
// 	//EINT0 as REDGE
//   SETBIT(EXTPOLAR,0);	
  CfgPortPinFunc(0,3,3);
    VICIntEnable = 1<<EINT0_VIC_CHNO;
    VICVectCntl1 = (1<<5) | EINT0_VIC_CHNO;
    VICVectAddr1 = (u32)eint0_isr;
    EXTMODE = 1<<1;   // Edge trigger
    // EXTPOLAR = 0;      // Falling edge
    // EXTINT = 1<<1;
}	
