// Header:
// File Name: 简单任务调度器，移植于APM
// Author:Z
// Date:2018/08/03

#include "Scheduler.h"
#include "status.h"
void battery_update(void)   //电压检测
{
    static float Voltage = 7.8;
    static uint8_t count;
    //Voltage = Voltage * 0.9 + 0.1 * ReadADC(ADC1,ADC1_CH10)*4.6;
    if(Voltage < 7200) //电池电压低于7.2V
    {
        count++;
    }
    if(count > 50)
    {
        count = 0;
        _status.low_power_flag = 1;        //低电压标志位置1
    }
}

//void ins_update(void)                   //更新姿态
//{
//    _AHRS.update();
//
//}
void notify_update(void)    //更新灯语，用于表示各种状态
{
    static uint16_t count;
    count++;
    if(_status.low_power_flag)   //低电压
    {
       if(count % 20 == 1)                     //闪灯报警
       {
            LED_Color_Reverse(red);         //红灯闪烁  感兴趣的可以使用红灯，蓝灯，紫灯等
       }
    }
//    else if()

}

void electromagnetic_update(void)  //更新电磁传感器
{
//    static int ADC_left,ADC_right,ADC_centre;
//    if(_status.electromagnetism)  //使用电磁传感器
//    {
//        ADC_left = ADC_left*0.9 + 0.1*ReadADC(ADC1,ADC1_CH4)*0.806;
//        ADC_right = ADC_left*0.9 + 0.1*ReadADC(ADC1,ADC1_CH2)*0.806;
//        ADC_centre = ADC_left*0.9 + 0.1*ReadADC(ADC1,ADC1_CH3)*0.806;
//    }
}


extern void Draw_Road(void);
void display_update(void)   //更新显示
{
    /*大概3ms*/
    Draw_Road();
}


short velocity_upper_left, velocity_upper_right, velocity_lower_left, velocity_lower_right;  //编码器速度
void report_update(void)    //上报数据给上位机
{
    if(_status.get_pid_group1)   //收到上位机发送的 读取飞控 指令   单片机发送pid参数给上位机
    {
        _status.get_pid_group1 = 0;
        ANO_DT_Send_PID(1, Motor_pid._kp, Motor_pid._ki, Motor_pid._kd, Servo_pid._kp, Servo_pid._ki, Servo_pid._kd, 0,0,0);
    }
    else 
    {
        ANO_DT_send_int16((short)(Motor_pid._pid_out_p),   //上报匿名上位机 PID数据
                          (short)(Motor_pid._pid_out_i), 
                          (short)(Motor_pid._pid_out_d), 
                          (short)(Motor_pid._pid_out), 
                          (short)(Motor_pid._integrator ), 
                          (short)(Motor_pid._last_error ), 
                          CURVE_SPEED, 
                          (short)velocity_lower_left);
    }
    
}



void camera_update(void)
{
    uint32_t fullCameraBufferAddr; //存放CSI满缓冲区地址
    counter ++;
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //等待摄像头CSI缓存区满
    {
    }
            // Convert the camera input picture to RGB format. 
    psBufferConfig.bufferAddr = fullCameraBufferAddr;    //设置PXP转换源地址
    PXP_SetProcessSurfaceBufferConfig(PXP, &psBufferConfig); // 对PXP输入进行配置
    
    if(counter % 2)
    {
        outputBufferConfig.buffer0Addr = (uint32_t)lcdFrameBuf[1]; //设置PXP转换的目的地址
    }
    else
    {
        outputBufferConfig.buffer0Addr = (uint32_t)lcdFrameBuf[0]; //设置PXP转换的目的地址
    }
    PXP_SetOutputBufferConfig(PXP, &outputBufferConfig);   // 对PXP输出进行配置
    
    PXP_Start(PXP);    //开启PXP转换
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列
}



void pxp_update(void)
{
    /*PXP图像处理   大概3ms  这里可以添加用户代码  执行完后再检查PXP转换完成*/
    while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(PXP)))  //等待pXP转换完成
    {
    }
    PXP_ClearStatusFlags(PXP, kPXP_CompleteFlag);   //清除标志位  

}



extern uint8_t Pixle [Use_ROWS][Use_Line];
extern void Camera_0_1_Handle(void);
extern void Pixle_Filter(void);
void camera_data_dispose(void)
{
    Camera_0_1_Handle();   //二值化
    Pixle_Filter();        //滤波
    LQ_Line_Hunting(Pixle);//寻找边线
    seek_road();           //得到偏差
}



void main_loop(void)
{
//    LQ_Control_Servo();                 //控制舵机
    velocity_lower_left = (int16_t)ENC_GetPositionDifferenceValue(ENC2);  //得到编码器微分值
    LQ_Control_Motor(velocity_lower_left);          //控制电机
}




sched_task_t sched_task[] =
{
//    { update_GPS,            2,     900        ,0},
//    { update_navigation,     10,    500 },
//    { medium_loop,           2,     700 },
//    函数指针     每隔多少周期执行一次  该函数运行一次的大概时间  上一次函数运行时tick
//      { baro_update,          10,                 20,               0   }, //更新高度
//	  { radio_update,          3,                200,               0   }, //更新遥控数据
    
//      { camera_update  ,       4,                850,               0   }, //更新摄像头数据
      { main_loop    ,         1,                 50,               0   }, //更新PWM
	  { notify_update,        10,                 50,               0   }, //更新led
	  { battery_update,       10,                100,               0   }, //更新电池电压

	  { report_update,        5,                500,               0   }, //上报数据给上位机
//      { pxp_update,            4,               1500,               0   }, //摄像头数据PXP处理
//      { camera_data_dispose,   4,               2000,               0   }, //摄像头数据处理
//      { display_update,       10,               4000,               0   }, //更新显示
      
      
      
//	  { motors_status_update,  5,                 10,               0   }, //更新电机状态
//    { fifty_hz_loop,         2,     950 },
//    { run_nav_updates,      10,     800 },
//    { slow_loop,            10,     500 },
//    { gcs_check_input,	   2,     700 },
//    { gcs_send_heartbeat,  100,     700 },
//    { gcs_data_stream_send,  2,    1500 },
//    { gcs_send_deferred,     2,    1200 },
//    { compass_accumulate,    2,     700 },
//    { barometer_accumulate,  2,     900 },
//    { super_slow_loop,     100,    1100 },
//    { perf_update,        1000,     500 }


};

static uint8_t num_task;  //任务数量
void shceduler_init()
{
	num_task = sizeof (sched_task) / sizeof (sched_task[0]);
}

static uint32_t tick_counter = 0;  //shceduler_run执行次数
void shceduler_tick()
{
	tick_counter++;
}

/*
  run one tick
  this will run as many scheduler tasks as we can in the specified time
 */
//#define UART_DEBUG

void shceduler_run(uint32_t time_available)
{
    uint64_t now;
    uint16_t dt;
    uint16_t interval_ticks;
    
	for (uint8_t i = 0; i < num_task; i++)
	{
		now = _systime.get_time_us();//获取当前时间
		dt = tick_counter - sched_task[i].last_tick_counter;//计算任务上次运行到现在相隔多少周期
        interval_ticks = sched_task[i].interval_ticks;  //任务相隔多少周期运行一次

		if( dt >= interval_ticks)
		{
			//调试时使用，任务长时间未运行，说明任务的运行周期太长或者shceduler_run的可运行时间太短
			if (dt >= interval_ticks*2)
			{
                // we've slipped a whole run of this task!
				#ifdef UART_DEBUG
				printf("Scheduler slip task[%u] (%u/%u/%u)\n",
									  (unsigned)i,
									  (unsigned)dt,
									  (unsigned)interval_ticks,
									  (unsigned)sched_task[i].max_time_micros);
				#endif //UART_DEBUG

            }
			if (sched_task[i].max_time_micros <= time_available)
			{
				//运行任务
				sched_task[i].task_func();
				//更新last_tick_counter
				sched_task[i].last_tick_counter = tick_counter;

				#ifdef UART_DEBUG
				uint32_t end_time = _systime.get_time_us() - now;
//如果任务运行过后发现时间超出shceduler_run的可运行时间time_available，说明任务的max_time_micros设置小了
				if(end_time > time_available)
				{
					printf("Scheduler slip task[%u] (%u/%u)\n",
                                          (unsigned)i,
                                          (unsigned)end_time,
                                          (unsigned)sched_task[i].max_time_micros);
					return;

				}
				#endif //UART_DEBUG

			}

		}
		//更新time_available
		time_available -= (_systime.get_time_us() - now);
	}
	//更新tick_counter
	_sched.tick();  // 该函数作用为tick_counter++
}


sched_t _sched =
{
	shceduler_init,
	shceduler_tick,
	shceduler_run,
};
