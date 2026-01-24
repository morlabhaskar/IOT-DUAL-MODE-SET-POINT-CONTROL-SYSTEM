#ifndef __ADC_DEFINES_H__
#define __ADC_DEFINES_H__

#define FOSC 12000000
#define CCLK (FOSC*5)//as per startup.s
#define PCLK (CCLK/4)//as per startup.s
#define ADC_CLK 3000000// <= 4.5 MHz
#define CLKDIV ((PCLK/ADC_CLK)-1)

//defines for ADCR SFR
#define CLKDIV_BITS 8//@ bit (8-15)
#define PDN_BIT 21//@ bit 21
#define START_CONV_BIT 24//@ bit 24

//defines for ADDR SFR
#define RESULT_BITS 6 //@ bit (6-15)
#define DONE_BIT 31 //@ bit 31

#endif
