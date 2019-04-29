#ifndef _OLED_H
#define _OLED_H
#include "system.h"


void LCD_WrDat(unsigned char data);
void LCD_WrCmd(unsigned char cmd);
void LCD_Set_Pos(unsigned char x, unsigned char y);
void LCD_CLS(void);
void LCD_Init(void);

void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_njust(void);
void LCD_Show_Frame94(void);
void LCD_PrintFloat(unsigned char x,unsigned char y,float num);

#endif

