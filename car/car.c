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

#include "system.h"

#define X1 78
#define Y1 277
#define X2 57
#define Y2 239
#define X3 40
#define Y3 186
/*
车   分为 方向、速度


*/

/*----------------------- 全局变量 ----------------------------*/

/* 车基速给定 */
short CarBaseSpeed = 0;


/*----------------------- 方法声明 ----------------------------*/

/* 曲率的计算 */
static void car_curvature_calculate(void);

/* 依据曲率的目标车速计算 */
static void car_speed_calculate(void);

/* 依据曲率的后轮差速计算 */
static void car_speed_differential(void);

/* 依据电机转速计算值的PID控制 */
static void car_speed_control(void);

/* 依据曲率的舵机转角控制 */
static void car_direction_control(void);




/*----------------------- 接口定义 ----------------------------*/

const car_calculate_operations_t car_calculate_ops = {
        .speed = car_speed_calculate,
        .differential = car_speed_differential,
        .curvature = car_curvature_calculate,
};

const car_control_operations_t car_control_ops = {
        .speed = car_speed_control,
        .direction = car_direction_control,

};


/* 外部操作接口 */
const car_device_t Car = {
        .calculate = &car_calculate_ops,
        .control = &car_control_ops,
};



/*----------------------- 方法实现 ----------------------------*/

const double N1[8] = { -0.0150792895845385, 0.724790208254393, -133.828519072425, 0.479859245171029, -0.0390347367641871, 246.078939652662, 0.0367756477299942, -0.000217827161952334 };
const double N2[8] = { 0.0478310147920351, 0.754572091335592, -145.273949103573, 0.542603428388435, -0.0236547096030482, 248.166384940324, 0.0381442174657022, 4.33612608899718e-05 };
const double N3[8] = { 0.0574754213423609, 0.578968077433388, -111.751496059287, -0.0560155656710943, 0.0227855980674026, 213.356999095967, 0.0241698103008391, 0.000553684893374724 };



struct Loc
{
	double x;
	double y;
};


double bianchang_Cal(double a, double b, double c, double d)  //边长计算函数,参数为坐标
{
	double x;
	x = sqrt(fabs((a - c)*(a - c) + (b - d)*(b - d)));
	return x;
}

double Cal(double a, double b, double c, double s)   //曲率公式函数，参数为三边长和面积
{
	double x;
	x = 4 * s / (a*b*c);
	return x;
}


struct Loc Loc_change(int line, int col ) //坐标变换函数，输入行&列，返回结构体
{
	struct Loc Real_Loc; //实际坐标

	if (line < 70)
	{
		Real_Loc.x = (int)(N3[0] * line + N3[1] * col + N3[2]) / (N3[6] * line + N3[7] * col + 1);
		Real_Loc.y = (int)(N3[3] * line + N3[4] * col + N3[5]) / (N3[6] * line + N3[7] * col + 1);
	}

	else if (line > 110)
	{
		Real_Loc.x = (int)(N1[0] * line + N1[1] * col + N1[2]) / (N1[6] * line + N1[7] * col + 1);
		Real_Loc.y = (int)(N1[3] * line + N1[4] * col + N1[5]) / (N1[6] * line + N1[7] * col + 1);
	}
	
	else
	{
		Real_Loc.x = (int)(N2[0] * line + N2[1] * col + N2[2]) / (N2[6] * line + N2[7] * col + 1);
		Real_Loc.y = (int)(N2[3] * line + N2[4] * col + N2[5]) / (N2[6] * line + N2[7] * col + 1);
	}

	return Real_Loc;
}

double curvature_Cal(int x1, int y1, int x2, int y2, int x3, int y3) //曲率计算函数
{
	double AB, BC, AC, S;
	double curvature;
	AB = bianchang_Cal(x1, y1, x2, y2);
	AC = bianchang_Cal(x1, y1, x3, y3);
	BC = bianchang_Cal(x2, y2, x3, y3);
	S = abs((x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1)) / 2;  //三点构成的三角形面积

	curvature = Cal(AB, AC, BC, S);

	return curvature;
}

struct Loc midline_Loc(double x, double y, double R) //输入变量为边线坐标和前方边线曲率半径，计算出中线的坐标进而求出偏差角，这里面只返回了中线坐标
{
	struct Loc mid_Loc;
	double k = (R - 22.5) / R;
	double sin = y / R;
	
	//mid_Loc.x = fabs(x) + 22.5 * sqrt( 1 - sin*sin  );

	if (x < 0)
	{
		mid_Loc.x = x - 22.5 * sqrt(1 - sin * sin);
	}

	else 
		mid_Loc.x = x + 22.5 * sqrt(1 - sin * sin);
	mid_Loc.y = y * k;

	return mid_Loc;
}

double Ackman_R(double x, double y)
{
	double R;

	R = (x*x + y * y) / (2 * y);

	return R;
}


/* 曲率的计算 */
static void car_curvature_calculate(void)
{
	double cur;//曲率
	double R;   //半径
	double x1, y1, x2, y2, x3, y3; //三点坐标

	x1 = Loc_change(X1, Y1).x;
	y1 = Loc_change(X1, Y1).y;

	x2 = Loc_change(X2, Y2).x;
	y2 = Loc_change(X2, Y2).y;

	x3 = Loc_change(X3, Y3).x;
	y3 = Loc_change(X3, Y3).y;

	cur = curvature_Cal(x1, y1, x2, y2, x3, y3);

	R = 1 / cur;
}



/* 依据曲率的目标车速计算 */
static void car_speed_calculate(void)
{



}


/* 依据曲率的后轮差速计算 */
static void car_speed_differential(void)
{


}

/* 依据电机转速计算值的PID控制 */
static void car_speed_control(void)
{


}

/* 依据曲率的舵机转角控制 */
static void car_direction_control(void)
{
  double x0,y0,x1,y1;
  static double alpha,last_alpha;
  /* 实际点位置 */
  x0 = Loc_change(90,187).x;
  y0 = Loc_change(90,187).y;
  
  /* 目标点位置 */
  x1 = Loc_change(90,midline[90]).x;
  y1 = Loc_change(90,midline[90]).y;
  alpha = atan(x1/y1);
  uint16_t pwm;
  pwm = (uint16_t)(3000 + 600*alpha);
  servo(pwm);

}

