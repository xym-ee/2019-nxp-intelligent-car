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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _I2C_H
#define _I2C_H


/* Includes ------------------------------------------------------------------*/
#include "system.h"

#ifdef MY_PCB

#define IIC1_SCL_PINMUX IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL   //J11
#define IIC1_SDA_PINMUX IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA   //K11 换成N3无法使用

#else

#define IIC1_SCL_PINMUX IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL
#define IIC1_SDA_PINMUX IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA

#endif



/*! @brief SCCB register address type. */
typedef enum _sccb_reg_addr
{
    kSCCB_RegAddr8Bit,  /*!< 8-bit register address. */
    kSCCB_RegAddr16Bit, /*!< 16-bit register address. */
} sccb_reg_addr_t;


/* Variable Declarations -----------------------------------------------------*/
extern lpi2c_master_config_t    lpi2c1_config;      //I2C1(主机)配置结构体
extern lpi2c_master_handle_t    lpi2c1_handle;      //I2C1(主机)句柄
extern volatile bool            lpi2c1_compflag;    //完成标志
extern volatile bool            lpi2c1_nakflag;     //nak标志



/* Function Declarations -----------------------------------------------------*/

/*!
 * @brief LPI2C1初始化
 *
 * @param 波特率选择,400000 3400000
 */
void LPI2C1_Init(uint32_t baudrate);

/*!
 * @brief I2C1读字节
 *
 * @param 读寄存器地址
 * @param 存储变量的指针
 * @param 设备地址
 * @retval 读取状态
 */
uint8_t iic1_read(uint16_t ReadAddr, uint8_t *val ,uint8_t addr);

/*!
 * @brief I2C1写字节
 *
 * @param 写寄存器地址
 * @param 写入的数据
 * @param 设备地址
 * @param 读取状态
 */
uint8_t iic1_write(uint8_t WriteAddr,uint8_t DataToWrite,uint8_t addr);
/*!
 * @brief Write value to the SCCB register.
 *
 * @param i2c SCCB I2C port.
 * @param i2cAddr SCCB I2C address.
 * @param addrType SCCB register address type.
 * @param reg The register to write.
 * @param value The value to write.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */


void IIC_Config(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(unsigned char ack);



status_t IIC_ReadData(LPI2C_Type *i2c, uint8_t DevAddr, uint8_t addrType, uint32_t RegAddr, void* value, uint32_t len);
status_t IIC_WriteData(LPI2C_Type *i2c, uint8_t DevAddr, uint8_t addrType, uint32_t RegAddr, void* value, uint32_t len);



#endif	/* _I2C_H */

/*------------------------14th NJSUT 2019--------------------END OF FILE------*/




