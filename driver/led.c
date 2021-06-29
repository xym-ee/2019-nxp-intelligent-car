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
/* LED设备初始化 */
static void led_pin_init(void);

/* LED操作 */
static void led_on(led_name_t choose);
static void led_off(led_name_t choose);
static void led_reverse(led_name_t color);
static void led_flash_fast(led_name_t color);
static void led_flash_slow(led_name_t color);
static void led_off_all(void);



/* ---------------------------- 外部接口 ------------------------------------ */
const led_operations_t led_ops = {
        .on = led_on,
        .off = led_off,
        .reverse = led_reverse,
        .flash_fast = led_flash_fast,
        .flash_slow = led_flash_slow,
        .off_a = led_off_all,
};

const led_device_t led = {
        .init = led_pin_init,
        .ops = &led_ops
};
/* ---------------------------- 方法实现 ------------------------------------ */

/*
GPIO2_IO23  -------->  管脚B12   ----- >  核心板G灯
GPIO3_IO26  -------->  管脚A7    ----- >  核心板R灯   车顶灯
GPIO3_IO27  -------->  管脚C7    ----- >  核心板B灯

GPIO2_IO22  -------->  管脚C12   ----- >  母板D0
GPIO2_IO25  -------->  管脚A13   ----- >  母板D1

从EX1靠近丝印侧从右往左 

舵机板的转向指示灯
F11 ADB004   
C12 B106     
车顶灯
A7  EMC40

*/
static void led_pin_init(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);           // IO口时钟使能
  
  /* F11 舵机板 右转 0亮*/
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_04_GPIO1_IO04, 0U); 
  /* C12 舵机板 左转 0亮 母版的D0 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_GPIO2_IO22,    0U);
  /* A7  核心板上的R 0亮车顶灯 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_40_GPIO3_IO26,   0U);
  /* B12 核心板上的G 0亮*/
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_GPIO2_IO23,    0U);
  /* C7  核心板上的B 0亮*/
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_41_GPIO3_IO27,   0U);
  /* A13 母板上的D1 */
  //IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_GPIO2_IO25,    0U);
  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_04_GPIO1_IO04, 0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_06_GPIO2_IO22,    0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_40_GPIO3_IO26,   0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_07_GPIO2_IO23,    0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_41_GPIO3_IO27,   0x10B0u);  
  //IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_GPIO2_IO25,    0x10B0u);
  
  gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIO为输出方向
                                        0,                   //低电平
                                        kGPIO_NoIntmode      //非中断模式
                                        };

  GPIO_PinInit(GPIO1,04, &GPIO_Output_Config);
  GPIO_PinInit(GPIO2,22, &GPIO_Output_Config);
  GPIO_PinInit(GPIO3,26, &GPIO_Output_Config);
  GPIO_PinInit(GPIO2,23, &GPIO_Output_Config);
  GPIO_PinInit(GPIO3,27, &GPIO_Output_Config);
  //GPIO_PinInit(GPIO2,25, &GPIO_Output_Config);
  
  led.ops->off_a(); /* 上电关闭所有灯 */
}

static void led_on(led_name_t choose)
{
  switch (choose)
  {
  case LeftLight    : GPIO_PinWrite(GPIO2,22, 0U); break;
  case RightLight   : GPIO_PinWrite(GPIO1,04, 0U); break;
  case UpLight      : GPIO_PinWrite(GPIO3,26, 0U); break;
  case BackLight    : GPIO_PinWrite(GPIO2,23, 0U); break;
  case WarningLight : GPIO_PinWrite(GPIO2,22, 0U);
                      GPIO_PinWrite(GPIO1,04, 0U); break;
  }
}

static void led_off(led_name_t choose)
{
  switch (choose)
  {
  case LeftLight    : GPIO_PinWrite(GPIO2,22, 1U); break;
  case RightLight   : GPIO_PinWrite(GPIO1,04, 1U); break;
  case UpLight      : GPIO_PinWrite(GPIO3,26, 1U); break;
  case BackLight    : GPIO_PinWrite(GPIO2,23, 1U); break;
  }
}

static void led_reverse(led_name_t choose)
{
  switch (choose)
  {
  case LeftLight    : GPIO_PinReverse(GPIO2,22); break;
  case RightLight   : GPIO_PinReverse(GPIO1,04); break;
  case UpLight      : GPIO_PinReverse(GPIO3,26); break;
  case BackLight    : GPIO_PinReverse(GPIO2,23); break;
  case WarningLight : GPIO_PinReverse(GPIO2,22);
                      GPIO_PinReverse(GPIO1,04); break;
  }  
}


/* 调用10次闪一次 */
static uint8_t lednum_fast = 0; 
static void led_flash_fast(led_name_t color)
{
  static led_name_t last_light = UpLight; /* 记录上次闪灯的类型 */
  if (last_light != color) //本次要闪烁的灯和上次不同
  {
    led.ops->off_a();
    last_light = color;
  }
  
  lednum_fast++;
  if(lednum_fast == 10)
  {
    led.ops->reverse(color);
    lednum_fast = 0;
  }
}


/* 调用25次闪一次 */
static uint8_t lednum_slow = 0; 
static void led_flash_slow(led_name_t color)
{
  static led_name_t last_light = UpLight;
  if (last_light != color) //本次要闪烁的灯和上次不同
  {
    led.ops->off_a();
    last_light = color;
  }
  lednum_slow++;
  if(lednum_slow == 25)
  {
    led.ops->reverse(color);
    lednum_slow = 0;
  }
}


static void led_off_all(void)
{
  led.ops->off(LeftLight);
  led.ops->off(RightLight);
  led.ops->off(UpLight);
  led.ops->off(BackLight);
}



