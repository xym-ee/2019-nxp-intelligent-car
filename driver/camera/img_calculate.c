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

/* 逆透视变换矩阵 */
const double N1[8] = { -0.0150792895845385, 0.724790208254393, -133.828519072425, 0.479859245171029, -0.0390347367641871, 246.078939652662, 0.0367756477299942, -0.000217827161952334 };
const double N2[8] = { 0.0478310147920351, 0.754572091335592, -145.273949103573, 0.542603428388435, -0.0236547096030482, 248.166384940324, 0.0381442174657022, 4.33612608899718e-05 };
const double N3[8] = { 0.0574754213423609, 0.578968077433388, -111.751496059287, -0.0560155656710943, 0.0227855980674026, 213.356999095967, 0.0241698103008391, 0.000553684893374724 };

/* 坐标变换：像素坐标->实际坐标 */
static point_t locaion_transform(uint16_t line, uint16_t col) //坐标变换函数，输入行&列，返回结构体
{
	point_t Real_Loc; //实际坐标
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

/* 三点（实际坐标）算曲率 */
static double curvature(point_t A, point_t B, point_t C) //曲率计算函数
{
  /* 三边长和三角形面积 */
	double AB, BC, AC, S;
	AB = distance(A, B);
	AC = distance(A, C);
	BC = distance(B, C);
	S = fabs((B.x - A.x)*(C.y - A.y) - (C.x - A.x)*(B.y - A.y)) / 2;  //三点构成的三角形面积
	return (4 * S / (AB*BC*AC));
}

point_t midline_Loc(double x, double y, double R) //输入变量为边线坐标和前方边线曲率半径，计算出中线的坐标进而求出偏差角，这里面只返回了中线坐标
{
	point_t mid_Loc;
	double k = (R - ROAD_HALF_WIDTH) / R;
	double sin = y / R;
	if (x < 0)
		mid_Loc.x = x - ROAD_HALF_WIDTH * sqrt(1 - sin * sin);
	else
		mid_Loc.x = x + ROAD_HALF_WIDTH * sqrt(1 - sin * sin);
	mid_Loc.y = y * k;
	return mid_Loc;
}

/* 赛道半径计算函数，返回单位cm */
double img_calculate_r(void)
{
  /* 三点算曲率 */
	point_t A, B, C;
  /* 边线数组指针 */
  int16_t *p_line;    
  /* 左弯曲率用右边算 */
  if (status.img_roadtype == RoadLeft)
    p_line = rightline;
  /* 右弯曲率用左边算 */
  else if (status.img_roadtype == RoadRight)
    p_line = leftline;
  else  /* 程序出错。直路不进入此函数 */
    return 0;
  
  /* 像素位置逆透视为实际位置，这三行位置可以改变 */
	A = locaion_transform(78, p_line[78]);
	B = locaion_transform(57, p_line[57]);
	C = locaion_transform(40, p_line[40]);  

	/* 半径 = 1/曲率 */
  return (1/curvature(A, B, C));
	//printf("x1 = %lf y1 = %lf\nx2 = %lf y2 = %lf\nx3 = %lf y3 = %lf\n前瞻曲率是：%lf\n道路半径R = %lf", A.x, A.y, B.x, B.y, C.x, C.y, cur, R);
}

