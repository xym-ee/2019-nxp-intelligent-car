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


/* 电磁传感器相关的全局变量 */
uint16_t adc_ind[4][5] = {0}; /* 电压值队列，记录5次取平均值 */
uint16_t   adc_data[4] = {0}; /* 最新电压值暂存 */
int8_t adc_err, adc_err1 = 3;
adc_circle_t adc_circle_status = NoCircle; /* 根据电磁判断的圆环状态标志位 */


adc_roaddata_t adc_roadtype = {
      .adcvalue = adc_data,
      .err      = 3,
      .err1     = 3,
      .status   = NoCircle,
};



/* ---------------------------- 方法声明 ------------------------------------ */
static void adc1_init(void);
static uint16_t adc1_get(uint8_t ch);
static void adc_datarefresh(void);
static void adc_test(void);
static int8_t adc1convert(void);
static void adc_circle_test(void);
static void adc_circle_check(void);

/* ---------------------------- 外部接口 ------------------------------------ */
const adc_operations_t adc_ops = {
    .get = adc1_get,
    .geterror = adc1convert,
};

const adc_device_t adc = {
    .init = adc1_init,
    .refresh = adc_datarefresh,
    .circle_check = adc_circle_check,
    .ops = &adc_ops,
    .test = adc_test,
    .circle_test = adc_circle_test,
};

/* ---------------------------- 方法实现 ------------------------------------ */



static void adc1_init(void)
{          
  adc_config_t          adc1_config;
  //ADC配置,ADC不需要配置相应的引脚！！！
  ADC_GetDefaultConfig(&adc1_config);             //先配置为默认值
  adc1_config.referenceVoltageSource    =	kADC_ReferenceVoltageSourceAlt0;	//参考电压源
  adc1_config.samplePeriodMode          =	kADC_SamplePeriod2or12Clocks;  		//采样周期模式
  adc1_config.clockSource               =	kADC_ClockSourceAD;     					//时钟源使用ADACK,噪声低
  adc1_config.clockDriver               =	kADC_ClockDriver1;      					//1分频
  adc1_config.resolution                =	kADC_Resolution12Bit;    					//分辨率，12位
  ADC_Init(ADC1,&adc1_config);                    													//初始化ADC1
  ADC_EnableHardwareTrigger(ADC1, false);         													//ADC1硬件触发关闭
  ADC_SetHardwareAverageConfig(ADC1, kADC_HardwareAverageCount32);          //硬件平均值    
  ADC_DoAutoCalibration(ADC1);                                              //硬件校准
}

/* 获取单个通道的电压值 */
static uint16_t adc1_get(uint8_t ch)
{
  adc_channel_config_t	adc1_chan_config;
  uint16_t adc_value;
  //ADC通道配置
  adc1_chan_config.channelNumber                          =	ch;
  adc1_chan_config.enableInterruptOnConversionCompleted   =	false;  //关闭转换完成中断
  ADC_SetChannelConfig(ADC1, 0, &adc1_chan_config);
  while(ADC_GetChannelStatusFlags(ADC1,0)==0){};                      //等待转换完成
  adc_value = ADC_GetChannelConversionValue(ADC1,0);                  //读取ADC值
  return adc_value;
}

static void adc_datarefresh(void)
{
  uint8_t i;
  for(i=3;i>0;i--)
  { /* 队列数据往后移动一个 */
    adc_ind[0][i+1] = adc_ind[0][i];
    adc_ind[0][i+1] = adc_ind[0][i];
    adc_ind[0][i+1] = adc_ind[0][i];
    adc_ind[0][i+1] = adc_ind[0][i];
  } 
  
  /* 获取最新值 */
  adc_ind[0][0] = adc1_get(3);
  adc_ind[1][0] = adc1_get(8);
  adc_ind[2][0] = adc1_get(7);
  adc_ind[3][0] = adc1_get(4);  
  
  /* 求取平均值 */
  for(i=0;i<4;i++)
    adc_data[i] = (adc_ind[i][0] + adc_ind[i][1] + adc_ind[i][2] + adc_ind[i][3] + adc_ind[i][4])/5;
  
}

/* 电磁判断路况，AD值转换为偏差量 */
static int8_t adc1convert(void)
{ /* 正常的直路和弯路 */
  if ( A1 && (!A2) && (!A3) && (!A4))         /* 1000 */
    return NB;
  else if ( A1 && A2 && (!A3) && (!A4) )      /* 1100 */
    return NM;
  else if ( (!A1) && A2 && (!A3) && (!A4) )   /* 0100 */
    return NS;  
  else if ( (!A1) && A2 && A3 && (!A4) )      /* 0110 */
    return ZO;
  else if ( (!A1) && (!A2) && A3 && (!A4) )   /* 0010 */
    return PS;  
  else if ( (!A1) && (!A2) && A3 && A4 )      /* 0011 */
    return PM;   
  else if ( (!A1) && (!A2) && (!A3) && A4 )   /* 0001 */
    return PB;
    
  /* 其他的信号过小的情况，离开电磁线了 */
  else
    return 99;
}

/* 在等待10ms中断时进行的操作 */
static void adc_circle_check(void)
{
  /* 圆环触发动作 */
  if ( adc_wire_status() == RightLine && adc_roadtype.status == NoCircle ) /* 右侧圆环触发 */
  { /* 位置计数清零 */
    ENC_DoSoftwareLoadInitialPositionValue(ENC1);
    ENC_DoSoftwareLoadInitialPositionValue(ENC2);
    adc_roadtype.status = RightCircleWaitIn;      /* 入环信号 */
    return;
  }
  /* 右环左线，准备出环 */
  if (adc_roadtype.status == RightCircleRun && adc_wire_status() == LeftLine)
  { /* 位置计数清零 */
    ENC_DoSoftwareLoadInitialPositionValue(ENC1);
    ENC_DoSoftwareLoadInitialPositionValue(ENC2);     
    adc_roadtype.status = RightCircleWaitOut;         /* 出环信号 */
    return;
  }
  
  
}


static void adc_circle_test(void)
{

}

/* ADC测试函数 */
static void adc_test(void)
{
  char txt[16];
  
  oled.init();
  oled.ops->clear();
  adc.init();
  motor.init();
  adc_circle_t circle_status = NoCircle;
  short s1, s2;
  int8_t err;   /* 根据电磁判断的大概偏差 */
  static int8_t err1 = 3;
  uint16_t servo_pwm;
  
  extern const signed char RuleBase[7][7];
  extern const signed char _servo[7];
  while (1)
  {
    adc.refresh();
    adc.refresh();
    adc.refresh();
    adc.refresh();
    adc.refresh(); 
    
    adc_roadtype.err = adc.ops->geterror(); /* 获得大概偏差 */
    
    sprintf(txt,"%2d",adc_roadtype.err);
    LCD_P6x8Str(0,0,(uint8_t*)txt);
    
    /* 圆环触发动作 */
    if ( adc_wire_status() == RightLine && circle_status == NoCircle ) /* 右侧圆环触发 */
    {
      ENC_DoSoftwareLoadInitialPositionValue(ENC1);
      ENC_DoSoftwareLoadInitialPositionValue(ENC2);
      circle_status = RightCircleWaitIn;
    }
    
    s1 = (int16_t)ENC_GetPositionValue(ENC1);
    s2 = (int16_t)ENC_GetPositionValue(ENC2);
    
    /* 圆环修正入环动作，距离超过2000 */
    if ( circle_status == RightCircleWaitIn && s2>2000)
    {
      if (err == 99) /* 99信号 */
        err = 5; /* 转弯 */
    }
    
    /* 环内运行标志置位 */
    if ( circle_status == RightCircleWaitIn && s2>8000 )
    {
      circle_status = RightCircleRun; /* 此距离判断为已经入环 */
      /* 切换摄像头 */
    }
    
    /* 修正摄像头出环偏差问题。只要电磁传感器偏差过大，立刻切换电磁 */
    if (circle_status == RightCircleRun)
    {
      if (err == 99)
      {
        //减速
        //切换电磁传感器运行
      }
    }
    
    /* 右环左线，准备出环 */
    if (circle_status == RightCircleRun && adc_wire_status() == LeftLine)
    {
      circle_status = RightCircleWaitOut;
      ENC_DoSoftwareLoadInitialPositionValue(ENC1);
      ENC_DoSoftwareLoadInitialPositionValue(ENC2);      
    }
    
     /* 圆环修正入环动作，距离超过2000 */
    if ( circle_status == RightCircleWaitOut && s2>2000)
    {/* 99给小左弯 */
      if (err == 99) /* 99信号 */
        err = 2; /* 转弯 */ 
    }   
    
    
    if (err == 99)
      err = err1;   /* 如果偏离了电磁线，偏差按偏离前计算 */
    
    servo_pwm = 1500 + _servo[RuleBase[err][err1]]*40;
    servo(servo_pwm);   
    

    sprintf(txt,"%2d",adc_data[0]);
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    sprintf(txt,"%2d",adc_data[1]);
    LCD_P6x8Str(0,2,(uint8_t*)txt);
    sprintf(txt,"%2d",adc_data[2]);
    LCD_P6x8Str(0,3,(uint8_t*)txt);
    sprintf(txt,"%2d",adc_data[3]);
    LCD_P6x8Str(0,4,(uint8_t*)txt);
    
    
    sprintf(txt,"ENC1: %6d ",s1); 
    LCD_P6x8Str(0,5,(uint8_t*)txt);
    
    sprintf(txt,"ENC2: %6d ",s2);
    LCD_P6x8Str(0,6,(uint8_t*)txt);
    
   
    led.ops->reverse(UpLight);  
    delayms(10);
  }
}
