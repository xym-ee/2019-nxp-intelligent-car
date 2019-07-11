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

/* Includes ------------------------------------------------------------------*/
#include "iic.h"
#include "system.h"


/* Global Variable Definition -----------------------------------------------*/
lpi2c_master_config_t       lpi2c1_config;              		//I2C1(主机)配置结构体
lpi2c_master_handle_t       lpi2c1_handle;              		//I2C1(主机)句柄
lpi2c_master_transfer_t     iic_config; 										//iic传输结构体配置
volatile bool 				lpi2c1_compflag = false;        //完成标志
volatile bool 				lpi2c1_nakflag 	= false;        //nak标志


/* Function Definition ------------------------------------------------------*/

/*!
 * @brief LPI2C1完成回调函数
 *
 * @param base GPIO base pointer.
 * @param pin GPIO port pin number.
 * @retval GPIO port input value.
 */
void LPI2C1_Mastercallback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData)
{
    //数据传输完成
    if(status==kStatus_Success)
        lpi2c1_compflag = true;
    if (status==kStatus_LPI2C_Nak)
        lpi2c1_nakflag = true;
}

/*!
 * @brief LPI2C1初始化
 *
 * @param 波特率选择,400000 3400000
 */
void LPI2C1_Init(uint32_t baudrate)
{
	uint32_t i2cclk = 0;

	//设置I2C的时钟源为USB1(PLL3)=480/8Mhz=60Mhz
	CLOCK_SetMux(kCLOCK_Lpi2cMux,0);   //LPI2C时钟源为PLL3=60Mhz
	CLOCK_SetDiv(kCLOCK_Lpi2cDiv,5);   //分频设置，分频值为5+1，所以LPI2C CLK=10Mhz

	//配置IO作为I2C
	IOMUXC_SetPinMux(IIC1_SCL_PINMUX,1); 
	IOMUXC_SetPinMux(IIC1_SDA_PINMUX,1); 

	//配置I2C引脚功能，
	//低转换速度,驱动能力为R0/6,速度为100Mhz，开路输出，使能pull/keepr
	//选择keeper功能，上拉22K Ohm，关闭Hyst
	IOMUXC_SetPinConfig(IIC1_SCL_PINMUX,0xD8B0); 
	IOMUXC_SetPinConfig(IIC1_SDA_PINMUX,0xD8B0); 

	i2cclk=((CLOCK_GetFreq(kCLOCK_Usb1PllClk)/8)/(5+1)); 

	//配置I2C1作为I2C主机
	LPI2C_MasterGetDefaultConfig(&lpi2c1_config);       //先配置为默认配置
	lpi2c1_config.baudRate_Hz = baudrate;                 //设置I2C速率
	LPI2C_MasterInit(LPI2C1,&lpi2c1_config,i2cclk);     //初始化I2C1 
  
	/*如果使用IIC非阻塞传输的话需要设置handler
	LPI2C_MasterTransferCreateHandle(LPI2C1,&lpi2c1_handle,LPI2C1_Mastercallback, NULL);
	*/
}


/*!
 * @brief I2C1读字节
 *
 * @param 读寄存器地址
 * @param 存储变量的指针
 * @param 设备地址
 * @retval 读取状态
 */
uint8_t iic1_read(uint16_t ReadAddr, uint8_t *val ,uint8_t addr)
{				  
	uint8_t status = 0;
	/* 配置传输结构体 */
	iic_config.slaveAddress     =	addr;     					//设备地址
	iic_config.direction        =	kLPI2C_Read;        //读数据
	iic_config.subaddress       =	(uint32_t)ReadAddr;      //要读取的寄存器地址
	iic_config.subaddressSize   =	1;              		//子地址长度
	iic_config.data             =	val;                //数据缓冲区
	iic_config.dataSize         =	1;                  //要读取的数据长度
	iic_config.flags            =	kLPI2C_TransferDefaultFlag;
	if(LPI2C_MasterTransferBlocking(LPI2C1, &iic_config) == kStatus_Fail)
		status = 1;
	return status;
}

/*!
 * @brief I2C1写字节
 *
 * @param 写寄存器地址
 * @param 写入的数据
 * @param 设备地址
 * @param 读取状态
 */
uint8_t iic1_write(uint8_t WriteAddr,uint8_t DataToWrite,uint8_t addr)
{	
	uint8_t status      = 0;
	uint8_t writedata	= DataToWrite;

	/* 配置传输结构体 */
	iic_config.slaveAddress     =	addr;     						//设备地址
	iic_config.direction        =	kLPI2C_Write;        	//写入数据
	iic_config.subaddress       =	(uint8_t)WriteAddr;    		//要读取的寄存器地址
	iic_config.subaddressSize   =	1;              			//地址长度一个字节
	iic_config.data             =	&writedata;           //要写入的数据
	iic_config.dataSize	        =	1;                    //写入数据长度1个字节
	iic_config.flags            =	kLPI2C_TransferDefaultFlag;
	if(LPI2C_MasterTransferBlocking(LPI2C1, &iic_config) == kStatus_Fail)
		status=1;
	return status;	 
}



status_t IIC_WriteData(LPI2C_Type *i2c, uint8_t DevAddr, uint8_t addrType, uint32_t RegAddr, void* value, uint32_t len)
{
    uint8_t Add[2];			//16位寄存器地址分割
    uint8_t size = 0;

    if (16 == addrType)
    {
        Add[size++] = (RegAddr >> 8U) & 0xFFU;	//16位地址类型，data[0]取高8位
    }
    Add[size++] = (uint8_t)(RegAddr);						//data[1]取低8位，如果不是直接取低8位
    status_t status;														//状态标志
    while (1)
    {
        status = LPI2C_MasterStart(i2c, DevAddr, kLPI2C_Write);			//起始信号 写设备地址
        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }
    LPI2C_MasterSend(i2c, Add, size);			//i2c主机发送寄存器地址
    LPI2C_MasterSend(i2c, value, len);		    //i2c主机发送数据
    return LPI2C_MasterStop(i2c);				//停止信号返回状态
}

status_t IIC_ReadData(LPI2C_Type *i2c, uint8_t DevAddr, uint8_t addrType, uint32_t RegAddr, void* value, uint32_t len)
{
    uint8_t Add[2];	//16位地址分割
    uint8_t size = 0;

    if (16 == addrType)
    {
        Add[size++] = (RegAddr >> 8U) & 0xFFU;
    }

    Add[size++] = (uint8_t)(RegAddr);

    status_t status;

    while (1)
    {
        status = LPI2C_MasterStart(i2c, DevAddr, kLPI2C_Write);//起始信号 写设备地址
        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }
    LPI2C_MasterSend(i2c, Add, size);				    //写寄存器地址
    LPI2C_MasterStop(i2c);							    //停止信号
    LPI2C_MasterStart(i2c, DevAddr, kLPI2C_Read);	 	//起始信号 读设备地址
    LPI2C_MasterReceive(i2c,(void*) value, len);		//接收数据    
    return LPI2C_MasterStop(i2c);				        //停止信号
}

/*------------------------14th NJSUT 2019--------------------END OF FILE------*/


