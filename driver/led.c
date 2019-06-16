/*
GPIO2_IO23  -------->  管脚B12   ----- >  核心板G灯
GPIO3_IO26  -------->  管脚A7    ----- >  核心板R灯   车顶灯
GPIO3_IO27  -------->  管脚C7    ----- >  核心板B灯

GPIO2_IO22  -------->  管脚C12   ----- >  母板D0
GPIO2_IO25  -------->  管脚A13   ----- >  母板D1
*/

#include "system.h"

void LED_Init(void)
{    
  CLOCK_EnableClock(kCLOCK_Iomuxc);           // IO口时钟使能
/*       核心板上的LED       */
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_GPIO2_IO23, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_40_GPIO3_IO26, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_41_GPIO3_IO27, 0U);

  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_07_GPIO2_IO23,0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_40_GPIO3_IO26,0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_41_GPIO3_IO27,0x10B0u);
  
  
  gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIO为输出方向
                                        0,                   //低电平
                                        kGPIO_NoIntmode      //非中断模式
                                        };
  // Init output LED GPIO.
  GPIO_PinInit(GPIO2,23, &GPIO_Output_Config);      //  
  GPIO_PinInit(GPIO3,26, &GPIO_Output_Config);      //  
  GPIO_PinInit(GPIO3,27, &GPIO_Output_Config);      //   
  
  /*             母板上的lED引脚             */
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_GPIO2_IO22, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_GPIO2_IO25, 0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_06_GPIO2_IO22,0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_GPIO2_IO25,0x10B0u);
  GPIO_PinInit(GPIO2,22, &GPIO_Output_Config);      //C12  D1
  GPIO_PinInit(GPIO2,25, &GPIO_Output_Config);      //A13  D0
}

/**
 *  指定灯色亮
 *  ----------------
 *  
 */
void LED_Color(LED_t color)
{
  switch(color)
  {
  case white:
    GPIO_PinWrite(GPIO2,23, 0U);//LED亮; 
    GPIO_PinWrite(GPIO3,26, 0U);//LED亮; 
    GPIO_PinWrite(GPIO3,27, 0U);//LED亮; 
    break;
  case black:
    GPIO_PinWrite(GPIO2,23, 1U);//LED灭; 
    GPIO_PinWrite(GPIO3,26, 1U);//LED灭; 
    GPIO_PinWrite(GPIO3,27, 1U);//LED灭; 
    break;
  case red:
    GPIO_PinWrite(GPIO2,23, 1U);//LED灭; 
    GPIO_PinWrite(GPIO3,26, 0U);//LED亮; 
    GPIO_PinWrite(GPIO3,27, 1U);//LED灭; 
    break;
  case green:
    GPIO_PinWrite(GPIO2,23, 0U);//LED灭; 
    GPIO_PinWrite(GPIO3,26, 1U);//LED亮; 
    GPIO_PinWrite(GPIO3,27, 1U);//LED灭; 
    break;
  case blue:
    GPIO_PinWrite(GPIO2,23, 1U);//LED灭; 
    GPIO_PinWrite(GPIO3,26, 1U);//LED亮; 
    GPIO_PinWrite(GPIO3,27, 0U);//LED灭; 
    break;
  case yellow:
    GPIO_PinWrite(GPIO2,23, 0U);//LED灭; 
    GPIO_PinWrite(GPIO3,26, 0U);//LED亮; 
    GPIO_PinWrite(GPIO3,27, 1U);//LED灭; 
    break;
  case violet:
    GPIO_PinWrite(GPIO2,23, 1U);//LED灭; 
    GPIO_PinWrite(GPIO3,26, 0U);//LED亮; 
    GPIO_PinWrite(GPIO3,27, 0U);//LED灭; 
    break;
  case cyan:
    GPIO_PinWrite(GPIO2,23, 0U);//LED灭; 
    GPIO_PinWrite(GPIO3,26, 1U);//LED亮; 
    GPIO_PinWrite(GPIO3,27, 0U);//LED灭; 
    break;
  }
}

/**
 *  指定灯色翻转
 *  ----------------
 *  
 */
void LED_Color_Reverse(LED_t color)
{
    static uint8_t count = 0;
    if(count++ % 2)
    {
        LED_Color(color);   //亮指定颜色的灯
    }
    else
    {
        LED_Color(black);   //灭
    }
    
}


/**
 *  控制IO输出高低电平
 *  ----------------
 *  LEDn_e ledno, 编号,LEDs_e sta 状态，亮灭
 */
void LED_Ctrl(LEDn_e ledno, LEDs_e sta)
{
  switch(ledno) 
  {
  case LED_G:
    if(sta==ON)        GPIO_PinWrite(GPIO2,23, 0U);//LED亮; 
    else if(sta==OFF) GPIO_PinWrite(GPIO2,23, 1U);//LED灭 
    else if(sta==RVS) GPIO_PinReverse (GPIO2, 23); //LED反转
    break;  
  case LED_R:
    if(sta==ON)        GPIO_PinWrite(GPIO3,26, 0U);//LED亮; 
    else if(sta==OFF) GPIO_PinWrite(GPIO3,26, 1U);//LED灭 
    else if(sta==RVS) GPIO_PinReverse (GPIO3, 26); //LED反转
    break;    
  case LED_B:
    if(sta==ON)        GPIO_PinWrite(GPIO3, 27, 0U);//LED亮; 
    else if(sta==OFF) GPIO_PinWrite(GPIO3, 27, 1U);//LED灭 
    else if(sta==RVS) GPIO_PinReverse (GPIO3, 27); //LED反转
    break; 
  case LEDALL:
    if(sta==ON) 
    {        
      GPIO_PinWrite(GPIO2,23, 0U);//LED亮; 
      GPIO_PinWrite(GPIO3,26, 0U);//LED亮; 
      GPIO_PinWrite(GPIO3,27, 0U);//LED亮; 
    }
    else if(sta==OFF)
    {  
      GPIO_PinWrite(GPIO2,23, 1U);//LED亮; 
      GPIO_PinWrite(GPIO3,26, 1U);//LED亮; 
      GPIO_PinWrite(GPIO3,27, 1U);//LED亮; 
    }
    else if(sta==RVS)
    {       
      GPIO_PinReverse (GPIO3, 27); //Toggle on target LED
      GPIO_PinReverse (GPIO3, 26); 
      GPIO_PinReverse (GPIO2, 23);
    }
    break;
  default:
    break;    
  }   
}


void status_lignt(void)
{

}

