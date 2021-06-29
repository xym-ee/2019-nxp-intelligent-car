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

#ifndef _GPT_H
#define _GPT_H



void delayms(uint32_t ms);
void delayus(uint32_t us);
void gpt_time_start(void);
uint32_t gpt_time_get(void);
void Test_GPT_Timer(void);
void gpt_init(void);



#endif  