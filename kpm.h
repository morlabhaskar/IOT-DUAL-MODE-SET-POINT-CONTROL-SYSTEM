#ifndef __KPM_H__
#define __KPM_H__
#include "types.h"
#include "kpm_defines.h"

void Init_KPM(void);
u32 ColScan(void);
u32 RowCheck(void);
u32 ColCheck(void);
u32 KeyScan(void);
int KeyScan_NonBlocking(void);

#endif
