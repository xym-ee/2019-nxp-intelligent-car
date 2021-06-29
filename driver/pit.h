/*  <The 14th National University Students Intelligent Car Race.>
 *  Copyright (C) <2019>  < github.com/Meng2015 >
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