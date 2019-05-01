#include "system.h"
/*******************************************************************************
*  SDK提供了两种在Noncacheable区定义缓冲区和变量的方法：
*  AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes)
*  AT_NONCACHEABLE_SECTION(var)
******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint16_t lcdFrameBuf[2][LCD_HEIGHT][LCD_WIDTH], FRAME_BUFFER_ALIGN);               //LCD数据缓存区

uint8_t counter;       //LCD有两个缓冲区，一个当前显示用，一个缓冲用

int OFFSET0=0;      //最远处，赛道中心值综合偏移量
int OFFSET1=0;      //第二格
int OFFSET2=0;      //最近，第三格

uint8_t Image_Use[Use_ROWS][Use_Line]; //压缩后之后用于存要使用数据
uint8_t Pixle[Use_ROWS][Use_Line];     //存放二值化后的数据
uint32_t fullCameraBufferAddr;
// 获取需要的图像数据
__ramfunc void Get_Use_Image(void)
{
  if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) 
  {//注意，使用csiFrameBuf数组时，最好刷新一下D-Cache 不然上次数据可能会存放在cache里面，造成数据错乱
    SCB_DisableDCache();
  }
  SCB_EnableDCache();
 
  for(int i = 0, x = 0; i < APP_CAMERA_HEIGHT; i+=2, x++)  //  480/4/2/2 = 30
  {
    for(int j = 0, y = 0; j < APP_CAMERA_WIDTH*2; j+=2, y++)//隔2列取一列  752*2/4/2 = 188   //两行数据 一行94像素  实际显示分辨率  94*60
    {
      if(y < Use_Line)
        Image_Use[x*2][y] = (*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j));
      else
        Image_Use[x*2 + 1][y - Use_Line] = (*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j));
    }
  }

}

/**
 *  OSTU最大类间方差法求动态阈值
 */
__ramfunc uint8_t GetOSTU(uint8_t tmImage[Use_ROWS][Use_Line]) 
{ 
  int16_t i,j; 
  uint32_t Amount = 0; 
  uint32_t PixelBack = 0; 
  uint32_t PixelIntegralBack = 0; 
  uint32_t PixelIntegral = 0; 
  int32_t PixelIntegralFore = 0; 
  int32_t PixelFore = 0; 
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差; 
  int16_t MinValue, MaxValue; 
  uint8_t Threshold = 0;
  uint8_t HistoGram[256];              //  
  
  for (j = 0; j < 256; j++)  HistoGram[j] = 0; //初始化灰度直方图 
  
  for (j = 0; j < Use_ROWS; j++) 
  { 
    for (i = 0; i < Use_Line; i++) 
    { 
      HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
    } 
  } 
  
  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值
  
  if (MaxValue == MinValue)     return MaxValue;         // 图像中只有一个颜色    
  if (MinValue + 1 == MaxValue)  return MinValue;        // 图像中只有二个颜色
  
  for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  像素总数
  
  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//灰度值总数
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];    //前景像素点数
    PixelFore = Amount - PixelBack;         //背景像素点数
    OmegaBack = (double)PixelBack / Amount;//前景像素百分比
    OmegaFore = (double)PixelFore / Amount;//背景像素百分比
    PixelIntegralBack += HistoGram[j] * j;  //前景灰度值
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
    MicroBack = (double)PixelIntegralBack / PixelBack;   //前景灰度百分比
    MicroFore = (double)PixelIntegralFore / PixelFore;   //背景灰度百分比
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//计算类间方差
    if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //返回最佳阈值;
} 

//二值化处理
__ramfunc void Camera_0_1_Handle(void)
{
  int i = 0,j = 0;
  uint8_t GaveValue;
  uint32_t tv=0;
  uint8_t Threshold = 0;
  //累加
  for(i = 0; i <Use_ROWS; i++)
  {    
    for(j = 0; j <Use_Line; j++)
    {                            
      tv+=Image_Use[i][j];   //累加  
    } 
  }
  GaveValue = tv/Use_ROWS/Use_Line;     //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100 
  //    Threshold = GetOSTU(Image_Use);  //大津法求阈值
  //按照均值的比例进行二值化
  Threshold = GaveValue*7/10+10;        //此处阈值设置，根据环境的光线来设定 
  for(i = 0; i < Use_ROWS; i++)
  {
    for(j = 0; j < Use_Line; j++)
    {                                
      if(Image_Use[i][j] >Threshold) //二值化    
        Pixle[i][j] =1;        
      else                                        
        Pixle[i][j] =0;
    }    
  }
  
}


/*!
* @brief 三面以上反数围绕清除噪点
*/
__ramfunc void Pixle_Filter(void)
{  
  int nr; //行
  int nc; //列
  
  for(nr=1; nr<Use_ROWS-1; nr++)
  {  	    
    for(nc=1; nc<Use_Line-1; nc=nc+1)
    {
      if((Pixle[nr][nc]==0)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]>2))         
      {
        Pixle[nr][nc]=1;
      }	
      else if((Pixle[nr][nc]==1)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]<2))         
      {
        Pixle[nr][nc]=0;
      }	
    }	  
  }  
}

/*!
* @brief oled上显示
*/
void mt9v_oledshow(void)
{ 	 
  uint8_t i = 0, j = 0,temp=0;
  for(i=0;i<56;i+=8)// 56行 
  {
    LCD_Set_Pos(2,i/8+1);
    for(j=0;j<Use_Line;j++) 
    { 
      temp=0;
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


int8_t mt9v_err(void)
{
  
  uint8_t i,left,right;

   //从中往左边找
   for(i=46;i>0;i--)
   {
      if(!Pixle[30][i]) //是黑色
      {
        left = i;
        break;
      }
   }
   if(Pixle[30][0])//最左边为白色
     left = 0;
   
   //从中往右边找
   for(i=47;i<94;i++)
   {
      if(!Pixle[30][i]) //是黑色
      {
        right = i;
        break;
      }
   }
   if(Pixle[30][93])//最右边为白色
     right = 93;
   
   return (left+right)/2-46;
}



/*!
* @brief oled+mt9v034二值化显示测试
*/ 
void mt9v_oled_test(void)
{
  LCD_Init();               //LCD初始化 
  LCD_CLS();                //LCD清屏 
  LCD_Show_Frame94();

  csi_init();
  //delayms(200);        //延时200毫秒  
  
  while (1)
  {     
    // Wait to get the full frame buffer to show. 
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //摄像头CSI缓存区已满
    {
    }   
    Get_Use_Image();                //获取使用数据
    Camera_0_1_Handle();            //二值化
    Pixle_Filter();                 //滤波

    mt9v_oledshow();                    //显示
        
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列  
    LED_Color_Reverse(red); //EVK LED闪烁  
  }
}
