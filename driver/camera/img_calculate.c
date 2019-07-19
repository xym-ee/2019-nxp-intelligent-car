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


/* ---------------------------- 方法声明 ------------------------------------ */

static double img_Ackman_R(void);
static void img_calculate_r_test(void);
static void img_Ackman_R_test(void);
static point_t img_locaion_transform(uint16_t pix_i, uint16_t pix_j);


static double _img_calculate_r(void);
static double _img_curvature(point_t A, point_t B, point_t C);
point_t midline_Loc(double x, double y, double R);
static point_t midline_Loc(double x, double y, double R);



/* ---------------------------- 外部接口 ------------------------------------ */
const imgcal_operations_t imgcal_ops = {
    .ackman_r = img_Ackman_R,
    .r_test = img_calculate_r_test,
    .A_R_test = img_Ackman_R_test,
    .transform = img_locaion_transform,    
};


/* ---------------------------- 方法实现 ------------------------------------ */

/* 逆透视变换矩阵 */
const double N1[8] = { -0.0649852534450704,2.53734549914701,-240.905004855923,1.72242380270896,-0.115684719493100,423.076434304688,0.130208451031495,-0.00342074612370382 };
const double N2[8] = { -0.0108680223123130,1.74103387838086,-172.804762452853,1.48135911232730,-0.324225542774878,294.738452846723,0.0903126321121198,-0.00636880777526755 };
const double N3[8] = { -0.0649852534450704,2.53734549914701,-240.905004855923,1.72242380270896,-0.115684719493100,423.076434304688,0.130208451031495,-0.00342074612370382 };

/* -------------- 外部函数 ------------ */
static double img_Ackman_R(void)
{
    if (status.img_roadtype == RoadStraight)  /* 直路阿克曼半径 */
    {
      return calculate_Ackman_R(img_locaion_transform(80, midline[80]));   /* 取pix_i = 160计算 */ 
    }
    else /* 弯道 */
    {  
      if (status.img_roadtype == RoadLeft) 
        return calculate_Ackman_R(midline_Loc(40, rightline[40], _img_calculate_r())); /* 取pix_j = 80计算 */ 
      else
        return calculate_Ackman_R(midline_Loc(40, leftline[40], _img_calculate_r()));
    }
}


/* 测试实际半径计算函数，结构显示在OLED上 */
static void img_calculate_r_test(void)
{
  lpuart1_init(115200);         /* 蓝牙发送串口启动 */
  key.init();                   /* 按键启动 */
  led.init();                   /* 指示灯启动 */
  oled.init();                  /* LCD启动 */
  img.init();                   /* 相机接口初始化 */
  char txt[16];
  delayms(200);                 /* 必要的延时，等待相机感光元件稳定 */
  
  while(1)
  {
    /* 获得道路类型和相关的数据 */
    img.refresh();
    
    /* 直线 */
    if (status.img_roadtype == RoadStraight)
    {
      sprintf(txt,"Straight    ");
      LCD_P6x8Str(0,0,(uint8_t*)txt);
    }
    else /* 弯道 */
    {
      if (status.img_roadtype == RoadLeft) 
      {
        sprintf(txt,"Left  r:%4d",(uint8_t)_img_calculate_r());
        LCD_P6x8Str(0,0,(uint8_t*)txt);
      }
      else
      {
        sprintf(txt,"Right r:%4d",(uint8_t)_img_calculate_r());
        LCD_P6x8Str(0,0,(uint8_t*)txt);      
      }
    }
    /* 灯光指示 */
    switch (status.img_roadtype)
    {
    case RoadStraight : led.ops->flash_fast(UpLight);     break;
    case RoadLeft     : led.ops->flash_fast(LeftLight);   break;
    case RoadRight    : led.ops->flash_fast(RightLight);  break;
    }
  }
}


static void img_Ackman_R_test(void)
{
  lpuart1_init(115200);         /* 蓝牙发送串口启动 */
  key.init();                   /* 按键启动 */
  led.init();                   /* 指示灯启动 */
  oled.init();                  /* LCD启动 */
  motor.init();                 /* 电机.包含ENC,PWM,PID参数初始化 */
  img.init();                   /* 相机接口初始化 */
  double kill;
  delayms(200);                 /* 必要的延时，等待相机感光元件稳定 */
  
  motor_speed.left = 0;
  motor_speed.right = 0;
  
  while(1)
  {
    /* 获得道路类型和相关的数据 */
    img.refresh();
    
    /* 直线 */
    if (status.img_roadtype == RoadStraight)  /* 直路阿克曼半径 */
    {
      kill = calculate_Ackman_R(img_locaion_transform(80, midline[80])); 

    }
        
    else /* 弯道 */
    {  

      if (status.img_roadtype == RoadLeft) 
        kill = calculate_Ackman_R(midline_Loc(40, rightline[40], _img_calculate_r()));
      else
        kill = calculate_Ackman_R(midline_Loc(40, leftline[40], _img_calculate_r()));
    }
        
    /* 灯光指示 */
    switch (status.img_roadtype)
    {
    case RoadStraight : led.ops->flash_fast(UpLight); break;
    case RoadLeft     : led.ops->flash_fast(LeftLight); break;
    case RoadRight    : led.ops->flash_fast(RightLight); break;
    }
  }
}


/* -------------- 内部函数 ------------ */

/* 坐标变换函数：像素坐标->实际坐标 */
static point_t img_locaion_transform(uint16_t pix_i, uint16_t pix_j) //坐标变换函数，输入行&列，返回结构体
{
	point_t real_coordinate;
	if (pix_i < 50)
	{
		real_coordinate.x = (N3[0] * pix_i + N3[1] * pix_j + N3[2]) / (N3[6] * pix_i + N3[7] * pix_j + 1) + 0.9;
		real_coordinate.y = (N3[3] * pix_i + N3[4] * pix_j + N3[5]) / (N3[6] * pix_i + N3[7] * pix_j + 1);
	}
	else if (pix_i > 70)
	{
		real_coordinate.x = (N1[0] * pix_i + N1[1] * pix_j + N1[2]) / (N1[6] * pix_i + N1[7] * pix_j + 1) + 0.9;
		real_coordinate.y = (N1[3] * pix_i + N1[4] * pix_j + N1[5]) / (N1[6] * pix_i + N1[7] * pix_j + 1);
	}
	else
	{
		real_coordinate.x = (N2[0] * pix_i + N2[1] * pix_j + N2[2]) / (N2[6] * pix_i + N2[7] * pix_j + 1) + 0.9;
		real_coordinate.y = (N2[3] * pix_i + N2[4] * pix_j + N2[5]) / (N2[6] * pix_i + N2[7] * pix_j + 1);
	}
	return real_coordinate;
}

/* 三点（实际坐标）算曲率 */
static double _img_curvature(point_t A, point_t B, point_t C)
{
  /* 三边长和三角形面积 */
	double AB, BC, AC, S;
	AB = distance(A, B);
	AC = distance(A, C);
	BC = distance(B, C);
	S = fabs((B.x - A.x)*(C.y - A.y) - (C.x - A.x)*(B.y - A.y)) / 2;  //三点构成的三角形面积
	return (4 * S / (AB*BC*AC));
}

/* 赛道半径计算函数，返回单位cm */
static double _img_calculate_r(void)
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
	A = img_locaion_transform(70, p_line[70]);
	B = img_locaion_transform(60, p_line[60]);
	C = img_locaion_transform(50, p_line[50]);  

	/* 半径 = 1/曲率 */
  return (1/_img_curvature(A, B, C));
}

static point_t midline_Loc(double x, double y, double R) //输入变量为边线坐标和前方边线曲率半径，计算出中线的坐标进而求出偏差角，这里面只返回了中线坐标
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


