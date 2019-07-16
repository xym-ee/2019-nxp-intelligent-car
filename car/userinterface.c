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

/*
  设计思路：
  上电后出现调试交互界面，选择电机速度、方向Kp、方向Kd，按核心板wup开始运行整个系统
  遥控器中断后重新回到交互界面。显示当前参数，按键修改后，按核心板wup再次调试

  K1 --> 参数切换
  K2 --> 参数减小
  K0 --> 参数增大
*/
static uint8_t OptionRefresh(uint8_t flag);
static uint8_t OptionPlus(uint8_t flag);
static uint8_t OptionMinus(uint8_t flag);



void UI_debugsetting(void)
{

  /* 调用先停车 */
  left_motor(0);      
  right_motor(0);
  
  /* 重新设置电机PID中的e,ec */
  motor.pidclear(&pid);

  char txt[16];
  uint8_t whichkey;
  uint8_t choose_flag = 1;//当前选项记录
  uint8_t setting_end_flag = 1;
  
  oled.ops->clear();
  
  sprintf(txt, "DEBUG SETTING");
  LCD_P6x8Str(30,0,(uint8_t*)txt); 
  
  sprintf(txt, "-> speed: %3d",motor_speed.left);
  LCD_P6x8Str(0,1,(uint8_t*)txt);
  
  sprintf(txt, "      Kp: %3.2f",pid.left->kp);
  LCD_P6x8Str(0,2,(uint8_t*)txt);
  
  sprintf(txt, "      Ki: %3.2f",pid.left->ki);
  LCD_P6x8Str(0,3,(uint8_t*)txt);  
  
  sprintf(txt, "      Kd: %3.2f",pid.left->kd);
  LCD_P6x8Str(0,4,(uint8_t*)txt);
  
  sprintf(txt, "      start");
  LCD_P6x8Str(0,5,(uint8_t*)txt);
  
  sprintf(txt, "-         +");
  LCD_P6x8Str(0,7,(uint8_t*)txt);
  sprintf(txt, "SELECT");
  LCD_P6x8Str(92,7,(uint8_t*)txt);
  
  while(setting_end_flag)
  {
    whichkey = key.ops->get(0);
    switch(whichkey)
    {
    case key_plus :   setting_end_flag = OptionPlus(choose_flag);    break;          //选项+
    case key_minus :  setting_end_flag = OptionMinus(choose_flag);   break;          //选项-
    case key_ok :     choose_flag = OptionRefresh(choose_flag);      break;          //选项更新
    }
  }
   /* 此函数的调用标志 */
  status.debug_mode = 0;
}

//私有函数
static uint8_t OptionRefresh(uint8_t flag)
{
  char txt[16];
  if(flag == 1)
  {
    sprintf(txt, "   speed: %3d",motor_speed.left);
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    
    sprintf(txt, "->    Kp: %3.2f",pid.left->kp);
    LCD_P6x8Str(0,2,(uint8_t*)txt);
    
    sprintf(txt, "      Ki: %3.2f",pid.left->ki);
    LCD_P6x8Str(0,3,(uint8_t*)txt);     
    
    sprintf(txt, "      Kd: %3.2f",pid.left->kd);
    LCD_P6x8Str(0,4,(uint8_t*)txt);
    
    sprintf(txt, "      start");
    LCD_P6x8Str(0,5,(uint8_t*)txt);
    return 2;
  }
  
  if(flag == 2)
  {
    sprintf(txt, "   speed: %3d",motor_speed.left);
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    
    sprintf(txt, "      Kp: %3.2f",pid.left->kp);
    LCD_P6x8Str(0,2,(uint8_t*)txt);
    
    sprintf(txt, "->    Ki: %3.2f",pid.left->ki);
    LCD_P6x8Str(0,3,(uint8_t*)txt);     
    
    sprintf(txt, "      Kd: %3.2f",pid.left->kd);
    LCD_P6x8Str(0,4,(uint8_t*)txt);
    
    sprintf(txt, "      start");
    LCD_P6x8Str(0,5,(uint8_t*)txt);
    return 3;
  }
  if(flag == 3)
  {
    sprintf(txt, "   speed: %3d",motor_speed.left);
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    
    sprintf(txt, "      Kp: %3.2f",pid.left->kp);
    LCD_P6x8Str(0,2,(uint8_t*)txt);
    
    sprintf(txt, "      Ki: %3.2f",pid.left->ki);
    LCD_P6x8Str(0,3,(uint8_t*)txt);     
    
    sprintf(txt, "->    Kd: %3.2f",pid.left->kd);
    LCD_P6x8Str(0,4,(uint8_t*)txt);
    
    sprintf(txt, "      start");
    LCD_P6x8Str(0,5,(uint8_t*)txt);
    return 4;
  }
  if(flag == 4)
  {
    sprintf(txt, "   speed: %3d",motor_speed.left);
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    
    sprintf(txt, "      Kp: %3.2f",pid.left->kp);
    LCD_P6x8Str(0,2,(uint8_t*)txt);
    
    sprintf(txt, "      Ki: %3.2f",pid.left->ki);
    LCD_P6x8Str(0,3,(uint8_t*)txt);     
    
    sprintf(txt, "      Kd: %3.2f",pid.left->kd);
    LCD_P6x8Str(0,4,(uint8_t*)txt);
    
    sprintf(txt, "->    start");
    LCD_P6x8Str(0,5,(uint8_t*)txt);
    
    return 5;
  }
    if(flag == 5)
  {
    sprintf(txt, "-> speed: %3d",motor_speed.left);
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    
    sprintf(txt, "      Kp: %3.1f",pid.left->kp);
    LCD_P6x8Str(0,2,(uint8_t*)txt);
    
    sprintf(txt, "      Ki: %3.1f",pid.left->ki);
    LCD_P6x8Str(0,3,(uint8_t*)txt);     
    
    sprintf(txt, "      Kd: %3.1f",pid.left->kd);
    LCD_P6x8Str(0,4,(uint8_t*)txt);
    
    sprintf(txt, "      start");
    LCD_P6x8Str(0,5,(uint8_t*)txt);
    
    return 1;
  }
  return 0;
}


static uint8_t OptionPlus(uint8_t flag)
{
  char txt[16];
  switch(flag)
  {
  case 1:
    motor_speed.left = motor_speed.left + 10;
    motor_speed.right = motor_speed.right + 10;
    sprintf(txt, "-> speed: %3d",motor_speed.left);
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    return 1;
  case 2:
    pid.left->kp = pid.left->kp + 0.1;
    pid.right->kp = pid.right->kp + 0.1;
    sprintf(txt, "->    Kp: %3.1f",pid.left->kp);
    LCD_P6x8Str(0,2,(uint8_t*)txt);
    return 1;
  case 3:
    pid.left->ki = pid.left->ki + 0.1;
    pid.right->ki = pid.right->ki + 0.1;
    sprintf(txt, "->    Ki: %3.1f",pid.left->ki);
    LCD_P6x8Str(0,3,(uint8_t*)txt);
    return 1;    
  case 4:
    pid.left->kd = pid.left->kd + 0.1;
    pid.right->kd = pid.right->kd + 0.1;
    sprintf(txt, "->    Kd: %3.1f",pid.left->kd);
    LCD_P6x8Str(0,4,(uint8_t*)txt);
    return 1;
  case 5:
    oled.ops->clear();
    return 0;//设置结束，启动系统
  }
  return 1;
}

static uint8_t OptionMinus(uint8_t flag)
{
  char txt[16];
  switch(flag)
  {
  case 1:
    motor_speed.left = motor_speed.left - 10;
    motor_speed.right = motor_speed.right - 10;
    sprintf(txt, "-> speed: %3d",motor_speed.left);
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    return 1;
  case 2:
    pid.left->kp = pid.left->kp - 0.1;
    pid.right->kp = pid.right->kp - 0.1;
    sprintf(txt, "->    Kp: %3.1f",pid.left->kp);
    LCD_P6x8Str(0,2,(uint8_t*)txt);
    return 1;
  case 3:
    pid.left->ki = pid.left->ki - 0.1;
    pid.right->ki = pid.right->ki - 0.1;
    sprintf(txt, "->    Ki: %3.1f",pid.left->ki);
    LCD_P6x8Str(0,3,(uint8_t*)txt);
    return 1;
  case 4:
    pid.left->kd = pid.left->kd - 0.1;
    pid.right->kd = pid.right->kd - 0.1;
    sprintf(txt, "->    Kd: %3.1f",pid.left->kd);
    LCD_P6x8Str(0,4,(uint8_t*)txt);
    return 1;
  case 5:
    oled.ops->clear();
    return 0;//设置结束，启动系统
  }
  return 1;
}

