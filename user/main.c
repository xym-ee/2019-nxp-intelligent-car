/**
 *  main.c
 *  ----------------
 *  2019.南京理工大学.四轮组
 *
 */

#include "system.h"
/*
  一些重要的全局变量:
  dir_pd_t pd

*/

int main(void)
{
  /* ---------------------      硬件初始化         -------------------------- */
  system_init();                /* MCU初始化 */
                                /* 单个功能测试函数位置 */
  lpuart1_init(115200);         /* 蓝牙发送串口启动 */
  key_init();                   /* 按键启动 */
  led.init();                   /* 指示灯启动 */
  NVIC_SetPriorityGrouping(2);  /* 2: 4个抢占优先级 4个子优先级*/
  LCD_Init();                   /* LCD启动 */
  ExInt_Init();                 /* 中断启动 */
  pid_control_init();           /* 车速PID控制初始化.包含ENC,PWM,PID参数初始化 */
  csi_init();                   /* 相机接口初始化 */
  
  delayms(200);                 /* 必要的延时，等待相机感光元件稳定 */
  
  
  while(1)
  {
    /* 遥控中断给出调试标志位 */
    if(_status.debug_mode == 1)
      UI_debugsetting();
    
    /* 偏差获取 */
    refresh_midline();          //获取道路信息与确定偏差和曲率
    
    /* 速度控制 */
    //car_speed(speedvalue);      //目标车速计算与PID控制
    
    /* 方向控制 */
    //direction_ctrl();           //方向自适应PD控制
    
    /* 图像显示 */
    mt9v_oledshow();            //摄像头采集图像OLED显示
    mt9v_send_to_pc();
    
    /* 灯光指示 */
    //status_lignt();             //车上状态指示灯指示运行状况
  }
}
/*
    单个功能调试，函数内自带硬件初始化，都为死循环，复制到预留位置运行即可。
    adc_test();          
    Test_GPIO_ExInt();   //测试GPIO输入及外部中断              按键、中断检测功能    
    Test_ANO_DT();       //匿名上位机观察数据波形  不使用DMA模式，请将宏定义#define USE_DMA  0 
    encoder_test();
    servo_test();        //舵机测试
    test_motor();        //电机测试
    pit_test();          //测试PIT定时中断功能 
    mt9v_oled_test();      //MT9V034 OLED显示
    tsl1401_test();      //1401循迹测试
    fuzzy_test();
*/
