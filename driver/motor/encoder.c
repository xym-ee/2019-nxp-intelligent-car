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
  //ENC1 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,1U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,1U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14,1U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05,1U); //  J3  用之前确保没有当作别的用途  
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04,1U); //  J4  用之前确保没有当作别的用途 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07,1U); //  K1   用之前确保没有当作别的用途

  //ENC3  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,0x10B0u);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,0x10B0u);                               
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14,0x10B0u); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05,0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04,0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07,0x10B0u);
}

static void enc_config(void)
{
  XBARA_Init(XBARA1);    //初始化XBARA1模块
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc1PhaseAInput);    //G13   A相
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc1PhaseBInput);    //F14   B相
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc1Index);       //M14   Z相
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc3PhaseAInput); //J3    A相
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc3PhaseBInput); //K1    B相
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc3Index);       //J4    Z相

  enc_config_t mEncConfigStruct;
  ENC_GetDefaultConfig(&mEncConfigStruct);
  mEncConfigStruct.decoderWorkMode = kENC_DecoderWorkAsSignalPhaseCountMode;  
  ENC_Init(ENC1, &mEncConfigStruct);     
  ENC_DoSoftwareLoadInitialPositionValue(ENC1); /* 位置计数器初始值更新成0. */

  mEncConfigStruct.enableReverseDirection = true;  //开启反方向计数
  ENC_Init(ENC3, &mEncConfigStruct);     
  ENC_DoSoftwareLoadInitialPositionValue(ENC3); /* 位置计数器初始值更新成0. */ 
}

void enc_init(void)
{
  enc_pinconfig();
  enc_config();
}


