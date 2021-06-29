#include "system.h"

/* ---------------------------- 方法声明 ------------------------------------ */
static uint8_t key_read(uint8_t mode);
static void key_barrier_check(void);
static void key_init(void);


/* ---------------------------- 外部接口 ------------------------------------ */
const key_operations_t key_ops = {
    .get = key_read,
};


const key_device_t key = {
    .init = key_init,
    .barrier_check = key_barrier_check,
    .ops = &key_ops,
};



/* ---------------------------- 方法实现 ------------------------------------ */
static void key_init(void)
{  
  CLOCK_EnableClock(kCLOCK_Iomuxc);          // IO口时钟使能
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_GPIO2_IO27,   0U);   //-  
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_GPIO2_IO30,   0U);   //+ 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0U);   //OK
  
  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_GPIO2_IO27,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_GPIO2_IO30,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0xF080);
  
  gpio_pin_config_t GPIO_Input_Config = {kGPIO_DigitalInput,    //GPIO为输入方向
                               1,                    //高电平
                               kGPIO_NoIntmode,      //不触发中断
                               };
  
  GPIO_PinInit(GPIO2,27,&GPIO_Input_Config);        // GPIO输入口，非中断
  GPIO_PinInit(GPIO2,30,&GPIO_Input_Config);        // GPIO输入口，非中断
  GPIO_PinInit(GPIO3,04,&GPIO_Input_Config);        // GPIO输入口，非中断
  
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_GPIO2_IO24,   0U);  //A12 光电开关
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_GPIO2_IO24, DISTANCE_SWITCH_MUX); //弱上拉
  GPIO_PinInit(GPIO2,24,&GPIO_Input_Config);        // GPIO输入口，非中断
  
}

/**
 *  读按键
 *  ----------------
 *  mode                模式设置             1连续按    0单次按
 *  ----------------
 *  
 */
static uint8_t key_read(uint8_t mode)
{
  static uint8_t key_up = 1;
  
  if(mode == 1) 
    key_up = 1;
  if(key_up && ((GPIO_PinRead(GPIO2,27)==0 || GPIO_PinRead(GPIO2,30)==0) || GPIO_PinRead(GPIO3,04)==0))
  {
    delayms(10);   //消抖
    key_up = 0;
    if(GPIO_PinRead(GPIO2,27)==0)      
      return key_minus;
    if(GPIO_PinRead(GPIO2,30)==0) 
      return key_plus;
    if(GPIO_PinRead(GPIO3,04)==0) 
      return key_ok;
  }
  if(GPIO_PinRead(GPIO2,27)==1 && GPIO_PinRead(GPIO2,30)==1 && GPIO_PinRead(GPIO3,4)==1) 
    key_up = 1;
  return no_key;   //无按键按下
} 


static void key_barrier_check(void)
{
  if ( BARRIER_CHECK && (status.barrier == 0) ) /* 检测到路障 */
  {
    // if ( 并且摄像头看到黑色 )
    status.barrier = 1;
    /* 距离计数清零 */
    ENC_DoSoftwareLoadInitialPositionValue(ENC1);
    ENC_DoSoftwareLoadInitialPositionValue(ENC2);
  }
}

