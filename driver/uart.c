/**
 *  uart.c
 *  ----------------
 *  uart初始化
 *  
 *          TX     RX
 *  uart1,  K14    L14
 *  uart2,  L11    M12
 *  
 */
#include "system.h"
#include "stdio.h"	

#define EN_LPUART1_RX  1

/* 加入以下代码,支持printf函数,而不需要选择use MicroLIB
  IAR，需要在options -> C/C++compiler -> Preprocessor 添加宏定义 _DLIB_FILE_DESCRIPTOR	*/
               
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
}
int fputc(int ch, FILE *f)
{ 	
	while((LPUART1->STAT&LPUART_STAT_TDRE_MASK)==0);
	LPUART1->DATA=(uint8_t)ch;	
	return ch;
}

//获取LPUART的时钟源频率，我们前面设置的是80MHz
//返回值：LPUART时钟源频率,根据我们的只是一般是80Mhz
uint32_t  LPUART_SrcFreqGet(void)
{
  uint32_t freq;

  if(CLOCK_GetMux(kCLOCK_UartMux)==0) 	//LPUART的时钟源选择PLL3/6
    freq=(CLOCK_GetPllFreq(kCLOCK_PllUsb1)/6U)/(CLOCK_GetDiv(kCLOCK_UartDiv)+1U);
  else									//LPUART的时钟源选择OSC
    freq=CLOCK_GetOscFreq()/(CLOCK_GetDiv(kCLOCK_UartDiv)+1U);
  return freq;
}


//初始化IO 串口1 
//bound:波特率
void lpuart1_init(uint32_t bound)
{	
	lpuart_config_t lpuart1_config;
	
	uint32_t freq = 0;							//串口的时钟源频率
	CLOCK_EnableClock(kCLOCK_Lpuart1);	//使能LPUART1时钟
	CLOCK_SetMux(kCLOCK_UartMux,0); 	//设置UART时钟源为PLL3 80Mhz，PLL3/6=480/6=80MHz
	CLOCK_SetDiv(kCLOCK_UartDiv,0); 	//设置UART时钟1分频，即UART时钟为80Mhz
	
	//LPUART1所使用的IO功能配置，即：从ALT0~ALT7选择合适的功能。
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TX,0U);	//GPIO_AD_B0_12设置为LPUART1_TX
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX,0U);	//GPIO_AD_B0_13设置为LPUART1_RX
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_12_LPUART1_TX,0x10B0u); 
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RX,0x10B0u); 

	freq = LPUART_SrcFreqGet();	
	
	LPUART_GetDefaultConfig(&lpuart1_config); 				//先设置为默认配置，后面在根据实际情况配置	
	lpuart1_config.baudRate_Bps = bound;						//波特率
	lpuart1_config.dataBitsCount = kLPUART_EightDataBits;		//8位
	lpuart1_config.stopBitCount = kLPUART_OneStopBit;			//1位停止位
	lpuart1_config.parityMode = kLPUART_ParityDisabled;		//无奇偶校验
	lpuart1_config.enableRx = true;							//使能接收
	lpuart1_config.enableTx = true;							//使能发送
	
	LPUART_Init(LPUART1,&lpuart1_config,freq);				//初始化LPUART1 
    
#if EN_LPUART1_RX									        //是否需要开启中断?	
	//LPUART中断设置
	LPUART_EnableInterrupts(LPUART1,kLPUART_RxDataRegFullInterruptEnable); //使能接收中断
    NVIC_SetPriority(LPUART1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
	EnableIRQ(LPUART1_IRQn);	                            //使能LPUART1中断
#endif
}	


//LPUART1中断服务函数，为了接收效率直接操作寄存器
void LPUART1_IRQHandler(void)
{
  uint8_t res = 0;
  if((LPUART1->STAT)&kLPUART_RxDataRegFullFlag) //接收中断
  {
    res = LPUART1->DATA;					//读取数据
    
  }
  __DSB();				//数据同步屏蔽指令
}

