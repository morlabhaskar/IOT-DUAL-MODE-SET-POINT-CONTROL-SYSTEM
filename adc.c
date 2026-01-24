#include<LPC21xx.h>
#include "types.h"
#include "delay.h"
#include "adc_defines.h"
#include "pin_functions_defines.h"
#include "pin_connect_block.h"

void Init_ADC(void){
    //cfg p0.27 as AINO input pin
    CfgPortPinFunc(0,27,3);
    //cfg ADCR
    ADCR = (1<<PDN_BIT)|(CLKDIV<<CLKDIV_BITS);
}
void Read_ADC(u32 chNo,u32 *adcDVal,f32 *analogReading){
    //clear any previous channel sel
    ADCR &= 0xFFFFFF00;
    //update with new/req channel & start conversion
    ADCR |= (1<<START_CONV_BIT)|(1<<chNo);
    //wait untill conversion time >=2.44us
    delay_us(3);
    //check status of conversion
    while(((ADDR>>DONE_BIT)&1)==0);
    //stop conversion
    ADCR &= ~(1<<START_CONV_BIT);
    //read the digital result after successfull conversion
    *adcDVal=((ADDR>>RESULT_BITS)&1023);
    //convert to equivalent analog reading in volts
    *analogReading=*adcDVal*(3.3/1024);
}

