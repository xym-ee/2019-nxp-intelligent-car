#ifndef __IMG_CALCULATE_H
#define __IMG_CALCULATE_H

#define X1 78
#define Y1 277
#define X2 57
#define Y2 239
#define X3 40
#define Y3 186

#define   ROAD_HALF_WIDTH    20

typedef struct _point
{
	double x;
	double y;
}point_t;



//操作类型定义
typedef struct _imgcal_operations imgcal_operations_t;  

struct _imgcal_operations
{
  double (*ackman_r)(void);
  void (*r_test)(void);
  void (*A_R_test)(void);
  point_t (*transform)(uint16_t i, uint16_t j);
};



/* 计算两点距离的内联函数 */
static inline double distance(point_t A, point_t B)  //边长计算函数,参数为坐标
{
	return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

/* 输入目标中点实际坐标，返回Ackman半径 */
static inline double calculate_Ackman_R(point_t P)
{
	return  (2 * P.x)/(P.x*P.x + P.y * P.y) ;
}



extern const imgcal_operations_t imgcal_ops;


#endif // !__IMG_CALCULATE_H


