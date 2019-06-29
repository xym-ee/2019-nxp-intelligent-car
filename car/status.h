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

typedef struct 
{
    uint8_t debug_mode        :        1;    //用户界面模式
    uint8_t stop              :        1;    //停车标志位
    uint8_t low_power_flag    :        1;    //低电压标志位
    uint8_t straight_speed    :        1;    //直道标志位
    uint8_t brake_speed       :        1;    //入弯标志位
    uint8_t curve_speed       :        1;    //弯道标志位
    uint8_t camera_run        :        1;    //使用摄像头 
    uint8_t electromagnetism  :        1;    //使用电磁
    uint8_t ins_calibration   :        1;    //陀螺仪校准
    
    uint8_t txOnGoing         :        1;    //串口TX正在发送标志位
    uint8_t rxOnGoing         :        1;    //串口RX正在接收标志位
    uint8_t txBufferFull      :        1;    //串口TX发送寄存器满标志位
    uint8_t rxBufferEmpty     :        1;    //串口RX接收寄存器空标志位
    
    uint8_t get_pid_group1    :        1;    //上位机请求，为1时，单片机需要发送pid参数给上位机
}_status_t;

extern _status_t _status;

#endif