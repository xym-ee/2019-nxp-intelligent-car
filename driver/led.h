#ifndef _LED_H
#define _LED_H

#include "system.h"

extern gpio_pin_config_t GPIO_Output_Config;  
/*******************************************************************************
* Definitions
******************************************************************************/
extern gpio_pin_config_t GPIO_Output_Config;

//定义模块号
typedef enum
{
  LED_G  = 1,
  LED_R  = 2,   
  LED_B  = 3,
  LEDALL = 5,//全部   
} LEDn_e;
typedef enum
{
  white  = 0,  //白色
  black  = 1,  //黑色
  red    = 2,  //红
  green  = 3,  //绿
  blue   = 4,  //蓝色 
  yellow = 5,  //黄色
  violet = 6,  //紫色
  cyan   = 7,  //青色
}LED_t;

typedef enum
{
  ON  =0,  //亮
  OFF =1, //灭
  RVS =2, //反转  
}LEDs_e;

extern void LED_Color(LED_t color);
extern void LED_Color_Reverse(LED_t color);
extern void LED_Init(void);
extern void LED_Ctrl(LEDn_e ledno, LEDs_e sta);

#endif 
