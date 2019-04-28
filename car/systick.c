// Header:
// File Name: 
// Author: Z
// Date:2018/08/03

#include "systick.h"

#define EACH_PER_MS    25   //每隔 25 ms 中断一次  systick 定时器是24位向下计数的定时器  最大装载值16777215 / 600 000 000= 0.2796 最大计时27ms

struct time{
	
    uint32_t fac_us;   //us分频系数
	uint32_t fac_ms;   //ms分频系数
	volatile uint32_t millisecond;   //ms
	uint64_t microsecond;   //us
	uint8_t ms_per_tick;         //1ms多少systick计数次数
	
}timer;
void systime_init()
{
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//选择APB时钟    600M Hz
	
	timer.fac_us = SystemCoreClock / 1000000;
	timer.fac_ms = SystemCoreClock / 1000;
	timer.ms_per_tick = EACH_PER_MS;
    timer.millisecond = 100;
	SysTick_Config((SystemCoreClock / 1000) * timer.ms_per_tick );   //开启systick中断

}

void SysTick_Handler(void)
{
	timer.millisecond += timer.ms_per_tick;
}


uint32_t get_current_time_ms(void)
{
    uint32_t val = SysTick->VAL;
	return timer.millisecond -  val/timer.fac_ms;
}

uint64_t get_current_time_us(void)
{
    uint32_t val = SysTick->VAL;
	return (uint64_t)((timer.millisecond * 1000) -  val / timer.fac_us);
}

//延时1us 实测为1.28us   延时10us 实测为10.27us   延时100us 实测为100.26us  如果要使用，代码关掉优化,否则不准
void delay_us( uint64_t us)     
{
    uint32_t val = SysTick->VAL;
    int now = 1;
	uint64_t end_time = ( -val + ((uint64_t)timer.millisecond * 1000 + us) * timer.fac_us);
	while( now > 0)
    {
        val = SysTick->VAL;
        now = (int)(end_time - ( -val + ((uint64_t)timer.millisecond * 1000) * timer.fac_us)) ;
    }
}

void delay_ms( uint32_t ms) //如果要使用，代码关掉优化
{
	delay_us(ms * 1000);
}

systime_t  _systime = 
{

	systime_init,
	get_current_time_us,
	get_current_time_ms,
	delay_us,
	delay_ms

};



