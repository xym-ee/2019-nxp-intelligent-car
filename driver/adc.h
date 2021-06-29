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

#ifndef _ADC_H
#define _ADC_H

/* ADC资源序号与对应的管脚，        管脚名称        BGA
ADC1_IN3  Analog channel 1 input3   GPIO_AD_B0_14   H14
ADC1_IN8  Analog channel 1 input8   GPIO_AD_B1_03   M12
ADC1_IN7  Analog channel 1 input7   GPIO_AD_B1_02   L11
ADC1_IN4  Analog channel 1 input4   GPIO_AD_B0_15   L10
*/

/* 依据电感判断的圆环 */
typedef enum _adc_circle
{  
  NoCircle           = 0U,   /* 无圆环 */
  CircleConditon     = 1U,    /* 圆环条件，给方向控制圆环分支使用 */
   RightCircleRun    = 2U,   /* 环内运行 */
  LeftCircleRun      = 3U,   /* 环内运行 */ 
  RightCircleWaitIn  = 4U,   /* 检测到圆环，等待入环 */
  RightCircleWaitOut = 5U,   /* 检测到直线，等待出环 */  
  LeftCircleWaitIn   = 6U,   /* 检测到圆环，等待入环 */
  LeftCircleWaitOut  = 7U,   /* 检测到直线，等待出环 */  
}adc_circle_t;

/* 电磁线状态定义 */
typedef enum _adc_wire_status
{  
  SingleLine  = 0U,   /* 单根电磁线 */
  LeftLine    = 1U,   /* 左侧有电磁线 */
  RightLine   = 2U,   /* 右侧有电磁线 */
  TwoLine     = 3U,
}adc_wire_status_t;

/* 电路判断的道路数据类型 */
typedef struct _adc_roaddata
{
  uint16_t      *adcvalue;    /* 原始电感数据存放地址 */
  int8_t        err;            
  int8_t        err1;
  adc_circle_t  status;
} adc_roaddata_t;


#define A1  (adc_data[0]>700)
#define A2  (adc_data[1]>700)
#define A3  (adc_data[2]>700)
#define A4  (adc_data[3]>700)




typedef struct _adc_operations adc_operations_t;  

struct _adc_operations
{
  uint16_t (*get)(uint8_t ch);
  int8_t (*geterror)(void);
};

//器件类型定义，器件一般包含数据操作
typedef struct _adc_device adc_device_t;

struct _adc_device
{
  void (*init)(void);
  void (*refresh)(void);
  void (*circle_check)(void);
  void (*error_check)(void);
  void (*test)(void);
  void (*circle_test)(void);
  const adc_operations_t *ops;
};


extern const adc_device_t   adc;
extern uint16_t         adc_data[4];
extern adc_roaddata_t adc_roaddata;

static inline adc_wire_status_t adc_wire_status(void)
{
  if ( (adc_data[0]<700) && (adc_data[1]>700) && (adc_data[2]>700) && (adc_data[3]>700) )         /* 0111 */
    return RightLine;
  else if ( (adc_data[0]>700) && (adc_data[1]>700) && (adc_data[2]>700) && (adc_data[3]<700) )    /* 1110 */
    return LeftLine;
  else if ((adc_data[0]>700) && (adc_data[1]>700) && (adc_data[2]<700) && (adc_data[3]>700) )      /* 1101 */
    return TwoLine;
  else
    return SingleLine;
}

#endif 
