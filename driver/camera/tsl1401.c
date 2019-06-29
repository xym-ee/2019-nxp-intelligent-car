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

void tsl1401_init(void)
{
  //用ENC3接口接TSL1401
  
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23,0U); //  K10  CLK
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16,0U); //  K1  SI
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23,0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16,0x10B0u);  
   
  
  
  gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIO为输出方向
                                          0,                   //低电平
                                          kGPIO_NoIntmode      //非中断模式
                                          };  
  GPIO_PinInit(GPIO1,23, &GPIO_Output_Config);      //  
  GPIO_PinInit(GPIO1,16, &GPIO_Output_Config);      // 
  
}

uint16_t ADC_ConvertedValue;

unsigned char pixel[128];

void RD_TSL(void) 
{
	uint8_t i = 0;

	//参考TSL1401技术文档第5页
	TSL_CLK = 1;
	TSL_SI = 0; 
	
//	__NOP();
    delayus(1);
	
	TSL_CLK = 0;  //低电平最窄持续50ns  
	TSL_SI = 1; 	//SI最窄20ns
	
//	__NOP();
//	__NOP();
//	__NOP();
//	__NOP();
    delayus(1);  
	TSL_CLK = 1;
	TSL_SI = 0;
	
//	__NOP();
	delayus(1);
	for(i=0;i<128;i++)
	{ 
		TSL_CLK = 0; 
		pixel[i] = adc1_get(11)/16; //adc中函数去掉static
		TSL_CLK = 1;
        delayus(1);
//		__NOP();
		//printf("%c",pixel[i]-1);//DEBUG查看原始数据
	}  
		//printf("%c",0xff);//DEBUG查看原始数据
}




unsigned char ccdimage[124];
//p为原始数据
__ramfunc void ccd_0_1(unsigned char *p)
{
  uint16_t i;
  for(i=2;i<126;i++)
    ccdimage[i-2]=(p[i-2]+2*p[i-1]+4*p[i]+2*p[i+1]+p[i+2])/10;//滤波
  
  uint8_t pixelCount[256];
  float pixelPro[256];
  
  //灰度点统计
  for(i=0;i<124;i++)
    pixelCount[ccdimage[i]]++;
  
  for(i=0;i<256;i++)
    pixelPro[i] = pixelCount[i]/128.0f;
  
  float P1 = 0;
  float P2 = 0;
  float m1 = 0;
  float m2 = 0;
  float u0 = 0;
  float u1 = 0;
  float mG = 0;
  float Sigma = 0;
  float SigmaMax = 0;
  uint8_t threshold;
  
  //动态阈值
  for(i=0;i<256;i++)
  {
    P1 = 0;
    P2 = 0;
    m1 = 0;
    m2 = 0;
    for(uint16_t j=0;j<256;j++)
    {
      if(j<i)
      {
        P1 = P1 + pixelPro[j];
        m1 = m1 +  j*pixelPro[j];
      }
      else
      {
        P2 = P2 + pixelPro[j];
        m2 =m2 + j*pixelPro[j];
      }
      
    }
    u0 = m1 / P1;
    u1 = m2 / P2;
    mG = m1 + m2;
    Sigma = P1 * (u0 - mG)*(u0 - mG) + P2 * (u1 - mG)*(u1 - mG);
    if(Sigma > SigmaMax)
    {
      SigmaMax = Sigma;
      threshold = i;
    }
  }
  
//二值化
  for(i=0;i<124;i++)
  {
    if(ccdimage[i]>=threshold)
      ccdimage[i] = 1;
    else
      ccdimage[i] = 0;
  }
}


static void outputccd(unsigned char *p)
{
  uint8_t i;
//  printf("%c",0xff);
//  for(i=0;i<128;i++)
//  {
//    if(p[i]==0xff)
//      printf("%c",p[i]-1);
//    else
//      printf("%c",p[i]);
//  
//  }
  
//  printf("%c",0x00);
//  printf("%c",0x00);
//  for(i=0;i<124;i++)
//  {
//    if(p[i])
//      printf("%c",0xfe);
//    else
//      printf("%c",0x00);
//  }
//  printf("%c",0x00);
//  printf("%c",0x00);
  
  LCD_Set_Pos(4,0);
  for(i=0;i<124;i++)
  {      
    if(p[i])
      LCD_WrDat(0xff);	
    else
      LCD_WrDat(0x00);	
  }
}


int8_t tsl1401err(void)
{
  
  uint8_t i,left,right;
   RD_TSL();
   ccd_0_1(pixel);
   
   //从中往左边找
   for(i=61;i>0;i--)
   {
      if(!ccdimage[i]) //是黑色
      {
        left=i;
        break;
      }
   }
   if(ccdimage[0])//最左边为白色
     left=0;
   
   //从中往右边找
   for(i=62;i<124;i++)
   {
      if(!ccdimage[i]) //是黑色
      {
        right=i;
        break;
      }
   }
   if(ccdimage[123])//最右边为白色
     right=123;
   
   return (left+right)/2-62;
}

void tsl1401_test(void)
{
  short pwm=0;
  short e,laste,ec;
  
  LCD_Init();
  LCD_CLS();
  adc1_init();
  tsl1401_init();

  pid_control_init();           //速度控制
  
  while(1)
  {
    RD_TSL();
    ccd_0_1(pixel);
    outputccd(ccdimage);
    
    e = tsl1401err();
    ec = e-laste;
    
    
    pwm = 3000 + 10*e + 5*ec;
    
    
    servo(pwm);
    laste = e;
    car_speed(50);
    delayms(10);
  }
}

