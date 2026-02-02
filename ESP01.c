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

/*sir*/
extern char buff[200];
extern unsigned char i;

void esp01_connectAP(){
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD("AT");
	delay_ms(1000);
	UART0_Str("AT\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK")){
		Write_CMD_LCD(0xC0);
		Write_str_LCD("OK");
		delay_ms(1000);		
	}else{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("ERROR");
		delay_ms(1000);		
		return;
	}
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD("ATE0");
	delay_ms(1000);
	UART0_Str("ATE0\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK")){
		Write_CMD_LCD(0xC0);
		Write_str_LCD("OK");
		delay_ms(1000);		
	}else{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("ERROR");
		delay_ms(1000);		
		return;
	}
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD("AT+CIPMUX");
	delay_ms(1000);
	UART0_Str("AT+CIPMUX=0\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK")){
		Write_CMD_LCD(0xC0);
		Write_str_LCD("OK");
		delay_ms(1000);		
	}
	else{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("ERROR");
		delay_ms(1000);		
		return;
	}
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD("AT+CWQAP");
	delay_ms(1000);
	UART0_Str("AT+CWQAP\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(1500);
	buff[i] = '\0';
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK")){
		Write_CMD_LCD(0xC0);
		Write_str_LCD("OK");
		delay_ms(1000);		
	}else{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("ERROR");
		delay_ms(1000);		
		return;
	}
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD("AT+CWJAP");
	delay_ms(1000);
	//need to change the wifi network name and password
	UART0_Str("AT+CWJAP=\"Airtel5\",\"vector@40114959\"\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(2500);
	buff[i] = '\0';
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD(buff);
	delay_ms(2000);
	if(strstr(buff,"WIFI CONNECTED")){
		Write_CMD_LCD(0xC0);
		Write_str_LCD("OK");
		delay_ms(1000);		
	}else{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("ERROR");
		delay_ms(1000);		
		return;
	}
}
void esp01_sendToThingspeak(char *val){
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD("AT+CIPSTART");
	delay_ms(1000);
	UART0_Str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	i=0;memset(buff,'\0',200);
	while(i<5);
	delay_ms(2500);
	buff[i] = '\0';
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD(buff);
	delay_ms(2000);
	if(strstr(buff,"CONNECT") || strstr(buff,"ALREADY CONNECTED")){
		Write_CMD_LCD(0xC0);
		Write_str_LCD("OK");
		delay_ms(1000);
		Write_CMD_LCD(0x01);
		Write_CMD_LCD(0x80);
		Write_str_LCD("AT+CIPSEND");
		delay_ms(1000);
		UART0_Str("AT+CIPSEND=51\r\n");
		i=0;memset(buff,'\0',200);
		//while(buff[i] != '>');
		delay_ms(500);
		//need to change the thingspeak write API key accordind to your channel
		UART0_Str("GET /update?api_key=PMSMTM72RNBJSXYH&field1=");
		UART0_Str(val);
		UART0_Str("\r\n\r\n");
		delay_ms(5000);
		delay_ms(5000);
		buff[i] = '\0';
		Write_CMD_LCD(0x01);
		Write_CMD_LCD(0x80);
		Write_str_LCD(buff);
		delay_ms(2000);
		if(strstr(buff,"SEND OK")){
			Write_CMD_LCD(0x01);
			Write_str_LCD("DATA UPDATED");
			delay_ms(1000);			
		}
		/*
		else if(!strstr(buff,"CLOSED"))
		{
			Write_CMD_LCD(0x01);
			Write_CMD_LCD(0x80);
			Write_str_LCD("AT+CIPCLOSE");
			delay_ms(1000);
			UART0_Str("AT+CIPCLOSE\r\n");
			i=0;memset(buff,'\0',200);
			while(i<5);
			delay_ms(2500);
			buff[i] = '\0';
			Write_CMD_LCD(0x01);
			Write_CMD_LCD(0x80);
			Write_str_LCD(buff);
			delay_ms(2000);
			if(strstr(buff,"OK"))
			{
				Write_CMD_LCD(0x01);
				Write_CMD_LCD(0x80);
				Write_str_LCD("OK");
				delay_ms(2000);				
			}
			else
			{
				Write_CMD_LCD(0x01);
				Write_CMD_LCD(0x80);
				Write_str_LCD("ERROR");
				delay_ms(2000);		
			}
		
		}*/
		else{
			Write_CMD_LCD(0x01);
			Write_str_LCD("DATA NOT UPDATED");
			delay_ms(1000);	
		}
	}else{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("ERROR");
		delay_ms(1000);		
		return;
	}	
}
