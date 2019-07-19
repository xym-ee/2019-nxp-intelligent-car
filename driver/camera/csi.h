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

#ifndef _CSI_H
#define _CSI_H

/*-------------------            csi硬件接口       ---------------------------*/

/*----------- 自己PCB的接口定义 ---------*/
#ifdef MY_PCB
#define CSI_PIXCLK_PINMUX   IOMUXC_GPIO_AD_B1_04_CSI_PIXCLK
#define CSI_VSYNC_PINMUX    IOMUXC_GPIO_AD_B1_06_CSI_VSYNC
#define CSI_HSYNC_PINMUX    IOMUXC_GPIO_AD_B1_07_CSI_HSYNC
#define CSI_DATA9_PINMUX    IOMUXC_GPIO_AD_B1_08_CSI_DATA09
#define CSI_DATA8_PINMUX    IOMUXC_GPIO_AD_B1_09_CSI_DATA08
#define CSI_DATA7_PINMUX    IOMUXC_GPIO_AD_B1_10_CSI_DATA07
#define CSI_DATA6_PINMUX    IOMUXC_GPIO_AD_B1_11_CSI_DATA06
#define CSI_DATA5_PINMUX    IOMUXC_GPIO_AD_B1_12_CSI_DATA05
#define CSI_DATA4_PINMUX    IOMUXC_GPIO_AD_B1_13_CSI_DATA04
#define CSI_DATA3_PINMUX    IOMUXC_GPIO_AD_B1_14_CSI_DATA03
#define CSI_DATA2_PINMUX    IOMUXC_GPIO_AD_B1_15_CSI_DATA02

/*----------- 龙邱母版接口定义 ---------*/
#else
#define CSI_PIXCLK_PINMUX   IOMUXC_GPIO_AD_B1_04_CSI_PIXCLK//L12
#define CSI_VSYNC_PINMUX    IOMUXC_GPIO_AD_B1_06_CSI_VSYNC//J12
#define CSI_HSYNC_PINMUX    IOMUXC_GPIO_AD_B1_07_CSI_HSYNC//K10
#define CSI_DATA9_PINMUX    IOMUXC_GPIO_AD_B1_08_CSI_DATA09//H13
#define CSI_DATA8_PINMUX    IOMUXC_GPIO_AD_B1_09_CSI_DATA08//M13
#define CSI_DATA7_PINMUX    IOMUXC_GPIO_AD_B1_10_CSI_DATA07//L13
#define CSI_DATA6_PINMUX    IOMUXC_GPIO_AD_B1_11_CSI_DATA06//J13
#define CSI_DATA5_PINMUX    IOMUXC_GPIO_AD_B1_12_CSI_DATA05//H12
#define CSI_DATA4_PINMUX    IOMUXC_GPIO_AD_B1_13_CSI_DATA04//H11
#define CSI_DATA3_PINMUX    IOMUXC_GPIO_AD_B1_14_CSI_DATA03//G12
#define CSI_DATA2_PINMUX    IOMUXC_GPIO_AD_B1_15_CSI_DATA02//J14

#endif



/*---------------------------------------- 其他位置勿动 ----------------------*/


/* 图像缓冲区尺寸 */
#define  PIX_PACK               2                   /* 2个像素组合为一个uint16 */
#define  IMG_BUF_WIDTH          (IMG_CAM_WIDTH)
#define  IMG_BUF_HIGH           (IMG_CAM_HIGH/2)    //一个uint16_t里面装了两个像素， 一行IMAGEW 里面其实装了两行的像素点，所以高度/2

/* 使用的图片尺寸 */
#define  IMG_HIGH               120               /* 使用的图片高度 */
#define  IMG_WIDTH              188                /* 使用的图片宽度 */

#define  APP_CAMERA_FRAME_BUFFER_COUNT          4 
#define  FRAME_BUFFER_ALIGN                     64  //对齐 


#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge) //使用摄像头的上升沿
void csi_init(void);
extern camera_device_handle_t cameraDevice;    
extern camera_receiver_handle_t cameraReceiver;
//extern uint16_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH];

extern camera_config_t cameraConfig;   //摄像头配置结构体


#endif


