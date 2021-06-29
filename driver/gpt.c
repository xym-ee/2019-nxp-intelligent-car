/*  <The 14th National University Students Intelligent Car Race.>
 *  Copyright (C) <2019>  < github.com:He0L1w  NJUST >
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "system.h"

volatile bool gptIsrFlag = false;

void GPT2_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    GPT_ClearStatusFlags(GPT2, kGPT_OutputCompare1Flag);
    gptIsrFlag = true;
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
    __DSB();
#endif
}


// 初始化GPT计数器  GPT1用计时  GPT2用作延时
void gpt_init(void)
{
    gpt_config_t gptConfig;
    /*Clock setting for GPT 150M*/
    CLOCK_SetMux(kCLOCK_PerclkMux, 0);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0);
    
    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.enableRunInDbg = true;   //开启debug情况下可以使用
    GPT_Init(GPT1, &gptConfig);
    GPT_Init(GPT2, &gptConfig);
    
    GPT_SetClockDivider(GPT1, 150);      // GPT 时钟 150M / 15 = 10M
    GPT_SetClockDivider(GPT2, 15);      // GPT 时钟 150M / 150 = 10M
    
    GPT_SetOutputCompareValue(GPT1, kGPT_OutputCompare_Channel1, 0XFFFFFFFF);   //GPT 是32为计数器 最大 0xFFFF FFFF 
    GPT_SetOutputCompareValue(GPT2, kGPT_OutputCompare_Channel1, 0XFFFFFFFF);   //GPT 是32为计数器 最大 0xFFFF FFFF 

}

/**
 *  定时器测试
 *  ----------------
 */
void Test_GPT_Timer(void)
{
    gpt_init();
    printf("GPT1 : %d \n", GPT_GetClockSource(GPT1));
    printf("GPT2 : %d \n", GPT_GetClockSource(GPT2));
    uint32_t time = 0, last_time = 0;
    gpt_time_start();
    while(1)
    {
        delayus(10);
        GPIO_PinReverse (GPIO2, 22); 
        time = gpt_time_get();
        printf("delay 10 us is %u \n", time - last_time);
        last_time = gpt_time_get(); //注意 打印也需要时间
        delayms(10);
        time = gpt_time_get();
        printf("delay 10 ms is %u \n", time - last_time);
        last_time = gpt_time_get(); //注意 打印也需要时间
    }
}

static void gpt_time_start(void)
{
    GPT_StopTimer(GPT1);
    GPT_StartTimer(GPT1);   //刷新 GPT1
    GPT_StartTimer(GPT1);
}

void delayus(uint32_t us)
{
    /* Start Timer */
    GPT_StartTimer(GPT2);
    uint32_t time = us * 10 - 7;
    while(GPT_GetCurrentTimerCount(GPT2) < time);
    
    GPT_StopTimer(GPT2);
}

void delayms(uint32_t ms)
{
    delayus(ms * 1000);
}

/**
 *  获得计数时长
 *  ----------------
 *  
 */
uint32_t gpt_time_get(void)
{
    if(GPT_GetStatusFlags(GPT1, kGPT_RollOverFlag))   //溢出
        return 0;
    else
        return GPT_GetCurrentTimerCount(GPT1) - 1;
}


