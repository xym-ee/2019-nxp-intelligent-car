#ifndef _ADC_H
#define _ADC_H
/* //ADC资源序号与对应的管脚，      管脚名称      BGA管脚编号
ADC1_IN0  Analog channel 1 input0   GPIO_AD_B1_11   J13
ADC1_IN1  Analog channel 1 input1   GPIO_AD_B0_12   K14 
ADC1_IN2  Analog channel 1 input2   GPIO_AD_B0_13   L14
ADC1_IN3  Analog channel 1 input3   GPIO_AD_B0_14   H14
ADC1_IN4  Analog channel 1 input4   GPIO_AD_B0_15   L10
ADC1_IN5  Analog channel 1 input5   GPIO_AD_B1_00   J11
ADC1_IN6  Analog channel 1 input6   GPIO_AD_B1_01   K11
ADC1_IN7  Analog channel 1 input7   GPIO_AD_B1_02   L11
ADC1_IN8  Analog channel 1 input8   GPIO_AD_B1_03   M12
ADC1_IN9  Analog channel 1 input9   GPIO_AD_B1_04   L12
ADC1_IN10 Analog channel 1 input10  GPIO_AD_B1_05   K12
ADC1_IN11 Analog channel 1 input11  GPIO_AD_B1_06   J12
ADC1_IN12 Analog channel 1 input12  GPIO_AD_B1_07   K10
ADC1_IN13 Analog channel 1 input13  GPIO_AD_B1_08   H13
ADC1_IN14 Analog channel 1 input14  GPIO_AD_B1_09   M13
ADC1_IN15 Analog channel 1 input15  GPIO_AD_B1_10   L13

ADC2_IN0  Analog channel 2 input0   GPIO_AD_B1_11   J13
ADC2_IN1  Analog channel 2 input1   GPIO_AD_B1_12   H12
ADC2_IN2  Analog channel 2 input2   GPIO_AD_B1_13   H11
ADC2_IN3  Analog channel 2 input3   GPIO_AD_B1_14   G12
ADC2_IN4  Analog channel 2 input4   GPIO_AD_B1_15   J14
ADC2_IN5  Analog channel 1 input5   GPIO_AD_B1_00   J11
ADC2_IN6  Analog channel 1 input6   GPIO_AD_B1_01   K11
ADC2_IN7  Analog channel 1 input7   GPIO_AD_B1_02   L11
ADC2_IN8  Analog channel 1 input8   GPIO_AD_B1_03   M12
ADC2_IN9  Analog channel 1 input9   GPIO_AD_B1_04   L12
ADC2_IN10 Analog channel 1 input10  GPIO_AD_B1_05   K12
ADC2_IN11 Analog channel 1 input11  GPIO_AD_B1_06   J12
ADC2_IN12 Analog channel 1 input12  GPIO_AD_B1_07   K10
ADC2_IN13 Analog channel 1 input13  GPIO_AD_B1_08   H13
ADC2_IN14 Analog channel 1 input14  GPIO_AD_B1_09   M13
ADC2_IN15 Analog channel 1 input15  GPIO_AD_B1_10   L13
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

extern uint16_t   adc_data[6];

uint16_t adc1_get(uint8_t ch);
void adc1_init(void);
void adc_test(void);
int8_t front_midloca(uint16_t *p);
int8_t back_midloca(uint16_t *p);
void adc_datarefresh(void);

#endif 
