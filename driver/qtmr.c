#include "system.h"

/*正交解码引脚初始化*/
void QTMR_QD_InitPins(void) 
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0,0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_QTIMER3_TIMER1,0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0,0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_QTIMER3_TIMER1,0x10B0u);
}

/*正交解码测试*/
void Test_QTMR_QD(void)
{
    qtmr_config_t qtmrConfig;
    QTMR_QD_InitPins();      //初始化定时器的输出引脚
    QTMR_GetDefaultConfig(&qtmrConfig);   //得到默认的参数
    qtmrConfig.primarySource = kQTMR_ClockCounter0InputPin;    //Counter0InputPin作为主要的输入源，用于脉冲输入，J11引脚与编码器的A相相连
    qtmrConfig.secondarySource = kQTMR_Counter1InputPin;       //Counter1InputPin作为次要的输入源，用于方向输入，K11引脚与编码器的Dir相连
    QTMR_Init(TMR3, kQTMR_Channel_0, &qtmrConfig);             //初始化TMR3的通道0
    QTMR_StartTimer(TMR3, kQTMR_Channel_0, kQTMR_PriSrcRiseEdgeSecDir);   //开启TMR3通道0的计数器，计数模式使用带方向计数模式
    while (1)
    {   
      printf("\r\nInput Captured value=%d\n", (int16_t)TMR3->CHANNEL[kQTMR_Channel_0].CNTR);  //打印位置
      TMR3->CHANNEL[kQTMR_Channel_0].CNTR = 0;            //清空计数器
      LED_Ctrl(LED_R, RVS); 
      delayms(100);
    }
    
}

/*pwm引脚配置*/
void QTMR_PWM_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);         
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_03_QTIMER2_TIMER0,0U);   //BGA管脚D8  GPIO_B0_03                              
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_04_QTIMER2_TIMER1,0U);   //BGA管脚C8  GPIO_B0_05
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_03_QTIMER2_TIMER0,0x10B0u); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_04_QTIMER2_TIMER1,0x10B0u); 
}

/*qtmr pwm功能配置*/
void QTMR2_PWM_Init(uint32_t  frequency, uint16_t duty)
{
    QTMR_PWM_InitPins();    //初始化定时器的输出引脚
    
    qtmr_config_t qtmrConfig;    //qtmr配置结构体
    QTMR_GetDefaultConfig(&qtmrConfig);        //得到默认结构体
    /* Use IP bus clock div by 64 */
    qtmrConfig.primarySource = kQTMR_ClockDivide_64;  //使用Ipg时钟的64分频  150Mhz / 64 = 2.34375 Mhz   qtmr是16位的计数器，最大65535  
    QTMR_Init(TMR2, kQTMR_Channel_0, &qtmrConfig);      //初始化TMR2 的 通道 0
    QTMR_Init(TMR2, kQTMR_Channel_1, &qtmrConfig);      //初始化TMR2 的 通道 1
    printf("\r\n****Ipg clock is %d.****\n",(CLOCK_GetFreq(kCLOCK_IpgClk)));   //打印IPG时钟
    //初始化PWM   TMR2   通道0          PWM频率   占空比  极性    时钟频率
    QTMR_SetupPwm(TMR2,kQTMR_Channel_0,frequency,duty,false,CLOCK_GetFreq(kCLOCK_IpgClk)/64); //初始化PWM
    QTMR_SetupPwm(TMR2,kQTMR_Channel_1,frequency,duty,false,CLOCK_GetFreq(kCLOCK_IpgClk)/64); 
    QTMR_StartTimer(TMR2,kQTMR_Channel_0,kQTMR_PriSrcRiseEdge); //通道0在primary时钟源的上升沿计数
    QTMR_StartTimer(TMR2,kQTMR_Channel_1,kQTMR_PriSrcRiseEdge); //通道0在primary时钟源的上升沿计数
}

/*设置占空比*/
void LQ_QTMR_Set_Duty(uint32_t  frequency, uint16_t duty)
{
    uint32_t srcclk,period,hightime,lowtime;
    
    srcclk=CLOCK_GetFreq(kCLOCK_IpgClk)/64;   // 时钟自己配置，例程上配置的是IPG的64分频 150Mhz / 64 = 2.34375 Mhz
    
    period=(srcclk/frequency);          //一个PWM周期需要的计数值 =  计数器1s计数次数  /  频率 
    hightime=(period*duty)/100;         //一个PWM周期中高电平时间(计数值)  hightime=(period*duty)/1000;  duty最大值 1000
    lowtime=period-hightime;            //一个PWM周期中低电平时间(计数值)
    
    TMR2->CHANNEL[kQTMR_Channel_0].CMPLD1=lowtime;
    TMR2->CHANNEL[kQTMR_Channel_0].CMPLD2=hightime;
    
    TMR2->CHANNEL[kQTMR_Channel_1].CMPLD1=hightime;     //和通道0相反
    TMR2->CHANNEL[kQTMR_Channel_1].CMPLD2=lowtime;

}

/*qtmr pwm测试*/
void Test_QTMR_PWM(void)
{
    QTMR2_PWM_Init(12000, 0);//BGA管脚D8  GPIO_B0_03 
                                //BGA管脚C8  GPIO_B0_04
    delayms(1000);
    uint8_t i = 1;
    while(1)
    {
        LQ_QTMR_Set_Duty(i * 100, i);
        i++;
        delayms(2000);
        if(i > 100) i = 1;
        LED_Color_Reverse(red); //红灯闪烁
    }

}


