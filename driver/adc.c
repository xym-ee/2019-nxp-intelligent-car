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

adc_roaddata_t adc_roaddata = {
      .adcvalue = adc_data,
      .err      = 3,
      .err1     = 3,
      .status   = NoCircle,
};


/* ---------------------------- 方法声明 ------------------------------------ */
static void adc1_init(void);
static uint16_t adc1_get(uint8_t ch);
static void adc_datarefresh(void);
static int8_t adc_geterror(void);
static void adc_circle_check(void);
static void adc_error_check(void);

/* ---------------------------- 外部接口 ------------------------------------ */
const adc_operations_t adc_ops = {
    .get = adc1_get,
    .geterror = adc_geterror,
};

const adc_device_t adc = {
    .init = adc1_init,
    .refresh = adc_datarefresh,
    .circle_check = adc_circle_check,
    .error_check = adc_error_check,
    .ops = &adc_ops,
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
  
  adc_roaddata.err = adc.ops->geterror(); 
  
  /* 偏差记录 */
  if (adc_roaddata.err == 99)
  {
    adc_roaddata.err = adc_roaddata.err1;
  }
  adc_roaddata.err1 = adc_roaddata.err;
}

/* 电磁判断路况，AD值转换为偏差量 */
static int8_t adc_geterror(void)
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

/* 电磁线偏离检查 */
static void adc_error_check(void)
{
  /* 偏差检测 */
  if (adc_roaddata.status < CircleConditon) /* 满足切换条件 */
  { /* 偏差太大 */ //0 1    5 6
    if ( (adc_roaddata.err>5) && (adc_roaddata.adcvalue[3]<500) )  /* 车靠左并且最右侧电感小于一定值 */
      status.sensor = Inductance; /* 切换电感 */
    if ( (adc_roaddata.err<1) && (adc_roaddata.adcvalue[0]<500) ) 
      status.sensor = Inductance; /* 切换电感 */
  }
}

/* 在等待10ms中断时进行的操作 */
static void adc_circle_check(void)
{
 /*------------    右侧圆环触发动作   -----------------------*/
  if ( adc_wire_status() > SingleLine && adc_roaddata.status == NoCircle) /* 不区分左右圆环 */
  { /* 位置计数清零 */
    motor.pidclear(&pid);
    left_motor(0);
    right_motor(0);
    ENC_DoSoftwareLoadInitialPositionValue(ENC1); /* 寄存器清零 */
    ENC_DoSoftwareLoadInitialPositionValue(ENC2);
    adc_roaddata.status = RightCircleWaitIn;      /* 入环信号 */
    status.sensor = Inductance;                   /* 切换电磁引导 */
    return;
  }
  
  /* 右环左线，准备出环 */
  if (adc_roaddata.status == RightCircleRun && adc_wire_status() > SingleLine)
  { /* 位置计数清零 */
    ENC_DoSoftwareLoadInitialPositionValue(ENC1);
    ENC_DoSoftwareLoadInitialPositionValue(ENC2);     
    adc_roaddata.status = RightCircleWaitOut;       /* 出环信号 */
    status.sensor = Inductance;                     /* 切换电磁引导 */
    return;
  }
}


