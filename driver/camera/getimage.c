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

/* 外部接口函数 */
static void img_refresh_midline(void);
static void img_oledshow(void);
static void img_uartsend(void);
static void img_roadtype_test(void);
static void img_inductance_run_roadcheck(void);
/* 内部函数 */
__ramfunc static void    _img_get(void);
__ramfunc static uint8_t _img_ostu(void);
__ramfunc static uint8_t _img_aver(void);
__ramfunc static void    _img_binary(void);
__ramfunc static void    _img_clearnoise(void);
__ramfunc static void    _img_roadtype(void);

/* ---------------------------- 操作接口 ------------------------------------ */

/*---------- 内部接口 -------------*/
const img_operations_t img_ops = {
  .get = _img_get,                  /* 图像获取 */
  .ostu = _img_ostu,                /* ostu法动态阈值 */
  .aver = _img_aver,                /* 平均灰度法动态阈值 */
  .binary = _img_binary,            /* 二值化 */
  .clearnoise = _img_clearnoise,    /* 三面以上环绕噪点清除 */
  .roadtype = _img_roadtype,        /* 获得中线 */
  .adc_roadcheck = img_inductance_run_roadcheck,
};

/*---------- 内部接口 -------------*/
const img_device_t img = {
  .refresh = img_refresh_midline,       /* 刷新图像，提取各种信息 */
  .display = img_oledshow,              /* OLED显示图像 */
  .send = img_uartsend,                 /* 串口发送图像 */
  .init = csi_init,                     /* CSI接口初始化 */
  .roadtype_test = img_roadtype_test,   /* 图像设备测试 */
  .ops = &img_ops,                      /* 图像硬件操作，滤波、二值化 */
  .cal_ops = &imgcal_ops,               /* 图像曲率、半径计算 */
};


/* ---------------------------- 方法实现 ------------------------------------ */


/*------------- 公有函数实现 ----------------- */

/* 刷新中线数组,内含等待摄像头数据，10ms回来一次 */ 
static void img_refresh_midline(void)
{
    //Wait to get the full frame buffer to show. 
//    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &CameraBufferAddr))  //摄像头CSI缓存区已满
//    {
//    }
    img.ops->get();
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, CameraBufferAddr);//将照相机缓冲区提交到缓冲队列
    img.ops->binary();
    //img.ops->clearnoise();
    img.ops->roadtype();
    //img.send();
}


/* oled上显示 */
static void img_oledshow(void)
{ 	 
  uint16_t i = 0, j = 0,temp=0;
  for(i=0;i<56;i+=8)// 56行 
  {
    LCD_Set_Pos(2,i/8+1);
    for(j=0;j<94;j++)
    { 
      temp = 0;
      if(Image[2*i][2*j]) 
        temp|=1;
      if(Image[2*i+2][2*j]) 
        temp|=2;
      if(Image[2*i+4][2*j]) 
        temp|=4;
      if(Image[2*i+6][2*j]) 
        temp|=8;
      if(Image[2*i+8][2*j]) 
        temp|=0x10;
      if(Image[2*i+10][2*j]) 
        temp|=0x20;
      if(Image[2*i+12][2*j]) 
        temp|=0x40;
      if(Image[2*i+14][2*j]) 
        temp|=0x80;
      oled.ops->data(temp); 	  	  	  
    }
  }  
}

/*!
* @brief 上位机发送
*/
static void img_uartsend(void)
{ 	 
  uint16_t i = 0, j = 0;
  printf("%c",0x00);
  printf("%c",0xff);
  printf("%c",0x01);
  printf("%c",0x01); 
  for (i=0; i<IMG_HIGH; i++)
    for(j=0; j<IMG_WIDTH; j++)
      printf("%c",Image[i][j]*255);  
}

static void img_roadtype_test(void)
{
  lpuart1_init(115200);
  led.init();
  key.init();                   /* 按键启动 */
  led.init();                   /* 指示灯启动 */
  NVIC_SetPriorityGrouping(2);  /* 2: 4个抢占优先级 4个子优先级*/
  oled.init();                   /* LCD启动 */
  img.init();                   /* 相机接口初始化 */
  delayms(200);                 /* 必要的延时，等待相机感光元件稳定 */
  led.ops->off_a();
  while (1)
  {
    
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &CameraBufferAddr))  //摄像头CSI缓存区已满
    {
    }
    img.refresh();
    
    img.display();
//    /* 灯光指示 */
//    switch (status.img_roadtype)
//    {
//    case RoadStraight : led.ops->flash_fast(UpLight); break;
//    case RoadLeft     : led.ops->flash_fast(LeftLight); break;
//    case RoadRight    : led.ops->flash_fast(RightLight); break;
//    }
    led.ops->reverse(UpLight);
    if(key.ops->get(0) == key_ok)
      img.send();
  }
}

static void img_inductance_run_roadcheck(void)
{
  if ( Image[115][93] ) /* 图像下方的这个位置变白 */
  {   /* 再次确认 */
    if (Image[115][92] && Image[115][94] && Image[114][92] && Image[114][93] && Image[114][94]) 
      status.inductance_run = 0; /* 退出电感运行 */
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

  /* 进行了均值滤波操作，pixle为指针读取数据的内联函数 */
  for(uint16_t i=0;i<IMG_HIGH;i++)
  {
    for(uint16_t j=0;j<IMG_WIDTH;j++)
    {
      if (i==0 || i==(IMG_HIGH-1) || j==1 || j==(IMG_WIDTH-1))
        Image[i][j] = *((uint8_t *)CameraBufferAddr + (i*IMG_WIDTH) + j);
      else
        Image[i][j] = pixle(i-1,j-1)/9 + pixle(i-1,j)/9 + pixle(i-1,j+1)/9 +
                      pixle(i,  j-1)/9 + pixle(i,  j)/9 + pixle(i,  j+1)/9 +
                      pixle(i+1,j-1)/9 + pixle(i+1,j)/9 + pixle(i+1,j+1)/9 ;
    }
  }
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
  //Threshold = _img_ostu();     /* 最大类间方差法 */
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


__ramfunc static void _img_roadtype(void)
{
  /* 简单的中线预处理，用最简单的 左加右除以二 */
  int16_t i,j;
  /* 默认的（最底下）中线位置 */
  int16_t mid = IMG_WIDTH/2;
  
  if ( !Image[115][93] ) /* 图像下方的这个位置变黑 */
  {   /* 再次确认 */
    if ( !Image[115][92] && !Image[115][94] && !Image[114][92] && !Image[114][93] && !Image[114][94]) 
      status.inductance_run = 1; /* 电磁模式 */
  } 

  /* 从下往上找 */
  for (i=IMG_HIGH-1;i>2;i--) /* 跳过上面几行 */
  {
    /* 从中线向左找 */
    leftline[i] = 0;         /* 默认左线在最左侧 */
    for (j=mid;j>0;j--)
    {
      if (!Image[i][j]) //是黑色
      {
        leftline[i] = j;
        break;
      }
    }
    
    /* 从中线向右找 */
    rightline[i] = IMG_WIDTH - 1;       /* 默认左线在最右侧 */
    for (j=mid;j<IMG_WIDTH - 1;j++)
    {
      if (!Image[i][j]) //是黑色
      {
        rightline[i] = j;
        break;
      }
    }
    
    mid = (leftline[i] + rightline[i])/2;   /* 继承当前中线位置 */
    
    if (leftline[i] == rightline[i])
      midline[i] = -1;
    else
      midline[i] = mid;
    //Image[i][mid] = 0; /* 在OLED上画出中线 */
  }
  
  /****** 道路类型判断 ******/
  int16_t k1,k2,deltaK;
  int16_t *p_line;
  //先看左右边线的交点在哪里
  for (i=IMG_HIGH;i>0;i--)
  {
    if (midline[i] == -1)
    {
      break; //i记录了交点位置
    }
      
  }
  
  if (i>90) //%左右线交点位于视野太靠下，肯定是弯道
  {
    if (midline[i+1]>IMG_WIDTH/2)
    {
      status.img_roadtype = RoadRight;
      return;
    }
    else
    {
      status.img_roadtype = RoadLeft;
      return;
    }
  }
  else //%其他情况
  {
    if(midline[i+1]<IMG_WIDTH/2)
      p_line = rightline;
    else
      p_line = leftline;
    /* 通过像素斜率大致判断路的类型 */
    k1 = p_line[K_IMG_i1] - p_line[K_IMG_i2];
    k2 = p_line[K_IMG_i2] - p_line[K_IMG_i3];  
    /* 用斜率的变化率ΔK = k1 - k2 来判断路的类型 */
    deltaK = k1 - k2;
    
    if ( deltaK<=6 && deltaK>=-6 )
    { /* 斜率的变化较小，判断直路 */
      status.img_roadtype = RoadStraight;
    }
    else /* 弯道 */
    {
      if (k1 < 0) /* 左弯 */
        status.img_roadtype = RoadLeft;
      else        /* 右弯 */
        status.img_roadtype = RoadRight;
    }      
  }
}




