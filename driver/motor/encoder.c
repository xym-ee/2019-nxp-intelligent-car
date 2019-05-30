/**
 *  encoder.c
 *  ----------------
 *  enc正交解码模块
 *  
 *  使用ENC1,ENC3
 *  
 */
#include "system.h"	

static void enc_pinconfig(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);  /* 打开io时钟 */ 
#ifdef LQ_PIN
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,1U);         /* G13 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,1U);         /* F14 */
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,0x10B0u);

  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05,1U);      /* J3 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07,1U);      /* K1 */ 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05,0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07,0x10B0u);
#endif
#ifdef MY_PIN 
  /* 就近选择的XBARA引脚连接到编码器 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_07_XBAR1_INOUT19,1U);          /* F12 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_03_XBAR1_INOUT17,1U);          /* G11 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_14_XBAR1_INOUT12,1U);             /* E10 */ 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_12_XBAR1_INOUT10,1U);             /* C10 */
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_07_XBAR1_INOUT19,0x10B0u); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_03_XBAR1_INOUT17,0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_14_XBAR1_INOUT12,0x10B0u);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_12_XBAR1_INOUT10,0x10B0u);
#endif
}

static void enc_config(void)
{
  XBARA_Init(XBARA1);    /*初始化XBARA1模块*/
#ifdef LQ_PIN
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc1PhaseAInput);    //G13   A相
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc1PhaseBInput);    //F14   B相
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc3PhaseAInput); //J3    A相
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc3PhaseBInput); //K1    B相
#endif
#ifdef MY_PIN 
  /* F12 A GPIO_AD_B0_07 */
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc1PhaseAInput);
  /* G11 B GPIO_AD_B0_03 */
  XBARA_SetSignalsConnecyion(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc1PhaseBInput); 
  /* E10 A GPIO_B0_14 */
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc2PhaseAInput);
  /* C10 B GPIO_B0_12 */
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout10, kXBARA1_OutputEnc2PhaseBInput); 
#endif

  enc_config_t mEncConfigStruct;
  ENC_GetDefaultConfig(&mEncConfigStruct);
  mEncConfigStruct.decoderWorkMode = kENC_DecoderWorkAsSignalPhaseCountMode;  
  ENC_Init(ENC1, &mEncConfigStruct);
  ENC_DoSoftwareLoadInitialPositionValue(ENC1);     /* 位置计数器初始值更新成0. */

  mEncConfigStruct.enableReverseDirection = true;  /* 开启反向计数 */
  ENC_Init(ENC2, &mEncConfigStruct);     
  ENC_DoSoftwareLoadInitialPositionValue(ENC2);     /* 位置计数器初始值更新成0. */ 
}

void enc_init(void)
{
  enc_pinconfig();
  enc_config();
}


void encoder_test(void)
{   
  LCD_Init();
  LCD_CLS();
  key_init();          //按键及输入口初始化
  enc_init();
  

    short velocity1, velocity2, velocity3, velocity4;
    char txt[16];
    
    while (1)
    { 
        
        delayms(100);        //延时100毫秒
        velocity1 = (int16_t)ENC_GetPositionDifferenceValue(ENC1);  //得到编码器微分值
        sprintf(txt,"ENC1:  %5d ",velocity1); 
        LCD_P6x8Str(0,0,(uint8_t*)txt);

        velocity2 = (int16_t)ENC_GetPositionDifferenceValue(ENC2);  //得到编码器微分值
        sprintf(txt,"ENC2:  %5d ",velocity2); 
        LCD_P6x8Str(0,1,(uint8_t*)txt);

        velocity3 = (int16_t)ENC_GetPositionDifferenceValue(ENC3);  //得到编码器微分值
        sprintf(txt,"ENC3:  %5d ",velocity3); 
        LCD_P6x8Str(0,2,(uint8_t*)txt);

        velocity4 = (int16_t)ENC_GetPositionDifferenceValue(ENC4);  //得到编码器微分值
        sprintf(txt,"ENC4:  %5d ",velocity4); 
        LCD_P6x8Str(0,3,(uint8_t*)txt);

        LED_Ctrl(LED_R, RVS);
        delayms(20);       //延时0.1秒
    }
}