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

#ifndef _DIRCON_H
#define _DIRCON_H

#define SERVO_HZ    50
#define SERVO_MID   1500


typedef struct _car_device car_device_t;

struct _car_device
{
    void (*direction_control)(void);
    void (*speed_caculate)(void);
    void (*control_inductance)(void);
};


void car_direction_barrier_test(void);



extern const car_device_t car;



//偏差描述量的定义
#define NB	(-3+3)
#define NM	(-2+3)
#define NS	(-1+3)
#define ZO	(0+3)
#define PS	(1+3)
#define PM	(2+3)
#define PB	(3+3)





#endif


