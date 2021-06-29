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

void ExInt_Init(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);          // IO口时钟使能
  
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_GPIO2_IO24,0U);    //光电避障
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_GPIO2_IO25,0U);    //A13
  //IOMUXC_SetPinMux(DISTANCE_INT_PINMUX,0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_GPIO2_IO24,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_GPIO2_IO25,0xF080);
  //IOMUXC_SetPinConfig(DISTANCE_INT_PINMUX,0xF080);

  /*GPIO 外部中断配置开始*/  
  gpio_pin_config_t GPIO_Input_Config =                     // GPIO初始化结构体
  {
    kGPIO_DigitalInput,                                     // GPIO为输入方向
    0,                                                      // 高电平
    kGPIO_IntRisingEdge,                                   // 下降沿触发中断
  };
 
  /* 终点磁铁触发中断 */
  GPIO_PinInit(GPIO2, 24, &GPIO_Input_Config);
  GPIO_PortEnableInterrupts(GPIO2,1<<24);
  
  /* 光电避障中断 */
  GPIO_PinInit(GPIO2, 25, &GPIO_Input_Config);
  GPIO_PortEnableInterrupts(GPIO2,1<<25);
  
  //优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
  NVIC_SetPriority(GPIO2_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
  EnableIRQ(GPIO2_Combined_16_31_IRQn);			          //使能GPIO2_16~31 IO的中断 
  
}

void GPIO2_Combined_16_31_IRQHandler(void)
{ 
  delayms(10);      //消抖
  
  /* clear the interrupt status */
  if(GPIO_GetPinsInterruptFlags(GPIO2)&(1<<24))    //磁铁触发中断
  {
    motor_speed.left = 0;
    motor_speed.right = 0;
    GPIO_PortClearInterruptFlags(GPIO2, 1U << 24);   //清除标志位
  }
  
  if(GPIO_GetPinsInterruptFlags(GPIO2)&(1<<25))    //遥控触发中断
  {
  
    /* 启动用户调试界面标志位 */
    status.debug_mode = 1;
    
    GPIO_PortClearInterruptFlags(GPIO2, 1U << 25);   //清除标志位
  }  
  
/* Change state of switch. */
  __DSB();				//数据同步屏蔽指令
}


