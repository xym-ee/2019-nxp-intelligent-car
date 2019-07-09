#ifndef __IMG_CALCULATE_H
#define __IMG_CALCULATE_H

#define X1 78
#define Y1 277
#define X2 57
#define Y2 239
#define X3 40
#define Y3 186

#define   ROAD_HALF_WIDTH    20

typedef struct Loc
{
	double x;
	double y;
}point_t;

/* 计算两点距离的内联函数 */
static inline double distance(point_t A, point_t B)  //边长计算函数,参数为坐标
{
	return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

/* 输入中点实际坐标，返回Ackman半径 */
static inline double Ackman_R(double x, double y)
{
	return (x*x + y * y) / (2 * y);
}


#endif // !__IMG_CALCULATE_H


