#include "system.h"

_pid_t leftpid;
_pid_t rightpid;

void pid_value_init(_pid_t* base)
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

void pid_change(_pid_t* base,short kp,short ki,short kd)
{
  base->_kp = kp;
  base->_ki = ki;
  base->_kd = kd;
}

static short pid_control(_pid_t* base,short targer,short real)
{
  base->_err = targer - real;
  base->_pid_out += base->_kp*(base->_err - base->_err1)
                  + base->_ki*base->_err
                  + base->_kd*(base->_err - 2*base->_err1 + 2*base->_err2);
  base->_err1 = base->_err;
  base->_err2 = base->_err1;
  return base->_pid_out;
}


/*
  电机转速PID控制初始化
*/
void pid_control_init(void)
{  
  enc_init();
  pwm_init();

  pid_value_init(&leftpid);
  pid_value_init(&rightpid);
  
  pid_change(&leftpid,5,0,2);
  pid_change(&rightpid,5,0,2);

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

  left_motor(pid_control(&leftpid,value,left_enc));
  right_motor(pid_control(&rightpid,value,right_enc));
}




