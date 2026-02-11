#ifndef __ESP01_H__
#define __ESP01_H__
#include "types.h"

void esp01_connectAP(void);
void esp01_sendToThingspeak(u32 val);
void esp01_sendToThingspeak_set_point(u32 val);
void esp01_readFromThingspeak(void);

#endif
