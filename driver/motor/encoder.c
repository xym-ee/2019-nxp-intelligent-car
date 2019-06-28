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

  IOMUXC_SetPinMux(ENC_LA_MUX,1U);      
  IOMUXC_SetPinMux(ENC_LB_MUX,1U);     
  IOMUXC_SetPinMux(ENC_RA_MUX,1U);     
  IOMUXC_SetPinMux(ENC_RB_MUX,1U);     
  
  IOMUXC_SetPinConfig(ENC_LA_MUX,0x10B0u);
  IOMUXC_SetPinConfig(ENC_LB_MUX,0x10B0u);
  IOMUXC_SetPinConfig(ENC_RA_MUX,0x10B0u);
  IOMUXC_SetPinConfig(ENC_RB_MUX,0x10B0u);
}

static void enc_config(void)
{
  XBARA_Init(XBARA1);    /*初始化XBARA1模块*/

  XBARA_SetSignalsConnection(XBARA1, ENC_LA_CHANNEL, kXBARA1_OutputEnc1PhaseAInput);
  XBARA_SetSignalsConnection(XBARA1, ENC_LB_CHANNEL, kXBARA1_OutputEnc1PhaseBInput);
  XBARA_SetSignalsConnection(XBARA1, ENC_RA_CHANNEL, kXBARA1_OutputEnc2PhaseAInput);
  XBARA_SetSignalsConnection(XBARA1, ENC_RB_CHANNEL, kXBARA1_OutputEnc2PhaseBInput);

  enc_config_t mEncConfigStruct;
  ENC_GetDefaultConfig(&mEncConfigStruct);
  mEncConfigStruct.decoderWorkMode = kENC_DecoderWorkAsSignalPhaseCountMode;  
  
  mEncConfigStruct.enableReverseDirection = true;  /* 开启反向计数 */
  ENC_Init(ENC1, &mEncConfigStruct);
  ENC_DoSoftwareLoadInitialPositionValue(ENC1);     /* 位置计数器初始值更新成0. */

  mEncConfigStruct.enableReverseDirection = false;  /* 开启反向计数 */
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
  
  short velocity1, velocity2;
  char txt[16];
  while (1)
  { 
    velocity1 = (int16_t)ENC_GetPositionDifferenceValue(ENC1);  //得到编码器微分值
    sprintf(txt,"ENC1:  %5d ",velocity1); 
    LCD_P6x8Str(0,0,(uint8_t*)txt);
    
    velocity2 = (int16_t)ENC_GetPositionDifferenceValue(ENC2);  //得到编码器微分值
    sprintf(txt,"ENC2:  %5d ",velocity2); 
    LCD_P6x8Str(0,1,(uint8_t*)txt);
     
    led.ops->ctrl(LED_R, RVS);
    delayms(100);       //延时0.1秒
  }
}