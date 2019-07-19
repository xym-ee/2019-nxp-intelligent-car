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
    .interrupt_10ms         = 0,
    .interrupt_500ms        = 0,
    .img_roadtype           = RoadStraight,
    .sensor                 = Camera,
    .barrier                = 0,    
};


/* ---------------------------- 方法声明 ------------------------------------ */
static void status_light_roadtype(void);
static void status_oled(void);


/* ---------------------------- 外部接口 ------------------------------------ */
const status_operations_t status_indicator = {
    .light_road = status_light_roadtype,
    .oled_circle = status_oled,
};



/* 灯光指示 */
static void status_light_roadtype(void)
{
  /* 电磁模式，开双闪 */
  if (status.sensor == Inductance)
  {
    led.ops->flash_fast(WarningLight);
  }
  else /* 摄像头模式，根据道路类型开指示灯 */
  {
    switch (status.img_roadtype)
    {
    case RoadStraight : led.ops->flash_fast(UpLight);     break;
    case RoadLeft     : led.ops->flash_fast(LeftLight);   break;
    case RoadRight    : led.ops->flash_fast(RightLight);  break;
    case RoadBarrier  : led.ops->on(WarningLight);        break;
    }
  }
}

/* oled指示 */
static void status_oled(void)
{
  char txt[16];
  /* 显示圆环相关信息 */
  switch (adc_roaddata.status)
  {
  case NoCircle           :  sprintf(txt,"NoCircle"); break;
  case RightCircleRun     :  sprintf(txt,"InRtCrcl"); break;
  case LeftCircleRun      :  sprintf(txt,"InLtCrcl"); break;
  case RightCircleWaitIn  :  sprintf(txt,"WtInRtCr"); break;
  case RightCircleWaitOut :  sprintf(txt,"WtOtRtCr"); break;
  case LeftCircleWaitIn   :  sprintf(txt,"WtInLtCr"); break;
  case LeftCircleWaitOut  :  sprintf(txt,"WtOtLtCr"); break;
  }

  LCD_P6x8Str(68,1,(uint8_t*)txt);
  
  /* 显示当前使用传感器 */
  switch (status.sensor)
  {
  case Inductance :  sprintf(txt,"Inductance"); break;
  case Camera     :  sprintf(txt,"Camera    "); break;
  } 
  LCD_P6x8Str(68,0,(uint8_t*)txt);
  
  sprintf(txt,"%2d",adc_roaddata.err);
  LCD_P6x8Str(0,4,(uint8_t*)txt);

}
