#include<LPC214x.h>
#include "lcd.h"
#include "delay.h"
#include "adc.h"
#include "rtc.h"
#include "lcd_defines.h"
#include "pin_connect_block.h"
s32 hr=0,min=0,sec=0;
u32 adcVal;
f32 analog;
int main(){
    Init_LCD();
    Init_ADC();
    RTC_Init();
    SetRTCTimeInfo(hr,min,sec);
    while(1){
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
