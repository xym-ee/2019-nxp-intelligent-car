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

/*******************************************************************************
*  SDK提供了两种在Noncacheable区定义缓冲区和变量的方法：
*  AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes)
*  AT_NONCACHEABLE_SECTION(var)
******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint16_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][IMG_BUF_HIGH][IMG_BUF_WIDTH], FRAME_BUFFER_ALIGN); //定义摄像头数据缓存区

static mt9v_resource_t mt9vresource = {   //摄像头初始化结构体
    .sccbI2C = LPI2C1,
    .inputClockFreq_Hz = 27000000,
};

camera_device_handle_t cameraDevice = {           //摄像头驱动配置结构体
    .resource = &mt9vresource,
    .ops = &mt9v_ops,               //保存了初始化函数的指针
};

/* connect to CSI. */
static csi_resource_t csiResource = {   //指向csi基地址
    .csiBase = CSI,
};

static csi_private_data_t csiPrivateData;  //csi私有数据

camera_receiver_handle_t cameraReceiver = {//摄像头接收配置结构体
    .resource = &csiResource,
    .ops = &csi_ops, 
    .privateData = &csiPrivateData,
};


extern void CSI_DriverIRQHandler(void);

void CSI_IRQHandler(void)
{
  CSI_DriverIRQHandler();

}


/* 摄像头初始化 */
//camera_config_t cameraConfig;   //摄像头配置结构体
camera_config_t cameraConfig = {
    .pixelFormat                = kVIDEO_PixelFormatYUYV,
    .bytesPerPixel              = PIX_PACK,                          /* 每个字几个像素点 */
    .resolution                 = FSL_VIDEO_RESOLUTION(IMG_BUF_WIDTH, IMG_BUF_HIGH), /* 分辨率 */
    .frameBufferLinePitch_Bytes = IMG_BUF_WIDTH * PIX_PACK,          /* 行间隔 */
    .interface                  = kCAMERA_InterfaceGatedClock,      /* 摄像机接口类型 */      
    .controlFlags               = APP_CAMERA_CONTROL_FLAGS,
    .framePerSec                = 100,                              /* FPS */
};

 
void csi_init(void)  
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */
  /* CSI接口配置 */
  IOMUXC_SetPinMux(CSI_PIXCLK_PINMUX,0U); /* L12 */
  IOMUXC_SetPinMux(CSI_VSYNC_PINMUX, 0U); /* J12 */
  IOMUXC_SetPinMux(CSI_HSYNC_PINMUX, 0U); /* K10 */
  IOMUXC_SetPinMux(CSI_DATA9_PINMUX, 0U); /* H13 */
  IOMUXC_SetPinMux(CSI_DATA8_PINMUX, 0U); /* M13 */
  IOMUXC_SetPinMux(CSI_DATA7_PINMUX, 0U); /* L13 */
  IOMUXC_SetPinMux(CSI_DATA6_PINMUX, 0U); /* J11 */
  IOMUXC_SetPinMux(CSI_DATA5_PINMUX, 0U); /* H12 */
  IOMUXC_SetPinMux(CSI_DATA4_PINMUX, 0U); /* H11 */
  IOMUXC_SetPinMux(CSI_DATA3_PINMUX, 0U); /* G12 */
  IOMUXC_SetPinMux(CSI_DATA2_PINMUX, 0U); /* J14 */
  
  /* I2C1引脚配置 */
  IOMUXC_SetPinMux(IIC1_SCL_PINMUX,1U); /* J11 */
  IOMUXC_SetPinMux(IIC1_SCL_PINMUX,1U); /* K11 */
  IOMUXC_SetPinConfig(IIC1_SCL_PINMUX,0xD8B0u);
  IOMUXC_SetPinConfig(IIC1_SCL_PINMUX,0xD8B0u);
  
  LPI2C1_Init(400000);
  
  /* CSI接口时钟配置 */
  CLOCK_SetMux(kCLOCK_CsiMux, 2);
  CLOCK_SetDiv(kCLOCK_CsiDiv, 2);

  /* Configure the camera. */
  CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);  //初始化csi
  CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);                  //初始化相机配置
  CAMERA_DEVICE_Start(&cameraDevice);                                //启动相机
  
  /* Submit the empty frame buffers to buffer queue. */
  for (uint32_t i = 0; i < APP_CAMERA_FRAME_BUFFER_COUNT; i++) //将空帧缓冲区提交到缓冲区队列
  {
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(csiFrameBuf[i]));
  }
  
  CAMERA_RECEIVER_Start(&cameraReceiver);   // 启动接收camera数据
}




