#ifndef _GETIMAGE_H
#define _GETIMAGE_H

#include "system.h"

#define LCD_WIDTH   128
#define LCD_HEIGHT  160

extern uint8_t  counter;
extern int      OFFSET2;
extern uint16_t lcdFrameBuf[2][LCD_HEIGHT][LCD_WIDTH];
extern uint8_t Pixle[Use_ROWS][Use_Line];
extern uint32_t fullCameraBufferAddr;
void mt9v_oled_test(void);
void int10ms(void);



#endif