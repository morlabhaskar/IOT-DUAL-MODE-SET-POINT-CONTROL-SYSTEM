#include<LPC214x.h>
#include<stdio.h>
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

#define BUZZER 23
#define EINT0_VIC_CHNO 15
s32 hr=0,min=0,sec=50;
u32 adcVal,key;
f32 analog;
char adcStr[10]; 
u8 data;
u32 menu_flag = 0;
void show_setpoints(){
	int i;
  s8 arr[5];
  for(i=0;i<3;i++){
    arr[i]=ByteRead(0x0010+i);
  }
  arr[i]='\0';
  CmdLCD(CLEAR_LCD);
  CmdLCD(GOTO_LINE1_POS0);
  StrLCD("DATA : ");
  CmdLCD(GOTO_LINE1_POS0+8);
  StrLCD(arr);
  delay_ms(2000);
  CmdLCD(CLEAR_LCD);
}
void update_setpoints(){
  int i;
  CmdLCD(CLEAR_LCD);
  CmdLCD(GOTO_LINE1_POS0);
  StrLCD("ENTER SET POINTS");
  for(i=0;i<3;i++){
    key=KeyScan();
    CmdLCD(GOTO_LINE2_POS0+i);
    CharLCD(key);
    ByteWrite(0X0010+i,key);
    while(ColScan()==0);
  }
  CmdLCD(CLEAR_LCD);
  StrLCD("WRITTEN");
  delay_ms(2000);
  CmdLCD(CLEAR_LCD);
}
void eint0_isr(void) __irq{
     menu_flag = 1;
    EXTINT = 1<<1;
    VICVectAddr = 0;
}
s32 prev_min = 100;
int main(){
    Init_system();
    Init_SPI0();
    UART0_Init();
    ConnectESP();
    IOCLR0 = 1<<BUZZER;
    CfgPortPinFunc(0,3,3);
    VICIntEnable = 1<<EINT0_VIC_CHNO;
    VICVectCntl0 = (1<<5) | EINT0_VIC_CHNO;
    VICVectAddr0 = (u32)eint0_isr;
    EXTMODE = 1<<1;   // Edge trigger
    EXTPOLAR = 0;      // Falling edge
    EXTINT = 1<<1;
    SetRTCTimeInfo(hr,min,sec);
    while(1){
      if(menu_flag){
        menu_flag = 0;
            CmdLCD(CLEAR_LCD);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("1:SHOW SET PTS");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("2:UPDATE SET PTS");
            key = KeyScan();
            while(ColScan()==0);
            if(key=='1') show_setpoints();
            else if(key=='2') update_setpoints();
        }
      GetRTCTimeInfo(&hr,&min,&sec);
      Read_ADC(1,&adcVal,&analog);
      sprintf(adcStr, "%lu", adcVal);
      if(min!=prev_min){
        prev_min=min;
        //   UART0_TxInt(adcVal);
        //   UART0_TxChar('-');
        updateThingSpeak(adcStr);
      }
      CmdLCD(GOTO_LINE1_POS0);
      DisplayRTCTime(hr,min,sec);
      CmdLCD(GOTO_LINE2_POS0);
      StrLCD("ADCval : ");
      CmdLCD(GOTO_LINE2_POS0+10);
      U32LCD(adcVal);
      CmdLCD(GOTO_LINE3_POS0);
      StrLCD("ENTER SWITCH");
      CmdLCD(GOTO_LINE4_POS0);
      StrLCD("FOR MENU");
    }
}
