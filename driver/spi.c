#include "spi.h"
#include "system.h"
/*FLEXIO 主模式*/
#define MASTER_FLEXIO_SPI_BASEADDR (FLEXIO2)
#define FLEXIO_SPI_SOUT_PIN 6U
#define FLEXIO_SPI_SIN_PIN 7U
#define FLEXIO_SPI_CLK_PIN 5U
#define FLEXIO_SPI_PCS_PIN 8U
#define SLAVE_FLEXIO_SPI_IRQ FLEXIO2_IRQn
/* Select USB1 PLL (480 MHz) as flexio clock source */
#define MASTER_FLEXIO_SPI_CLOCK_SELECT (3U)
/* Clock pre divider for flexio clock source */
#define MASTER_FLEXIO_SPI_CLOCK_PRE_DIVIDER (4U)
/* Clock divider for flexio clock source */
#define MASTER_FLEXIO_SPI_CLOCK_DIVIDER (7U)
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY                                            \
    (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / (MASTER_FLEXIO_SPI_CLOCK_PRE_DIVIDER + 1U) / \
     (MASTER_FLEXIO_SPI_CLOCK_DIVIDER + 1U))
#define FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL (2U)
#define FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL (3U)        
#define EXAMPLE_TX_DMA_SOURCE (kDmaRequestMuxFlexIO2Request0Request1)
#define EXAMPLE_RX_DMA_SOURCE (kDmaRequestMuxFlexIO2Request2Request3)        
#define FLEXIO_TX_SHIFTER_INDEX 0U
#define FLEXIO_RX_SHIFTER_INDEX 2U    
        
/*FLEXIO 从模式*/
#define SLAVE_FLEXIO_SPI_BASEADDR (FLEXIO2)
#define FLEXIO_SPI_SOUT_PIN 6U
#define FLEXIO_SPI_SIN_PIN 7U
#define FLEXIO_SPI_CLK_PIN 5U
#define FLEXIO_SPI_PCS_PIN 8U

#define SLAVE_FLEXIO_SPI_IRQ FLEXIO2_IRQn
/* Select USB1 PLL (480 MHz) as flexio clock source */
#define SLAVE_FLEXIO_SPI_CLOCK_SELECT (3U)
/* Clock divider for flexio clock source */
#define SLAVE_FLEXIO_SPI_CLOCK_PRE_DIVIDER (4U)
/* Clock divider for flexio clock source */
#define SLAVE_FLEXIO_SPI_CLOCK_DIVIDER (7U)        
        
/*LPSPI 主模式*/
/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define MASTER_LPSPI_CLOCK_SELECT (1U)
/* Clock divider for lpspi clock source */
#define MASTER_LPSPI_CLOCK_DIVIDER (7U)
#define MASTER_LPSPI_CLOCK_FREQUENCY (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (MASTER_LPSPI_CLOCK_DIVIDER + 1U))
#define MASTER_LPSPI_PCS_FOR_INIT (kLPSPI_Pcs0)
/*LPSPI 从模式*/
#define SLAVE_LPSPI_BASEADDR (LPSPI1)
#define SLAVE_LPSPI_IRQ_HANDLE (LPSPI1_DriverIRQHandler)
#define SLAVE_LPSPI_IRQN (LPSPI1_IRQn)

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define SLAVE_LPSPI_CLOCK_SELECT (1U)
/* Clock divider for lpspi clock source */
#define SLAVE_LPSPI_CLOCK_DIVIDER (7U)

#define SLAVE_LPSPI_PCS_FOR_INIT (kLPSPI_Pcs0)
#define SLAVE_LPSPI_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)        
#define TRANSFER_SIZE 256U        /*! Transfer dataSize */
/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_INIT(uint8_t masterRxData[TRANSFER_SIZE]) = {0U};
AT_NONCACHEABLE_SECTION_INIT(uint8_t masterTxData[TRANSFER_SIZE]) = {0U};
AT_NONCACHEABLE_SECTION_INIT(uint8_t slaveRxData[TRANSFER_SIZE]) = {0U};
AT_NONCACHEABLE_SECTION_INIT(uint8_t slaveTxData[TRANSFER_SIZE]) = {0U};

flexio_spi_master_edma_handle_t g_m_handle;
FLEXIO_SPI_Type spiDev;


edma_handle_t txHandle;
edma_handle_t rxHandle;

volatile bool isSlaveTransferCompleted = false;
volatile bool isMasterTransferCompleted = false;
bool isMasterIrqInIntmux = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
//FLEXIO  SPI 主模式回调函数  当SPI传输完成，会自动调用该函数 可以自行修改
//void FLEXIO_SPI_MasterUserCallback(FLEXIO_SPI_Type *base,
//                                   flexio_spi_master_edma_handle_t *handle,
//                                   status_t status,
//                                   void *userData)
//{
//    if (status == kStatus_Success)
//    {
//        __NOP();
//    }
//    isMasterTransferCompleted = true;
//}
////FLEXIO  SPI 从模式回调函数  当SPI传输完成，会自动调用该函数 可以自行修改
//void FLEXIO_SPI_SlaveUserCallback(FLEXIO_SPI_Type *base,
//                                  flexio_spi_slave_edma_handle_t *handle,
//                                  status_t status,
//                                  void *userData)
//{
//    if (status == kStatus_Success)
//    {
//        __NOP();
//    }
//
//    isSlaveTransferCompleted = true;
//}
//LPSPI 主模式回调函数  当SPI传输完成，会自动调用该函数 可以自行修改
void LPSPI_MasterUserCallback(LPSPI_Type *base, lpspi_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        __NOP();
    }
    isMasterTransferCompleted = true;
}
//LPSPI 从模式回调函数  当SPI传输完成，会自动调用该函数 可以自行修改
void LPSPI_SlaveUserCallback(LPSPI_Type *base, lpspi_slave_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        __NOP();
    }

    if (status == kStatus_LPSPI_Error)
    {
        __NOP();
    }

    isSlaveTransferCompleted = true;
}
//SPI 引脚初始化  自行修改
void SPI_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

//  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_05_FLEXIO2_FLEXIO05,     0U);                                    
//  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_06_FLEXIO2_FLEXIO06,     0U);                                    
//  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_07_FLEXIO2_FLEXIO07,     0U);                                    
//  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_FLEXIO2_FLEXIO08,     0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_LPSPI1_SCK,        0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_LPSPI1_PCS0,       0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_LPSPI1_SDO,        0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_LPSPI1_SDI,        0U);    
  
                               
  
//  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_05_FLEXIO2_FLEXIO05,0x10B0u);                               
//                                           
//  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_06_FLEXIO2_FLEXIO06,0x10B0u);                               
//                                           
//  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_07_FLEXIO2_FLEXIO07,0x10B0u); 
//  
//  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_08_FLEXIO2_FLEXIO08,0x10B0u);                               
                                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_LPSPI1_SCK,0x10B0u);                               
                                          
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_LPSPI1_PCS0,0x10B0u);                               
                                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_LPSPI1_SDO,0x10B0u);                               
                                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_LPSPI1_SDI,0x10B0u);                               
                                            
}


////初始化FLEXIO SPI 主模式初始化
////baudrate:波特率，单位Mhz
//FLEXIO_SPI_Type spiDev;    //SPI IO口配置结构体
//void FLEXIO_SPI_Master_Init(uint32_t baudrate)
//{
//    SPI_InitPins();    //
//    /* 配置 Flexio 时钟*/
//    CLOCK_SetMux(kCLOCK_Flexio2Mux, MASTER_FLEXIO_SPI_CLOCK_SELECT);    //选择 USB1 PLL (480 MHz) 作为 flexio 时钟源  
//    CLOCK_SetDiv(kCLOCK_Flexio2PreDiv, MASTER_FLEXIO_SPI_CLOCK_PRE_DIVIDER);  // 480M / 5 = 96M
//    CLOCK_SetDiv(kCLOCK_Flexio2Div, MASTER_FLEXIO_SPI_CLOCK_DIVIDER);   //96M / 8 = 12M
//    
//    flexio_spi_master_config_t masterConfig;
//    
//    /* Master config */
//    FLEXIO_SPI_MasterGetDefaultConfig(&masterConfig);  //获取FLEXIO SPI主模式默认配置
//    masterConfig.baudRate_Bps = baudrate;              //波特率
//    masterConfig.dataMode = kFLEXIO_SPI_8BitMode;      //8位数据
//    
//    spiDev.flexioBase = MASTER_FLEXIO_SPI_BASEADDR;    //FLEXIO 
//    spiDev.SDOPinIndex = FLEXIO_SPI_SOUT_PIN;          //SDO   数据输出
//    spiDev.SDIPinIndex = FLEXIO_SPI_SIN_PIN;           //SDI   数据输入
//    spiDev.SCKPinIndex = FLEXIO_SPI_CLK_PIN;           //SCK   时钟
//    spiDev.CSnPinIndex = FLEXIO_SPI_PCS_PIN;           //CS    片选
//    spiDev.shifterIndex[0] = FLEXIO_TX_SHIFTER_INDEX;  //发送移相器
//    spiDev.shifterIndex[1] = FLEXIO_RX_SHIFTER_INDEX;  //接收移相器
//    spiDev.timerIndex[0] = 0U;
//    spiDev.timerIndex[1] = 1U;
//
//    FLEXIO_SPI_MasterInit(&spiDev, &masterConfig, MASTER_FLEXIO_SPI_CLOCK_FREQUENCY);  //初始化
//}
////FLEXIO SPI 从模式初始化
//void FLEXIO_SPI_Slave_Init(void)
//{
//    SPI_InitPins();
//    /* 配置 Flexio 时钟*/
//    CLOCK_SetMux(kCLOCK_Flexio2Mux, SLAVE_FLEXIO_SPI_CLOCK_SELECT);    //选择 USB1 PLL (480 MHz) 作为 flexio 时钟源  
//    CLOCK_SetDiv(kCLOCK_Flexio2PreDiv, SLAVE_FLEXIO_SPI_CLOCK_PRE_DIVIDER);  // 480M / 5 = 96M
//    CLOCK_SetDiv(kCLOCK_Flexio2Div, SLAVE_FLEXIO_SPI_CLOCK_DIVIDER);   //96M / 8 = 12M
//    
//    flexio_spi_slave_config_t slaveConfig;
//    
//    /* Master config */
//    FLEXIO_SPI_SlaveGetDefaultConfig(&slaveConfig);  //获取FLEXIO SPI从模式默认配置
//
//    spiDev.flexioBase = MASTER_FLEXIO_SPI_BASEADDR;    //FLEXIO 
//    spiDev.SDOPinIndex = FLEXIO_SPI_SOUT_PIN;          //SDO   数据输出
//    spiDev.SDIPinIndex = FLEXIO_SPI_SIN_PIN;           //SDI   数据输入
//    spiDev.SCKPinIndex = FLEXIO_SPI_CLK_PIN;           //SCK   时钟
//    spiDev.CSnPinIndex = FLEXIO_SPI_PCS_PIN;           //CS    片选
//    spiDev.shifterIndex[0] = FLEXIO_TX_SHIFTER_INDEX;  //发送移相器
//    spiDev.shifterIndex[1] = FLEXIO_RX_SHIFTER_INDEX;  //接收移相器
//    spiDev.timerIndex[0] = 0U;
//
//    FLEXIO_SPI_SlaveInit(&spiDev, &slaveConfig);  //初始化
//}

//LP SPI 主模式初始化
lpspi_master_config_t masterConfig;
void LP_SPI_Master_Init(LPSPI_Type * base, uint32_t baudrate)
{
    SPI_InitPins();
    /* 配置 LPSPI 时钟*/
    CLOCK_SetMux(kCLOCK_LpspiMux, MASTER_LPSPI_CLOCK_SELECT);  //选择USB1 PLL PFD0 (720 MHz) 作为LPSPI时钟源
    CLOCK_SetDiv(kCLOCK_LpspiDiv, MASTER_LPSPI_CLOCK_DIVIDER); //720M / 8 = 90M
    
    lpspi_master_config_t masterConfig;
    
    /*Master config*/
    masterConfig.baudRate = baudrate;
    masterConfig.bitsPerFrame = 8;                     //设置SPI的数据大小:SPI发送接收8位帧结构
    masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh; //串行同步时钟高电平有效
    masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;    //串行同步时钟的第一个跳变沿（上升或下降）数据被采样
    masterConfig.direction = kLPSPI_MsbFirst;          //MSB在前

    masterConfig.pcsToSckDelayInNanoSec = 10;       //片选拉低到开始传输的时间
    masterConfig.lastSckToPcsDelayInNanoSec = 10;   //最后一个信号到片选拉高时间
    masterConfig.betweenTransferDelayInNanoSec = 10;//两次传输之间的时间

    masterConfig.whichPcs = MASTER_LPSPI_PCS_FOR_INIT;     //片选引脚
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow; //片选信号低电平有效

    masterConfig.pinCfg = kLPSPI_SdiInSdoOut;              //SDI输入引脚，SDO输出引脚
    masterConfig.dataOutConfig = kLpspiDataOutRetained;    //保留输出数据

    LPSPI_MasterInit(base, &masterConfig, MASTER_LPSPI_CLOCK_FREQUENCY);

}
//LP SPI 从模式初始化
void LP_SPI_Slave_Init(LPSPI_Type * base)
{
    SPI_InitPins();
    /* 配置 LPSPI 时钟*/
    CLOCK_SetMux(kCLOCK_LpspiMux, MASTER_LPSPI_CLOCK_SELECT);  //选择USB1 PLL PFD0 (720 MHz) 作为LPSPI时钟源
    CLOCK_SetDiv(kCLOCK_LpspiDiv, MASTER_LPSPI_CLOCK_DIVIDER); //720M / 8 = 90M
    
    lpspi_slave_config_t slaveConfig;
    
    /*Slave config*/
    slaveConfig.bitsPerFrame = 8;
    slaveConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    slaveConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
    slaveConfig.direction = kLPSPI_MsbFirst;

    slaveConfig.whichPcs = SLAVE_LPSPI_PCS_FOR_INIT;
    slaveConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    slaveConfig.pinCfg = kLPSPI_SdiInSdoOut;
    slaveConfig.dataOutConfig = kLpspiDataOutRetained;

    LPSPI_SlaveInit(base, &slaveConfig);

}


////SPI 读写一个字节函数
//uint8_t FLEXIO_SPI_ReadWriteByte(uint8_t txData)
//{
//    uint8_t masterTxData = txData;
//    uint8_t masterRxData = 0;
//    flexio_spi_transfer_t spi_transfer;
//    spi_transfer.txData = &masterTxData;   //发送数组
//    spi_transfer.rxData = &masterRxData;   //接收数组
//    spi_transfer.dataSize = 1;             //长度
//    spi_transfer.flags = kFLEXIO_SPI_8bitMsb;//高八位在前
//    FLEXIO_SPI_MasterTransferBlocking(&spiDev, &spi_transfer);         //堵塞传输
//    return masterRxData;
//}
//SPI 读写一个字节函数
uint8_t LP_SPI_ReadWriteByte(LPSPI_Type * base, uint8_t txData)
{
    uint8_t masterTxData = txData;
    uint8_t masterRxData = 0;
    lpspi_transfer_t spi_tranxfer;
    spi_tranxfer.txData = &masterTxData;   //发送数组
    spi_tranxfer.rxData = &masterRxData;   //接收数组
    spi_tranxfer.dataSize = 1;             //长度
    spi_tranxfer.configFlags = kLPSPI_MasterPcs0|kLPSPI_MasterPcsContinuous;     //片选用PCS0 
    LPSPI_MasterTransferBlocking(base, &spi_tranxfer);         //spi堵塞传输
    return masterRxData;
}

