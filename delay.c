#include "types.h"
void delay_us(u32 dlyUs){
    dlyUs *= 12;
    while(--dlyUs);
}
void delay_ms(u32 dlyMs){
    dlyMs *= 12000;
    while(--dlyMs);
}
