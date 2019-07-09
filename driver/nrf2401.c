#include "system.h"

/************************************************************************
*代码移植修改区
*只需要根据原理图修改对应的端口时钟 端口 引脚
************************************************************************/
//设置引脚电平
#define NRF_SCN_LOW   GPIO2_BASE_PTR->DRs.DR0 = 0
#define NRF_SCN_HIGH  GPIO2_BASE_PTR->DRs.DR0 = 1
#define NRF_CE_LOW    GPIO2_BASE_PTR->DRs.DR1 = 0
#define NRF_CE_HIGH   GPIO2_BASE_PTR->DRs.DR1 = 1
//读取引脚电平
//#define NRF_IRQ_READ  (NRF_IRQ_PORT->IDR & NRF_IRQ)
#define NRFAddrMax 50 //NRF最后一个字节地址最大为50
uint8_t NRFaddr = 0xFF; //初始化NRF最后一字节地址

uint8_t NRF_TX_DATA[TX_PAYLO_WIDTH];//NRF发送缓冲区
uint8_t NRF_RX_DATA[RX_PAYLO_WIDTH];//NRF接收缓冲区

uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0xFF}; //发送地址
uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0xFF}; //接收地址

void NRF24L01_Config(void);
void NRF_GetAddr(void);

uint8_t NRF_WriteReadByte(uint8_t data)
{
    return LP_SPI_ReadWriteByte(LPSPI1, data);
}

/*****************************************************************************
*函  数：void NRF24l01_Init(void)
*功  能：NRF引脚GPIO初始化
*参  数：无
*返回值：无
*备  注：无
*****************************************************************************/
void NRF24l01_Init(void)
{
  
  CLOCK_EnableClock(kCLOCK_Iomuxc);
  
  gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIO为输出方向
                                          0,                   //低电平
                                          kGPIO_NoIntmode      //非中断模式
                                          };
  //片选 CS
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_00_GPIO2_IO00,     0U);                                          
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_00_GPIO2_IO00,0x10B0u);  
  GPIO_PinInit(GPIO2,0, &GPIO_Output_Config); 
  //CE
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_01_GPIO2_IO01,     0U);                                          
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_01_GPIO2_IO01,0x10B0u); 
  GPIO_PinInit(GPIO2,1, &GPIO_Output_Config); 
  
  LP_SPI_Master_Init(LPSPI1, 8000000);
  NRF24L01_Check(); //检查NRF24L01是否与MCU通信                                    
  
  NRF_SCN_HIGH; //失能NRF
  NRF_CE_LOW; //待机模式
  
  // NRF24L01_Config();  //配置NRF并初始化为接收模式
}

/*****************************************************************************
*函  数：uint8_t NRF24l01_write_reg(uint8_t reg,uint8_t value)
*功  能：写一字节数据到寄存器
*参  数：reg： 寄存器地址
*        val:  要写入的数据
*返回值：status
*备  注：NRF2401代码移植只需把SPI驱动修改成自己的即可
*****************************************************************************/
uint8_t NRF24l01_write_reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	NRF_SCN_LOW;
	status=NRF_WriteReadByte(reg);
	NRF_WriteReadByte(value);
	NRF_SCN_HIGH;
	return status;
}

/*****************************************************************************
*函  数：uint8_t NRF24l01_read_reg(uint8_t reg)
*功  能：读一字节数据到寄存器
*参  数：reg： 寄存器地址
*返回值：reg_val
*备  注：NRF2401代码移植只需把SPI驱动修改成自己的即可
*****************************************************************************/
uint8_t NRF24l01_read_reg(uint8_t reg)
{
	uint8_t reg_val;
	NRF_SCN_LOW;
	NRF_WriteReadByte(reg);
	reg_val = NRF_WriteReadByte(0xff);
	NRF_SCN_HIGH;
	return reg_val;
}

/*****************************************************************************
*函  数：uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
*功  能：写一组数据到寄存器
*参  数：reg： 寄存器地址
*       pBuf： 要写入数据的地址
*        len:  要写入的数据长度
*返回值：status
*备  注：NRF2401代码移植只需把SPI驱动修改成自己的即可
*****************************************************************************/
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status;
	int i;
	NRF_SCN_LOW;
	status = NRF_WriteReadByte(reg);
	for( i=0;i<len;i++)
	{
		NRF_WriteReadByte(*pBuf);
		pBuf++;
	}
	NRF_SCN_HIGH;
	return status;
}

/*****************************************************************************
*函  数：uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
*功  能：读一组数据到寄存器
*参  数：reg： 寄存器地址
*       pBuf： 要读取数据的地址
*        len:  要读取的数据长度
*返回值：status
*备  注：NRF2401代码移植只需把SPI驱动修改成自己的即可
*****************************************************************************/
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status;
	int i;
	NRF_SCN_LOW;
	status = NRF_WriteReadByte(reg);
	for(i = 0;i < len ;i++)
	{
		*pBuf = NRF_WriteReadByte(0xff);
		pBuf++;
	}
	NRF_SCN_HIGH;
	return status;
}

/*****************************************************************************
*函  数：void NRFset_Mode(uint8_t mode)
*功  能：切换NRF2401的工作模式模式
*参  数：无
*返回值：无
*备  注：无
*****************************************************************************/
void NRFset_Mode(uint8_t mode)
{
	if(mode == IT_TX)
	{
        NRF_CE_LOW;
        NRF24l01_write_reg(W_REGISTER+CONFIG,IT_TX);//不用中断
//        NRF24l01_write_reg(W_REGISTER+STATUS,0X7E); //清除所有中断,防止一进去发送模式就触发中断	
        NRF_CE_HIGH;
        // delay_us(15);
	}
	else
	{
        NRF_CE_LOW;
        NRF24l01_write_reg(W_REGISTER+CONFIG,IT_RX);//配置为接收模式 不用中断
//        NRF24l01_write_reg(W_REGISTER+STATUS,0X7E); //清除所有中断,防止一进去接收模式就触发中断
        NRF_CE_HIGH;
     	delayus(200);
	}
    
}

/*****************************************************************************
*函  数：void NRF24L01_Config(void)
*功  能：NRF基本参数配置，并初始化为接收模式
*参  数：无
*返回值：无
*备  注：无
*****************************************************************************/
void NRF24L01_Config(void)
{
	NRF_CE_LOW;
	NRF24l01_write_reg(W_REGISTER+SETUP_AW, 0x03); //配置通信地址的长度，默认值时0x03,即地址长度为5字节
	NRF24L01_Write_Buf(W_REGISTER+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH); //写TX节点地址 
	NRF24L01_Write_Buf(W_REGISTER+RX_ADDR_P0,(uint8_t*)TX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK
	NRF24l01_write_reg(W_REGISTER+SETUP_RETR,0x1A); //设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 0x1A
	
	NRF24l01_write_reg(W_REGISTER+EN_RXADDR,0x01);//使能通道0的接收地址  
	NRF24l01_write_reg(W_REGISTER+EN_AA,0x01); //使能通道0自动应答
	NRF24l01_write_reg(W_REGISTER+RX_PW_P0,RX_PAYLO_WIDTH);//选择通道0的有效数据宽度  
	NRF24L01_Write_Buf(W_REGISTER+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //写RX节点地址
	NRF24l01_write_reg(W_REGISTER+RF_CH,30); //设置RF通道为40hz(1-64Hz都可以)
	NRF24l01_write_reg(W_REGISTER+RF_SETUP,0x07); //设置TX发射参数,0db增益,2Mbps,低噪声增益关闭 （注意：低噪声增益关闭/开启直接影响通信,要开启都开启，要关闭都关闭0x0f）
	NRFset_Mode(IT_RX); //默认为接收模式
	
	NRF_CE_HIGH;
}	

/*****************************************************************************
*函  数：uint8_t NRF24L01_testConnection(void)
*功  能：检查NRF2401与MCU的SPI总线是否通信正常
*参  数：无
*返回值：1已连接 0未连接
*备  注：无
*****************************************************************************/
uint8_t NRF24L01_testConnection(void)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i; 	 
	NRF24L01_Write_Buf(W_REGISTER+TX_ADDR,buf,5); //写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)
        if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 0; //检测24L01错误	
	return 1;	//检测到24L01
}	
void NRF24L01_Check(void)
{
	while(!NRF24L01_testConnection())
	{
		printf("\rNRF2401 no connect...\r\n");
        led.ops->on(UpLight);
	}
}


/*****************************************************************************
*函  数：uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
*功  能：NRF发送一包数据
*参  数：txbuf：要发送数据地址
*返回值：无 
*备  注：无
*****************************************************************************/
void NRF24L01_TxPacket(uint8_t *txbuf)
{
	NRF_CE_LOW;	
	NRF24L01_Write_Buf(W_REGISTER+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);  //写TX节点地址 
	NRF24L01_Write_Buf(W_REGISTER+RX_ADDR_P0,(uint8_t*)TX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK
	NRF24L01_Write_Buf(W_RX_PAYLOAD,txbuf,TX_PAYLO_WIDTH); //写数据到TX_BUFF
    NRF24l01_write_reg(W_REGISTER+CONFIG,0x7e);	//设置为发送模式,不使用所有中断
//	NRF24l01_write_reg(W_REGISTER+STATUS,0X7E); //清除所有中断,防止一进去发送模式就触发中断
    NRF_CE_HIGH;
	delayus(10);  //CE持续高电平10us
}

/*****************************************************************************
*函  数：uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
*功  能：NRF接收一包数据
*参  数：rxbuf：接收数据存储地址
*返回值：无
*备  注：无
*****************************************************************************/
void NRF24L01_RxPacket(uint8_t *rxbuf)
{
	NRF_CE_LOW;
	NRF24L01_Read_Buf(R_RX_PAYLOAD,rxbuf,TX_PAYLO_WIDTH);//读取RX的有效数据
	NRF24l01_write_reg(FLUSH_RX,0xff); //清除RX FIFO(注意：这句话很必要)
	NRF_CE_HIGH;
}


/*****************************************************************************
*函  数：void NRF_Test(void)
*功  能：MRF通信测试函数
*参  数：无
*返回值：无 
*备  注：测试时用
*****************************************************************************/
void NRF_Test(void)
{
	uint8_t t=0;
	static uint8_t mode,key;
	mode = ' ';
	key=mode;
	for(t=0;t<32;t++)
	{
		key++;
		if(key>('~'))key=' ';
		NRF_TX_DATA[t]=key;	
	}
	mode++; 
	if(mode>'~')mode=' ';  	  		
	NRF24L01_TxPacket(NRF_TX_DATA);
}
