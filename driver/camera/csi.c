#include "system.h"

/*******************************************************************************
*  SDK提供了两种在Noncacheable区定义缓冲区和变量的方法：
*  AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes)
*  AT_NONCACHEABLE_SECTION(var)
******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint16_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH], FRAME_BUFFER_ALIGN); //定义摄像头数据缓存区

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


/**
 *  摄像头初始化
 *  ----------------
 */
camera_config_t cameraConfig;   //摄像头配置结构体
// Configure camera device and receiver.
camera_config_t cameraConfig = {
    .pixelFormat                = kVIDEO_PixelFormatYUYV,
    .bytesPerPixel              = APP_BPP,                                  //每个像素点几个数据
    .resolution                 = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT), //分辨率
    .frameBufferLinePitch_Bytes = APP_CAMERA_WIDTH * APP_BPP,                   //行间隔
    .interface                  = kCAMERA_InterfaceGatedClock,                  //摄像机接口类型
    .controlFlags               = APP_CAMERA_CONTROL_FLAGS,
    .framePerSec                = 100,           //fps 修改需要修改曝光时间 和 分辨率 配合
};

 
void csi_init(void)  
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */
  /* CSI接口配置 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_CSI_PIXCLK,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_CSI_VSYNC, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_CSI_HSYNC, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_CSI_DATA09,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_CSI_DATA08,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_CSI_DATA07,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_CSI_DATA06,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_CSI_DATA05,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_CSI_DATA04,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_CSI_DATA03,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_CSI_DATA02,0U);
  
  /* I2C1引脚配置 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,1U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,1U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,0xD8B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,0xD8B0u);
  
  LPI2C1_Init(400000);
  
  /* CSI接口时钟配置 */
  CLOCK_SetMux(kCLOCK_CsiMux, 2);
  CLOCK_SetDiv(kCLOCK_CsiDiv, 2);

  /*
  * Configure the camera.
  */
  CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);  //初始化csi
  CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);                  //初始化相机配置
  CAMERA_DEVICE_Start(&cameraDevice);                                //启动相机
  
  /* Submit the empty frame buffers to buffer queue. */
  for (uint32_t i = 0; i < APP_CAMERA_FRAME_BUFFER_COUNT; i++) //将空帧缓冲区提交到缓冲区队列
  {
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(csiFrameBuf[i]));
  }
  
  CAMERA_RECEIVER_Start(&cameraReceiver);   // 启动接收camera数据
  delayms(200);        //延时200毫秒  摄像头不是重新上电 可以不要延时
}




