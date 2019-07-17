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
static inline int16_t speed_base(void)
{
    return 151;
}


static inline int16_t speed_differential(int16_t base_speed)
{
  double R;
  R = calculate_Ackman_R(img.cal_ops->transform(80,midline[80]));
  if (R<-0.006 && R>0.006)
  {
    return (int16_t)(base_speed * 7.5*6.67*R);
  }
  else if ( (R>=-0.006 && R<-0.004) || (R>0.004 && R<=0.006) )
  {
    return (int16_t)(base_speed * 7.5*4.5*R);
  }
  else 
    return 0;
}



void car_speed_calculate(void)
{
  int16_t speed,differential;
  speed = speed_base();
  differential = speed_differential(speed);
  motor_speed.left = speed + differential;
  motor_speed.right = speed - differential;
  

/**/
}
