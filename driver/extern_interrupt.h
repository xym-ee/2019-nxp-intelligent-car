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

#ifndef _EXT_INT_H
#define _EXT_INT_H

#include "system.h"


/* ----------------------龙邱母版传感器外部中断------------------------------ */
//在GPIO2   16-31引脚之间随意切换，否则需要修改中断使能和中断服务函数名
///* 终点磁铁触发中断 */
//#define MAGNET_INT_PINMUX       IOMUXC_GPIO_B1_01_GPIO2_IO17    /* B11 */
//#define MAGNET_INT_PORT         GPIO2
//#define MAGNET_INT_PIN          17
//
///* 远程遥控触发中断 */
//#define REMOTE_INT_PINMUX       IOMUXC_GPIO_B1_02_GPIO2_IO18    /* C11 */
//#define REMOTE_INT_PORT         GPIO2
//#define REMOTE_INT_PIN          18
//
///* 光电开关触发中断 */
//#define DISTANCE_INT_PINMUX     IOMUXC_GPIO_B1_06_GPIO2_IO22
//#define DISTANCE_INT_PORT       GPIO2
//#define DISTANCE_INT_PIN        22


/* ----------------------自己母版外部中断------------------------------ */







void ExInt_Init(void);



#endif 
