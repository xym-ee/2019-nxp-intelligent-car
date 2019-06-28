#include "system.h"
  
void adc1_init(void)
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
  ADC_SetHardwareAverageConfig(ADC1, kADC_HardwareAverageCount32);//硬件平均值    
  ADC_DoAutoCalibration(ADC1);                                    //硬件校准
   
}

adc_channel_config_t	adc1_chan_config;
uint16_t adc1_get(uint8_t ch)
{
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
//0前左，1前右，2后左，3后右，4横左，5横右
uint16_t adc_ind[6][5] = {0};
uint16_t   adc_data[6] = {0};

//以下位置描述为车中线相对于道路  
int8_t front_midloca(uint16_t *p)
{
  /*左侧电感*/ 
//  if(p[0]<FRONT_LEFT_1)//左电感距离引导线很远
//    goto front_right_check;
  if((p[0]>=FRONT_LEFT_1)&&(p[0]<FRONT_LEFT_2))//右Ⅰ区
    return -1;
  if((p[0]>=FRONT_LEFT_2)&&(p[0]<FRONT_LEFT_3))//右Ⅱ区
    return -2;
  if((p[0]>=FRONT_LEFT_3)&&(p[0]<FRONT_LEFT_4))//右Ⅲ区
    return -3;
  if((p[0]>=FRONT_LEFT_4)&&(p[0]<FRONT_LEFT_5))//右Ⅳ区
    return -4;
  if((p[0]>=FRONT_LEFT_5)&&(p[0]<FRONT_LEFT_6))//右Ⅴ区
    return -5;
  if((p[0]>=FRONT_LEFT_6)&&(p[0]<FRONT_LEFT_7))//右Ⅵ区
    return -6;      
  if((p[0]>=FRONT_LEFT_7)&&(p[0]<FRONT_LEFT_8))//右Ⅶ区
    return -7;      
  if(p[0]>=FRONT_LEFT_8)//
    return -8;

//   /*右侧电感*/
//  if(p[1]<FRONT_RIGHT_1)//右电感距离引导线很远
//    return 0;
  if((p[1]>=FRONT_RIGHT_1)&&(p[1]<FRONT_RIGHT_2))//左Ⅰ区
    return 1;
  if((p[1]>=FRONT_RIGHT_2)&&(p[1]<FRONT_RIGHT_3))//左Ⅱ区
    return 2;
  if((p[1]>=FRONT_RIGHT_3)&&(p[1]<FRONT_RIGHT_4))//左Ⅲ区
    return 3;
  if((p[1]>=FRONT_RIGHT_4)&&(p[1]<FRONT_RIGHT_5))//左Ⅳ区
    return 4;
  if((p[1]>=FRONT_RIGHT_5)&&(p[1]<FRONT_RIGHT_6))//左Ⅴ区
    return 5;
  if((p[1]>=FRONT_RIGHT_6)&&(p[1]<FRONT_RIGHT_7))//左Ⅵ区
    return 6;      
  if((p[1]>=FRONT_RIGHT_7)&&(p[1]<FRONT_RIGHT_8))//左Ⅶ区
    return 7;      
  if(p[1]>=FRONT_RIGHT_8)//
    return 8;
  return 0;
}

//以下位置描述为车中线相对于道路  
int8_t back_midloca(uint16_t *p)
{
  /*左侧电感*/
//  if(p[2]<BACK_LEFT_1)//左电感距离引导线很远
//    return 0;
  if((p[2]>=BACK_LEFT_1)&&(p[2]<BACK_LEFT_2))//右Ⅰ区
    return -1;
  if((p[2]>=BACK_LEFT_2)&&(p[2]<BACK_LEFT_3))//右Ⅱ区
    return -2;
  if((p[2]>=BACK_LEFT_3)&&(p[2]<BACK_LEFT_4))//右Ⅲ区
    return -3;
  if((p[2]>=BACK_LEFT_4)&&(p[2]<BACK_LEFT_5))//右Ⅳ区
    return -4;
  if((p[2]>=BACK_LEFT_5)&&(p[2]<BACK_LEFT_6))//右Ⅴ区
    return -5;
  if((p[2]>=BACK_LEFT_6)&&(p[2]<BACK_LEFT_7))//右Ⅵ区
    return -6;      
  if((p[2]>=BACK_LEFT_7)&&(p[2]<BACK_LEFT_8))//右Ⅶ区
    return -7;      
  if(p[2]>=BACK_LEFT_8)//
    return -8;
  
   /*右侧电感*/
//  if(p[3]<BACK_RIGHT_1)//右电感距离引导线很远
//    return 0;
  if((p[3]>=BACK_RIGHT_1)&&(p[3]<BACK_RIGHT_2))//左Ⅰ区
    return 1;
  if((p[3]>=BACK_RIGHT_2)&&(p[3]<BACK_RIGHT_3))//左Ⅱ区
    return 2;
  if((p[3]>=BACK_RIGHT_3)&&(p[3]<BACK_RIGHT_4))//左Ⅲ区
    return 3;
  if((p[3]>=BACK_RIGHT_4)&&(p[3]<BACK_RIGHT_5))//左Ⅳ区
    return 4;
  if((p[3]>=BACK_RIGHT_5)&&(p[3]<BACK_RIGHT_6))//左Ⅴ区
    return 5;
  if((p[3]>=BACK_RIGHT_6)&&(p[3]<BACK_RIGHT_7))//左Ⅵ区
    return 6;      
  if((p[3]>=BACK_RIGHT_7)&&(p[3]<BACK_RIGHT_8))//左Ⅶ区
    return 7;      
  if(p[3]>=BACK_RIGHT_8)//
    return 8;
  return 0;
}


void adc_datarefresh(void)
{
  uint8_t i;
  for(i=4;i>0;i--)
  {
    adc_ind[0][i] = adc_ind[0][i-1];
    adc_ind[1][i] = adc_ind[1][i-1];
    adc_ind[2][i] = adc_ind[2][i-1];
    adc_ind[3][i] = adc_ind[3][i-1];
    adc_ind[4][i] = adc_ind[4][i-1];
    adc_ind[5][i] = adc_ind[5][i-1];
  } 
  adc_ind[0][0] = adc1_get(4);
  adc_ind[1][0] = adc1_get(8);
  adc_ind[2][0] = adc1_get(10);
  adc_ind[3][0] = adc1_get(2);
  adc_ind[4][0] = adc1_get(7);
  adc_ind[5][0] = adc1_get(3);
  
  /*求取平均值*/
  for(i=0;i<6;i++)
  {
    adc_data[i] = (adc_ind[i][0] + adc_ind[i][1] + adc_ind[i][2] + adc_ind[i][3] + adc_ind[i][4])/5;
  }

}





void adc_test(void)
{
  char txt[16];
  
  LCD_Init();
  LCD_CLS();
  
  adc1_init();            //电源低压报警ADC初始化

  while (1)
  {  
    adc_datarefresh();
    
    sprintf(txt,"f:%4d",front_midloca(adc_data));
    LCD_P6x8Str(0,0,(uint8_t*)txt);
    
    sprintf(txt,"b:%4d",back_midloca(adc_data));
    LCD_P6x8Str(0,1,(uint8_t*)txt);

    led.ops->reverse(red);  
    delayms(100);
  }
}
