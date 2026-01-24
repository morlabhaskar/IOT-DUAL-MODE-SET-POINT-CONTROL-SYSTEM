#include "types.h"
void delay_ms(u32 dlyMs){
    dlyMs *= 12000;
    while(--dlyMs);
}
