#include "types.h"
#include "delay.h"
#include "arduino_defines.h"
#include "arduino.h"
#include "lcd_defines.h"

void WriteLCD(u8 byte){
    //select write operations
    digitalWrite(LCD_RW,0);//17
    //write any byte onto data pins(d0-d7)
    write2Pins(LCD_DATA,8,byte);//8
    //provide high to low pulse for latching
    digitalWrite(LCD_EN,1);//18
    delay_us(1);
    digitalWrite(LCD_EN,0);//18
    //byte between byte time
    delay_ms(2);
}
void CmdLCD(u8 cmdByte){
    //select cmd register
    digitalWrite(LCD_RS,0);//16
    //write to cmd register via data pins
    WriteLCD(cmdByte);
}
void Init_LCD(void){
    //cfg lcd data pins & 3 control pins as gpio out
    portMode(LCD_DATA,11,OUTPUT);//8
    //power on delay
    delay_ms(15);
    //initialize sequence start
    CmdLCD(MODE_8BIT_1LINE);//0x30
    delay_us(4100);
    CmdLCD(MODE_8BIT_1LINE);//0x30
    delay_us(100);
    CmdLCD(MODE_8BIT_1LINE);//0x30
    CmdLCD(MODE_8BIT_2LINE);//0x38
    CmdLCD(DSP_ON_CUR_OFF);//0x0C
    // CmdLCD(DSP_ON_CUR_BLINK);
    CmdLCD(CLEAR_LCD);//0x01
    CmdLCD(SHIFT_CUR_RIGHT);//0x06
    //initialization sequence end
}
void CharLCD(u8 asciival){
    //select data register
    digitalWrite(LCD_RS,1);//16
    //write to DDRAM / display via data register
    WriteLCD(asciival);
}
void StrLCD(s8 *p){
    while(*p){
        CharLCD(*p++);
    }
}
void U32LCD(u32 n){
    u8 digit[10];
    s32 i=0;
    if(n==0) CharLCD('0');
    else{
        while(n>0){
            digit[i++]=(n%10)+48;
            n/=10;
        }
        for(--i;i>=0;i--){
            CharLCD(digit[i]);
        }
    }
}
void S32LCD(s32 n){
    if(n>0){
        CharLCD('-');
        n=-n;
    }
    U32LCD(n);
}
void F32LCD(f32 fn,u32 nDp){
    u32 n,i;
    if(fn<0.0){
        CharLCD('-');
        fn=-fn;
    }
    n=fn;
    U32LCD(n);
    CharLCD('.');
    for(i=0;i<nDp;i++){
        fn=(fn-n)*10;
        n=fn;
        CharLCD(n+48);
    }
}
void BuildCGRAM(u8 *p,u8 nBytes){
    u32 i;
    //point to CGRAM start
    CmdLCD(GOTO_CGRAM_START);
    //select data register
    digitalWrite(LCD_RS,1);
    //write to CGRAM , required bytes for custom char
    for(i=0;i<nBytes;i++){
        WriteLCD(p[i]);
    }
    //point back DDRAM
    CmdLCD(GOTO_LINE1_POS0);
}
int is_digit(char dig){//'a'
    if(dig>='0' && dig<='9') return 1;
    return 0;
}
int my_atoi(char *str){
    int i=0,sign=1,decimal=0;
    if(str[i]=='-'||str[i]=='+'){
        if(str[i]=='-'){
            sign=-1;
        }
        i++;
    }
    while(is_digit(str[i])){
        decimal = decimal * 10 + (str[i]-'0');
        i++;                                
    }                                         
    return decimal*sign;
}

