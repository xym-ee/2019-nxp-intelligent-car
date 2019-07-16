#ifndef _PIT_H
#define _PIT_H

void pit_test(void);
void pit_init(pit_chnl_t channel, uint32_t count);

/* 启动定时 */
static inline void timer_start(pit_chnl_t channel,uint32_t ms)
{
  pit_init(channel, 1000*ms);
}
/* 停止定时 */
static inline void timer_stop(pit_chnl_t channel)
{
  PIT_StopTimer(PIT, channel);
}
#endif  