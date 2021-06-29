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

#ifndef __STATUS_H
#define __STATUS_H

#include "system.h"

/* 道路类型定义 */
typedef enum _status_roadtype
{ /* 一般类型 */
  RoadStraight    = 0U,
  RoadLeft        = 1U,
  RoadRight       = 2U,
  
  /* 特殊类型 */
  RoadCross       = 3U,
  RoadBarrier     = 4U, /* 路障 */
  RoadBreak       = 5U, /* 断路 */
  
}status_roadtype_t;

/* 传感器类型定义 */
typedef enum _status_sensor
{
  Inductance  = 0U,
  Camera      = 1U,
}status_sensor_t;


typedef struct 
{
    uint8_t               debug_mode          ;    //用户界面模式
    volatile uint8_t      interrupt_10ms      ;
    volatile uint8_t      interrupt_500ms     ;
    uint8_t               car_stop            ;    //停车标志位
    uint8_t               low_power           ;    //低电压标志位
    status_roadtype_t     img_roadtype        ;    //道路类型标志位
    status_sensor_t       sensor              ;     /* 当前使用传感器类型 */
    uint8_t               barrier             ;
} carstatus_t;

extern carstatus_t status;



//操作类型定义
typedef struct _status_operations status_operations_t;  

struct _status_operations
{
    void (*light_road)(void);
    void (*oled_circle)(void);
};

extern const status_operations_t status_indicator;


#endif