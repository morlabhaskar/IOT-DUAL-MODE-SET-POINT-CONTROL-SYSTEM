#include <LPC21xx.h>
#include "types.h"
#include "spi_defines.h"
#include "spi.h"
#include "spi_eeprom_defines.h"
#include "delay.h"

void CmdEEPROM(u8 cmd){
    IOCLR0 = (1<<CS);
    SPI0(cmd);
    IOSET0 = (1<<CS);
}
void ByteWrite(u16 addr,u8 data){
    CmdEEPROM(WREN);//activate write enable latch
    IOCLR0 = (1<<CS);
    SPI0(WRITE);//issue WRITE instruction
    SPI0(addr>>8);//msbyte of 16-bit address loc to be written into
    SPI0(addr);//lsbyte of 16-bit address loc to be written into
    SPI0(data);//data for that address loc
    IOSET0 = (1<<CS);
    delay_ms(10);
    CmdEEPROM(WRDI);//deactivate write enable latch
}
u8 ByteRead(u16 addr){
    u8 data;
    IOCLR0 = (1<<CS);
    SPI0(READ);//issue READ instruction
    SPI0(addr>>8);//msbyte of 16-bit address loc to be written into
    SPI0(addr);//lsbyte of 16-bit address loc to be written into

    data=SPI0(0x00);//sending garbage & reading data at loc
    IOSET0 = (1<<CS);
    return data;
}
void PageWrite(u16 pageStartAddr,u8 *ptr){
    CmdEEPROM(WREN);//activate write enable latch
    IOCLR0 = (1<<CS);
    SPI0(WRITE);//issue WRITE instruction
    SPI0(pageStartAddr>>8);//msbyte of 16-bit address loc to be written into
    SPI0(pageStartAddr);//lsbyte of 16-bit address loc to be written into
    while(*ptr){
        SPI0(*ptr++);
    }
    IOSET0 = (1<<CS);
    delay_ms(10);
    CmdEEPROM(WRDI);//deactivate write enable latch
}
