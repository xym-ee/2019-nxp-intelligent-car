#ifndef _CSI_H
#define _CSI_H

#define  APP_CAMERA_WIDTH       (IMAGEW)
#define  APP_CAMERA_HEIGHT      (IMAGEH/2)  //一个uint16_t里面装了两个像素， 一行IMAGEW 里面其实装了两行的像素点，所以高度/2
#define  APP_BPP                2 //像素格式，
#define  IMG_HIGH               60    //使用图像高度
#define  IMG_WIDTH               94   //使用图像宽度


#define  APP_CAMERA_FRAME_BUFFER_COUNT          4 
#define  FRAME_BUFFER_ALIGN                     64  //对齐 


#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge) //使用摄像头的上升沿
void csi_init(void);
extern camera_device_handle_t cameraDevice;    
extern camera_receiver_handle_t cameraReceiver;
//extern uint16_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH];

extern camera_config_t cameraConfig;   //摄像头配置结构体


#endif


