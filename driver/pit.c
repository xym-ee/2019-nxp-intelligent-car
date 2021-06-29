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

/*PIT中断服务函数*/
void PIT_IRQHandler(void)
{
  /* 通道1：定时10ms，作为PID控制周期判断标志 */
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_0)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {   
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag); /* Clear flag.*/
    status.interrupt_10ms ++;
  }
  
  /* 通道2：定时500ms，作为圆环的硬件延时标志位 */
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_1)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag); /* Clear flag.*/
    status.interrupt_500ms ++;
    PIT_StopTimer(PIT, kPIT_Chnl_1); /* 单词定时 */
  }
  
  /* 通道3：未使用 */
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_2)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
     PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag); /* Clear flag.*/
  }
  
  /* 通道3：未使用 */
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_3)&kPIT_TimerFlag)==kPIT_TimerFlag)
  {
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag); /* Clear flag.*/
  }
  
}



void pit_test(void)
{ 
  led.init();
  timer_start(kPIT_Chnl_1,500);
  while (1)
  {
    while(status.interrupt_500ms == 0)
    {
    }
    led.ops->reverse(UpLight);
    
    status.interrupt_500ms = 0;    
  }
}
