/**
 *  main.c
 *  ----------------
 *  主函数
 *
 *
 */
#include "system.h"


int main(void)
{
  BOARD_ConfigMPU();                   /* 初始化内存保护单元 */      
  BOARD_InitSDRAMPins();               /* SDRAM初始化 */
  BOARD_BootClockRUN();                /* 初始化开发板时钟 */   


  gpt_init();                          //GPT延时和计时功能初始化
  LED_Init();                          //初始化核心板和开发板上的LED接口
  //LPUART1_Init(115200);       //串口1初始化 可以使用 printf函数
  _systime.init();                     //开启systick定时器
  NVIC_SetPriorityGrouping(2);  /*2: 4个抢占优先级 4个子优先级*/

  LCD_Init();               //LCD初始化 
  LCD_CLS();                //LCD清屏 
  LCD_Show_Frame94();
  csi_init();
  delayms(200);        //延时200毫秒 
  
  pit_init(kPIT_Chnl_0, 10000);
  
  
  while(1)
  {
    LED_Color_Reverse(red);
    delayms(300);
   }

  /*    uint64_t now = _systime.get_time_us();         //计时功能  得到当前时间   
  *    代码
  *    uint64_t time = _systime.get_time_us() - now;  //得到时差
  *    printf("time is %llu \r\n", time);
  */    

  /*
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
  */

  
  
//    adc_test();          
//    Test_GPIO_ExInt();   //测试GPIO输入及外部中断              按键、中断检测功能    
  
//    Test_GPT_Timer();    //测试GPT定时器延时和计时功能
//    Test_ANO_DT();       //匿名上位机观察数据波形  不使用DMA模式，请将宏定义#define USE_DMA  0  
//    Test_PWM_nCH();      //测试PWM                             PWM1， 产生PWM波  用示波器观察
//    Test_QTMR_PWM();     //测试QTMR 产生的PWM                  示波器观察
//    Test_QTMR_QD();      //测试QTMR正交解码功能
 //   servo_test();        //舵机测试
 //   Test_Motor();        //电机测试
//     pit_test();          //测试PIT定时中断功能 
//    encoder_test();
//    mt9v_oled_test();      //MT9V034 OLED显示
//    tsl1401_test();             //1401循迹测试
//     pid_control_init();
//    fuzzy_test();
    
    
}
