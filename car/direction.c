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
static void car_direction_control_pd(void);
static void car_direction_control_inductance(void);



const car_device_t car = {
    .direction_control = car_direction_control,
    .speed_caculate = car_speed_calculate,
};



/* 全局方向控制函数 */
static void car_direction_control(void)
{
  /* 电磁模式运行 */
  if ( status.inductance_run )  /* img.roadtype进行断路检查 或者 电磁偏离过大检查 */   
  {
    car_direction_control_inductance();  /* 电磁方向控制 */
    img.ops->adc_roadcheck();           /* 道路检查，切换摄像头 */
    return ;
  }
  
  /* 电磁判断、摄像头运行的圆环方向控制 */
  /* adc_circle_check 函数控制此分支入口 */
  if ( adc_circle_status > CircleConditon )   /* 圆环开关操作条件满足 */
  {
    /* 圆环修正 */
//    if ()
//    {}
    
  }

  /* 若前方函数未退出执行，普通道路，摄像头计算舵机打角 */
  car_direction_control_arcman();
}



/* 基于阿克曼半径的方向控制 */
static void car_direction_control_arcman(void)
{
  char txt[16];
  double R;
  double arc_err,ud;
  static double arc_err1 = 0;
  static double ud1 = 0;
  uint16_t servo_pwm;
  point_t temp;
  R = calculate_Ackman_R(img.cal_ops->transform(71,midline[71]));
  
  /*
  将Ackman_R和速度建立对应关系
  直线速度最大值
  弯道速度最大值
  */
  arc_err = 980*R;
  ud = 0.4*0.4*(arc_err - arc_err1) + 0.6*ud1;
  if (R<=0.004)  /*直线*/
    servo_pwm = (uint16_t)(SERVO_MID + 20*arc_err);
  else if(R>0.004 && R<0.006)  /*大弯*/
    servo_pwm = (uint16_t)(SERVO_MID + 30*arc_err );
  else if(R>0.006) /*小弯*/
    servo_pwm = (uint16_t)(SERVO_MID + 40*arc_err);
  servo(servo_pwm);
  arc_err1 = arc_err;
  ud1 = ud;
  
  sprintf(txt, "PWM: %4d", servo_pwm);
  LCD_P6x8Str(0,0,(uint8_t*)txt); 
  sprintf(txt, "R:  %7.5f", R);
  LCD_P6x8Str(0,1,(uint8_t*)txt);  
}


/* 普通的分段PD控制 */
static void car_direction_control_pd(void)
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



//根据err,err1选择固定的舵机打角
const signed char RuleBase[7][7]={
//err |NB  NM  NS  ZE  PS  PM  PB | ↓err_1
      {PB, PB, PB, PB, PM, PS, ZO},//NB
      {PB, PB, PB, PM, PS, ZO, NS},//NM
      {PB, PB, PM, PS, ZO, NS, NM},//NS
      {PB, PM, PS, ZO, NS, NM, NB},//ZE
      {PM, PS, ZO, NS, NM, NB, NB},//PS
      {PS, ZO, NS, NM, NB, NB, NB},//PM
      {ZO, NS, NM, NB, NB, NB, NB},//PB
};

const signed char _servo[7] = { 3,2,1,0,-1,-2,-3 };

/* 使用电感的方向控制 */
static void car_direction_control_inductance(void)
{
  int8_t err;   /* 根据电磁判断的大概偏差 */
  static int8_t err1 = 0;
  uint16_t servo_pwm;
  
  char txt[16];
  
  err = adc.ops->geterror();   /* 获得大概偏差 */
  
  if (err == 99)
    err = err1;   /* 如果偏离了电磁线，偏差按偏离前计算 */
  
  servo_pwm = 1500 + _servo[RuleBase[err][err1]]*40;
  servo(servo_pwm);
  
  sprintf(txt,"%d",_servo[err]);
  LCD_P6x8Str(6,0,(uint8_t*)txt); 

  err1 = err;
}




/* 路障距离检测 */
void car_direction_distance(void)
{
  key.init();
  oled.init();
  
  char txt[16];
  
  while(1)
  {
    sprintf(txt,"%d",GPIO_PinRead(GPIO2, 25));
    LCD_P6x8Str(0,0,(uint8_t*)txt); 
    delayms(100);
  }
}
