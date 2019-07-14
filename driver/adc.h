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

#define A1  (adc_data[0]>700)
#define A2  (adc_data[1]>700)
#define A3  (adc_data[2]>700)
#define A4  (adc_data[3]>700)




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
extern uint16_t   adc_data[4];


#endif 
