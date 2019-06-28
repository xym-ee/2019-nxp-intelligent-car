#ifndef _CSI_H
#define _CSI_H

/*-------------------            csi硬件接口       ---------------------------*/

/*----------- 龙邱母版接口定义 ---------*/
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

/*----------- 自己PCB的接口定义 ---------*/
//#define CSI_PIXCLK_PINMUX   IOMUXC_GPIO_B1_12_CSI_PIXCLK
//#define CSI_VSYNC_PINMUX    IOMUXC_GPIO_B1_13_CSI_VSYNC
//#define CSI_HSYNC_PINMUX    IOMUXC_GPIO_B1_14_CSI_HSYNC
//#define CSI_DATA9_PINMUX    IOMUXC_GPIO_AD_B1_08_CSI_DATA09
//#define CSI_DATA8_PINMUX    IOMUXC_GPIO_AD_B0_05_CSI_DATA08
//#define CSI_DATA7_PINMUX    IOMUXC_GPIO_AD_B1_10_CSI_DATA07
//#define CSI_DATA6_PINMUX    IOMUXC_GPIO_AD_B1_11_CSI_DATA06
//#define CSI_DATA5_PINMUX    IOMUXC_GPIO_AD_B0_08_CSI_DATA05
//#define CSI_DATA4_PINMUX    IOMUXC_GPIO_AD_B0_09_CSI_DATA04
//#define CSI_DATA3_PINMUX    IOMUXC_GPIO_AD_B0_10_CSI_DATA03
//#define CSI_DATA2_PINMUX    IOMUXC_GPIO_AD_B1_15_CSI_DATA02



/*---------------------------------------- 其他位置勿动 ----------------------*/


#define  APP_CAMERA_WIDTH       (IMAGEW)
#define  APP_CAMERA_HEIGHT      (IMAGEH/2)  //一个uint16_t里面装了两个像素， 一行IMAGEW 里面其实装了两行的像素点，所以高度/2
#define  APP_BPP                2 //像素格式，
#define  IMG_HIGH               60    //从缓冲区取出的图片高度
#define  IMG_WIDTH              94    //从缓冲区取出的图片宽度
#define  Use_HIGH               60    //使用图像高度
#define  Use_WIDTH               94   //使用图像宽度

#define  APP_CAMERA_FRAME_BUFFER_COUNT          4 
#define  FRAME_BUFFER_ALIGN                     64  //对齐 


#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge) //使用摄像头的上升沿
void csi_init(void);
extern camera_device_handle_t cameraDevice;    
extern camera_receiver_handle_t cameraReceiver;
//extern uint16_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH];

extern camera_config_t cameraConfig;   //摄像头配置结构体


#endif


