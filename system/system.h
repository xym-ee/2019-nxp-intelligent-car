#ifndef _SYSTEM_H
#define _SYSTEM_H

    #include "stdio.h"
    #include "math.h"
//SDK底层驱动库
    #include "fsl_adc.h"
    #include "fsl_cache.h"
    #include "fsl_csi.h"
    #include "fsl_csi_camera_adapter.h"
    #include "fsl_camera.h"
    #include "fsl_camera_receiver.h"
    #include "fsl_camera_device.h"
    #include "fsl_csi.h"
    #include "fsl_common.h"
    #include "fsl_clock.h"
    #include "fsl_debug_console.h"
    #include "fsl_elcdif.h"
    #include "fsl_enc.h"    
    #include "fsl_gpio.h"
    #include "fsl_gpt.h"
    #include "fsl_iomuxc.h"
    #include "fsl_lpuart.h"
    #include "fsl_lpi2c.h"
    //#include "fsl_pxp.h"
    #include "fsl_pwm.h"
    #include "fsl_pit.h"
    //#include "fsl_qtmr.h"
    //#include "fsl_trng.h"
    //#include "fsl_usdhc.h"
    #include "fsl_video_common.h"
    #include "fsl_wdog.h"
    #include "fsl_xbara.h"
    #include "board.h"
    #include "fsl_flexspi.h"
    #include "fsl_lpuart_edma.h"
    #include "fsl_edma.h"
    #include "fsl_dmamux.h"

//用户自定义文件    
    #include "iic.h"
    #include "mt9v034.h"   //神眼摄像头V3   
    #include "tsl1401.h"
    #include "csi.h"       //摄像头接口
    #include "getimage.h" //TFT1.8寸彩屏测试
    #include "oled.h"      //OLED模块函数定义，包括IIC和SPI两种接口
    #include "led.h"       //LED指示
    #include "key.h"       //独立按键
    #include "pwm.h"       //电机，舵机PWM控制 XBARA
    #include "uart.h"       //UART串口
    #include "adc.h"       //ADC转换采集
    #include "encoder.h"   //编码器正交解码数据采集
    #include "systick.h"    //systick 定时器，用于计时
    #include "ANO_DT.h"       //匿名上位机协议
    #include "status.h"       //标志位
    #include "nrf2401.h"   //NRF2401驱动程序
    #include "spi.h"
    #include "gpt.h"
    #include "motor_pid.h"
    #include "fuzzy.h"
    #include "directioncontrol.h"
    #include "pit.h"       //PIT定时
    #include "test.h"

  
#endif
