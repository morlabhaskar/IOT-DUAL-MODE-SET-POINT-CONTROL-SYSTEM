
#include "esp01.h"
#include "uart0.h"
#include "uart0.h"
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"
#include <string.h>

/* fake connecton testing 
void esp01_connectAP(void)
 { UART0_Str("AT\r\n");
    delay_ms(200);
    UART0_Str("OK\r\n");

    UART0_Str("ATE0\r\n");
    delay_ms(200);
    UART0_Str("OK\r\n");

    UART0_Str("AT+CIPMUX=0\r\n");
    delay_ms(200);
    UART0_Str("OK\r\n");

    UART0_Str("AT+CWJAP=\"SIM\",\"1234\"\r\n");
    delay_ms(200);
    UART0_Str("WIFI CONNECTED\r\n");
	}*/
	
//unsigned char i;

// sir code
void esp01_connectAP(void){
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("AT");
	delay_ms(1000);
	UART0_Str("AT\r\n");
	i=0;
	memset((void*)buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD((s8*)buff);
	delay_ms(2000);
	if(strstr((char*)buff,"OK")){
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
	UART0_Str("ATE0\r\n");
	i=0;
	memset((void*)buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD((s8*)buff);
	delay_ms(2000);
	if(strstr((char*)buff,"OK")){
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
	UART0_Str("AT+CIPMUX=0\r\n");//Single TCP/UDP connection mode
	i=0;
	memset((void*)buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD((s8*)buff);
	delay_ms(2000);
	if(strstr((char*)buff,"OK")){
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
	UART0_Str("AT+CWQAP\r\n");
	i=0;memset((void*)buff,'\0',200);
	while(i<4);
	delay_ms(1500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD((s8*)buff);
	delay_ms(2000);
	if(strstr((char*)buff,"OK")){
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
	UART0_Str("AT+CWJAP=\"NANI\",\"6303431098\"\r\n");
	i=0;memset((void*)buff,'\0',200);
	while(i<4);
	delay_ms(2500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD((s8*)buff);
	delay_ms(2000);
	if(strstr((char*)buff,"WIFI CONNECTED")){
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
void esp01_sendToThingspeak(char *val){
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("AT+CIPSTART");
	delay_ms(1000);
	UART0_Str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	i=0;memset((void*)buff,'\0',200);
	while(i<5);
	delay_ms(2500);
	buff[i] = '\0';
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD((s8*)buff);
	delay_ms(2000);
	if(strstr((char*)buff,"CONNECT") || strstr((char*)buff,"ALREADY CONNECTED")){
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("OK");
		delay_ms(1000);
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("AT+CIPSEND");
		delay_ms(1000);
		UART0_Str("AT+CIPSEND=51\r\n");
		i=0;memset((void*)buff,'\0',200);
		//while(buff[i] != '>');
		delay_ms(500);
		//need to change the thingspeak write API key accordind to your channel
		UART0_Str("GET /update?api_key=ZNGKKV1G8F3C6F0Z&field1=");
		UART0_Str(val);
		UART0_Str("\r\n\r\n");
		delay_ms(5000);
		delay_ms(5000);
		buff[i] = '\0';
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD((s8*)buff);
		delay_ms(2000);
		if(strstr((char*)buff,"SEND OK")){
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

/*
// corrected by chatgpt
void esp01_connectAP(void){
    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("AT");
    UART0_Str("AT\r\n");
    r_flag = 0;                      // reset RX flag
    while(r_flag == 0);              // wait till ISR completes RX
    r_flag = 0;
    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD((s8*)buff);                    // show response
    delay_ms(1500);
    if(!strstr((char*)buff, "OK"))
        return;

    CmdLCD(CLEAR_LCD);
    StrLCD("ATE0");
    UART0_Str("ATE0\r\n");

    r_flag = 0;
    while(r_flag == 0);
    r_flag = 0;

    if(!strstr((char*)buff, "OK"))
        return;

    CmdLCD(CLEAR_LCD);
    StrLCD("CIPMUX");

    UART0_Str("AT+CIPMUX=0\r\n");

    r_flag = 0;
    while(r_flag == 0);
    r_flag = 0;

    if(!strstr((char*)buff, "OK"))
        return;

    CmdLCD(CLEAR_LCD);
    StrLCD("CONNECTING");

    UART0_Str("AT+CWJAP=\"NANI\",\"6303431098\"\r\n");

    r_flag = 0;
    while(r_flag == 0);              // WiFi response
    r_flag = 0;

    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD((s8*)buff);

    delay_ms(2000);
}
// coorected by chatgpt 
void esp01_sendToThingspeak(char *val)
{
    CmdLCD(CLEAR_LCD);
    StrLCD("CIPSTART");

    UART0_Str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");

    r_flag = 0;
    while(r_flag == 0);
    r_flag = 0;

    if(!(strstr((char*)buff,"CONNECT") || strstr((char*)buff,"ALREADY")))
        return;

    CmdLCD(CLEAR_LCD);
    StrLCD("CIPSEND");

    UART0_Str("AT+CIPSEND=51\r\n");

    r_flag = 0;
    while(r_flag == 0);
    r_flag = 0;

    if(!strstr((char*)buff, ">"))
        return;

    // UART0_Str("GET /update?api_key=PMSMTM72RNBJSXYH&field1=");
    UART0_Str("GET /update?api_key=ZNGKKV1G8F3C6F0Z&field1=");
    UART0_Str(val);
    UART0_Str("\r\n\r\n");

    r_flag = 0;
    while(r_flag == 0);
    r_flag = 0;

    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD((s8*)buff);

    delay_ms(2000);
}
*/
/* ===== INTERNAL HELPER ===== */
/*static unsigned char ESP01_WaitForResponse(char *resp, unsigned int timeout_ms)
{
    unsigned int t = 0;

    while(!r_flag && t < timeout_ms)
    {
        delay_ms(1);
        t++;
    }

    if(!r_flag)
        return 0;     // Timeout

    r_flag = 0;

    if(strstr((char*)buff, resp))
        return 1;

    return 0;
}

unsigned char ESP01_SendCmd(char *cmd, char *resp, unsigned int timeout_ms)
{
    UART0_Str(cmd);
    UART0_Str("\r\n");

    memset((char*)buff, 0, sizeof(buff));
    r_flag = 0;

    return ESP01_WaitForResponse(resp, timeout_ms);
}

void ESP01_Init(void)
{
    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("ESP INIT");

    ESP01_SendCmd("AT", "OK", 1000);
    ESP01_SendCmd("ATE0", "OK", 1000);
    ESP01_SendCmd("AT+CWMODE=1", "OK", 1000);
}

unsigned char ESP01_ConnectAP(char *ssid, char *pwd)
{
    char cmd[80];

    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("WIFI CONNECT");

    strcpy(cmd, "AT+CWJAP=\"");
    strcat(cmd, ssid);
    strcat(cmd, "\",\"");
    strcat(cmd, pwd);
    strcat(cmd, "\"");

    return ESP01_SendCmd(cmd, "WIFI CONNECTED", 8000);
}

unsigned char ESP01_SendToThingSpeak(char *api_key, char *field_val)
{
    char cmd[120];

    if(!ESP01_SendCmd(
        "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80",
        "CONNECT",
        5000))
        return 0;

    sprintf(cmd,
        "GET /update?api_key=%s&field1=%s\r\n\r\n",
        api_key, field_val);

    ESP01_SendCmd("AT+CIPSEND=51", ">", 3000);
    UART0_Str(cmd);

    return ESP01_WaitForResponse("SEND OK", 6000);
}
*/
