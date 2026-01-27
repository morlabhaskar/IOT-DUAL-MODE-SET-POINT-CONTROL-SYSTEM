//spi.c
#include<LPC214x.h>
#include "types.h"
#include "delay.h"
#include "spi_defines.h"

void Init_SPI0(void){
    PINSEL0 = SCK0 | MISO0 | MOSI0 ;//select SPI pin functionality 
    S0SPCCR = 60;// bit clock rate
    // S0SPCR = ((1<<MASTER_MODE_BIT)|Mode_3);//spi module in master mode,CPOL =1,CCPHA = 1. MSB first
    S0SPCR = ((1<<MASTER_MODE_BIT)|Mode_0);//spi module in master mode,CPOL =1,CCPHA = 1. MSB first
    // IOPIN0 |= (1<<CS);
    IODIR0 |= (1<<CS);
    IOSET0 = (1<<CS); 
    // S0SPCCR = 8;// bit clock rate
}
u8 SPI0(u8 data){
    // u8 status;
    // status = S0SPSR;//clear SPIF
    S0SPDR = data; //load spi tx reg
    while(((S0SPSR>>SPIF_BIT)&1)==0);//wait for tx to complete
    return S0SPDR; // read data from SPI data reg,place into buffer
}




