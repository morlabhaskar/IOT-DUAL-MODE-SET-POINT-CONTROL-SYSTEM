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
#include <stdio.h>

#define BUZZER 23
#define EINT0_VIC_CHNO 15

s32 hr = 0, min = 0, sec = 50;
s32 prev_min = -1;
u32 adcVal, key;
f32 analog;
u32 menu_flag = 0;

void show_setpoints(void)
{
    int i;
    s8 arr[4];

    for(i = 0; i < 3; i++)
        arr[i] = ByteRead(0x0010 + i);

    arr[i] = '\0';

    CmdLCD(CLEAR_LCD);
    StrLCD("DATA : ");
    CmdLCD(GOTO_LINE1_POS0 + 8);
    StrLCD(arr);

    delay_ms(2000);
    CmdLCD(CLEAR_LCD);
}

void update_setpoints(void)
{
    int i;

    CmdLCD(CLEAR_LCD);
    StrLCD("ENTER SET PTS");

    for(i = 0; i < 3; i++)
    {
        key = KeyScan();
        CmdLCD(GOTO_LINE2_POS0 + i);
        CharLCD(key);
        ByteWrite(0x0010 + i, key);
        while(ColScan() == 0);
    }

    CmdLCD(CLEAR_LCD);
    StrLCD("WRITTEN");
    delay_ms(1500);
    CmdLCD(CLEAR_LCD);
}

void eint0_isr(void) __irq
{
    menu_flag = 1;
    EXTINT = 1 << 1;
    VICVectAddr = 0;
}

int main(void)
{
    Init_system();
    Init_SPI0();
    InitUART0();        

    delay_ms(500);

    esp01_connectAP();

    CmdLCD(CLEAR_LCD);
    StrLCD("WIFI CONNECTED");
    delay_ms(1000);
    CmdLCD(CLEAR_LCD);

    IOCLR0 = 1 << BUZZER;
    CfgPortPinFunc(0, 3, 3);

    VICIntEnable |= 1 << EINT0_VIC_CHNO;
    VICVectCntl0 = (1 << 5) | EINT0_VIC_CHNO;
    VICVectAddr0 = (u32)eint0_isr;

    EXTMODE  = 1 << 1;
    EXTPOLAR = 0;
    EXTINT   = 1 << 1;

    SetRTCTimeInfo(hr, min, sec);

    while(1)
    {
        if(menu_flag)
        {
            menu_flag = 0;

            CmdLCD(CLEAR_LCD);
            StrLCD("1:SHOW SETPTS");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("2:UPDATE SET");

            key = KeyScan();
            while(ColScan() == 0);

            if(key == '1')      show_setpoints();
            else if(key == '2') update_setpoints();
        }

        GetRTCTimeInfo(&hr, &min, &sec);
        Read_ADC(1, &adcVal, &analog);

        if(min != prev_min)
        {
            char adc_str[10];
            prev_min = min;

            sprintf(adc_str, "%lu", adcVal);
            esp01_sendToThingspeak(adc_str);

            UART0_Str("ADC Sent: ");
            UART0_Str(adc_str);
            UART0_Str("\r\n");
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


/*#include<LPC21xx.h>
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

#define BUZZER 23
#define EINT0_VIC_CHNO 15
s32 hr=0,min=0,sec=0;
u32 adcVal,i;
f32 analog;
char data[3];
u32 menu_flag = 0;
u32 key;
void show_setpoints()
{
    CmdLCD(CLEAR_LCD);
    StrLCD("Stored: ");

    for(i = 0; i < 3; i++)
    {
        data[i] = ByteRead(0x0010 + i);
        CharLCD(data[i]);
    }

    delay_ms(2000);
    CmdLCD(CLEAR_LCD);
}
void update_setpoints()
{
    CmdLCD(CLEAR_LCD);
    StrLCD("Enter 3 digits");

    for(i = 0; i < 3; i++)
    {
        key = KeyScan();          
        data[i] = key + '0';      
        CharLCD(data[i]);       
        ByteWrite(0x0010 + i, data[i]);
        delay_ms(300);
    }

    CmdLCD(CLEAR_LCD);
    StrLCD("SUCCESS");
    delay_ms(2000);
    CmdLCD(CLEAR_LCD);
}
//using interrupt 1
void eint0_isr(void) __irq{
     menu_flag = 1;
	
    EXTINT = 1<<1;
    VICVectAddr = 0;
	
}
u8 d;
u32 i;


u32 main()
{
	Init_system();
	Init_SPI0();
	 CmdLCD(GOTO_LINE1_POS0);
	SetRTCTimeInfo(hr,min,sec);
	IOCLR0 = 1<<BUZZER;
	
	
CfgPortPinFunc(0,3,3);
	//PINSEL0=((PINSEL0 &~ (3<<(3*2))) | (3<<(3*2)));
    VICIntEnable = 1<<EINT0_VIC_CHNO;//15
    VICVectCntl0 = (1<<5) | EINT0_VIC_CHNO;
    VICVectAddr0 = (u32)eint0_isr;
    EXTMODE  = 1<<1;   // Edge trigger
    EXTPOLAR = 0;      // Falling edge	
	
	
	while(1)
	{
		   if(menu_flag) {
            menu_flag = 0;
           CmdLCD(CLEAR_LCD);
            StrLCD("1:Set-Point");
            CmdLCD(GOTO_LINE2_POS0);
				 StrLCD("2:Updated-Point");
				 CmdLCD(0x90);
				 StrLCD("Choose: ");
            while(KeyScan() != 0);  
            delay_ms(50); 
            do {
                key = KeyScan();
            } while(key == 0);
            delay_ms(50); 

            CmdLCD(0x90 + 9);
            CharLCD(key);

            if(key == '1') {
                update_setpoints();
            }
            else if(key == '2') {
                show_setpoints();
            }
            else {
                CmdLCD(CLEAR_LCD);
                StrLCD("**Invalid**");
                delay_ms(1000);
            }
     update_setpoints();     
    }
		
		 GetRTCTimeInfo(&hr,&min,&sec);
     Read_ADC(0,&adcVal,&analog);
      CmdLCD(GOTO_LINE1_POS0);
      DisplayRTCTime(hr,min,sec);
		  CmdLCD(GOTO_LINE2_POS0);
		  StrLCD("adcVal : ");
      CmdLCD(GOTO_LINE2_POS0+9);
      U32LCD(adcVal);
	}
}



/*
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
*/

