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

/*!
 * @brief 系统初始化
 * -------------------------------
 *    初始化内存保护单元、SDRAM、时钟与延时功能
 */
void system_init(void)
{
  BOARD_ConfigMPU();                   /* 初始化内存保护单元 */      
  BOARD_InitSDRAMPins();               /* SDRAM初始化 */
  BOARD_BootClockRUN();                /* 初始化开发板时钟 */   
  gpt_init();                          //GPT延时和计时功能初始化
}

void print_clock(void)
{
  printf("\r\n");
  printf("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
  printf("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
  printf("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
  printf("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
  printf("Video:           %d Hz\r\n", CLOCK_GetFreq(kCLOCK_VideoPllClk));
  printf("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
  printf("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
  printf("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
  printf("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
  printf("USB1PLL:         %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
  printf("USB1PLLPFD0:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
  printf("USB1PLLPFD1:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
  printf("USB1PLLPFD2:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
  printf("USB1PLLPFD3:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));	
}










