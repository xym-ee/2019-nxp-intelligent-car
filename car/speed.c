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

#include "system.h"

/*  速度pid控制，   基于曲率的速度控制 */


/* ---------------------------- 方法声明 ------------------------------------ */






/*----------------------- 接口定义 ----------------------------*/








/* ---------------------------- 方法实现 ------------------------------------ */







/* 根据道路类型（或者曲率，弯道半径，斜率变化率）计算基速值 */
static int16_t speed_base(void)
{
  if (status.img_roadtype == RoadStraight) //直路速度
  {
    
    return 80;
  }
  else  //弯道
  {
  
    return 50;
  }
}


static int16_t speed_differential(void)
{
   if (status.img_roadtype == RoadStraight) //直路速度
  {
    return 0;
  }
  else  //弯道
  {
    return 15;
  } 
}



void car_speed_refresh(void)
{
  motor_speed.left = speed_base() + speed_differential();
  motor_speed.right = speed_base() + speed_differential();
}





