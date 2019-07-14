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


/* 基于阿克曼半径的方向控制 */
void car_direction_control(void)
{
  char txt[16];
  double R;
  double arc_err,ud;
  static double arc_err1 = 0;
  static double ud1 = 0;
  uint16_t servo_pwm;
  point_t temp;
  //目标点的半径转换
  if (status.img_roadtype == RoadRight)
  {
    temp = img.cal_ops->transform(80,leftline[80]);
    temp.x = temp.x + 40.0;
    R = calculate_Ackman_R(temp);
  } 
  else if(status.img_roadtype == RoadLeft)
  {
    temp = img.cal_ops->transform(80,rightline[80]);
    temp.x = temp.x - 40.0;
    R = calculate_Ackman_R(temp);
  }
  else /* 直线 */
  {
    R = calculate_Ackman_R(img.cal_ops->transform(80,midline[80]));
  }
  
  arc_err = 980*R;
  ud = 6*0.17*(arc_err - arc_err1) + 0.83*ud1;
  servo_pwm = (uint16_t)(1500 + 6.5*arc_err + ud);
  servo(servo_pwm);
  arc_err1 = arc_err;
  ud1 = ud;
  
  
  
  sprintf(txt, "PWM: %4d", servo_pwm);
  LCD_P6x8Str(0,0,(uint8_t*)txt); 
  sprintf(txt, "R:  %7.5f", R);
  LCD_P6x8Str(0,1,(uint8_t*)txt);  
}

/* 普通的分段PD控制 */
void car_direction_control_pd(void)
{
  char txt[16];
  int16_t dir_kp,dir_kd;
  int16_t err,ec;
  int16_t servo_pwm;
  static int16_t err1 = 0;
  if (status.img_roadtype == RoadStraight)/* 直线 */
  {
    dir_kp = 2;
    dir_kd = 1;
    err = midline[35] - 94;
    Image[40][midline[40]] = 0;
    Image[39][midline[40]] = 0;
    Image[41][midline[40]] = 0;
  } 
  else /* 其他 */
  {
    dir_kp = 8;
    dir_kd = 8;
    err = midline[83] - 94;
    Image[83][midline[83]] = 0;
    Image[83][midline[83]] = 0;
    Image[83][midline[83]] = 0;
  }

  ec = err - err1;
  
  servo_pwm = (uint16_t)(1500 + dir_kp*err + dir_kd*ec);
  
  servo(servo_pwm);
  err1 = err;
  
  sprintf(txt, "PWM: %4d", servo_pwm);
  LCD_P6x8Str(0,0,(uint8_t*)txt); 

}
