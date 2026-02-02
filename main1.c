#include <LPC21xx.h>
#include "types.h"
#include "spi.h"
#include "spi_eeprom.h"
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"

#define RLED (1<<17)

int main(void)
{
    u8 rData;

    // LED as output
    IODIR0 |= RLED;
    IOCLR0  = RLED;

    // Init peripherals
    Init_SPI0();
    Init_LCD();

    // LCD message
    CmdLCD(CLEAR_LCD);
    StrLCD("EEPROM Test");

    // Write one byte to EEPROM
    ByteWrite(0x0000, 'A');
    delay_ms(20);

    // Read back from EEPROM
    rData = ByteRead(0x0000);

    // Move cursor to 2nd line
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Read: ");

    CmdLCD(GOTO_LINE2_POS0+6);
    // Display read byte
    CharLCD(rData);

    // If read success, turn ON LED
    if(rData == 'A')
        IOSET0 = RLED;

    while(1);
}
