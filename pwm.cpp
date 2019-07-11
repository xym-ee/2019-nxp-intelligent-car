#include <stdio.h>
#include <math.h>

#define servo_mid 3000



pwm_t _servo_PWM(double Ackman_R)
{
	pwm_t servo_pwm;

	if (Ackman_R > 200)		/* 半径大，是直路 */
		servo_pwm.Left_PWM = servo_pwm.Right_PWM = servo_mid;

	else if (Ackman_R>30 && Ackman_R<200)	
	{
		servo_pwm.Left_PWM = 0.0003 * pow(Ackman_R, 3.0) - 0.09*pow(Ackman_R, 2.0) + 11.20 * Ackman_R + 2390.0;
		servo_pwm.Right_PWM = -0.0003 * pow(Ackman_R, 3.0) + 0.09*pow(Ackman_R, 2.0) - 11.20 * Ackman_R + 3610.0;
	}

	return servo_pwm;
}
