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

#include "status.h"

/* 小车状态全局标志位 */
carstatus_t status = 
{
    .debug_mode             = 1,  //按键调试模式
    .car_stop               = 0,  //停车标志位
    .low_power              = 0,  //低电压标志位
    .interrupt_10ms         = 1,
    .img_roadtype           = RoadStraight,
   
    .camera_run             = 1,  //使用摄像头 
    .electromagnetism       = 0,  //使用电磁
    .ins_calibration        = 0,  //陀螺仪校准
    
    /*使用DMA发送标志位*/
    .txOnGoing              = 0,  //串口TX正在发送标志位
    .rxOnGoing              = 0,  //串口RX正在接收标志位
    .txBufferFull           = 0,  //串口TX发送寄存器满标志位
    .rxBufferEmpty          = 0,  //串口RX接收寄存器空标志位
    
    .get_pid_group1         = 0,  //上位机请求，为1时，单片机需要发送pid参数给上位机
    
};


/* ---------------------------- 方法声明 ------------------------------------ */
static void status_light_roadtype(void);



/* ---------------------------- 外部接口 ------------------------------------ */
const status_operations_t status_light = {
    .roadtype = status_light_roadtype,
};




 

static void status_light_roadtype(void)
{
    /* 灯光指示 */
    switch (status.img_roadtype)
    {
    case RoadStraight : led.ops->flash_fast(UpLight); break;
    case RoadLeft     : led.ops->flash_fast(LeftLight); break;
    case RoadRight    : led.ops->flash_fast(RightLight); break;
    }
}


