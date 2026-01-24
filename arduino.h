#ifndef __ARDUINO_H__
#define __ARDUINO_H__
#include "types.h"

u32 digitalRead(u32 pinNo);
void digitalWrite(u32 pinNo,u32 bit);
void portMode(u32 pinStartNo,u32 nPins,u32 pinsDir);
void write2Pins(u32 pinStartNo,u32 nPins,u32 data);

#endif
