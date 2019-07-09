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

#ifndef __CAR_H
#define __CAR_H

#include "system.h"

/*---------------- 数据操作类型定义 ------------------------*/

typedef struct _car_calculate_operations car_calculate_operations_t;

struct _car_calculate_operations
{
    void (*speed)(void);
    void (*differential)(void);
    void (*curvature)(void);
};


typedef struct _car_control_operations car_control_operations_t;

struct _car_control_operations
{
    void (*speed)(void);
    void (*direction)(void);
};


/*---------------- 设备类型定义 ------------------------*/
typedef struct _car_device car_device_t;

struct _car_device
{
    void (*init)(void);
    const car_calculate_operations_t *calculate;
    const car_control_operations_t *control;
};



extern const car_device_t Car;




#endif
