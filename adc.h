#ifndef __ADC_H__
#define __ADC_H__

#include "types.h"
void Init_ADC(void);
u32 Read_ADC(u32 chNo,u32 *adcDVal,f32 *analogReading);

#endif
