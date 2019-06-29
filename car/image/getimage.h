#ifndef _GETIMAGE_H
#define _GETIMAGE_H

#include "system.h"

extern uint8_t midline[IMG_HIGH];
extern uint32_t fullCameraBufferAddr;
void mt9v_oled_test(void);
void refresh_midline(void);
void mt9v_oledshow(void);
void mt9v_send_to_pc(void);


#endif