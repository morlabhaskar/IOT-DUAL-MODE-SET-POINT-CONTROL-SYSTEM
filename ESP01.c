#include"types.h"
#include"delay.h"
#include"uart0.h"
#include<string.h>
#include<stdio.h>
char txBuf[150];
void ESP01_Init(void){
    UART0_TxString("AT\r\n");
    delay_ms(1000);

    UART0_TxString("AT+CWMODE=1\r\n");     // Station mode
    delay_ms(1000);

    UART0_TxString("AT+CWJAP=\"Excitel_2.4G\",\"12345678\"\r\n");
    delay_ms(6000);
}
void ThingSpeak_Update(u32 value){
    /* Open TCP connection */
    UART0_TxString(
        "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    delay_ms(2000);

    /* Build GET request */
    sprintf(txBuf,
            "GET /update?api_key=ZNGKKV1G8F3C6F0Z&field1=%lu\r\n",
            value);

    /* Send data length */
    sprintf(txBuf,
            "AT+CIPSEND=%d\r\n",
            strlen(txBuf));
    UART0_TxString(txBuf);
    delay_ms(500);

    /* Send actual GET request */
    sprintf(txBuf,
            "GET /update?api_key=ZNGKKV1G8F3C6F0Z&field1=%lu\r\n",
            value);
    UART0_TxString(txBuf);
    delay_ms(1500);

    /* Close connection */
    UART0_TxString("AT+CIPCLOSE\r\n");
}
