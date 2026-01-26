#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
int main(){
    u32 key,i=0;
    Init_LCD();
    StrLCD("BHASKAR");
    while(1){
      key=KeyScan();
      i++;
      while(ColScan()==0);
      CmdLCD(GOTO_LINE2_POS0+i);
      U32LCD(key-'0');
    }
}
