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

#ifndef _TSL1401_H
#define _TSL1401_H


//TSL1401引脚定义，方便控制
#define TSL_CLK				GPIO1_BASE_PTR->DRs.DR23
#define TSL_SI				GPIO1_BASE_PTR->DRs.DR16


void tsl1401_init(void);
void RD_TSL(void);
void tsl1401_test(void);

#endif
