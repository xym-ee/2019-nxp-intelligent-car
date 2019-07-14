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
  



/* ---------------------------- 方法声明 ------------------------------------ */
static void adc1_init(void);
static uint16_t adc1_get(uint8_t ch);
static void adc_datarefresh(void);
static void adc_test(void);

/* ---------------------------- 外部接口 ------------------------------------ */
const adc_operations_t adc_ops = {
    .refresh = adc_datarefresh,
    .get = adc1_get,
};

const adc_device_t adc = {
    .init = adc1_init,
    .ops = &adc_ops,
    .test = adc_test,
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

//记录5次取平均值
uint16_t adc_ind[4][5] = {0};
uint16_t   adc_data[4] = {0};




static void adc_datarefresh(void)
{
  uint8_t i;
  for(i=3;i>0;i--)
  {
    adc_ind[0][i+1] = adc_ind[0][i];
    adc_ind[0][i+1] = adc_ind[0][i];
    adc_ind[0][i+1] = adc_ind[0][i];
    adc_ind[0][i+1] = adc_ind[0][i];
  } 
  
  adc_ind[0][0] = adc1_get(3);
  adc_ind[1][0] = adc1_get(8);
  adc_ind[2][0] = adc1_get(7);
  adc_ind[3][0] = adc1_get(4);  
  
  /*求取平均值*/
  for(i=0;i<4;i++)
    adc_data[i] = (adc_ind[i][0] + adc_ind[i][1] + adc_ind[i][2] + adc_ind[i][3] + adc_ind[i][4])/5;
}


static int8_t adc2logic(void)
{
  if ( A1 && (!A2) && (!A3) && (!A4) )
    return -3;
  else if( (A1) && (A2) && (!A3) && (!A4))
    return -2;  
  else if( (!A1) && (A2) && (!A3) && (!A4))
    return -1;    
  else if( (!A1) && (A2) && (A3) && (!A4))
    return 0;
  else if( (!A1) && (!A2) && (A3) && (!A4))
    return 1;  
  else if( (!A1) && (!A2) && (A3) && (A4))
    return 2;
  else if( (!A1) && (!A2) && (!A3) && (A4))
    return 3;
  else
    return 0;
}


static void adc_test(void)
{
  char txt[16];
  
  oled.init();

  oled.ops->clear();
  
  adc.init();
  
  motor.init();
  uint16_t pwm;

  while (1)
  {  
    adc.ops->refresh();
    
    pwm = 1500 + 50*adc2logic();
    servo(pwm);
    
    sprintf(txt,"%4d",pwm);
    LCD_P6x8Str(0,0,(uint8_t*)txt);    
    
//    sprintf(txt,"1:%4d",adc_data[0]);
//    LCD_P6x8Str(0,0,(uint8_t*)txt);
//    
//    sprintf(txt,"2:%4d",adc_data[1]);
//    LCD_P6x8Str(0,1,(uint8_t*)txt);
//    
//    sprintf(txt,"3:%4d",adc_data[2]);
//    LCD_P6x8Str(0,2,(uint8_t*)txt);
//    
//    sprintf(txt,"4:%4d",adc_data[3]);
//    LCD_P6x8Str(0,3,(uint8_t*)txt);
    
    led.ops->reverse(UpLight);  
    delayms(10);
  }
}
