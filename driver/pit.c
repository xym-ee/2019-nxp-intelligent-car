#include "system.h"


/*!
 * @brief PIT定时中断初始化
 *
 * @param channel   通道选择
 * ----------------------------------------
 * pit_init(kPIT_Chnl_0, 100000);  //通道0中断，100000us中断一次
 */
void pit_init(pit_chnl_t channel, uint32_t count)
{
     /* PIT初始化结构体 */
    pit_config_t pitConfig;  
    
    /* Set PERCLK_CLK source to OSC_CLK*/
    CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
    /* Set PERCLK_CLK divider to 1 */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);

    /*
     * pitConfig.enableRunInDebug = false;  //默认参数
     */
    PIT_GetDefaultConfig(&pitConfig);   //得到PIT默认参数

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);    //初始化PIT

    /* Set timer period for channel  */
    PIT_SetTimerPeriod(PIT, channel, USEC_TO_COUNT(count, CLOCK_GetFreq(kCLOCK_OscClk))); //设置PIT通道和定时时间

    /* Enable timer interrupts for channel  */
    PIT_EnableInterrupts(PIT, channel, kPIT_TimerInterruptEnable);  //使能通道中断

    //优先级配置 抢占优先级0  子优先级2   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(PIT_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,2));
    
    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQn);   //使能中断
    
    PIT_StartTimer(PIT, channel);   //开启定时器

}

/* 记录中断次数，定义为volatile型*/
volatile uint16_t pitIsrCnt0 = 0; 
volatile uint16_t pitIsrCnt1 = 0; 
volatile uint16_t pitIsrCnt2 = 0; 
volatile uint16_t pitIsrCnt3 = 0; 


/*PIT中断服务函数*/
void PIT_IRQHandler(void)
{
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_0)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {   
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag); /* Clear flag.*/
    
    status.interrupt_10ms = 1;
  }
  
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_1)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag); /* Clear flag.*/
    
    /* 中断服务 ***********************************************************/
    
    pitIsrCnt1++;
    /* *****************************************************************/
  }
  
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_2)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
   
    
    /* 中断服务 ***********************************************************/
    led.ops->reverse(UpLight);
    pitIsrCnt2++;
    /* *****************************************************************/
     PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag); /* Clear flag.*/
  }
  
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_3)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
    
    
    /* 中断服务 ***********************************************************/
    //led.ops->reverse(blue);
    pitIsrCnt3++;
    /* *****************************************************************/
    
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag); /* Clear flag.*/
  }
  
}




