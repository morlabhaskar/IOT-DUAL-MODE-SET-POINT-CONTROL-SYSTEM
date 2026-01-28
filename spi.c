//spi.c
#include<LPC214x.h>
#include "types.h"
#include "delay.h"
#include "spi_defines.h"
#include "pin_connect_block.h"

void Init_SPI0(void){
    // PINSEL0 = SCK0 | MISO0 | MOSI0 ;//select SPI pin functionality 
    // S0SPCCR = 60;// bit clock rate
    // // S0SPCR = ((1<<MASTER_MODE_BIT)|Mode_3);//spi module in master mode,CPOL =1,CCPHA = 1. MSB first
    // S0SPCR = ((1<<MASTER_MODE_BIT)|Mode_0);//spi module in master mode,CPOL =1,CCPHA = 1. MSB first
    // // IOPIN0 |= (1<<CS);
    // IOSET0 = (1<<CS); 
    // IODIR0 |= (1<<CS);
    // // S0SPCCR = 8;// bit clock rate

    //cfg p0.4 pin as SCK pin  
   CfgPortPinFunc(0,4,1); 
   //cfg p0.5 pin as MIS0 pin
   CfgPortPinFunc(0,5,1); 
   //cfg p0.6 pin as MOSI pin  
   CfgPortPinFunc(0,6,1); 
   //make p0.7(cs) as high using IOSET0/IOPIN0 
   IOSET0=1<<7;  
   //cfg p0.7 as gpio output pin using IODIR0 
   IODIR0|=1<<7; 
   //cfg spi clock speed using S0SPCCR 
	 if(SPI_DIVIDER>=8)
	 {
     if(SPI_DIVIDER%2==0)
     {			 
       S0SPCCR=SPI_DIVIDER; 
		 }
     else
     {			 
       S0SPCCR=SPI_DIVIDER+1; 
		 }
   }		 
   //cfg spi peripheral for Master Mode,Mode3,MSBF(def)
    S0SPCR=1<<MSTR_BIT|1<<CPOL_BIT|1<<CPHA_BIT; 
}
u8 SPI0(u8 data){
    u8 status;
    S0SPDR = data; //load spi tx reg
    while(((S0SPSR>>SPIF_BIT)&1)==0);//wait for tx to complete
    status = S0SPSR;//clear SPIF
    return status; // read data from SPI data reg,place into buffer
}




