// final
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
u8 adc_str[16];
s32 hr = 0, min = 0, sec = 0;
s32 prev_min = -1, prev_cloud_read = -1;
u32 key;
f32 temperature;
u32 menu_flag = 0;
u8 value[20], setPoint[20];
void update_eeprom(s8 *arr){
    s32 i;
    for (i = 0; i < 3; i++){
        ByteWrite(i, arr[i]);
    }
}
void show_setpoints(void){
    int i;
    u8 arr[4];
    for (i = 0; i < 3; i++){
        arr[i] = ByteRead(i);
    }
    arr[i] = '\0';
    strcpy((char *)setPoint, (char *)arr);
    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("   CURRENT     ");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("EEPROM SETPOINT");
    CmdLCD(GOTO_LINE3_POS0);
    StrLCD((s8 *)setPoint);
    delay_ms(2000);
    CmdLCD(CLEAR_LCD);
}
void update_setpoints(void){
    s32 i;
    u8 arr[10];
    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("ENTER SETPOINT");
    i=0;
    do{
        key = KeyScan();
        if(key!='+'){
            arr[i] = key;
            CmdLCD(GOTO_LINE2_POS0 + i);
            CharLCD(key);
        }
        while (ColScan() == 0);
        i++;
    }while(key!='+');
    /*
    for (i = 0; i < 3; i++){
        key = KeyScan();
        arr[i] = key;
        CmdLCD(GOTO_LINE3_POS0 + i);
        CharLCD(key);
        while (ColScan() == 0);
    }*/
    arr[i] = '\0';
    i = 0;
    while(arr[i]!='\0'){
        ByteWrite(i, arr[i++]);
    }
    /*
    for (i = 0; i < 3; i++){
        ByteWrite(i, arr[i]);
    }
    */
    delay_ms(500);
    strcpy((char *)setPoint, (char *)arr);
    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("UPDATE SET");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("IN EEPROM");
    delay_ms(1000);
    CmdLCD(CLEAR_LCD);
    esp01_sendToThingspeak_set_point((u32)atoi((char *)setPoint));
    CmdLCD(CLEAR_LCD);
}
void Read_LM35(f32 *temperature){
    u32 adcVal;
    f32 analog;
    Read_ADC(1, &adcVal, &analog);
    *temperature=(analog*100.0);
}
void floatToString(float val, char *str){
    int ip = (int)val;
    int fp = (int)((val - ip) * 100);
    if(fp < 0) fp = -fp;
    sprintf(str, "%d.%02d", ip, fp);
}
int main(void){
    Init_LCD();
    Init_ADC();
    RTC_Init();
    Init_KPM();
    Init_SPI0();
    uart0_init();
    EINT0_Init();
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("***IOT BASED****");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("***DUAL MODE****");
    CmdLCD(GOTO_LINE3_POS0);
    StrLCD("***SET POINT****");
    CmdLCD(GOTO_LINE4_POS0);
    StrLCD("*CONTROL SYSTEM*");
    delay_ms(3000);
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
    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("   UPDATED    ");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("SETPOINT IN   ");
    CmdLCD(GOTO_LINE3_POS0);
    StrLCD("   CLOUD IS  ");
    CmdLCD(GOTO_LINE4_POS0);
    StrLCD((s8 *)value);
    delay_ms(2000);
    strcpy((char *)setPoint, (char *)value);
    update_eeprom((s8 *)setPoint);
    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("SETPOINT UPDATED");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("***IN EEPROM****");
    delay_ms(2000);
    CmdLCD(CLEAR_LCD);
    while (1){
        if ((int)temperature > atoi((char *)setPoint)){
            IOSET0 = 1 << BUZZER;
        }
        else{
            IOCLR0 = 1 << BUZZER;
        }
        while(menu_flag){
            menu_flag = 0;
            CmdLCD(CLEAR_LCD);
            StrLCD("1:SHOW SETPTS");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("2:UPDATE SET");
            CmdLCD(GOTO_LINE3_POS0);
            StrLCD("3:EXIT");
            key = KeyScan();
            while (ColScan() == 0);
            if (key == '1')
                show_setpoints();
            else if (key == '2')
                update_setpoints();
            else if (key == '3'){
                CmdLCD(CLEAR_LCD);
                break;
            }
            menu_flag=0;
            CmdLCD(CLEAR_LCD);
        }
        GetRTCTimeInfo(&hr, &min, &sec);
        Read_LM35(&temperature);
        floatToString(temperature, (char *)adc_str);
        if ((min % 3 == 0) && (min != prev_min)){
            // if((min != prev_min)){
            prev_min = min;
            esp01_sendToThingspeak((u32)temperature);
            uart0_str("ADC Sent: ");
            uart0_str((u8 *)adc_str);
            uart0_str("\r\n");
        }
        if ((min % 5 == 0) && (min != 0) && (prev_cloud_read != min)){
            prev_cloud_read = min;
            esp01_readFromThingspeak();
            CmdLCD(CLEAR_LCD);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("*SETPOINT READ**");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("***FROM CLOUD***");
            delay_ms(2000);
            strcpy((char *)setPoint, (char *)value);
            update_eeprom((s8 *)setPoint);
            CmdLCD(CLEAR_LCD);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("****SETPOINT****");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("*****UPDATED****");
            CmdLCD(GOTO_LINE3_POS0);
            StrLCD("***IN EEPROM****");
            delay_ms(2000);
            CmdLCD(CLEAR_LCD);
        }
        CmdLCD(GOTO_LINE1_POS0);
        DisplayRTCTime(hr, min, sec);
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("TEMP : ");
        CmdLCD(GOTO_LINE2_POS0 + 8);
        StrLCD((s8*)adc_str);
        // F32LCD(temperature,2);
        CmdLCD(GOTO_LINE2_POS0 + 15);
        CharLCD('C');
        CmdLCD(GOTO_LINE3_POS0);
        StrLCD("**PRESS SWITCH**");
        CmdLCD(GOTO_LINE4_POS0);
        StrLCD("****FOR MENU****");
    }
}
