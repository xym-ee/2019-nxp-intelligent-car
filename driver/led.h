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

#ifndef _LED_H
#define _LED_H

#include "system.h"


/*******************************************************************************
* Definitions
******************************************************************************/

/* --- LED类型定义 --- */
typedef enum
{
  LED_G  = 1,
  LED_R  = 2,   
  LED_B  = 3,
  LEDALL = 5,//全部   
} LEDn_e;
typedef enum
{
  white  = 0,  //白色
  black  = 1,  //黑色
  red    = 2,  //红
  green  = 3,  //绿
  blue   = 4,  //蓝色 
  yellow = 5,  //黄色
  violet = 6,  //紫色
  cyan   = 7,  //青色
}LED_t;

typedef enum
{
  ON  =0,  //亮
  OFF =1, //灭
  RVS =2, //反转  
}LEDs_e;


//led操作类型定义
typedef struct _led_operations led_operations_t;  

struct _led_operations
{
    void (*color)(LED_t color);
    void (*reverse)(LED_t color);
    void (*ctrl)(LEDn_e ledno, LEDs_e sta);
};


//led器件类型定义，包含led操作
typedef struct _led_device led_device_t;

struct _led_device
{
    void (*init)(void);
    const led_operations_t *ops;
};



/* led器件接口 */
extern const led_device_t led;




#endif 
