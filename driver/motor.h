#ifndef _MOTOR_H
#define _MOTOR_H

#include "system.h"

/* -----------------------------  硬件接口   ---------------------------------*/

/*-------------- 龙邱母版引脚 --------------*/
#define ENC_LA_MUX      IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05
#define ENC_LB_MUX      IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07
#define ENC_RA_MUX      IOMUXC_GPIO_AD_B0_10_XBAR1_IN22
#define ENC_RB_MUX      IOMUXC_GPIO_AD_B0_09_XBAR1_IN21

#define ENC_LA_CHANNEL  kXBARA1_InputIomuxXbarInout05
#define ENC_LB_CHANNEL  kXBARA1_InputIomuxXbarInout07
#define ENC_RA_CHANNEL  kXBARA1_InputIomuxXbarIn22
#define ENC_RB_CHANNEL  kXBARA1_InputIomuxXbarIn21


/* PWM1 */
#define PWM_OUT1_PINMUX     IOMUXC_GPIO_SD_B1_00_FLEXPWM1_PWMA03        /* L5 */
/* PWM2 */
#define PWM_OUT2_PINMUX     IOMUXC_GPIO_SD_B1_01_FLEXPWM1_PWMB03        /* M5 */
/* PWM3 */
#define PWM_OUT3_PINMUX     IOMUXC_GPIO_B0_06_FLEXPWM2_PWMA00           /* A8 */
/* PWM4 */
#define PWM_OUT4_PINMUX     IOMUXC_GPIO_B0_07_FLEXPWM2_PWMB00           /* A9 */
/* PWM Servo1 */
#define PWM_SERVO1_PINMUX     IOMUXC_GPIO_SD_B1_02_FLEXPWM2_PWMA03
/* PWM Servo2 */
#define PWM_SERVO2_PINMUX     IOMUXC_GPIO_SD_B1_03_FLEXPWM2_PWMB03

/* --------------    自己PCB分配     --------- */
//#define ENC_LA_MUX  IOMUXC_GPIO_AD_B0_11_XBAR1_IN23
//#define ENC_LB_MUX  IOMUXC_GPIO_B1_03_XBAR1_INOUT17
//#define ENC_RA_MUX  IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08
//#define ENC_RB_MUX  IOMUXC_GPIO_SD_B0_05_XBAR1_INOUT09
//
//#define ENC_LA_CHANNEL  kXBARA1_InputIomuxXbarIn23
//#define ENC_LB_CHANNEL  kXBARA1_InputIomuxXbarInout17
//#define ENC_RA_CHANNEL  kXBARA1_InputIomuxXbarInout09
//#define ENC_RB_CHANNEL  kXBARA1_InputIomuxXbarInout08
//如果计数异常 更换AB相

/*---- 重新改引脚,只改了引脚映射，没改PWM模块 ------*/
///* PWM1 */ 
//#define PWM_OUT1_PINMUX     IOMUXC_GPIO_B1_00_FLEXPWM1_PWMA03           /* A11 左电机红线 */
///* PWM2 */
//#define PWM_OUT2_PINMUX     IOMUXC_GPIO_B1_01_FLEXPWM1_PWMB03           /* B11 左电机黑线 */
///* PWM3 */
//#define PWM_OUT3_PINMUX     IOMUXC_GPIO_B0_06_FLEXPWM2_PWMA00           /* A8 右电机黑线 */
///* PWM4 */
//#define PWM_OUT4_PINMUX     IOMUXC_GPIO_B0_07_FLEXPWM2_PWMB00           /* A9 右电机红线 */
///* PWM Servo1 */
//#define PWM_SERVO1_PINMUX   IOMUXC_GPIO_B1_02_FLEXPWM2_PWMA03           /* C11 */
///* PWM Servo2 */
//#define PWM_SERVO2_PINMUX   IOMUXC_GPIO_B1_03_FLEXPWM2_PWMB03           /* D11 */



/* -----------------------------  硬件接口   ---------------------------------*/


#define DUTY_MAX  10000     //duty最大值




typedef struct
{
  short     _kp;            //P
  short     _ki;            //I
  short     _kd;            //D
  short     _pid_out;       //pid输出
  short     _err;           //偏差
  short     _err1;          //上次偏差
  short     _err2;          //上上次偏差
  short     _sume;
}_pid_t;



typedef struct _pid_operations pid_operations_t;

struct _pid_operations
{
	void (*init)(_pid_t* base);
	void (*change)(_pid_t* base,short kp,short ki,short kd);
	short (*control)(_pid_t* base,short targer,short real);
};


typedef struct _motor_device motor_device_t;

struct _motor_device
{
	void (*init)(void);

	const pid_operations_t *pid_ops;
};



extern short speedvalue;
extern _pid_t leftpid;
extern _pid_t rightpid;

/**
 *  车模速度控制初始化
 *  ----------------
 *  采用了PID控制算法
 */
void pid_control_init(void);
/**
 *  车模速度设置
 *  ----------------
 */
void car_speed(short value);
void pid_value_init(_pid_t* base);
void pid_change(_pid_t* base,short kp,short ki,short kd);

#endif 
