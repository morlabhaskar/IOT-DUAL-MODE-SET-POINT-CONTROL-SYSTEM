#include <string.h>
#include "uart0.h"
#include "delay.h"
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"

extern u8 value[20];
extern char buff[200];
extern unsigned char i;

void esp01_connectAP(){
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("AT");
	delay_ms(300);
	uart0_str("AT\r\n");
	i = 0;
	memset(buff, '\0', 200);
	while(i < 4);
	delay_ms(500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff, "OK")){
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_ms(1000);
	}
	else{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_ms(1000);
	}
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("ATE0");
	delay_ms(300);
	uart0_str("ATE0\r\n");
	i = 0;
	memset(buff, '\0', 200);
	while(i < 4);
	delay_ms(500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff, "OK")){
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_ms(1000);
	}
	else{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_ms(1000);
	}
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("AT+CIPMUX");
	delay_ms(300);
	uart0_str("AT+CIPMUX=0\r\n");
	i = 0;
	memset(buff, '\0', 200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff, "OK")){
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_ms(1000);
	}
	else{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_ms(1000);
	}
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("AT+CWQAP");
	delay_ms(300);
	uart0_str("AT+CWQAP\r\n");
	i = 0;
	memset(buff, '\0', 200);
	while(i < 4);
	delay_ms(1500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff, "OK")){
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_ms(1000);
	}
	else{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_ms(1000);
	}
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("AT+CWJAP");
	delay_ms(300);
	// need to change the wifi network name and password
	uart0_str("AT+CWJAP=\"NANI\",\"12345678\"\r\n");
	i = 0;
	memset(buff, '\0', 200);
	while(i < 4);
	delay_ms(2500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff, "WIFI CONNECTED")){
		CmdLCD(0xC0);
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("*WIFI CONNECTED*");
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("**SUCCESSFULLY**");
		// StrLCD("OK");
		delay_ms(1000);
	}
	else{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_ms(1000);
		return;
	}
}
void esp01_sendToThingspeak(u32 val){
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("AT+CIPSTART");
	delay_ms(1000);
	uart0_str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	i = 0;
	memset(buff, '\0', 200);
	while(i < 5);
	delay_ms(2500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff, "CONNECT") || strstr(buff, "ALREADY CONNECTED")){
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_ms(1000);
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD("AT+CIPSEND");
		delay_ms(300);
		uart0_str("AT+CIPSEND=51\r\n");
		i = 0;
		memset(buff, '\0', 200);
		// while(buff[i] != '>');
		delay_ms(500);
		//https://api.thingspeak.com/update?api_key=EHNQ27M29JOAZ1NC&field1=500
		// need to change the thingspeak write API key accordind to your channel
		uart0_str("GET /update?api_key=EHNQ27M29JOAZ1NC&field1=");
		// uart0_str(val);
		uart0_int(val);
		// uart0_float(val);
		uart0_str("\r\n\r\n");
		delay_ms(5000);
		delay_ms(5000);
		buff[i] = '\0';
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD(buff);
		delay_ms(2000);
		if(strstr(buff,"SEND OK")){
			CmdLCD(0x01);
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD("**TEMP UPDATED**");
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD("****IN CLOUD****");
			delay_ms(2000);
			CmdLCD(CLEAR_LCD);
		}
		/*
		else if(!strstr(buff, "CLOSED")){
			CmdLCD(0x01);
			CmdLCD(0x80);
			StrLCD("AT+CIPCLOSE");
			delay_ms(300);
			uart0_str("AT+CIPCLOSE\r\n");
			i = 0;
			memset(buff, '\0', 200);
			while(i < 5);
			delay_ms(2500);
			buff[i] = '\0';
			CmdLCD(0x01);
			CmdLCD(0x80);
			StrLCD(buff);
			delay_ms(2000);
			if(strstr(buff, "OK")){
				CmdLCD(0x01);
				CmdLCD(0x80);
				StrLCD("OK");
				delay_ms(2000);
			}
			else{
				CmdLCD(0x01);
				CmdLCD(0x80);
				StrLCD("ERROR");
				delay_ms(2000);
			}
		}
		*/
		else{
			CmdLCD(0x01);
			StrLCD("TEMP NOT UPDATED");
			delay_ms(1000);
			CmdLCD(CLEAR_LCD);
		}
	}
	else{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_ms(1000);
		CmdLCD(CLEAR_LCD);
		return;
	}
}
void esp01_sendToThingspeak_set_point(u32 val){
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("AT+CIPSTART");
	delay_ms(300);
	uart0_str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	i = 0;
	memset(buff, '\0', 200);
	while(i < 5);
	delay_ms(2500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_ms(2000);
	if(strstr(buff, "CONNECT") || strstr(buff, "ALREADY CONNECTED")){
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_ms(1000);
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD("AT+CIPSEND");
		delay_ms(300);
		uart0_str("AT+CIPSEND=52\r\n");
		i = 0;
		memset(buff, '\0', 200);
		// while(buff[i] != '>');
		delay_ms(500);
		//https://api.thingspeak.com/update?api_key=EHNQ27M29JOAZ1NC&field2=100
		// need to change the thingspeak write API key accordind to your channel
		uart0_str("GET /update?api_key=EHNQ27M29JOAZ1NC&field2=");
		// uart0_str(val);
		uart0_int(val);
		// uart0_float(val);
		uart0_str("\r\n\r\n");
		delay_ms(5000);
		delay_ms(5000);
		buff[i] = '\0';
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD(buff);
		delay_ms(2000);
		if(strstr(buff, "OK")){
			CmdLCD(0x01);
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD("SETPOINT UPDATED");
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD("****IN CLOUD****");
			delay_ms(1000);
			CmdLCD(CLEAR_LCD);
		}
		/*
		else if(!strstr(buff, "CLOSED")){
			CmdLCD(0x01);
			CmdLCD(0x80);
			StrLCD("AT+CIPCLOSE");
			delay_ms(300);
			uart0_str("AT+CIPCLOSE\r\n");
			i = 0;
			memset(buff, '\0', 200);
			while (i < 5);
			delay_ms(2500);
			buff[i] = '\0';
			CmdLCD(0x01);
			CmdLCD(0x80);
			StrLCD(buff);
			delay_ms(2000);
			if (strstr(buff, "OK")){
				CmdLCD(0x01);
				CmdLCD(0x80);
				StrLCD("OK");
				delay_ms(2000);
			}
			else{
				CmdLCD(0x01);
				CmdLCD(0x80);
				StrLCD("ERROR");
				delay_ms(2000);
			}
		}
		*/
		else{
			CmdLCD(0x01);
			StrLCD("DATA NOT UPDATED");
			delay_ms(1000);
			CmdLCD(CLEAR_LCD);
		}
	}
	else{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_ms(1000);
		CmdLCD(CLEAR_LCD);
		return;
	}
}
void esp01_readFromThingspeak(void){
	char *ptr = NULL, *p = NULL;
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("CIPSTART");
	delay_ms(300);
	uart0_str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	i = 0;
	memset(buff, '\0', 200);
	while (i < 5);
	delay_ms(2500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_ms(2000);
	if (strstr(buff, "CONNECT") || strstr(buff, "ALREADY CONNECTED")){
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD("CIPSEND");
		delay_ms(300);
		/* 48 bytes approx */
		uart0_str("AT+CIPSEND=66\r\n");
		//https://api.thingspeak.com/channels/3250917/feeds/last.json?api_key=K822G9MXGK99RY26
		delay_ms(500);
		uart0_str("GET /channels/3250917/feeds/last.json?api_key=K822G9MXGK99RY26\r\n\r\n");
		delay_ms(5000);
		buff[i] = '\0';
		ptr = strrchr(buff, ':');
		p = strrchr(buff, '}');
		p -= 1;
		ptr += 2;
		ptr[p - (ptr)] = '\0';
		// value=ptr;
		strcpy((char *)value, ptr);
		if ((char *)value){
			CmdLCD(0x01);
			CmdLCD(0x80);
			StrLCD((s8 *)value);
			delay_ms(2000);
			CmdLCD(CLEAR_LCD);
		}
		uart0_str("AT+CIPCLOSE\r\n");
		delay_ms(1000);
		CmdLCD(CLEAR_LCD);
	}
	else{
		CmdLCD(0x01);
		StrLCD("ERROR");
		delay_ms(2000);
		CmdLCD(CLEAR_LCD);
		return;
	}
}
