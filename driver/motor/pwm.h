#include "system.h"
#ifndef _PWM_H
#define _PWM_H


/*-------------------            OLED硬件接口       -------------------*/

///*-------------- 龙邱母版引脚 --------------*/
///* PWM1 */
//#define PWM_OUT1_PINMUX     IOMUXC_GPIO_SD_B1_00_FLEXPWM1_PWMA03        /* L5 */
///* PWM2 */
//#define PWM_OUT2_PINMUX     IOMUXC_GPIO_SD_B1_01_FLEXPWM1_PWMB03        /* M5 */
///* PWM3 */
//#define PWM_OUT3_PINMUX     IOMUXC_GPIO_B0_06_FLEXPWM2_PWMA00           /* A8 */
///* PWM4 */
//#define PWM_OUT4_PINMUX     IOMUXC_GPIO_B0_07_FLEXPWM2_PWMB00           /* A9 */
///* PWM Servo1 */
//#define PWM_SERVO1_PINMUX     IOMUXC_GPIO_SD_B1_02_FLEXPWM2_PWMA03
///* PWM Servo2 */
//#define PWM_SERVO2_PINMUX     IOMUXC_GPIO_SD_B1_03_FLEXPWM2_PWMB03


/*-------------- 重新改引脚,只改了引脚映射，没改PWM模块 --------------*/
/* PWM1 */
#define PWM_OUT1_PINMUX     IOMUXC_GPIO_B1_00_FLEXPWM1_PWMA03           /* A11 左电机红线 */
/* PWM2 */
#define PWM_OUT2_PINMUX     IOMUXC_GPIO_B1_01_FLEXPWM1_PWMB03           /* B11 左电机黑线 */
/* PWM3 */
#define PWM_OUT3_PINMUX     IOMUXC_GPIO_B0_06_FLEXPWM2_PWMA00           /* A8 右电机黑线 */
/* PWM4 */
#define PWM_OUT4_PINMUX     IOMUXC_GPIO_B0_07_FLEXPWM2_PWMB00           /* A9 右电机红线 */
/* PWM Servo1 */
#define PWM_SERVO1_PINMUX   IOMUXC_GPIO_B1_02_FLEXPWM2_PWMA03           /* C11 */
/* PWM Servo2 */
#define PWM_SERVO2_PINMUX   IOMUXC_GPIO_B1_03_FLEXPWM2_PWMB03           /* D11 */



/*-------------------------------    改其他地方必出BUG！  ------------------  */



#define DUTY_MAX  10000     //duty最大值




/**
 *  初始化电机、舵机pwm输出
 *  ----------------
 */
void pwm_init(void);

/**
 *  设置电机占空比（速度）
 *  ----------------
 *  duty                占空比             -DUTY_MAX ~ DUTY_MAX
 *  ----------------
 *  duty最大值10000定义在DUTY_MAX
 *  
 *  left_motor(-2000)  //左电机反转  占空比  2000 / 10000 
 */
void left_motor(short duty);
void right_motor(short duty);

/**
 *  设置舵机角度
 *  ----------------
 *  duty                占空比             2000-4000
 *  ----------------
 *  舵机中值3000
 */
void servo(uint16_t duty);


void servo_test(void);
void test_motor(void);




#endif 



