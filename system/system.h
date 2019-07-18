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

#ifndef _SYSTEM_H
#define _SYSTEM_H

    #include "stdio.h"
    #include "math.h"


    #include "pad_config.h"
    #include "fsl_adc.h"
    #include "fsl_cache.h"
    #include "fsl_csi.h"
    #include "fsl_csi_camera_adapter.h"
    #include "fsl_camera_device.h"
    #include "fsl_common.h"
    #include "fsl_clock.h"
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
 

    #include "status.h"   
    #include "iic.h"
    #include "mt9v034.h"  
    #include "tsl1401.h"
    #include "csi.h"
    #include "img_calculate.h"
    #include "getimage.h"
    #include "oled.h"
    #include "led.h"
    #include "key.h"
    #include "pwm.h"
    #include "uart.h"
    #include "adc.h"
    #include "encoder.h"
    //#include "posturesensor.h"
    #include "systick.h"
    #include "ANO_DT.h"
    #include "nrf2401.h"
    #include "spi.h"
    #include "gpt.h"
    #include "motor_pid.h"
    #include "direction.h"
    #include "pit.h"
    #include "test.h"
    #include "extern_interrupt.h"

    #include "speed.h"


    #include "userinterface.h"
    

    #include "CodeStyle.h"

void system_init(void);

 
#endif
