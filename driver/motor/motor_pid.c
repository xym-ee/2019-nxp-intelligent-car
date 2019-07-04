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

/* 速度PID参数 */
pid_t leftpid;
pid_t rightpid;



/* ---------------------------- 方法声明 ------------------------------------ */
static void motor_pid_clear(pid_t* base);
static void motor_pid_change(pid_t* base,short kp,short ki,short kd);
static short motor_pid_control(pid_t* base,short targer,short real);


static void motor_pid_device_init(void);



/* ---------------------------- 外部接口 ------------------------------------ */

const mpid_operations_t mpid_ops = {
        .clear = motor_pid_clear,
        .change = motor_pid_change,
        .result = motor_pid_control,
};

const mpid_device_t MotorPid = {
        .deviceinit = motor_pid_device_init,
        .value_ops = &mpid_ops,
};


/* ---------------------------- 方法实现 ------------------------------------ */

/*
  电机转速PID控制初始化
*/
static void _pid_control_init(void)
{  
  MotorPid.value_ops->clear(&leftpid);
  MotorPid.value_ops->clear(&rightpid);
  
  MotorPid.value_ops->change(&leftpid,5,0,2);
  MotorPid.value_ops->change(&rightpid,5,0,2);  

}



/* PID参数值的初始化为0 */
static void motor_pid_clear(pid_t* base)
{
  base->_kp = 0;
  base->_ki = 0;
  base->_kd = 0;
  base->_err = 0;
  base->_err1 = 0;
  base->_err2 = 0;
  base->_pid_out = 0;
  base->_sume = 0;
}


static void motor_pid_change(pid_t* base,short kp,short ki,short kd)
{
  base->_kp = kp;
  base->_ki = ki;
  base->_kd = kd;
}

static short motor_pid_control(pid_t* base,short targer,short real)
{
  base->_err = targer - real;
  base->_pid_out += base->_kp*(base->_err - base->_err1)
                  + base->_ki*base->_err
                  + base->_kd*(base->_err - 2*base->_err1 + 2*base->_err2);
  base->_err1 = base->_err;
  base->_err2 = base->_err1;
  return base->_pid_out;
}


static void motor_pid_device_init(void)
{
  enc_init();
  pwm_init();
  _pid_control_init();
}



short speedvalue = 0;
void car_speed(short value)
{
  char txt[16];
  short left_enc,right_enc;
  
  left_enc = (int16_t)ENC_GetPositionDifferenceValue(ENC1);  //得到编码器微分值
  sprintf(txt,"L:  %5d ",left_enc);
  LCD_P6x8Str(1,0,(uint8_t*)txt);
    
  right_enc = (int16_t)ENC_GetPositionDifferenceValue(ENC2);  //得到编码器微分值
  sprintf(txt,"R:  %5d ",right_enc); 
  LCD_P6x8Str(70,0,(uint8_t*)txt);
  
  left_motor(MotorPid.value_ops->result(&leftpid,value,left_enc));
  right_motor(MotorPid.value_ops->result(&rightpid,value,right_enc));
  
//  left_motor(pid_control(&leftpid,value,left_enc));
//  right_motor(pid_control(&rightpid,value,right_enc));
}

