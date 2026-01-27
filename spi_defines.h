#ifndef __SPI_DEFINES_H__
#define __SPI_DEFINES_H__

//defines for SPI0 Pin Function Select
#define SCK0     0x00000100  //P0.4 
#define MISO0    0x00000400  //P0.5
#define MOSI0    0x00001000  //P0.6  

#define CS 7 //p0.7
#define DUMMY 0xFF
#define WIP 0 



//SxSPCR register
#define Mode_0  0x00  // CPOL 0 CPHA 0
#define Mode_1  0x08  // CPOL 0 CPHA 1
#define Mode_2  0x10  // CPOL 1 CPHA 0
#define Mode_3  0x18  // CPOL 1 CPHA 1
#define MASTER_MODE_BIT 5 //Master Slave Transmit Recieve

//SxSPSR register
#define SPIF_BIT 7 //SP Interrupt Flag

#endif
