/*  <The 14th National University Students Intelligent Car Race.>
 *  Copyright (C) <2019>  < github.com:He0L1w  NJUST >
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _GETIMAGE_H
#define _GETIMAGE_H

#include "system.h"

/* 图像尺寸和帧率设置       
 * # 1 -> 188*120       192fps
 * # 2 -> 376*240       112fps
 */
#define     IMG_SIZE    2
#define     FPS         100

/* 图像缓冲区的地址 */
extern uint32_t CameraBufferAddr;

/* 从图像缓冲区取一个像素 188*120 -> 93*56 */
//__ramfunc static inline uint8_t pixle(uint8_t x,uint8_t y)
//{  
//  return *((uint8_t *)(CameraBufferAddr + 2*188*(x+1) + 2*(y+1)));
//}
/* 376*240 -> 376*240 */
__ramfunc static inline uint8_t pixle(uint16_t x,uint16_t y)
{  
  return *((uint8_t *)CameraBufferAddr + (x*IMG_WIDTH) + y);
}


extern int16_t midline[IMG_HIGH];
extern int16_t leftline[IMG_HIGH];
extern int16_t rightline[IMG_HIGH];


typedef struct _img_device img_device_t;  

struct _img_device
{
    void (*refresh)(void);
    void (* roadcondition)(void);
    void (*display)(void);
    void (*send)(void);
    void (*init)(void);
    void (*roadtype_test)(void);
};


typedef struct _img_operations img_operations_t;  

struct _img_operations
{
    void (*get)(void);
    uint8_t (*ostu)(void);
    uint8_t (*aver)(void);
    void (*binary)(void);
    void (*clearnoise)(void);
    void (*roadtype)(void);
};



extern const img_device_t Img;



#endif