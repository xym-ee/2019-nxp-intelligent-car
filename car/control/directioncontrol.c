#include "system.h"

/* 10套PD参数,更加丝滑的控制 */ 
//dir_pd_t pd[10]={8,4,       // <50
//                 7,3,
//                 12,5,
//                 12,5,
//                 15,6,
//                 18,8,
//                 21,10,
//                 24,12,
//                 27,14,
//                 30,16};

//根据车速选择一套PD值
static uint8_t pd_setting(short speed)
{
  if (speed>SPEED9)
    return 9;
  else if (speed>SPEED8)
    return 8;
  else if (speed>SPEED7)
    return 7;
  else if (speed>SPEED6)
    return 6;
  else if (speed>SPEED5)
    return 5;
  else if (speed>SPEED4)
    return 4;
  else if (speed>SPEED3)
    return 3;
  else if (speed>SPEED2)
    return 2;
  else if (speed>SPEED1)
    return 1;
  else
    return 0;
}


dir_pd_t pd = {12, 5};
short pwm = 0,laste = 0;
short e,ec;
void direction_ctrl(void)
{
  e = midline[25]-46;
  ec = e-laste;
  //uint8_t i = pd_setting(speedvalue);     //根据速度设置PD
  pwm = 3000 + pd._dkp*e + pd._dkd*ec;
  servo(pwm);
  laste = e;
}
