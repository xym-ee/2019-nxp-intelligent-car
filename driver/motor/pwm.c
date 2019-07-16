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


/**
 *  设置电机占空比（速度）
 *  ----------------
 *  duty                占空比             -DUTY_MAX ~ DUTY_MAX
 *  ----------------
 *  duty最大值10000定义在DUTY_MAX
 *  
 *  left_motor(-2000)  //左电机反转  占空比  2000 / 10000 
 */
void left_motor(short duty)
{
  if(duty >= 0)  //  正转
  {
    if (duty >= DUTY_MAX)
      duty = DUTY_MAX;
    PWM_UpdateDuty(PWM1, kPWM_Module_3, kPWM_PwmA, 0); 
    PWM_UpdateDuty(PWM1, kPWM_Module_3, kPWM_PwmB, duty); 
    PWM_SetPwmLdok(PWM1, 1u<<kPWM_Module_3, true);        //设置pwm的 load ok位
  }
  else
  {
    if (duty <= -DUTY_MAX)
      duty = -DUTY_MAX;
    PWM_UpdateDuty(PWM1, kPWM_Module_3, kPWM_PwmA, -duty); 
    PWM_UpdateDuty(PWM1, kPWM_Module_3, kPWM_PwmB, 0); 
    PWM_SetPwmLdok(PWM1, 1u<<kPWM_Module_3, true);        //设置pwm的 load ok位                
  }
}
void right_motor(short duty)
{
  if(duty >= 0)  //  正转
  {
    PWM_UpdateDuty(PWM2, kPWM_Module_0, kPWM_PwmA, 0); 
    PWM_UpdateDuty(PWM2, kPWM_Module_0, kPWM_PwmB, duty); 
    PWM_SetPwmLdok(PWM2, 1u<<kPWM_Module_0, true);        //设置pwm的 load ok位
  }
  else
  {
    PWM_UpdateDuty(PWM2, kPWM_Module_0, kPWM_PwmA, -duty); 
    PWM_UpdateDuty(PWM2, kPWM_Module_0, kPWM_PwmB, 0); 
    PWM_SetPwmLdok(PWM2, 1u<<kPWM_Module_0, true);        //设置pwm的 load ok位
  }
}
/**
 *  设置舵机pwm(角度)
 *  ----------------
 *  duty   2600-2400
 *  ----------------
 *  <3000 车左转，>3000车右转
 *  
 */
void servo(uint16_t duty)  
{
    if (duty < 1280)
      duty = 1280;
    if (duty > 1680)
      duty = 1680;
    PWM_UpdateDuty(PWM2, kPWM_Module_3, kPWM_PwmA, duty);
    PWM_SetPwmLdok(PWM2, 1u<<kPWM_Module_3, true);  
}


/**
 *  PWM引脚配置
 */
static void pwm_pinconfig(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);          /* 打开io时钟 */
  
  IOMUXC_SetPinMux(PWM_OUT1_PINMUX, 0U);    //L5
  IOMUXC_SetPinMux(PWM_OUT2_PINMUX, 0U);    //M5
  IOMUXC_SetPinMux(PWM_OUT3_PINMUX, 0U);    //A8
  IOMUXC_SetPinMux(PWM_OUT4_PINMUX, 0U);    //A9
  IOMUXC_SetPinMux(PWM_SERVO1_PINMUX, 0U);
//  IOMUXC_SetPinMux(PWM_SERVO2_PINMUX, 0U);
  
  IOMUXC_SetPinConfig(PWM_OUT1_PINMUX, 0x10B0u);  
  IOMUXC_SetPinConfig(PWM_OUT2_PINMUX, 0x10B0u); 
  IOMUXC_SetPinConfig(PWM_OUT3_PINMUX, 0x10B0u); 
  IOMUXC_SetPinConfig(PWM_OUT4_PINMUX, 0x10B0u);
  IOMUXC_SetPinConfig(PWM_SERVO1_PINMUX, 0x10B0u); 
//  IOMUXC_SetPinConfig(PWM_SERVO2_PINMUX, 0x10B0u);
}

/**
 *  PWM模式配置
 *  ----------------
 *  PWMclock 200MHz的32分频 = 6,250,000  PWM->VAL1 16bit 6,250,000/VAL1  = 96Hz
 *  96hz以下的PWM信号，可以降低PWM的时钟频率 
 */
static void pwm_config(void)
{
  CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0); /* Set AHB PODF to 0, divide by 1 */ //600MHz 
  CLOCK_SetDiv(kCLOCK_IpgDiv, 0x2); /* Set IPG PODF to 2, divede by 3 */ //600MHz 3分频 200MHz
  
  uint32_t              pwmSourceClockInHz;   //PWM时钟源
  pwm_config_t          pwmConfig;
  pwm_signal_param_t    pwmSignal[2];     //PWM子模块初始化结构体 
  pwmSourceClockInHz = CLOCK_GetFreq(kCLOCK_IpgClk);//时钟源  200MHz

  PWM_GetDefaultConfig(&pwmConfig);  //得到默认的PWM初始化结构体
  pwmConfig.reloadLogic       = kPWM_ReloadPwmFullCycle;   //循环输出
  pwmConfig.enableDebugMode   = true;    
  pwmConfig.prescale          = kPWM_Prescale_Divide_32;        //PWM时钟为 pwmSourceClockInHz 32分频 
  
  pwmSignal[0].pwmChannel       = kPWM_PwmA;        //默认使用通道A
  pwmSignal[0].level            = kPWM_HighTrue;    //输出电平为高电平
  pwmSignal[0].dutyCyclePercent = 0;                //初始占空比 0%
  pwmSignal[0].deadtimeValue    = 0;                //死区时间
  /*当AB两通道同时使用 才有作用*/
  pwmSignal[1].pwmChannel       = kPWM_PwmB;        // 使用PWMB
  pwmSignal[1].level            = kPWM_HighTrue;    //输出电平为高电平
  pwmSignal[1].dutyCyclePercent = 0;                //初始占空比 0%
  pwmSignal[1].deadtimeValue    = 0;      //死区时间
  
  PWM_Init(PWM2, kPWM_Module_0, &pwmConfig);
  PWM2->SM[kPWM_Module_0].DISMAP[0]=0;      //屏蔽故障检测功能 
  /*设置pwm的时钟 = pwmSourceClockInHz，频率 = Frequency 对齐方式 = kPWM_SignedCenterAligned，*/
  PWM_SetupPwm(PWM2, kPWM_Module_0, pwmSignal, 2, kPWM_SignedCenterAligned, 12000,pwmSourceClockInHz); 
  PWM_SetPwmLdok(PWM2, 1u<<kPWM_Module_0, true);    //设置pwm的 load ok位
  PWM_StartTimer(PWM2, 1u<<kPWM_Module_0);          //开启定时器
  
  PWM_Init(PWM1, kPWM_Module_3, &pwmConfig);
  PWM1->SM[kPWM_Module_3].DISMAP[0]=0;      //屏蔽故障检测功能
  PWM_SetupPwm(PWM1, kPWM_Module_3, pwmSignal, 2, kPWM_SignedCenterAligned, 12000,pwmSourceClockInHz); 
  PWM_SetPwmLdok(PWM1, 1u<<kPWM_Module_3, true);    //设置pwm的 load ok位
  PWM_StartTimer(PWM1, 1u<<kPWM_Module_3);          //开启定时器 
  
  PWM_Init(PWM2, kPWM_Module_3, &pwmConfig);
  PWM2->SM[kPWM_Module_3].DISMAP[0]=0;      //屏蔽故障检测功能
  PWM_SetupPwm(PWM2, kPWM_Module_3, pwmSignal, 1, kPWM_SignedCenterAligned, 100,pwmSourceClockInHz); 
  PWM_SetPwmLdok(PWM2, 1u<<kPWM_Module_3, true);    //设置pwm的 load ok位
  PWM_StartTimer(PWM2, 1u<<kPWM_Module_3);          //开启定时器 
  servo(SERVO_MID);
}

void pwm_init(void)
{
  pwm_pinconfig();
  pwm_config();
}


/**
 *  舵机测试
 *  ----------------
 *  中值3000 频率200hz
 *  
 */
void servo_test(void)
{
  char txt[16];
  int servopwm = 0; 
  
  oled.init();
  oled.ops->clear();
  key.init();
  pwm_init();
  servo(SERVO_MID);  //中值
  
  while (1)
  {    
    switch(key.ops->get(0))  //检测按键
    {
    case no_key:
      break;
    case key_minus:
      servopwm -= 10;
      servo(SERVO_MID + servopwm);//刷新servopwm频率
      break; 
    case key_plus:           
      servopwm += 10;
      servo(SERVO_MID + servopwm);//刷新servopwm频率
      break;
    case key_ok:           
      servopwm = 0;
      servo(SERVO_MID + servopwm);//刷新servopwm频率
      break;
    }
    sprintf(txt, "PWM: %4d", SERVO_MID + servopwm);
    LCD_P6x8Str(0,0,(uint8_t*)txt); 

    led.ops->reverse(UpLight);     //红灯   
    delayms(50);
  }
}

void test_motor(void)
{    
  key.init();
  enc_init();
  pwm_init();
  oled.init();
  char txt[16];
  short motorpwm=0;  
  short left_enc,right_enc;

  while (1)
  {        
    switch(key.ops->get(1))  //检测按键
    {
    case no_key:
      break;
    case key_minus:
      motorpwm -= 100;
      if(motorpwm < -10000) motorpwm = -10000;
      left_motor(motorpwm);
      right_motor(motorpwm);
      break;           
    case key_plus:           
      motorpwm += 100;
      if(motorpwm > 10000) motorpwm = 10000;
      left_motor(motorpwm);
      right_motor(motorpwm);
      break;
    case key_ok:
      motorpwm = 0;
      left_motor(motorpwm);
      right_motor(motorpwm);
      break;
    }
    
    sprintf(txt,"PWM: %4.2f %",motorpwm/100.0f);
    LCD_P6x8Str(0,0,(uint8_t*)txt);

    left_enc = (int16_t)ENC_GetPositionDifferenceValue(ENC1);  //得到编码器微分值
    right_enc = (int16_t)ENC_GetPositionDifferenceValue(ENC2);  //得到编码器微分值

    sprintf(txt,"L:  %5d ",left_enc);
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    sprintf(txt,"R:  %5d ",right_enc); 
    LCD_P6x8Str(0,2,(uint8_t*)txt);
    //LED闪烁
    led.ops->reverse(UpLight);  
    delayms(50);
  }
}
