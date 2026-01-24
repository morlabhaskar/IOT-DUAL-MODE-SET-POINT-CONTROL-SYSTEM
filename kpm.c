#include "types.h"
#include "kpm_defines.h"
#include<LPC21xx.h>
u32 kpmLUT[4][4] = {{'7', '8', '9', '/'}, {'4', '5', '6', 'x'}, {'1', '2', '3', '-'}, {'C', '0', '=', '+'}};

void Init_KPM(void){
    //rows output & grounded
    IODIR1 |= 15<<ROW0;
    //cols input & high, any by default
}
u32 ColScan(void){
    u32 status;
    status = (((IOPIN1>>COL0)&15)<15) ? 0 : 1;
    return status;
}
u32 RowCheck(void){
    u32 r;
    for(r=0;r<=3;r++){
        //starting with row0,upto row3
        IOPIN1 = ((IOPIN1 &~(15<<ROW0))|(~(1<<r)<<ROW0));
        //check if key was pressed in that row
        if(ColScan()==0){
            break;
        }
    }
    //re-initialize rows to their state
    IOCLR1 = 15<<ROW0;
    return r;
}
u32 ColCheck(void){
    u32 c;
    for(c=0;c<=3;c++){
        //starting with col0,upto col3
        //check one col at a time for low
        if(((IOPIN1>>(COL0+c))&1)==0){
            break;
        }
    }
    return c;
}
u32 KeyScan(void){
    u32 r,c,keyV;
    //Initialize KPM
    Init_KPM();
    //wait for any key press detection
    while(ColScan());
    //upon key press detection
    //identify row in which key is pressed
    r=RowCheck();
    //identify col in which key is pressed
    c=ColCheck();
    //extract key value from LUT
    keyV = kpmLUT[r][c];
    return keyV;
}