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

/* 图像缓冲区的地址 */
uint32_t CameraBufferAddr;

/* 存储图像（灰度或位图）的二维数组 */
uint8_t Image[IMG_HIGH][IMG_WIDTH]; 

/* 储存边线赛道信息 */
int16_t midline[IMG_HIGH];
int16_t leftline[IMG_HIGH];
int16_t rightline[IMG_HIGH];

/* ---------------------------- 方法声明 ------------------------------------ */

static void img_refresh_midline(void);
static void img_oledshow(void);
static void img_uartsend(void);
static void img_test(void);


__ramfunc static void    _img_get(void);
__ramfunc static uint8_t _img_ostu(void);
__ramfunc static uint8_t _img_aver(void);
__ramfunc static void    _img_binary(void);
__ramfunc static void    _img_clearnoise(void);
__ramfunc static void    _img_getline(void);
__ramfunc static void    _img_midcorrection(void);

/* ---------------------------- 外部接口 ------------------------------------ */

const img_device_t Img = {
  .refresh = img_refresh_midline,
  .display = img_oledshow,
  .send = img_uartsend,
  .init = csi_init,
  .test = img_test,
};

/*---------- 内部接口 -------------*/
const img_operations_t img_ops = {
  .get = _img_get,
  .ostu = _img_ostu,
  .aver = _img_aver,
  .binary = _img_binary,
  .clearnoise = _img_clearnoise,
  .getline = _img_getline,
  .midcorrection = _img_midcorrection,
};


/* ---------------------------- 方法实现 ------------------------------------ */


/*------------- 公有函数实现 ----------------- */

/* 刷新中线数组,内含等待摄像头数据，10ms回来一次 */ 
static void img_refresh_midline(void)
{
    //Wait to get the full frame buffer to show. 
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &CameraBufferAddr))  //摄像头CSI缓存区已满
    {
    } 
    img_ops.get();
    //img_ops.binary();
    //img_ops.clearnoise();
    //img_ops.getline();
    //img_ops.midcorrection();
    Img.send();
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, CameraBufferAddr);//将照相机缓冲区提交到缓冲队列
}



/* oled上显示 */
static void img_oledshow(void)
{ 	 
  uint16_t i = 0, j = 0,temp=0;
  for(i=0;i<IMG_HIGH;i+=8)// 56行 
  {
    LCD_Set_Pos(2,i/8+1);
    for(j=0;j<IMG_WIDTH;j++)
    { 
      temp = 0;
      if(Image[0+i][j]) 
        temp|=1;
      if(Image[1+i][j]) 
        temp|=2;
      if(Image[2+i][j]) 
        temp|=4;
      if(Image[3+i][j]) 
        temp|=8;
      if(Image[4+i][j]) 
        temp|=0x10;
      if(Image[5+i][j]) 
        temp|=0x20;
      if(Image[6+i][j]) 
        temp|=0x40;
      if(Image[7+i][j]) 
        temp|=0x80;
      LCD_WrDat(temp); 	  	  	  	  
    }
  }  
}

/*!
* @brief 上位机发送
*/
static void img_uartsend(void)
{ 	 
  uint16_t i = 0, j = 0,temp=0;
  printf("%c",0x00);
  printf("%c",0xff);
  printf("%c",0x01);
  printf("%c",0x01); 
  for (i=0; i<IMG_HIGH; i++)
    for(j=0; j<IMG_WIDTH; j++)
      printf("%c",Image[i][j]); 
       

  
  
//  printf("%c",0x55);
//  printf("%c",0x55);
//  for(i=0;i<IMG_HIGH;i+=8)// 56行 
//  {
//    for(j=0;j<IMG_WIDTH;j++)
//    { 
//      temp = 0;
//      if(Image[0+i][j]) 
//        temp|=1;
//      if(Image[1+i][j]) 
//        temp|=2;
//      if(Image[2+i][j]) 
//        temp|=4;
//      if(Image[3+i][j]) 
//        temp|=8;
//      if(Image[4+i][j]) 
//        temp|=0x10;
//      if(Image[5+i][j]) 
//        temp|=0x20;
//      if(Image[6+i][j]) 
//        temp|=0x40;
//      if(Image[7+i][j]) 
//        temp|=0x80;
//      printf("%c",temp); 	  	  	  	  
//    }
//  }  
}


/*!
* @brief oled+mt9v034二值化显示测试
*/ 
static void img_test(void)
{
  LCD_Init();               //LCD初始化 
  LCD_CLS();
  ExInt_Init();
  LCD_Show_Frame94();
  MotorPid.deviceinit();
  //pid_control_init();       //电机速度PID控制初始化
  csi_init();               //相机接口初始化
  delayms(200);             //延时200毫秒，等待相机运行稳定
  //speedvalue = 110;
  uint8_t lednum = 0;
  while (1)
  {
    //中断中给出调试标志位
    if(status.debug_mode == 1)
      UI_debugsetting();
    
    img_refresh_midline();          //偏差获取
    car_speed(speedvalue);      //速度控制
    direction_ctrl();           //方向控制
    Img.display();            //显示，显示应该避免中断打断造成显示异常
    
    lednum++;
    if(lednum == 50)
    {
      led.ops->reverse(red);
      lednum = 0;
    }

  }
}



/*------------- 私有函数实现 ----------------- */

/* 从缓冲区读取图像 */
__ramfunc static void _img_get(void)
{
  if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) 
  {//注意，使用csiFrameBuf数组时，最好刷新一下D-Cache 不然上次数据可能会存放在cache里面，造成数据错乱
    SCB_DisableDCache();        
  }
  SCB_EnableDCache();

  /* 进行了均值滤波操作 */
  for(uint16_t i=0;i<IMG_HIGH;i++)  //缓存区图像高
  {
    for(uint16_t j=0;j<IMG_WIDTH;j++)     //取188的中间128像素，第31-第158共128个
    {
      if (i==0 || i==(IMG_HIGH-1) || j==1 || j==(IMG_WIDTH-1))
        Image[i][j] = *((uint8_t *)CameraBufferAddr + (i*IMG_WIDTH) + j);
      else
        Image[i][j] = pixle(i-1,j-1)/9 + pixle(i-1,j)/9 + pixle(i-1,j+1)/9 +
                      pixle(i,  j-1)/9 + pixle(i,  j)/9 + pixle(i,  j+1)/9 +
                      pixle(i+1,j-1)/9 + pixle(i+1,j)/9 + pixle(i+1,j+1)/9 ;
    }
  }
  
  
//  for(uint8_t i=0; i<IMG_HIGH; i++)
//    for(uint8_t j=0; j<IMG_WIDTH; j++)
//      Image[i][j] = pixle(i-1,j-1)/9 + pixle(i-1,j)/9 + pixle(i-1,j+1)/9 +
//                    pixle(i,  j-1)/9 + pixle(i,  j)/9 + pixle(i,  j+1)/9 +
//                    pixle(i+1,j-1)/9 + pixle(i+1,j)/9 + pixle(i+1,j+1)/9 ;
}

/* OSTU最大类间方差法返回动态阈值 */
__ramfunc static uint8_t _img_ostu(void) 
{ 
  int16_t   i,j; 
  uint32_t  Amount              = 0; 
  uint32_t  PixelBack           = 0; 
  uint32_t  PixelIntegralBack   = 0; 
  uint32_t  PixelIntegral       = 0; 
  int32_t   PixelIntegralFore   = 0; 
  int32_t   PixelFore           = 0; 
  double    OmegaBack,OmegaFore,MicroBack,MicroFore,SigmaB,Sigma; // 类间方差; 
  int16_t   MinValue,MaxValue; 
  uint8_t   Threshold           = 0;
  uint8_t   HistoGram[256]      = {0};
  for(j=0;j<IMG_HIGH;j++) 
  { 
    for (i=0;i<IMG_WIDTH;i++) 
      HistoGram[Image[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
  } 
  
  for(MinValue=0;MinValue<256 && HistoGram[MinValue]==0;MinValue++) ;        //获取最小灰度的值
  for(MaxValue=255;MaxValue>MinValue && HistoGram[MinValue]==0; MaxValue--) ; //获取最大灰度的值
  
  if(MaxValue==MinValue)
    return MaxValue;                    // 图像中只有一个颜色    
  if(MinValue+1 == MaxValue)
    return MinValue;                    // 图像中只有二个颜色
  for (j=MinValue;j<=MaxValue;j++)
    Amount += HistoGram[j];       //  像素总数
  
  PixelIntegral = 0;
  for(j=MinValue;j<=MaxValue;j++)
  {
    PixelIntegral += HistoGram[j] * j;//灰度值总数
  }
  SigmaB = -1;
  for(j=MinValue;j<MaxValue;j++)
  {
    PixelBack           = PixelBack + HistoGram[j];                 //前景像素点数
    PixelFore           = Amount - PixelBack;                       //背景像素点数
    OmegaBack           = (double)PixelBack / Amount;               //前景像素百分比
    OmegaFore           = (double)PixelFore / Amount;               //背景像素百分比
    PixelIntegralBack   += HistoGram[j] * j;                        //前景灰度值
    PixelIntegralFore   = PixelIntegral - PixelIntegralBack;        //背景灰度值
    MicroBack           = (double)PixelIntegralBack / PixelBack;    //前景灰度百分比
    MicroFore           = (double)PixelIntegralFore / PixelFore;    //背景灰度百分比
    Sigma               = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//计算类间方差
    if(Sigma>SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //返回最佳阈值;
} 

/* 简单的平均灰度求动态阈值 */
__ramfunc static uint8_t _img_aver(void)
{
  uint16_t   i = 0,j = 0;
  uint8_t   GaveValue;
  uint32_t  tv = 0;
  
  for(i=0;i<IMG_HIGH;i++)
  {    
    for(j=0;j<IMG_WIDTH;j++)
      tv += Image[i][j];                    /* 全局灰度求和 */
  }
  GaveValue = tv/IMG_HIGH/IMG_WIDTH;        /* 平均灰度 */
  return (GaveValue*7/10 + 10);             /* 均值灰度比例 */
}

/*!
 * @brief 图像二值化处理
 */
__ramfunc static void _img_binary(void)
{
  uint8_t Threshold;
  //Threshold = _img_ostu(Image_Use);     /* 最大类间方差法 */
  Threshold = _img_aver();          /* 均值灰度比例 */
  for(uint16_t i=0;i<IMG_HIGH;i++)
    for(uint16_t j=0;j<IMG_WIDTH;j++)
      (Image[i][j]>Threshold) ? (Image[i][j] = 1) : (Image[i][j] = 0);
}

/* 三面环绕噪点消除 */
__ramfunc static void _img_clearnoise(void)
{  
  for(uint16_t i=1; i<IMG_HIGH-1; i++)
  {  	    
    for(uint16_t j=1; j<IMG_WIDTH-1; j=j+1)
    {
      if((Image[i][j]==0)&&(Image[i-1][j]+Image[i+1][j]+Image[i][j+1]+Image[i][j-1]>2))         
        Image[i][j] = 1;
      else if((Image[i][j]==1)&&(Image[i-1][j]+Image[i+1][j]+Image[i][j+1]+Image[i][j-1]<2))         
        Image[i][j] = 0;
    }	  
  }  
}






__ramfunc static void _img_getline(void)
{
  int16_t i,j;
  /* 默认的（最底下）中线位置 */
  int16_t mid = 46;
  
  /* 从下往上找 */
  for(i=IMG_HIGH-1;i>2;i--) /* 跳过上面几行 */
  {
    /* 从中线向左找 */
    leftline[i] = 0;         /* 默认左线在最左侧 */
    for(j=mid;j>0;j--)
    {
      if(!Image[i][j]) //是黑色
      {
        leftline[i] = j;
        break;
      }
    }
    
    /* 从中线向右找 */
    rightline[i] = 93;       /* 默认左线在最左侧 */
    for(j=mid;j<94;j++)
    {
      if(!Image[i][j]) //是黑色
      {
        rightline[i] = j;
        break;
      }
    }

    mid = (leftline[i] + rightline[i])/2;   /* 继承当前中线位置 */
    midline[i] = mid;
    Image[i][mid] = 0; /* 在OLED上画出中线 */
    
//    if(leftline[i] == rightline[i])
//      midline[i] = -1;      /* 边线与中线相交标志 */
//    else
//    {
//      midline[i] = mid;
//      //Image[i][mid] = 0; /* 在OLED上画出中线 */
//    }
  }
}

__ramfunc static void _img_midcorrection(void)
{
  int16_t i;
  int16_t *p1;   /* 指向边线位置数组指针 */
  int16_t *p2;   /* 指向边线位置数组指针 */
  int16_t BoundValue;    /* 赛道边界值 */
  
  int16_t CrossoverPointY = 0;  /* 存放赛道边线与中线的交点纵坐标，默认无交点 */
  int16_t RoadLowBound    = 50; /* 存放赛道边线与视野的交点纵坐标，默认在靠下 */    
  /* 找中线和边线交点 */
  for (i=IMG_HIGH-1;i>2;i--)  /* 跳过上面几行 */
  {
    if (midline[i] == -1)
    {
      CrossoverPointY = i;
      break;    
    }
  }
  
  /* 找中线和边线交点 */
  if (CrossoverPointY != 0)  /* 赛道边线与中线有交点 */
  {
    if ( (midline[CrossoverPointY] + 1)<46 ) /* 交点在左侧，赛道左弯 */
    {
       p1 = leftline;
       p2 = rightline;
       BoundValue = 0;  
    }
    else    /* 交点在右侧，赛道右弯 */
    {
      p1 = rightline;
      p2 = leftline;  
      BoundValue = IMG_WIDTH - 1;    
    }
    
    /* 找赛道边线和视野边线的下侧交点 */
    for (i=CrossoverPointY+1;i<40;i++)
    {
      if ( *(p1+i) != BoundValue )
      {
        RoadLowBound = i;
        break;
      }
    }
    
    /* 用赛道边线代替中线的偏移量 */
    int8_t shift = *(p2+RoadLowBound) - *(midline+RoadLowBound);
    
    /* 中线修正 */
    for ( i=CrossoverPointY;i<RoadLowBound;i++ )
      midline[i] = *(p2+i) - shift;
    
    /* 交点下的中线设置在视野边界 */
    midline[CrossoverPointY+1] = BoundValue;
    
    /* 新中线一阶滤波 */
    for ( i=CrossoverPointY+1;i<IMG_HIGH-2;i++ )
    {
      midline[i] = (int8_t)(0.7*midline[i] + (1-0.7)*midline[i+1]);
      Image[i][midline[i]] =  0;  /* 在OLED上画出中线 */
    }
  }
}






