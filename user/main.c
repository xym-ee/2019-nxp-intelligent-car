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
  system_init();            /* 系统启动 */
  lpuart1_init(115200);     /* 串口启动 */
  key_init();
  LED_Init();                          //初始化核心板和开发板上的LED接口
  NVIC_SetPriorityGrouping(2);          /*2: 4个抢占优先级 4个子优先级*/

  
//    adc_test();          
//    Test_GPIO_ExInt();   //测试GPIO输入及外部中断              按键、中断检测功能    
//    Test_ANO_DT();       //匿名上位机观察数据波形  不使用DMA模式，请将宏定义#define USE_DMA  0 
      encoder_test();
//    servo_test();        //舵机测试
//    test_motor();        //电机测试
//    pit_test();          //测试PIT定时中断功能 
//    mt9v_oled_test();      //MT9V034 OLED显示
//    tsl1401_test();      //1401循迹测试
//    fuzzy_test();
    while(1)
    {

    }
}
