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



static void car_direction_control(void);
static void car_direction_control_arcman(void);
static inline void car_direction_control_inductance(void);
static void car_direction_control_circle(void);
static void car_direction_barrier_control(void);


const car_device_t car = {
    .direction_control = car_direction_control,
    .speed_caculate = car_speed_calculate,
};



/* 全局方向控制函数 */
static void car_direction_control(void)
{
  /* 路障，纯开环控制 */
  if ( status.barrier == 1 )
  {
    car_direction_barrier_control();    
  }
  else
  {
    /* 电磁判断、摄像头运行的圆环方向控制 */
    /* adc_circle_check 函数控制此分支入口 */
    if ( adc_roaddata.status > CircleConditon )   /* 圆环开关操作条件满足， */
    {
      car_direction_control_circle();
    }  
    
    /* img.roadtype进行断路检查 或者 电磁偏离过大检查 */
    if ( status.sensor ==  Camera)  
      car_direction_control_arcman();
    else /* 电磁模式运行 */
    {   
      car_direction_control_inductance();  /* 电磁方向控制 */
      img.ops->adc_roadcheck();            /* 道路检查，切换摄像头 */
    }    
  }
  
}


/* 基于数字图像计算阿克曼半径的方向控制 */
static void car_direction_control_arcman(void)
{
  char txt[16];
  double R;
  double arc_err,ud;
  static double arc_err1 = 0;
  static double ud1 = 0;
  uint16_t servo_pwm;
  R = calculate_Ackman_R(img.cal_ops->transform(65,midline[65]));
  
  /*
  将Ackman_R和速度建立对应关系
  直线速度最大值
  弯道速度最大值
  */
  arc_err = 980*R;
  ud = 35*0.4*(arc_err - arc_err1) + 0.6*ud1;
  
  if (R<=0.004 && R>=-0.004)  /*直线*/
    servo_pwm = (uint16_t)(SERVO_MID + 8*arc_err +ud);
  
  else if(R>0.004 && R<0.006) /*右大弯*/
    servo_pwm = (uint16_t)(SERVO_MID + 40*arc_err - 125.44 + ud); 
  else if(R>-0.006 && R<-0.004)  /*左大弯*/
    servo_pwm = (uint16_t)(SERVO_MID + 42.5*arc_err + 135.24 + ud);
  
  else if(R>0.006) /*右小弯*/
    servo_pwm = (uint16_t)(SERVO_MID + 48*arc_err - 172.48 + ud);
  else if(R<-0.006)/*左小弯*/
    servo_pwm = (uint16_t)(SERVO_MID + 50*arc_err + 179.34 + ud);
  
  servo(servo_pwm);
  arc_err1 = arc_err;
  ud1 = ud;
  
  sprintf(txt, "DJP%4d", servo_pwm);
  LCD_P6x8Str(0,0,(uint8_t*)txt); 
  sprintf(txt, "R:%7.5f", R);
  LCD_P6x8Str(0,1,(uint8_t*)txt);  
}



/* 电磁引导的固定打角方向控制，用于大偏差或者断路，对摄像头控制进行验证 */
static inline void car_direction_control_inductance(void)
{
  servo(1500 + (adc_roaddata.err - 3)*40);
}


/* 电磁引导入环 */
static void car_direction_control_circle(void)
{
  /* 条件触发在adc.c内 */
  /*----------------- 右侧入环 ----------------------*/
  /* 入环给定转角 */
  
  /* 环内 */
  if(adc_roaddata.status == RightCircleWaitIn && ENC_GetPositionValue(ENC2)>15000)
  {
    adc_roaddata.status = RightCircleRun;
    return;
  }  
  
  if(adc_roaddata.status == RightCircleWaitIn && ENC_GetPositionValue(ENC2)>3300)
  {
    adc_roaddata.err = 5;
    return;
  }
  

   /* 出环完成 */
  if ( adc_roaddata.status == RightCircleWaitOut && ENC_GetPositionValue(ENC2)>6000 )
  {
    adc_roaddata.status = NoCircle;
    status.sensor = Camera;   /* 切换摄像头 */
    return;
  }  
  
  /* 出环修正，距离超过3000 */
  if ( adc_roaddata.status == RightCircleWaitOut && ENC_GetPositionValue(ENC2)>4000 )
  {
    adc_roaddata.err = 3;    
    return;
  }
}


/* 避障开环控制
     |
      \  避障入左打角
      /   避障出回正
     |   避障出右打角
*/
static void car_direction_barrier_control(void)
{
  /* 固定左转 */
  while (ENC_GetPositionValue(ENC2)<2300)
    servo(1330);
  /* 回正 */
  while(ENC_GetPositionValue(ENC2)<3700)
    servo(1500);
  /* 右转 */
  while(ENC_GetPositionValue(ENC2)<7000)
    servo(1630);
  status.barrier = 2;
}

