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

#ifndef _ADC_H
#define _ADC_H
/* ADC资源序号与对应的管脚，        管脚名称        BGA
ADC1_IN3  Analog channel 1 input3   GPIO_AD_B0_14   H14
ADC1_IN8  Analog channel 1 input8   GPIO_AD_B1_03   M12
ADC1_IN7  Analog channel 1 input7   GPIO_AD_B1_02   L11
ADC1_IN4  Analog channel 1 input4   GPIO_AD_B0_15   L10
*/


/* adc分区边界值宏定义 */
/* 左电感值 车往右移*/
#define FRONT_LEFT_0    780
#define FRONT_LEFT_1    950
#define FRONT_LEFT_2    1250   
#define FRONT_LEFT_3    1550
#define FRONT_LEFT_4    2030
#define FRONT_LEFT_5    2570
#define FRONT_LEFT_6    3000
#define FRONT_LEFT_7    3260
#define FRONT_LEFT_8    3410

#define FRONT_RIGHT_0   600
#define FRONT_RIGHT_1   730
#define FRONT_RIGHT_2   1040
#define FRONT_RIGHT_3   1440
#define FRONT_RIGHT_4   1900
#define FRONT_RIGHT_5   2410
#define FRONT_RIGHT_6   3000
#define FRONT_RIGHT_7   3330
#define FRONT_RIGHT_8   3555

/* 右电感值 车往左移*/
#define BACK_LEFT_0     2340
#define BACK_LEFT_1     2410    
#define BACK_LEFT_2     2595
#define BACK_LEFT_3     2920
#define BACK_LEFT_4     3150
#define BACK_LEFT_5     3330
#define BACK_LEFT_6     3360
#define BACK_LEFT_7     3370
#define BACK_LEFT_8     3380

#define BACK_RIGHT_0    2100
#define BACK_RIGHT_1    2240
#define BACK_RIGHT_2    2490
#define BACK_RIGHT_3    2800
#define BACK_RIGHT_4    3040
#define BACK_RIGHT_5    3190
#define BACK_RIGHT_6    3260
#define BACK_RIGHT_7    3270
#define BACK_RIGHT_8    3280




typedef struct _adc_operations adc_operations_t;  

struct _adc_operations
{
  void (*refresh)(void);
  uint16_t (*get)(uint8_t ch);
};

//器件类型定义，器件一般包含数据操作
typedef struct _adc_device adc_device_t;

struct _adc_device
{
  void (*init)(void);
  void (*test)(void);
  const adc_operations_t *ops;
};


extern const adc_device_t adc;
extern uint16_t   adc_data[6];


#endif 
