#include "system.h"

uint8_t Image_Use[IMG_HIGH][IMG_WIDTH]; //压缩后之后用于存要使用数据
uint8_t Pixle[IMG_HIGH][IMG_WIDTH];     //存放二值化后的数据
uint32_t fullCameraBufferAddr;
// 获取需要的图像数据
__ramfunc static void Get_Use_Image(void)
{
  if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) 
  {//注意，使用csiFrameBuf数组时，最好刷新一下D-Cache 不然上次数据可能会存放在cache里面，造成数据错乱
    SCB_DisableDCache();
  }
  SCB_EnableDCache();
 
  for(int i=0,x=0;i<APP_CAMERA_HEIGHT;i+=2,x++)  //  480/4/2/2 = 30
  {
    for(int j=0,y=0;j<APP_CAMERA_WIDTH*2;j+=2,y++)//隔2列取一列  752*2/4/2 = 188   //两行数据 一行94像素  实际显示分辨率  94*60
    {
      if(y<IMG_WIDTH)
        Image_Use[x*2][y] = (*((uint8_t *)fullCameraBufferAddr +  i*APP_CAMERA_WIDTH*2 + j));
      else
        Image_Use[x*2 + 1][y - IMG_WIDTH] = (*((uint8_t *)fullCameraBufferAddr +  i*APP_CAMERA_WIDTH*2 + j));
    }
  }

}

/**
 *  OSTU最大类间方差法求动态阈值
 */
__ramfunc static uint8_t GetOSTU(uint8_t tmImage[IMG_HIGH][IMG_WIDTH]) 
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
      HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
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
    if (Sigma>SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //返回最佳阈值;
} 

/*!
 * @brief 图像二值化处理
 *
 */
__ramfunc static void Camera_0_1_Handle(void)
{
  int       i = 0,j = 0;
  uint8_t   GaveValue;
  uint32_t  tv=0;
  uint8_t   Threshold = 0;
  for(i=0;i<IMG_HIGH;i++)
  {    
    for(j=0;j<IMG_WIDTH;j++)
      tv += Image_Use[i][j];            /* 全局灰度求和 */
  }
  GaveValue = tv/IMG_HIGH/IMG_WIDTH;     /* 平均灰度 */
  //Threshold = GetOSTU(Image_Use);     /* 最大类间方差法 */
  Threshold = GaveValue*7/10 + 10;        /* 均值灰度比例 */
  for(i=0;i<IMG_HIGH;i++)
  {
    for(j=0;j<IMG_WIDTH;j++)
    {                                
      if(Image_Use[i][j] > Threshold) //二值化    
        Pixle[i][j] = 0;        
      else                                        
        Pixle[i][j] = 1;
    }    
  }
}


/*!
* @brief 三面以上反数围绕清除噪点
*/
__ramfunc static void Pixle_Filter(void)
{  
  int nr; //行
  int nc; //列
  
  for(nr=1; nr<IMG_HIGH-1; nr++)
  {  	    
    for(nc=1; nc<IMG_WIDTH-1; nc=nc+1)
    {
      if((Pixle[nr][nc]==0)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]>2))         
      {
        Pixle[nr][nc] = 0;
      }	
      else if((Pixle[nr][nc]==1)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]<2))         
      {
        Pixle[nr][nc] = 1;
      }	
    }	  
  }  
}

uint8_t left[60] = {0};
uint8_t right[60] = {0};
uint8_t up[94] = {0};
uint8_t down[94] = {0};




/*!
* @brief oled上显示
*/
void mt9v_oledshow(void)
{ 	 
  uint8_t i = 0, j = 0,temp=0;
  for(i=0;i<56;i+=8)// 56行 
  {
    LCD_Set_Pos(2,i/8+1);
    for(j=0;j<IMG_WIDTH;j++)
    { 
      temp = 0;
      if(Pixle[0+i][j]) 
        temp|=1;
      if(Pixle[1+i][j]) 
        temp|=2;
      if(Pixle[2+i][j]) 
        temp|=4;
      if(Pixle[3+i][j]) 
        temp|=8;
      if(Pixle[4+i][j]) 
        temp|=0x10;
      if(Pixle[5+i][j]) 
        temp|=0x20;
      if(Pixle[6+i][j]) 
        temp|=0x40;
      if(Pixle[7+i][j]) 
        temp|=0x80;
      LCD_WrDat(temp); 	  	  	  	  
    }
  }  
}



/*!
 * @brief 刷新中线数组
 * 内含等待摄像头数据，10ms回来一次
 */ 
void refresh_midline(void)
{
    //Wait to get the full frame buffer to show. 
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //摄像头CSI缓存区已满
    {
    } 
    Get_Use_Image();                //获取使用数据
    Camera_0_1_Handle();            //二值化
    Pixle_Filter();                 //滤波
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列
}

/*!
* @brief oled+mt9v034二值化显示测试
*/ 
void mt9v_oled_test(void)
{
  LCD_Init();               //LCD初始化 
  LCD_CLS();                //LCD清屏 
  LCD_Show_Frame94();
  pid_control_init();
  csi_init();
  delayms(200);        //延时200毫秒  
  speedvalue = 110;
  while (1)
  {
    refresh_midline();          //偏差获取
    //car_speed(speedvalue);      //速度控制
    //direction_ctrl();           //方向控制
    mt9v_oledshow();            //显示
    //LED_Color_Reverse(red); //EVK LED闪烁  
  }
}

