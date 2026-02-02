#ifndef __ESP01_H__
#define __ESP01_H__
#include"types.h"

void ESP01_Init(void);
void ThingSpeak_Update(u32 value);

void esp01_connectAP();
void esp01_sendToThingspeak(char *val);

#endif
