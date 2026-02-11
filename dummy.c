// uart0.c
#include <lpc21xx.h>
#include "pin_connect_block.h"
#include "pin_functions_definition.h"
#include "uart0_defines.h"
#include "uart0.h"
#include "types.h"
char buff[200] = "hello", dummy;
unsigned char i = 0, ch, r_flag;
void uart0_isr(void) __irq

{

    if ((U0IIR & 0x04)) // check if receive interrupt

    {

        ch = U0RBR; /* Read to Clear Receive Interrupt */

        if (i < 200)

            buff[i++] = ch;
    }

    else

    {

        dummy = U0IIR; // Read to Clear transmit interrupt
    }

    VICVectAddr = 0; /* dummy write */
}
void uart0_init()
{
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
void uart0_txchar(u8 txbyte)
{
    /*  U0THR=txbyte;
           while(((U0LSR>>TEMT_BIT)&1)==0);  */
    while (!(U0LSR & 0x20))
        ;

    U0THR = txbyte;
}
u8 uart0_rxchar()
{
    while (!(U0LSR & 0x01))
        ;

    return (U0RBR);
}
void uart0_str(u8 *s)
{
    while (*s)

        uart0_txchar(*s++);
}

void uart0_float(f32 f)
{
    int x;
    float temp;
    x = f;
    uart0_int(x);
    uart0_txchar('.');
    temp = (f - x) * 100;
    x = temp;
    uart0_int(x);
}
void uart0_int(u32 n)
{
    unsigned char a[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int i = 0;
    if (n == 0)
    {
        uart0_txchar('0');
        return;
    }
    else
    {
        while (n > 0)
        {
            a[i++] = (n % 10) + 48;
            n = n / 10;
        }
        --i;
        for (; i >= 0; i--)
        {
            uart0_txchar(a[i]);
        }
    }
}

// esp01.c=================================================================================================

#include <string.h>
#include "uart0.h"
#include "delay.h"
#include "types.h"
#include "lcd.h"

extern u8 value[20];
extern char buff[200];
extern unsigned char i;

void esp01_connectAP()
{
    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("AT");
    delay_ms(1000);
    uart0_str("AT\r\n");
    i = 0;
    memset(buff, '\0', 200);
    while (i < 4)
        ;
    delay_ms(500);
    buff[i] = '\0';
    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd(buff);
    delay_ms(2000);
    if (strstr(buff, "OK"))
    {
        cmdlcd(0xC0);
        strlcd("OK");
        delay_ms(1000);
    }
    else
    {
        cmdlcd(0xC0);
        strlcd("ERROR");
        delay_ms(1000);
    }

    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("ATE0");
    delay_ms(1000);
    uart0_str("ATE0\r\n");
    i = 0;
    memset(buff, '\0', 200);
    while (i < 4)
        ;
    delay_ms(500);
    buff[i] = '\0';
    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd(buff);
    delay_ms(2000);
    if (strstr(buff, "OK"))
    {
        cmdlcd(0xC0);
        strlcd("OK");
        delay_ms(1000);
    }
    else
    {
        cmdlcd(0xC0);
        strlcd("ERROR");
        delay_ms(1000);
    }

    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("AT+CIPMUX");
    delay_ms(1000);
    uart0_str("AT+CIPMUX=0\r\n");
    i = 0;
    memset(buff, '\0', 200);
    while (i < 4)
        ;
    delay_ms(500);
    buff[i] = '\0';
    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd(buff);
    delay_ms(2000);
    if (strstr(buff, "OK"))
    {
        cmdlcd(0xC0);
        strlcd("OK");
        delay_ms(1000);
    }
    else
    {
        cmdlcd(0xC0);
        strlcd("ERROR");
        delay_ms(1000);
    }

    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("AT+CWQAP");
    delay_ms(1000);
    uart0_str("AT+CWQAP\r\n");
    i = 0;
    memset(buff, '\0', 200);
    while (i < 4)
        ;
    delay_ms(1500);
    buff[i] = '\0';
    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd(buff);
    delay_ms(2000);
    if (strstr(buff, "OK"))
    {
        cmdlcd(0xC0);
        strlcd("OK");
        delay_ms(1000);
    }
    else
    {
        cmdlcd(0xC0);
        strlcd("ERROR");
        delay_ms(1000);
    }

    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("AT+CWJAP");
    delay_ms(1000);
    // need to change the wifi network name and password
    uart0_str("AT+CWJAP=\"123\",\"12345678\"\r\n");
    i = 0;
    memset(buff, '\0', 200);
    while (i < 4)
        ;
    delay_ms(2500);
    buff[i] = '\0';
    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd(buff);
    delay_ms(2000);
    if (strstr(buff, "WIFI CONNECTED"))
    {
        cmdlcd(0xC0);
        strlcd("OK");
        delay_ms(1000);
    }
    else
    {
        cmdlcd(0xC0);
        strlcd("ERROR");
        delay_ms(1000);
        return;
    }
}

void esp01_sendToThingspeak(f32 val)
{
    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("AT+CIPSTART");
    delay_ms(1000);
    uart0_str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    i = 0;
    memset(buff, '\0', 200);
    while (i < 5)
        ;
    delay_ms(2500);
    buff[i] = '\0';
    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd(buff);
    delay_ms(2000);
    if (strstr(buff, "CONNECT") || strstr(buff, "ALREADY CONNECTED"))
    {
        cmdlcd(0xC0);
        strlcd("OK");
        delay_ms(1000);

        cmdlcd(0x01);
        cmdlcd(0x80);
        strlcd("AT+CIPSEND");
        delay_ms(1000);
        uart0_str("AT+CIPSEND=52\r\n");
        i = 0;
        memset(buff, '\0', 200);
        // while(buff[i] != '>');
        delay_ms(500);
        // need to change the thingspeak write API key accordind to your channel
        uart0_str("GET /update?api_key=AW76V91LT5W2U63Z&field1=");
        // uart0_str(val);
        uart0_float(val);
        uart0_str("\r\n\r\n");
        delay_ms(5000);
        delay_ms(5000);
        buff[i] = '\0';
        cmdlcd(0x01);
        cmdlcd(0x80);
        strlcd(buff);
        delay_ms(2000);
        if (strstr(buff, "SEND OK"))
        {
            cmdlcd(0x01);
            strlcd("DATA UPDATED");
            delay_ms(1000);
        }

        else if (!strstr(buff, "CLOSED"))
        {
            cmdlcd(0x01);
            cmdlcd(0x80);
            strlcd("AT+CIPCLOSE");
            delay_ms(1000);
            uart0_str("AT+CIPCLOSE\r\n");
            i = 0;
            memset(buff, '\0', 200);
            while (i < 5)
                ;
            delay_ms(2500);
            buff[i] = '\0';
            cmdlcd(0x01);
            cmdlcd(0x80);
            strlcd(buff);
            delay_ms(2000);
            if (strstr(buff, "OK"))
            {
                cmdlcd(0x01);
                cmdlcd(0x80);
                strlcd("OK");
                delay_ms(2000);
            }
            else
            {
                cmdlcd(0x01);
                cmdlcd(0x80);
                strlcd("ERROR");
                delay_ms(2000);
            }
        }
        else
        {
            cmdlcd(0x01);
            strlcd("DATA NOT UPDATED");
            delay_ms(1000);
        }
    }
    else
    {
        cmdlcd(0xC0);
        strlcd("ERROR");
        delay_ms(1000);
        return;
    }
}
^M

    esp01_sendToThingspeak_set_point(float val) ^
    M {
      ^M
          cmdlcd(0x01);
      cmdlcd(0x80);
      strlcd("AT+CIPSTART");
      delay_ms(1000);
      uart0_str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
      i = 0;
      memset(buff, '\0', 200);
      while (i < 5)
          ;
      delay_ms(2500);
      buff[i] = '\0';
      cmdlcd(0x01);
      cmdlcd(0x80);
      strlcd(buff);
      delay_ms(2000);
      if (strstr(buff, "CONNECT") || strstr(buff, "ALREADY CONNECTED"))
      {
          cmdlcd(0xC0);
          strlcd("OK");
          delay_ms(1000);

          cmdlcd(0x01);
          cmdlcd(0x80);
          strlcd("AT+CIPSEND");
          delay_ms(1000);
          uart0_str("AT+CIPSEND=52\r\n");
          i = 0;
          memset(buff, '\0', 200);
          // while(buff[i] != '>');
          delay_ms(500);
          // need to change the thingspeak write API key accordind to your channel
          uart0_str("GET /update?api_key=2Q89OQYP2T26DANX&field1=");
          // uart0_str(val);
          uart0_float(val);
          uart0_str("\r\n\r\n");
          delay_ms(5000);
          delay_ms(5000);
          buff[i] = '\0';
          cmdlcd(0x01);
          cmdlcd(0x80);
          strlcd(buff);
          delay_ms(2000);
          if (strstr(buff, "OK"))
          {
              cmdlcd(0x01);
              strlcd("DATA UPDATED");
              delay_ms(1000);
          }

          else if (!strstr(buff, "CLOSED"))
          {
              cmdlcd(0x01);
              cmdlcd(0x80);
              strlcd("AT+CIPCLOSE");
              delay_ms(1000);
              uart0_str("AT+CIPCLOSE\r\n");
              i = 0;
              memset(buff, '\0', 200);
              while (i < 5)
                  ;
              delay_ms(2500);
              buff[i] = '\0';
              cmdlcd(0x01);
              cmdlcd(0x80);
              strlcd(buff);
              delay_ms(2000);
              if (strstr(buff, "OK"))
              {
                  cmdlcd(0x01);
                  cmdlcd(0x80);
                  strlcd("OK");
                  delay_ms(2000);
              }
              else
              {
                  cmdlcd(0x01);
                  cmdlcd(0x80);
                  strlcd("ERROR");
                  delay_ms(2000);
              }
          }
          else
          {
              cmdlcd(0x01);
              strlcd("DATA NOT UPDATED");
              delay_ms(1000);
          }
      }
      else
      {
          cmdlcd(0xC0);
          strlcd("ERROR");
          delay_ms(1000);
          return;
      }
      ^M
    }

    void esp01_readFromThingspeak()
{
    char *ptr = NULL, *p = NULL;
    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("CIPSTART");
    delay_ms(1000);

    uart0_str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    i = 0;
    memset(buff, '\0', 200);
    while (i < 5)
        ;
    delay_ms(2500);
    buff[i] = '\0';

    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd(buff);
    delay_ms(2000);

    if (strstr(buff, "CONNECT") || strstr(buff, "ALREADY CONNECTED"))
    {
        cmdlcd(0x01);
        cmdlcd(0x80);
        strlcd("CIPSEND");
        delay_ms(1000);

        /* 48 bytes approx */
        uart0_str("AT+CIPSEND=66\r\n");
        delay_ms(500);

        uart0_str("GET /channels/3240963/feeds/last.json?api_key=E589ZQ1IHA7665RJ\r\n\r\n");
        delay_ms(5000);

        buff[i] = '\0';
        ptr = strrchr(buff, ':');
        ^M
            p = strrchr(buff, '}');
        ^M
            p -= 1;
        ^M
            ptr += 2;
        ^M
            ptr[p - (ptr)] = '\0';
        ^M
            // value=ptr;^M
            strcpy((char *)value, ptr);
        if ((char *)value)
        {
            cmdlcd(0x01);
            cmdlcd(0x80);
            strlcd((s8 *)value);
            delay_ms(2000);
        }

        uart0_str("AT+CIPCLOSE\r\n");
        delay_ms(1000);
    }
    else
    {
        cmdlcd(0x01);
        strlcd("ERROR");
        delay_ms(2000);
        ^M return;
    }
}

// eint.c==========================================================

#include <lpc21xx.h>
#include "types.h"
#include "pin_connect_block.h"
#include "pin_functions_definition.h"
#include "lcd.h"
#include "lcd_defines.h"
#define EINT0_VIC_CHNO 15
#define EINT0_STATUS_LED 3
#define EINT0_PIN_0_1 3
extern int flag;
void eint0_isr(void) __irq;
void set_point_change(void);
void eint()
{
    IODIR1 |= 1 << EINT0_STATUS_LED;
    cfgportpinfunc(0, 3, EINT0_PIN_0_1);
    VICIntEnable = 1 << EINT0_VIC_CHNO;
    VICVectCntl1 = (1 << 5) | EINT0_VIC_CHNO;
    VICVectAddr1 = (u32)eint0_isr;
    EXTMODE = 1 << 1;
}
void eint0_isr(void) __irq
{
    // set_point_change();
    flag = 1;
    cmdlcd(0x01);
    EXTINT = 1 << 1;
    VICVectAddr = 0;
}

//
