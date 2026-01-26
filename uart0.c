#include<LPC21xx.h>
#include "uart_defines.h"
#include "types.h"
#include "delay.h"


void UART0_Init(void){
    //cfg p0.0 as TXDO and p0.1 as RXDO 
    // CfgPortPinFunc(0,0,TXDO_PIN_0_1);
    // CfgPortPinFunc(0,1,RXDO_PIN_0_1);
    PINSEL0 = ((PINSEL0 &~(3<<(0*2))) | (1<<(0*2)));
    PINSEL0 = ((PINSEL0 &~(3<<(1*2))) | (1<<(1*2)));
    //cfg UART0 line control for:
    //8 data bits,No parity,1 stop bit(8N1)
    //DLAB = 1 (to access baud rate divisor register)
    U0LCR = 0x83; //U0LCR = WORD_LEN | (1<<DLAB_BIT);
    // U0DLL = DIVISOR;
    // U0DLM = DIVISOR >> 8;  //U0DLM = 0;
    U0DLL = 97;
    U0DLM = 0;
    //Disable DLAB (to access THR and RBR sfrs)
    U0LCR &= ~(1<<DLAB_BIT);   //U0LCR = 0x03
}
void UART0_TxChar(u8 TxByte){
    //load the data byte into UART0 Transmit Buffer
    U0THR = TxByte;
    //wait untill Transmission complete status
    while(((U0LSR>>TEMT_BIT)&1)==0);
}
u8 UART0_RxChar(void){
    //wait untill Receiver Data Ready(RDR==1)
    while(((U0LSR>>RDR_BIT)&1)==0);
    //Return the received data byte from Receiver Buffer Register
    return U0RBR;
}

void UART0_TxInt(int num){
    char buf[10];
    int i = 0;
    if(num == 0){
        UART0_TxChar('0');
        return;
    }
    if(num < 0){
        UART0_TxChar('-');
        num = -num;
    }
    while(num > 0){
        buf[i++] = (num % 10) + '0';
        num /= 10;
    }
    while(i--)
        UART0_TxChar(buf[i]);
}

void UART0_TxString(const char *str) {
    while(*str) {
        UART0_TxChar(*str++); 
        // delay_ms(500);
    }
}

void UART0_TxFloat(f32 f,u32 ndp){
    u32 i,j;
    if(f<0.0){
        UART0_TxChar('-');
        f=-f;
    }
    i=f;
    UART0_TxInt(i);
    UART0_TxChar('.');
    for(j=0;j<ndp;j++){
        f=(f-i)*10;
        i=f;
        UART0_TxChar(i+48);
    }
}

void UART0_TxHex(u32 n){
    s32 i=0,nibble;
    u8 a[8];
    if(n==0)
        UART0_TxChar('0');
    else{
        UART0_TxChar('0');
        UART0_TxChar('x');
        while(n){
            nibble=n%16;
            nibble=(nibble>9)?((nibble-10)+'A'):(nibble+48);
            a[i++]=nibble;
            n/=16;
        }
        for(--i;i>=0;i--){
            UART0_TxChar(a[i]);
        }
    }
}

void UART0_TxOct(u32 n){
    u8 a[15];
    s32 i=0,x;
    if(n==0)
        UART0_TxChar('0');
    else{
        UART0_TxChar('O');
        while(n){
            x=n%8;
            a[i++]=x+48;
            n/=8;
        }
        for(--i;i>=0;i--)
            UART0_TxChar(a[i]);
    }
}

