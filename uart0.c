// #include<LPC21xx.h>
// #include<LPC214x.h>
// #include "uart_defines.h"
// #include "types.h"
// #include "pin_connect_block.h"
// #include "delay.h"

// volatile s8 buf[100];
// volatile u8 x = 0;
// volatile u8 ch;
// volatile u8 r_flag = 0;
// volatile u8 dummy;

// void UART0_isr(void) __irq{
//     if((U0IIR & 0x04)){ //check if receive interrupt
// 		ch = U0RBR;	/* Read to Clear Receive Interrupt */
// 		if(ch != 0x0D)
// 			buf[x++] = ch;    
// 		else{
// 			buf[x] = '\0';
// 			x=0;
// 			r_flag = 1;
// 		}
//     }
//     else{
//         dummy=U0IIR; //Read to Clear transmit interrupt
//     }
//    VICVectAddr = 0; /* dummy write */
// }

// void UART0_Init(void){
//     //cfg p0.0 as TXDO and p0.1 as RXDO 
//     CfgPortPinFunc(0,0,TXDO_PIN_0_1);
//     CfgPortPinFunc(0,1,RXDO_PIN_0_1);
//     U0LCR = 0x83; //U0LCR = WORD_LEN | (1<<DLAB_BIT);
//     // U0DLL = DIVISOR;
//     // U0DLM = DIVISOR >> 8;  //U0DLM = 0;
//     U0DLL = 90;
//     U0DLM = 0;
//     U0LCR &= ~(1<<DLAB_BIT);   //U0LCR = 0x03
//     #if UART_INT_ENABLE > 0
//         VICIntSelect = 0x00000000; // IRQ
//         VICVectAddr0 = (unsigned)UART0_isr;
//         VICVectCntl0 = 0x20 | 6; /* UART0 Interrupt */
//         VICIntEnable = 1 << 6;   /* Enable UART0 Interrupt */
//         // U0IIR = 0xc0;
//         // U0FCR = 0xc7;
//         U0IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */
//     #endif
     
// }
// void UART0_TxChar(u8 TxByte){
//     //load the data byte into UART0 Transmit Buffer
//     U0THR = TxByte;
//     //wait untill Transmission complete status
//     while(((U0LSR>>TEMT_BIT)&1)==0);
// }
// u8 UART0_RxChar(void){
//     //wait untill Receiver Data Ready(RDR==1)
//     while(((U0LSR>>RDR_BIT)&1)==0);
//     //Return the received data byte from Receiver Buffer Register
//     return U0RBR;
// }

// void UART0_TxInt(int num){
//     char buf[10];
//     int i = 0;
//     if(num == 0){
//         UART0_TxChar('0');
//         return;
//     }
//     if(num < 0){
//         UART0_TxChar('-');
//         num = -num;
//     }
//     while(num > 0){
//         buf[i++] = (num % 10) + '0';
//         num /= 10;
//     }
//     while(i--)
//         UART0_TxChar(buf[i]);
// }

// void UART0_TxString(const char *str) {
//     while(*str) {
//         UART0_TxChar(*str++); 
//         // delay_ms(500);
//     }
// }

// void UART0_TxFloat(f32 f,u32 ndp){
//     u32 i,j;
//     if(f<0.0){
//         UART0_TxChar('-');
//         f=-f;
//     }
//     i=f;
//     UART0_TxInt(i);
//     UART0_TxChar('.');
//     for(j=0;j<ndp;j++){
//         f=(f-i)*10;
//         i=f;
//         UART0_TxChar(i+48);
//     }
// }

// void UART0_TxHex(u32 n){
//     s32 i=0,nibble;
//     u8 a[8];
//     if(n==0)
//         UART0_TxChar('0');
//     else{
//         UART0_TxChar('0');
//         UART0_TxChar('x');
//         while(n){
//             nibble=n%16;
//             nibble=(nibble>9)?((nibble-10)+'A'):(nibble+48);
//             a[i++]=nibble;
//             n/=16;
//         }
//         for(--i;i>=0;i--){
//             UART0_TxChar(a[i]);
//         }
//     }
// }

// void UART0_TxOct(u32 n){
//     u8 a[15];
//     s32 i=0,x;
//     if(n==0)
//         UART0_TxChar('0');
//     else{
//         UART0_TxChar('O');
//         while(n){
//             x=n%8;
//             a[i++]=x+48;
//             n/=8;
//         }
//         for(--i;i>=0;i--)
//             UART0_TxChar(a[i]);
//     }
// }


#include "uart0.h"
#include<LPC214x.h>

volatile char buff[200];
volatile unsigned int rx_index = 0;
volatile unsigned char r_flag = 0;
volatile unsigned char i;

void UART0_isr(void) __irq{
    char ch;
    if(U0IIR & 0x04){        // RX interrupt
        ch = U0RBR;
        if(ch != '\r'){
            buff[rx_index++] = ch;
            if(rx_index >= 199)
                rx_index = 0;
        }
        else{
            buff[rx_index] = '\0';
            rx_index = 0;
            r_flag = 1;
        }
    }
    VICVectAddr = 0;
}
/*void UART0_ISR(void) __irq
{
    char c = U0RBR;
    buff[i++] = c;
    UART0_Tx(c);   // echo back
    VICVectAddr = 0;
}*/

void InitUART0(void){
    PINSEL0 |= 0x00000005;

    U0LCR = 0x83;
    U0DLL = 97;
    U0DLM = 0;
    U0LCR = 0x03;

    VICIntSelect &= ~(1<<6);
    VICVectAddr1 = (unsigned)UART0_isr;
    VICVectCntl1 = 0x20 | 6;
    VICIntEnable |= (1<<6);

    U0IER = 0x01;   // RX interrupt only
}

void UART0_Tx(char ch){
    while(!(U0LSR & 0x20));
    U0THR = ch;
}

void UART0_Str(char *s){
    while(*s)
        UART0_Tx(*s++);
}

void UART0_Int(unsigned int n){
    char buf[10];
    int i=0;

    if(n==0){ UART0_Tx('0'); return; }

    while(n){
        buf[i++] = (n%10)+'0';
        n/=10;
    }
    while(i--) UART0_Tx(buf[i]);
}


/*#include<LPC21xx.h>
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
}*/
