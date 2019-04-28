#include "status.h"


_status_t _status = 
{
    .stop                = 0,  //停车标志位
    .low_power_flag      = 0,  //低电压标志位
    .straight_speed      = 1,  //直道标志位
    .brake_speed         = 0,  //入弯标志位
    .curve_speed         = 0,  //弯道标志位
    .camera_run          = 1,  //使用摄像头 
    .electromagnetism    = 1,  //使用电磁
    .ins_calibration     = 1,  //陀螺仪校准
    
    /*使用DMA发送标志位*/
    .txOnGoing           = 0,  //串口TX正在发送标志位
    .rxOnGoing           = 0,  //串口RX正在接收标志位
    .txBufferFull        = 0,  //串口TX发送寄存器满标志位
    .rxBufferEmpty       = 0,  //串口RX接收寄存器空标志位
    
    .get_pid_group1      = 0,  //上位机请求，为1时，单片机需要发送pid参数给上位机
    
    
    
};


