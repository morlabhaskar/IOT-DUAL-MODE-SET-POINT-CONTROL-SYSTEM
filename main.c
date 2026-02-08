#include <LPC214x.h>
#include "lcd.h"
#include "system.h"
#include "delay.h"
#include "adc.h"
#include "rtc.h"
#include "spi_eeprom.h"
#include "kpm.h"
#include "lcd_defines.h"
#include "esp01.h"
#include "pin_connect_block.h"
#include "spi.h"
#include "uart0.h"
#include "uart_defines.h"

#define BUZZER 23
#define EINT0_VIC_CHNO 15

s32 hr = 0, min = 0, sec = 50;
s32 prev_min = -1;
u32 adcVal, key;
f32 analog;
u32 menu_flag = 0;
u8 value[20];
void show_setpoints(void){
    int i;
    s8 arr[4];
    for (i = 0; i < 3; i++)
        arr[i] = ByteRead(0x0010 + i);
    arr[i] = '\0';
    CmdLCD(CLEAR_LCD);
    StrLCD("DATA : ");
    CmdLCD(GOTO_LINE1_POS0 + 8);
    StrLCD(arr);
    delay_ms(2000);
    CmdLCD(CLEAR_LCD);
}
void update_setpoints(void){
    int i;
    CmdLCD(CLEAR_LCD);
    StrLCD("ENTER SET PTS");
    for (i = 0; i < 3; i++){
        key = KeyScan();
        CmdLCD(GOTO_LINE2_POS0 + i);
        CharLCD(key);
        ByteWrite(0x0010 + i, key);
        while (ColScan() == 0);
    }
    CmdLCD(CLEAR_LCD);
    StrLCD("WRITTEN");
    delay_ms(1500);
    CmdLCD(CLEAR_LCD);
}
void eint0_isr(void) __irq{
    menu_flag = 1;
    EXTINT = 1 << 1;
    VICVectAddr = 0;
}
int main(void){
    Init_system();
    Init_SPI0();
    uart0_init();
    delay_ms(500);
    esp01_connectAP();
    CmdLCD(CLEAR_LCD);
    // StrLCD("WIFI CONNECTED");
    delay_ms(1000);
    CmdLCD(CLEAR_LCD);
    IOCLR0 = 1 << BUZZER;
    CfgPortPinFunc(0, 3, 3);
    VICIntEnable |= 1 << EINT0_VIC_CHNO;
    VICVectCntl0 = (1 << 5) | EINT0_VIC_CHNO;
    VICVectAddr0 = (u32)eint0_isr;
    EXTMODE = 1 << 1;
    EXTPOLAR = 0;
    EXTINT = 1 << 1;
    SetRTCTimeInfo(hr, min, sec);
    while (1){
        if(menu_flag){
            menu_flag = 0;
            CmdLCD(CLEAR_LCD);
            StrLCD("1:SHOW SETPTS");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("2:UPDATE SET");
            key = KeyScan();
            while (ColScan() == 0);
            if (key == '1')
                show_setpoints();
            else if (key == '2')
                update_setpoints();
        }
        GetRTCTimeInfo(&hr, &min, &sec);
        Read_ADC(1, &adcVal, &analog);
        if (min != prev_min){
            char adc_str[10];
            prev_min = min;
            sprintf(adc_str, "%lu", adcVal);
            esp01_sendToThingspeak(adc_str);
            uart0_str("ADC Sent: ");
            uart0_str((u8 *)adc_str);
            uart0_str("\r\n");
        }
        CmdLCD(GOTO_LINE1_POS0);
        DisplayRTCTime(hr, min, sec);
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("ADC:");
        CmdLCD(GOTO_LINE2_POS0 + 5);
        U32LCD(adcVal);
        CmdLCD(GOTO_LINE3_POS0);
        StrLCD("**PRESS SWITCH**");
        CmdLCD(GOTO_LINE4_POS0);
        StrLCD("FOR MENU");
    }
}
