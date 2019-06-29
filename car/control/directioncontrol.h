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



typedef struct 
{
  uint8_t _dkp;
  uint8_t _dkd;
}dir_pd_t;

extern dir_pd_t pd;




#define SPEED1  50
#define SPEED2  90
#define SPEED3  120
#define SPEED4  150
#define SPEED5  180
#define SPEED6  210
#define SPEED7  240
#define SPEED8  260
#define SPEED9  280



void direction_ctrl(void);

#endif


