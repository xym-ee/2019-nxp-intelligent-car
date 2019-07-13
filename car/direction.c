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
  double arc_err,arc_ec;
  static double arc_err1 = 0;
  uint16_t servo_pwm;
  point_t temp;
  //目标点的半径转换
  if (status.img_roadtype == RoadRight)
  {
    temp = img.cal_ops->transform(160,leftline[160]);
    temp.x = temp.x + 25.0;
    R = calculate_Ackman_R(temp);
  } 
  else if(status.img_roadtype == RoadLeft)
  {
    temp = img.cal_ops->transform(160,rightline[160]);
    temp.x = temp.x - 25.0;
    R = calculate_Ackman_R(temp);
  }
  else /* 直线 */
  {
    R = calculate_Ackman_R(img.cal_ops->transform(80,midline[80]));
  }
  
    
  if ( R<0 && R>-250 )
  {/* 内层根据偏差进行分段比例控制 */
    arc_err = 250 + R;
    arc_ec = arc_err - arc_err1;
    
    if (arc_err<=140)
      servo_pwm = (uint16_t)(SERVO_MID - 0.7134*arc_err - 0.1*arc_ec );
    else if (arc_err>140 && arc_err<=191)
      servo_pwm = (uint16_t)(SERVO_MID - 1.94*arc_err - 0.5*arc_ec + 175.0);
    else if(arc_err>191)
      servo_pwm = (uint16_t)(SERVO_MID - 4.73*arc_err - 2*arc_ec + 705);
  }
  else if (R>0 && R<250)
  {
    arc_err = 250 - R;
    arc_ec = arc_err - arc_err1;
    
    if (arc_err<=140)
      servo_pwm = (uint16_t)(SERVO_MID + 0.7134*arc_err + 0.1*arc_ec );
    else if (arc_err>140 && arc_err<=191)
      servo_pwm = (uint16_t)(SERVO_MID + 1.94*arc_err + 0.5*arc_ec - 175.0);
    else if(arc_err>191)
      servo_pwm = (uint16_t)(SERVO_MID + 4.73*arc_err + 2*arc_ec - 705);
  }
  else
    servo_pwm = SERVO_MID;
  
  servo(servo_pwm);
  
  sprintf(txt, "PWM: %4d", servo_pwm);
  LCD_P6x8Str(0,0,(uint8_t*)txt); 
  sprintf(txt, "R:  %5.2f", R);
  LCD_P6x8Str(0,1,(uint8_t*)txt);  
}


