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


typedef enum _status_roadtype
{
  RoadStraight = 0U,
  RoadLeft = 1U,
  RoadRight = 2U,
  RoadInCurve = 3U,
}status_roadtype_t;


typedef struct 
{
    uint8_t debug_mode          ;    //用户界面模式
    uint8_t interrupt_10ms      ;

    uint8_t car_stop            ;    //停车标志位
    uint8_t low_power           ;    //低电压标志位
    status_roadtype_t img_roadtype        ;    //道路类型标志位
    uint8_t camera_run          ;    //使用摄像头 
    uint8_t electromagnetism    ;    //使用电磁
    uint8_t ins_calibration     ;    //陀螺仪校准
    
    uint8_t txOnGoing           ;    //串口TX正在发送标志位
    uint8_t rxOnGoing           ;    //串口RX正在接收标志位
    uint8_t txBufferFull        ;    //串口TX发送寄存器满标志位
    uint8_t rxBufferEmpty       ;    //串口RX接收寄存器空标志位
    
    uint8_t get_pid_group1      ;    //上位机请求，为1时，单片机需要发送pid参数给上位机
} carstatus_t;

extern carstatus_t status;



//操作类型定义
typedef struct _status_operations status_operations_t;  

struct _status_operations
{
    void (*roadtype)(void);
};

extern const status_operations_t status_light;

#endif