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

short speedvalue;

/* 单个电机的速度PID参数 */
_motor_pid_t motor_leftpid;
_motor_pid_t motor_rightpid;

/* 电机PID参数 */
motor_pid_t pid = {
  .left = &motor_leftpid,
  .right = &motor_rightpid,    
};

/* 目标电机速度，初始为0 */
motor_speed_t motor_speed = {
    .left = 0,
    .right = 0,
};


/* ---------------------------- 方法声明 ------------------------------------ */
static void motor_pid_device_init(void);
static void motor_pid_clear(motor_pid_t* base);
static void motor_pid_change(motor_pid_t* base,short kp,short ki,short kd);
static void motor_pid_control(motor_speed_t *speed);
static void motor_pid_test(void);

/* ---------------------------- 外部接口 ------------------------------------ */

const mpid_operations_t motor = {
    .init       = motor_pid_device_init,
    .pidclear   = motor_pid_clear,
    .pidchange  = motor_pid_change,
    .pidcontrol = motor_pid_control,
    .pidtest    = motor_pid_test,
};


/* ---------------------------- 方法实现 ------------------------------------ */

static void motor_pid_device_init(void)
{
  enc_init();
  pwm_init();
  motor.pidchange(&pid,5,0,2);
}

static void motor_pid_clear(motor_pid_t* base)
{
  base->left->err = 0;
  base->left->err1 = 0;
  base->left->err2 = 0;
  base->left->int_err = 0;
  base->left->ut = 0;
  
  base->right->err = 0;
  base->right->err1 = 0;
  base->right->err2 = 0;
  base->right->int_err = 0;
  base->right->ut = 0;
}

static void motor_pid_change(motor_pid_t* base,short p,short i,short d)
{
  base->left->kp = p;
  base->left->ki = i;
  base->left->kd = d;
  
  base->right->kp = p;
  base->right->ki = i;
  base->right->kd = d;
}


static void motor_pid_control(motor_speed_t *speed)
{
  short enc_left,enc_right;
  
  enc_left = (int16_t)ENC_GetPositionDifferenceValue(ENC1);  //得到编码器微分值
  enc_right = (int16_t)ENC_GetPositionDifferenceValue(ENC2);  //得到编码器微分值

  pid.left->err = speed->left - enc_left;
  pid.left->ut += pid.left->kp*(pid.left->err - pid.left->err1)
                + pid.left->ki*pid.left->err
                + pid.left->kd*(pid.left->err - 2*pid.left->err1 + pid.left->err2);
  
  pid.right->err = speed->right - enc_right;
  pid.right->ut += pid.right->kp*(pid.right->err - pid.right->err1)
                + pid.right->ki*pid.right->err
                + pid.right->kd*(pid.right->err - 2*pid.right->err1 + pid.right->err2); 
  
  
  left_motor(pid.left->ut);
  right_motor(pid.right->ut);
}



static void motor_pid_test(void)
{
  key.init();
  enc_init();
  pwm_init();
  oled.init();
  
  char txt[16]; 
  short left_enc,right_enc;
  short speed_set = 0;

  while (1)
  {        
    switch(key.ops->get(1))  //检测按键
    {
    case no_key:
      break;
    case key_minus:
      speed_set += 10;
      break;           
    case key_plus:           
      speed_set -= 10;
      break;
    case key_ok:
      break;
    }
    
    motor_speed.left = speed_set;
    motor_speed.right = speed_set;
    motor_pid_control(&motor_speed);

    left_enc = (int16_t)ENC_GetPositionDifferenceValue(ENC1);  //得到编码器微分值
    right_enc = (int16_t)ENC_GetPositionDifferenceValue(ENC2);  //得到编码器微分值

    sprintf(txt,"L:  %5d ",left_enc);
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    sprintf(txt,"R:  %5d ",right_enc); 
    LCD_P6x8Str(0,2,(uint8_t*)txt);
    //LED闪烁
    led.ops->reverse(UpLight);  
    delayms(10);
  }
}