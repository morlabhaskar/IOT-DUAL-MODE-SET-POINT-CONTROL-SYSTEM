#include <LPC214x.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "system.h"
#include "delay.h"
#include "adc.h"
#include "rtc.h"
#include "spi_eeprom.h"
#include "kpm.h"
#include "lcd_defines.h"
#include "pin_connect_block.h"
#include "spi.h"
#include "uart0.h"
#include "ESP01.h"

#define BUZZER              23
#define EINT0_VIC_CHNO      15
s32 hr = 0, min = 0, sec = 50;
s32 prev_min = -1;
char adcStr[10];      // to store adcVal as string
u32 adcVal, key;
float analog;
u32 menu_flag = 0;

void eint0_isr(void) __irq{
    menu_flag = 1;
    EXTINT = (1 << 1);
    VICVectAddr = 0;
}

int main(void){
    Init_system();
    Init_SPI0();
    UART0_Init();
    delay_ms(2000);
    // ESP01_Init();
    esp01_connectAP();   // CONNECT TO WIFI ONLY ONCE
    IOCLR0 = (1 << BUZZER);

    /* EINT0 configuration */
    CfgPortPinFunc(0, 3, 3);
    VICIntEnable = (1 << EINT0_VIC_CHNO);
    VICVectCntl0 = (1 << 5) | EINT0_VIC_CHNO;
    VICVectAddr0 = (u32)eint0_isr;
    EXTMODE  = (1 << 1);
    EXTPOLAR = 0;
    EXTINT   = (1 << 1);
    SetRTCTimeInfo(hr, min, sec);
    while(1){
        GetRTCTimeInfo(&hr, &min, &sec);
        Read_ADC(1, &adcVal, &analog);
        sprintf(adcStr, "%lu", adcVal);
        if(min != prev_min){
            prev_min = min;
            // ThingSpeak_Update(adcVal);
            esp01_sendToThingspeak(adcStr);
        }
        CmdLCD(GOTO_LINE1_POS0);
        DisplayRTCTime(hr, min, sec);
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("ADC:");
        CmdLCD(GOTO_LINE2_POS0 + 4);
        U32LCD(adcVal);
    }
}
