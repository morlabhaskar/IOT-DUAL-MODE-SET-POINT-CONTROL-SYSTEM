#include "types.h"

void CfgPortPinFunc(u32 PortNo,u32 PinNo,u32 PinFunc);
void Init_SPI(void);
//SPI Send & Receive
u8 spi(u8 sData);
//EEPROM Write Enable
void writeEnable(void);
//EEPROM Write Disable
void writeDisable(void);
//EEPROM Byte Write
void byteWrite(u16 wBuffAddr,u8 wByte);
//EEPROM Byte Read
u8 byteRead(u16 rBuffAddr);
u8 readStatus(void);
void waitWriteComplete(void);