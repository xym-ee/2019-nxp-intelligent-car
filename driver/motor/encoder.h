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

#ifndef _ENCODER_H
#define _ENCODER_H

/*-------------------            编码器硬件接口       -------------------*/

/*-------------- 龙邱母版引脚 --------------*/
//龙邱母版的引脚分配
#define ENC_LA_MUX      IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05
#define ENC_LB_MUX      IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07
#define ENC_RA_MUX      IOMUXC_GPIO_AD_B0_10_XBAR1_IN22
#define ENC_RB_MUX      IOMUXC_GPIO_AD_B0_09_XBAR1_IN21


#define ENC_LA_CHANNEL  kXBARA1_InputIomuxXbarInout05
#define ENC_LB_CHANNEL  kXBARA1_InputIomuxXbarInout07
#define ENC_RA_CHANNEL  kXBARA1_InputIomuxXbarIn22
#define ENC_RB_CHANNEL  kXBARA1_InputIomuxXbarIn21

/* ----------自己PCB分配--------- */
//#define ENC_LA_MUX  IOMUXC_GPIO_AD_B0_11_XBAR1_IN23
//#define ENC_LB_MUX  IOMUXC_GPIO_B1_03_XBAR1_INOUT17
//#define ENC_RA_MUX  IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08
//#define ENC_RB_MUX  IOMUXC_GPIO_SD_B0_05_XBAR1_INOUT09
//
//#define ENC_LA_CHANNEL  kXBARA1_InputIomuxXbarIn23
//#define ENC_LB_CHANNEL  kXBARA1_InputIomuxXbarInout17
//#define ENC_RA_CHANNEL  kXBARA1_InputIomuxXbarInout09
//#define ENC_RB_CHANNEL  kXBARA1_InputIomuxXbarInout08
//如果计数异常 更换AB相

/*---------------- 其他位置不要改！！！！！ ----------------------*/

/**
 *  初始化ENC1、ENC3
 *  ----------------
 *  使用ENC_GetPositionDifferenceValue(ENC1)函数读取计数值
 *  
 */
void enc_init(void);
void encoder_test(void);
void encoder_position_test(void);
#endif 
