/*******************************************************************************
 *  
 *
 *  文件名称：motor.c
 *  简要描述：
 *      ENC电机编码器接口初始化
 *      PWM模块初始化
 *      电机PID闭环控制实现
 *      ENC、PWM单独测试（oeld显示）
 *        
 *  最后修改日期：2019/6/17
 *
 ******************************************************************************/
#include "system.h"
 


short speedvalue = 0;
_pid_t leftpid;
_pid_t rightpid;



static void motor_init(void);
static void left_motor(short duty);
static void right_motor(short duty);


static void pid_value_init(_pid_t* base);
static void pid_value_change(_pid_t* base,short kp,short ki,short kd);
static short pid_control(_pid_t* base,short targer,short real);



const pid_operations_t pid_value_ops = {
		.init = pid_value_init,
		.change = pid_value_change,
		.control = pid_control,
};

const motor_device_t motor = {
		.init = motor_init,
		
		.pid_ops = &pid_value_ops,
};





static void enc_init(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);  /* 打开io时钟 */ 

  IOMUXC_SetPinMux(ENC_LA_MUX,1U);      
  IOMUXC_SetPinMux(ENC_LB_MUX,1U);     
  IOMUXC_SetPinMux(ENC_RA_MUX,1U);     
  IOMUXC_SetPinMux(ENC_RB_MUX,1U);     
  
  IOMUXC_SetPinConfig(ENC_LA_MUX,0x10B0u);
  IOMUXC_SetPinConfig(ENC_LB_MUX,0x10B0u);
  IOMUXC_SetPinConfig(ENC_RA_MUX,0x10B0u);
  IOMUXC_SetPinConfig(ENC_RB_MUX,0x10B0u);

  XBARA_Init(XBARA1);    /*初始化XBARA1模块*/

  XBARA_SetSignalsConnection(XBARA1, ENC_LA_CHANNEL, kXBARA1_OutputEnc1PhaseAInput);
  XBARA_SetSignalsConnection(XBARA1, ENC_LB_CHANNEL, kXBARA1_OutputEnc1PhaseBInput);
  XBARA_SetSignalsConnection(XBARA1, ENC_RA_CHANNEL, kXBARA1_OutputEnc2PhaseAInput);
  XBARA_SetSignalsConnection(XBARA1, ENC_RB_CHANNEL, kXBARA1_OutputEnc2PhaseBInput);

  enc_config_t mEncConfigStruct;
  ENC_GetDefaultConfig(&mEncConfigStruct);
  mEncConfigStruct.decoderWorkMode = kENC_DecoderWorkAsSignalPhaseCountMode;  
  
  mEncConfigStruct.enableReverseDirection = true;  /* 开启反向计数 */
  ENC_Init(ENC1, &mEncConfigStruct);
  ENC_DoSoftwareLoadInitialPositionValue(ENC1);     /* 位置计数器初始值更新成0. */

  mEncConfigStruct.enableReverseDirection = false;  /* 开启反向计数 */
  ENC_Init(ENC2, &mEncConfigStruct);     
  ENC_DoSoftwareLoadInitialPositionValue(ENC2);     /* 位置计数器初始值更新成0. */ 
}


static void pwm_init(void)
{
  /* PWM引脚配置 */
  CLOCK_EnableClock(kCLOCK_Iomuxc);          /* 打开io时钟 */
  
  IOMUXC_SetPinMux(PWM_OUT1_PINMUX, 0U);        //L5
  IOMUXC_SetPinMux(PWM_OUT2_PINMUX, 0U);        //M5
  IOMUXC_SetPinMux(PWM_OUT3_PINMUX, 0U);        //A8
  IOMUXC_SetPinMux(PWM_OUT4_PINMUX, 0U);        //A9
  IOMUXC_SetPinMux(PWM_SERVO1_PINMUX, 0U);
//  IOMUXC_SetPinMux(PWM_SERVO2_PINMUX, 0U);
  
  IOMUXC_SetPinConfig(PWM_OUT1_PINMUX, 0x10B0u);  
  IOMUXC_SetPinConfig(PWM_OUT2_PINMUX, 0x10B0u); 
  IOMUXC_SetPinConfig(PWM_OUT3_PINMUX, 0x10B0u); 
  IOMUXC_SetPinConfig(PWM_OUT4_PINMUX, 0x10B0u);
  IOMUXC_SetPinConfig(PWM_SERVO1_PINMUX, 0x10B0u); 
//  IOMUXC_SetPinConfig(PWM_SERVO2_PINMUX, 0x10B0u);

  
 /**
 *  PWM模式配置
 *  ----------------
 *  PWMclock 200MHz的32分频 = 6,250,000  PWM->VAL1 16bit 6,250,000/VAL1  = 96Hz
 *  96hz以下的PWM信号，可以降低PWM的时钟频率 
 */
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
  PWM_SetupPwm(PWM2, kPWM_Module_3, pwmSignal, 1, kPWM_SignedCenterAligned, 200,pwmSourceClockInHz); 
  PWM_SetPwmLdok(PWM2, 1u<<kPWM_Module_3, true);    //设置pwm的 load ok位
  PWM_StartTimer(PWM2, 1u<<kPWM_Module_3);          //开启定时器 
  servo(3000);
}



static void motor_init(void)
{
  enc_init();
  pwm_init();
}






/**
 *  设置电机占空比（速度）
 *  ----------------
 *  duty                占空比             -DUTY_MAX ~ DUTY_MAX
 *  ----------------
 *  duty最大值10000定义在DUTY_MAX
 *  
 *  left_motor(-2000)  //左电机反转  占空比  2000 / 10000 
 */
static void left_motor(short duty)
{
  if(duty >= 0)  //  正转
  {
    PWM_UpdateDuty(PWM1, kPWM_Module_3, kPWM_PwmA, 0); 
    PWM_UpdateDuty(PWM1, kPWM_Module_3, kPWM_PwmB, duty); 
    PWM_SetPwmLdok(PWM1, 1u<<kPWM_Module_3, true);        //设置pwm的 load ok位
  }
  else
  {
    PWM_UpdateDuty(PWM1, kPWM_Module_3, kPWM_PwmA, -duty); 
    PWM_UpdateDuty(PWM1, kPWM_Module_3, kPWM_PwmB, 0); 
    PWM_SetPwmLdok(PWM1, 1u<<kPWM_Module_3, true);        //设置pwm的 load ok位                
  }
}
static void right_motor(short duty)
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




static void pid_value_init(_pid_t* base)
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

static void pid_value_change(_pid_t* base,short kp,short ki,short kd)
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
  
  pid_value_change(&leftpid,5,0,2);
  pid_value_change(&rightpid,5,0,2);
}


static void car_speed(short value)
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



void encoder_test(void)
{   
  LCD_Init();
  LCD_CLS();
  key_init();          //按键及输入口初始化
  enc_init();
  
  short velocity1, velocity2;
  char txt[16];
  while (1)
  { 
    velocity1 = (int16_t)ENC_GetPositionDifferenceValue(ENC1);  //得到编码器微分值
    sprintf(txt,"ENC1:  %5d ",velocity1); 
    LCD_P6x8Str(0,0,(uint8_t*)txt);
    
    velocity2 = (int16_t)ENC_GetPositionDifferenceValue(ENC2);  //得到编码器微分值
    sprintf(txt,"ENC2:  %5d ",velocity2); 
    LCD_P6x8Str(0,1,(uint8_t*)txt);
     
    led.ops->ctrl(LED_R, RVS);

    delayms(100);       //延时0.1秒
  }
}





void test_motor(void)
{    
  char txt[16];
  short motorpwm=0;   
  LCD_Init();
  LCD_CLS();
  key_init();          //按键及输入口初始化

  motor.init();
  
  short left_enc,right_enc;

  while (1)
  {        
    switch(key_read(1))  //检测按键
    {
    case 0:
      break;
    case 1:
      motorpwm -= 100;
      if(motorpwm < -10000) motorpwm = -10000;
      left_motor(motorpwm);
      right_motor(motorpwm);
      break;           
    case 3:           
      motorpwm += 100;
      if(motorpwm > 10000) motorpwm = 10000;
      left_motor(motorpwm);
      right_motor(motorpwm);
      break;
    case 2:
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
    led.ops->color(red);  
    delayms(50);
    led.ops->color(blue);   
    delayms(50);
  }
}



