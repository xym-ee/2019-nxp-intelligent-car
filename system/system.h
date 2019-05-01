#ifndef _SYSTEM_H
#define _SYSTEM_H

    #include "stdio.h"
    #include "math.h"

    #include "fsl_adc.h"
    #include "fsl_cache.h"
    #include "fsl_csi.h"
    #include "fsl_csi_camera_adapter.h"
    #include "fsl_camera_device.h"
    #include "fsl_common.h"
    #include "fsl_clock.h"
    //#include "fsl_debug_console.h"
    #include "fsl_elcdif.h"
    #include "fsl_enc.h"    
    #include "fsl_gpio.h"
    #include "fsl_gpt.h"
    #include "fsl_iomuxc.h"
    #include "fsl_lpuart.h"
    #include "fsl_lpi2c.h"
    #include "fsl_pwm.h"
    #include "fsl_pit.h"
    #include "fsl_wdog.h"
    #include "fsl_xbara.h"
    #include "board.h"
    #include "fsl_flexspi.h"
    #include "fsl_lpuart_edma.h"
    #include "fsl_edma.h"
    #include "fsl_dmamux.h"

 
    #include "iic.h"
    #include "mt9v034.h"  
    #include "tsl1401.h"
    #include "csi.h"
    #include "getimage.h"
    #include "oled.h"
    #include "led.h"
    #include "key.h"
    #include "pwm.h"
    #include "uart.h"
    #include "adc.h"
    #include "encoder.h"
    #include "systick.h"
    #include "ANO_DT.h"
    #include "status.h"
    #include "nrf2401.h"
    #include "spi.h"
    #include "gpt.h"
    #include "motor_pid.h"
    #include "fuzzy.h"
    #include "directioncontrol.h"
    #include "pit.h"
    #include "test.h"



void system_init(void);
  
#endif
