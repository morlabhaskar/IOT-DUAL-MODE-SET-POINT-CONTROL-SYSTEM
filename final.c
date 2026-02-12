#include <LPC214x.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lcd.h"
#include "eint.h"
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
u8 adc_str[10];
s32 hr = 0, min = 0, sec = 0;
s32 prev_min = -1, prev_cloud_read = -1;
u32 adcVal, key;
f32 analog;
u32 menu_flag = 0;
u8 value[20], setPoint[20];
void update_eeprom(s8 *arr)
{
    s32 i;
    for (i = 0; i < 3; i++)
    {
        ByteWrite(0x0000 + i, arr[i]);
    }
}
void show_setpoints(void)
{
    int i;
    u8 arr[4];
    for (i = 0; i < 3; i++)
    {
        arr[i] = ByteRead(i);
    }
    arr[i] = '\0';
    strcpy((char *)setPoint, (char *)arr);
    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("EEPROM SETPOINT");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD((s8 *)setPoint);
    delay_ms(2000);
    CmdLCD(CLEAR_LCD);
}
void update_setpoints(void)
{
    s32 i;
    u8 arr[10];
    CmdLCD(CLEAR_LCD);
    StrLCD("ENTER SET PTS");
    for (i = 0; i < 3; i++)
    {
        key = KeyScan();
        arr[i] = key;
        CmdLCD(GOTO_LINE2_POS0 + i);
        CharLCD(key);
        while (ColScan() == 0)
            ;
    }
    arr[i] = '\0';
    i = 0;
    for (i = 0; i < 3; i++)
    {
        ByteWrite(i, arr[i]);
    }
    delay_ms(500);
    strcpy((char *)setPoint, (char *)arr);
    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("UPDATE SET");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("IN EEPROM");
    delay_ms(1000);
    esp01_sendToThingspeak_set_point((u32)atoi((char *)setPoint));
    CmdLCD(CLEAR_LCD);
}
int main(void)
{
    Init_LCD();
    Init_ADC();
    RTC_Init();
    Init_KPM();
    Init_SPI0();
    uart0_init();
    EINT0_Init();
    delay_ms(500);
    esp01_connectAP();
    delay_ms(1000);
    CmdLCD(CLEAR_LCD);
    IODIR0 |= 1 << BUZZER;
    IOCLR0 = 1 << BUZZER;
    SetRTCTimeInfo(hr, min, sec);
    esp01_readFromThingspeak();
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("*SETPOINT READ**");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("***FROM CLOUD***");
    delay_ms(2000);
    StrLCD((s8 *)value);
    delay_ms(2000);
    strcpy((char *)setPoint, (char *)value);
    update_eeprom((s8 *)setPoint);

    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("SETPOINT UPDATED");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("***IN EEPROM****");
    delay_ms(2000);

    CmdLCD(CLEAR_LCD);
    while (1)
    {
        if (strcmp((char *)setPoint, (char *)adc_str) > 0)
        {
            IOSET0 = 1 << BUZZER;
        }
        while (menu_flag)
        {

            CmdLCD(CLEAR_LCD);
            StrLCD("1:SHOW SETPTS");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("2:UPDATE SET");
            key = KeyScan();
            while (ColScan() == 0)
                ;
            if (key == '1')
                show_setpoints();
            else if (key == '2')
                update_setpoints();
            menu_flag = 0;
        }
        GetRTCTimeInfo(&hr, &min, &sec);
        Read_ADC(1, &adcVal, &analog);
        sprintf((char *)adc_str, "%lu", adcVal);
        if ((min % 2 == 0) && (min != prev_min))
        {
            // if((min != prev_min)){
            prev_min = min;
            esp01_sendToThingspeak(adcVal);
            uart0_str("ADC Sent: ");
            uart0_str((u8 *)adc_str);
            uart0_str("\r\n");
        }
        if ((min % 5 == 0) && (min != 0) && (prev_cloud_read != min))
        {
            prev_cloud_read = min;
            esp01_readFromThingspeak();
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("*SETPOINT READ**");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("***FROM CLOUD***");
            delay_ms(2000);
            strcpy((char *)setPoint, (char *)value);
            update_eeprom((s8 *)setPoint);
            CmdLCD(CLEAR_LCD);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("UPDATE SETPOINT");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("***IN EEPROM****");
            delay_ms(2000);
            CmdLCD(CLEAR_LCD);
        }
        CmdLCD(GOTO_LINE1_POS0);
        DisplayRTCTime(hr, min, sec);
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("ADC : ");
        CmdLCD(GOTO_LINE2_POS0 + 7);
        U32LCD(adcVal);
        CmdLCD(GOTO_LINE2_POS0 + 11);
        CharLCD('C');
        CmdLCD(GOTO_LINE3_POS0);
        StrLCD("**PRESS SWITCH**");
        CmdLCD(GOTO_LINE4_POS0);
        StrLCD("****FOR MENU****");
    }
}
