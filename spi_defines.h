#ifndef __SPI_DEFINES_H__
#define __SPI_DEFINES_H__

//defines for SPI0 Pin Function Select
#define SCK0     0x00000100  //P0.4 
#define MISO0    0x00000400  //P0.5
#define MOSI0    0x00001000  //P0.6  

#define CS 7 //p0.7
#define DUMMY 0xFF
#define WIP 0 




//SxSPCR Bits Setting  
#define CPHA_BIT  3  // 0,1,0,1
#define CPOL_BIT  4  // 0,0,1,1
#define MSTR_BIT  5  // 1=Master,0=Slave  
#define LSBF_BIT  6  // 0=MSB first,1=LSB first 

#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define SPI_CLK 1000000
#define SPI_DIVIDER (PCLK/SPI_CLK)






//SxSPCR register
#define Mode_0  0x00  // CPOL 0 CPHA 0
#define Mode_1  0x08  // CPOL 0 CPHA 1
#define Mode_2  0x10  // CPOL 1 CPHA 0
#define Mode_3  0x18  // CPOL 1 CPHA 1
#define MASTER_MODE_BIT 5 //Master Slave Transmit Recieve

//SxSPSR register
#define SPIF_BIT 7 //SP Interrupt Flag

#endif
