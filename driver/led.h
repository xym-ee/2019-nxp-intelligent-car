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

typedef enum _led_name_t
{
  LeftLight     = 0U,
  RightLight    = 1U,
  UpLight       = 2U,
  BackLight     = 3U,
  WarningLight  = 4U, /* 左右一起闪光 */
}led_name_t;

//led操作类型定义
typedef struct _led_operations led_operations_t;  

struct _led_operations
{
  void (*on)(led_name_t choose);
  void (*off)(led_name_t choose);
  void (*reverse)(led_name_t color);
  void (*flash_fast)(led_name_t color);
  void (*flash_slow)(led_name_t color);
  void (*off_a)(void);
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
