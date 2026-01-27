#ifndef __SPI_EEPROM_H__
#define __SPI_EEPROM_H__

#include"types.h"
void CmdEEPROM(u8 cmd);
void ByteWrite(u16 addr,u8 data);
u8 ByteRead(u16 addr);
void PageWrite(u16 pageStartAddr,u8 *ptr);

#endif
