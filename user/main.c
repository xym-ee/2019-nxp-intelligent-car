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

int main(void)
{
  /* ---------------------      硬件初始化         -------------------------- */
  system_init();                /* MCU初始化 */
  //pit_test();
  //servo_test();
  //adc.circle_test();
  //adc.test();
  //img.roadtype_test();   /* 单个功能测试函数位置 */
  lpuart1_init(115200);         /* 蓝牙发送串口启动 */
  key.init();                   /* 按键启动 */
  led.init();                   /* 指示灯启动 */
  NVIC_SetPriorityGrouping(2);  /* 2: 4个抢占优先级 4个子优先级*/
  oled.init();                   /* LCD启动 */
  //ExInt_Init();                 /* 中断启动 */
  adc.init();
  motor.init();         /* 车速PID控制初始化.包含ENC,PWM,PID参数初始化 */       
  img.init();                   /* 相机接口初始化 */
  delayms(200);                 /* 必要的延时，等待相机感光元件稳定 */
  UI_debugsetting();
  pit_init(kPIT_Chnl_0, 10000);
  
  while(1)
  {
	  /* 等待10ms中断，等待时检查调试中断信号，实时刷新电磁信息  */
	  while (status.interrupt_10ms == 0)
	  {
      adc.refresh();    /* 更新赛道电磁引导线信息 */
      adc.circle_check(); /* 圆环判断 */

      
		  /* 遥控中断给出调试标志位 */
//		  if(status.debug_mode == 1)
//			  UI_debugsetting();
	  }

	  /* 如果图像就绪，图像刷新，道路类型判断 */
	  if(kStatus_Success == CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &CameraBufferAddr))
    {
		  img.refresh();            /* 更新图像和偏差等控制信息 */
      
      car.direction_control();  /* 舵机打角更新 */
      
      car_speed_calculate();      /* 更新一次左右电机目标速度 */
      
      status_light.roadtype();  /* 状态灯指示更新 */
    }
    
    /* 两个电机转速控制 */
    motor.pidcontrol(&motor_speed);
    
    /* 中断复位 */
    status.interrupt_10ms = 0;
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
    img.roadtype_test();  //道路类型判断转向灯显示测试,按ok键发送到上位机显示
    motor.pidtest();       //电机闭环测试，matlab plot画数据
    
*/
