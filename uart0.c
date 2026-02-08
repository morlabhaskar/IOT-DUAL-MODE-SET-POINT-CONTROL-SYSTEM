#include <lpc21xx.h>
#include "pin_connect_block.h"
#include "pin_functions_defines.h"
#include "uart_defines.h"
#include "uart0.h"
#include "types.h"
char buff[200] = "hello", dummy;
volatile unsigned char i = 0, ch, r_flag;
void uart0_isr(void) __irq{
    if ((U0IIR & 0x04)){ // check if receive interrupt
        ch = U0RBR; /* Read to Clear Receive Interrupt */
        if (i < 200)
            buff[i++] = ch;
    }
    else{
        dummy = U0IIR; // Read to Clear transmit interrupt
    }
    VICVectAddr = 0; /* dummy write */
}
void uart0_init(){
    /* cfgportpinfunc(0,0,TXD0_PIN_0_0 );
           cfgportpinfunc(0,1,RXD0_PIN_0_1);
           U0LCR=WORD_LEN|(1<<DLAB_BIT);
           U0DLL=DIVISOR;
           U0DLM=DIVISOR>>8;
           U0LCR&=~(1<<DLAB_BIT); */
    PINSEL0 |= 0x00000005; /* Enable RxD0 and TxD0              */
    U0LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit     */
    U0DLL = DIVISOR; /* 9600 Baud Rate @ CCLK/4 VPB Clock  */
    U0DLM = DIVISOR >> 8;
    U0LCR = 0x03; /* DLAB = 0  */
    #if UART_INT_ENABLE > 0
        VICIntSelect = 0x00000000; // IRQ
        VICVectAddr0 = (unsigned)uart0_isr;
        VICVectCntl0 = 0x20 | 6; /* UART0 Interrupt */
        VICIntEnable = 1 << 6; /* Enable UART0 Interrupt */
        U0IER = 0x03; /* Enable UART0 RX and THRE Interrupts */
    #endif
}
void uart0_txchar(u8 txbyte){
    /*  U0THR=txbyte;
        while(((U0LSR>>TEMT_BIT)&1)==0);  */
    while (!(U0LSR & 0x20));
    U0THR = txbyte;
}
u8 uart0_rxchar(){
    while (!(U0LSR & 0x01));
    return (U0RBR);
}
void uart0_str(u8 *s){
    while (*s)
        uart0_txchar(*s++);
}
void uart0_float(f32 f){
    int x;
    float temp;
    x = f;
    uart0_int(x);
    uart0_txchar('.');
    temp = (f - x) * 100;
    x = temp;
    uart0_int(x);
}
void uart0_int(u32 n){
    unsigned char a[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int i = 0;
    if (n == 0){
        uart0_txchar('0');
        return;
    }
    else{
        while (n > 0){
            a[i++] = (n % 10) + 48;
            n = n / 10;
        }
        --i;
        for (; i >= 0; i--){
            uart0_txchar(a[i]);
        }
    }
}
