#ifndef __LCD_H__
#define __LCD_H__
#include "types.h"

void Init_LCD(void);
void CmdLCD(u8);
void CharLCD(u8);
void WriteLCD(u8);
void StrLCD(s8*);
void U32LCD(u32);
void S32LCD(s32);
void F32LCD(f32,u32);
void BuildCGRAM(u8*,u8);
int my_atoi(char *str);
int is_digit(char dig);

#endif
