#include"types.h"
#include"delay.h"
#include"lcd.h"
#include"uart0.h"
#include<string.h>
#include<stdio.h>
#include"lcd_defines.h"
// extern char buff[200];
// extern unsigned char i;
char buff[200];
unsigned char i;

void ConnectESP(void){
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("AT");
	delay_ms(1000);
	UART0_TxString("AT\r\n");
	i=0;
	memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK")){
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("OK");
		delay_ms(1000);		
	}else{
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("ERROR");
		delay_ms(1000);		
		return;
	}
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("ATE0");
	delay_ms(1000);
	UART0_TxString("ATE0\r\n");
	i=0;
	memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK")){
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("OK");
		delay_ms(1000);		
	}else{
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("ERROR");
		delay_ms(1000);		
		return;
	}
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("AT+CIPMUX");
	delay_ms(1000);
	UART0_TxString("AT+CIPMUX=0\r\n");//Single TCP/UDP connection mode
	i=0;
	memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK")){
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("OK");
		delay_ms(1000);		
	}
	else{
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("ERROR");
		delay_ms(1000);		
		return;
	}
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("AT+CWQAP");
	delay_ms(1000);
	UART0_TxString("AT+CWQAP\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(1500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK")){
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("OK");
		delay_ms(1000);		
	}else{
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("ERROR");
		delay_ms(1000);		
		return;
	}
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("AT+CWJAP");
	delay_ms(1000);
	//need to change the wifi network name and password
	UART0_TxString("AT+CWJAP=\"NANI\",\"6303431098\"\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(2500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff,"WIFI CONNECTED")){
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("OK");
		delay_ms(1000);		
	}else{
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("ERROR");
		delay_ms(1000);		
		return;
	}
}
void updateThingSpeak(char *val){
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("AT+CIPSTART");
	delay_ms(1000);
	UART0_TxString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	i=0;memset(buff,'\0',200);
	while(i<5);
	delay_ms(2500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff,"CONNECT") || strstr(buff,"ALREADY CONNECTED")){
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("OK");
		delay_ms(1000);
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("AT+CIPSEND");
		delay_ms(1000);
		UART0_TxString("AT+CIPSEND=51\r\n");
		i=0;memset(buff,'\0',200);
		//while(buff[i] != '>');
		delay_ms(500);
		//need to change the thingspeak write API key accordind to your channel
		UART0_TxString("GET /update?api_key=PMSMTM72RNBJSXYH&field1=");
		UART0_TxString(val);
		UART0_TxString("\r\n\r\n");
		delay_ms(5000);
		delay_ms(5000);
		buff[i] = '\0';
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD(buff);
		delay_ms(2000);
		if(strstr(buff,"SEND OK")){
			CmdLCD(CLEAR_LCD);
			StrLCD("DATA UPDATED");
			delay_ms(1000);			
		}
		else{
			CmdLCD(CLEAR_LCD);
			StrLCD("DATA NOT UPDATED");
			delay_ms(1000);	
		}
	}else{
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("ERROR");
		delay_ms(1000);		
		return;
	}	
}
