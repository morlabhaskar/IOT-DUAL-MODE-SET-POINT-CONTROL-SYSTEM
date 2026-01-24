#ifndef RTC_DEFINES_H
#define RTC_DEFINES_H

// System clock and peripheral clock Macros
#define FOSC 12000000
#define CCLK (5*FOSC)    
#define PCLK (CCLK/4) 

// RTC Macros
#define PREINT_VAL ((PCLK/32768)-1)//  15,000,000 / 32,768 = 457.76   → PREINT_VAL = 456
#define PREFRAC_VAL (PCLK-(PREINT_VAL+1)*32768) //PREFRAC = 15,000,000 – 457×32768 = 15,000,000 – 149,585, or similar.
//These values make RTC tick exactly 1 time per second.↑

//CCR register bits 
#define RTC_ENABLE  (1<<0)
#define RTC_RESET   (1<<1)
#define RTC_CLKSRC  (1<<4)

//#define _LPC2148
#endif
