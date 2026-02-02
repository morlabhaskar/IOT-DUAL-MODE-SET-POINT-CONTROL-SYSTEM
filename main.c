#include<LPC214x.h>
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
u32 adcVal,key;
f32 analog;
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
  delay_ms(1000);
  CmdLCD(CLEAR_LCD);


  // CmdLCD(CLEAR_LCD);
  // data=ByteRead(0x0010);
  // if(data=='A'){
  //   StrLCD("SAME");
  // }else{
  //   StrLCD("NOT SAME");
  // }
  // delay_ms(2000);
  // CmdLCD(CLEAR_LCD);
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
  delay_ms(1000);
  CmdLCD(CLEAR_LCD);

  // CmdLCD(CLEAR_LCD);
  // ByteWrite(0x0010,'A');
  // delay_ms(10);
  // StrLCD("WRITTEN");
  // delay_ms(2000);
  // CmdLCD(CLEAR_LCD);
  // data=ByteRead(0x0010);
  // if(data=='A'){
  //   StrLCD("SAME");
  // }else{
  //   StrLCD("NOT SAME");
  // }
  // delay_ms(2000);
  // CmdLCD(CLEAR_LCD);
}
void eint0_isr(void) __irq{
     menu_flag = 1;
    EXTINT = 1<<1;
    //clear EINT0 status in VIC peripheral
    VICVectAddr = 0;
}

int main(){
    Init_system();
    Init_SPI0();
    IOCLR0 = 1<<BUZZER;
    //cfg p0.1 pin as EINT0 input pin
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
      CmdLCD(GOTO_LINE1_POS0);
      DisplayRTCTime(hr,min,sec);
      CmdLCD(GOTO_LINE2_POS0);
      StrLCD("adcVal : ");
          CmdLCD(GOTO_LINE2_POS0+9);
          U32LCD(adcVal);
    }
}
