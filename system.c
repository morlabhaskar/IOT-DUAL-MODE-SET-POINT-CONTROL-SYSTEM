#include<LPC21xx.h>
#include"adc.h"
#include"lcd.h"
#include"rtc.h"
#include"kpm.h"
// #include"spi.h"
void Init_system(){
    IODIR0 |= 1<<23;
    Init_LCD();
    Init_ADC();
    RTC_Init();
    Init_KPM();
    // Init_SPI0();
}
