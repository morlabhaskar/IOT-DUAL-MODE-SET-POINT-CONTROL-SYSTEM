
#ifndef _ESP01_H_
#define _ESP01_H_

void esp01_connectAP(void);
void esp01_sendToThingspeak(char *);

#endif

/*#ifndef __ESP01_H__
#define __ESP01_H__

void ESP01_Init(void);
unsigned char ESP01_SendCmd(char *cmd, char *resp, unsigned int timeout_ms);
unsigned char ESP01_ConnectAP(char *ssid, char *pwd);
unsigned char ESP01_SendToThingSpeak(char *api_key, char *field_val);

#endif
*/
