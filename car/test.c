#include "system.h"




void test(void)
{
  LCD_Init();
  LCD_CLS();
  adc1_init();
  
  pid_control_init();           //速度控制
  
  pid_value_init(&direction);
  pid_change(&direction,30,0,4);        //方向控制
  
  tsl1401_init();
  

  pit_init(kPIT_Chnl_0, 10000);
  
  while(1)
  {
    delayms(500);
    LED_Color_Reverse(red);
  }
}