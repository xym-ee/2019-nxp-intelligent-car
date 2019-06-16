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










