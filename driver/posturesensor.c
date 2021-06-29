#include "system.h"


extern fxos_handle_t g_fxosHandle;
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】初始化 龙邱九轴模块中的8700
【软件版本】V1.0
【最后更新】2019年03月13日 
【函数名】
【返回值】无 0：初始化成功   1：失败
【参数值】无 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
status_t FXOS_Init(fxos_handle_t *fxos_handle)
{
    uint8_t tmp[1] = {0};
    g_fxosHandle.xfer.slaveAddress = 0x1e;                                   //8700地址
    FXOS_ReadReg(fxos_handle, WHO_AM_I_REG, tmp, 1);                         //读取WHO_AM_I 寄存器

    if (tmp[0] != kFXOS_WHO_AM_I_Device_ID)                                  //判断 WHO_AM_I 的值是否正确
    {
        return kStatus_Fail;
    }


    FXOS_ReadReg(fxos_handle, CTRL_REG1, tmp, 1);                            //读取CTRL_REG1寄存器
    
    FXOS_WriteReg(fxos_handle, CTRL_REG1, tmp[0] & (uint8_t)~ACTIVE_MASK);   //写CTRL_REG1寄存器，使FX8700处于待机状态 

    FXOS_WriteReg(fxos_handle, F_SETUP_REG, F_MODE_DISABLED);                //禁用FIFO

    FXOS_WriteReg(fxos_handle, CTRL_REG2, MOD_HIGH_RES);                     //使用高分辨率模式

    FXOS_WriteReg(fxos_handle, M_CTRL_REG1, (M_RST_MASK | M_OSR_MASK | M_HMS_MASK));//混合模式，加计和地磁计同时使用

    FXOS_WriteReg(fxos_handle, M_CTRL_REG2, (M_HYB_AUTOINC_MASK));           //当hyb_autoinc_mode = 1且快读模式被禁用(ctrl l_reg1 [f_read] = 0)时，在读完寄存器x06 (OUT_Z_LSB)后，寄存器地址将自动前进到寄存器x33 (M_OUT_X_MSB)。对于hyb_autoinc_mode = 1和启用快速读取模式(ctrl l_reg1 [f_read = 1])，在burstread模式下读取寄存器x05 (OUT_Z_MSB)后，寄存器地址将自动前进到寄存器x33 (M_OUT_X_MSB)。

    FXOS_WriteReg(fxos_handle, XYZ_DATA_CFG_REG, FULL_SCALE_4G);             //加计 正负4g模式

    FXOS_WriteReg(fxos_handle, CTRL_REG1, (HYB_DATA_RATE_200HZ | ACTIVE_MASK));//设置数据输出频率 200hz 并且激活FX8700

    FXOS_ReadReg(fxos_handle, CTRL_REG1, tmp, 1);                            //确保FX8700不是在待机状态

    if ((tmp[0] & ACTIVE_MASK) != ACTIVE_MASK)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】读取8700中的加速度和地磁极原始数据
【软件版本】V1.0
【最后更新】2019年03月13日 
【函数名】
【返回值】无 0：初始化成功   1：失败
【参数值】无 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
status_t FXOS_ReadSensorData(fxos_handle_t *fxos_handle, fxos_data_t *sensorData)    //读FX8700原始数据函数
{
    g_fxosHandle.xfer.slaveAddress = 0x1e;   //8700地址
    status_t status = kStatus_Success;
    uint8_t tmp_buff[6] = {0};
    uint8_t i = 0;

    if (!FXOS_ReadReg(fxos_handle, OUT_X_MSB_REG, tmp_buff, 6) == kStatus_Success)  //读取加计数据
    {
        status = kStatus_Fail;
    }

    for (i = 0; i < 6; i++)
    {
        ((int8_t *)sensorData)[i] = tmp_buff[i];                                     //将数据存入缓冲区
    }

    if (!FXOS_ReadReg(fxos_handle, M_OUT_X_MSB_REG, tmp_buff, 6) == kStatus_Success) //读取地磁计数据
    {
        status = kStatus_Fail;
    }

    for (i = 0; i < 6; i++)
    {
        ((int8_t *)sensorData)[i + 6] = tmp_buff[i];                                  //将数据存入缓冲区
    }

    return status;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】读取2100的角速度原始数据
【软件版本】V1.0
【最后更新】2019年03月13日 
【函数名】
【返回值】无 0：初始化成功   1：失败
【参数值】无 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
status_t FXOS2100_ReadSensorData(fxos_handle_t *fxos_handle, fxos2100_data_t *sensorData)  //读FX2100原始数据
{
    g_fxosHandle.xfer.slaveAddress = 0x20;   //2100地址
    status_t status = kStatus_Success;
    uint8_t tmp_buff[6] = {0};
    uint8_t i = 0;

    if (!FXOS_ReadReg(fxos_handle, OUT_X_MSB_REG, tmp_buff, 6) == kStatus_Success)      //读取角速度计数据
    {
        status = kStatus_Fail;
    }

    for (i = 0; i < 6; i++)
    {
        ((int8_t *)sensorData)[i] = tmp_buff[i];                                        //将数据存入缓冲区
    } 
    return status;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】初始化 龙邱九轴模块中的2100
【软件版本】V1.0
【最后更新】2019年03月13日 
【函数名】
【返回值】无 0：初始化成功   1：失败
【参数值】无 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
status_t Init2100(fxos_handle_t *fxos_handle)                                           //角速度计初始化
{
    g_fxosHandle.xfer.slaveAddress = 0x20;   //2100地址
    if(FXOS_WriteReg(fxos_handle, 0x0d, 0x00) != kStatus_Success)  //写CTRL_REG1寄存器 配置高低通滤波 和 量程 ±2000dps
    {
        return kStatus_Fail;
    }
    if(FXOS_WriteReg(fxos_handle, 0x13, 0x02) != kStatus_Success)  //写CTRL_REG1寄存器  启动 和 配置输出速率 800Hz
    {
        return kStatus_Fail;
    }
    return kStatus_Success;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】读寄存器函数
【软件版本】V1.0
【最后更新】2019年03月13日 
【函数名】
【返回值】无 0：初始化成功   1：失败
【参数值】无 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
status_t FXOS_ReadReg(fxos_handle_t *handle, uint8_t reg, uint8_t *val, uint8_t bytesNumber)  //读寄存器函数
{
    return IIC_ReadReg(handle, reg, val, bytesNumber);  //读寄存器函数
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】写寄存器函数
【软件版本】V1.0
【最后更新】2019年03月13日 
【函数名】
【返回值】无 0：初始化成功   1：失败
【参数值】无 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
status_t FXOS_WriteReg(fxos_handle_t *handle, uint8_t reg, uint8_t val)    //写寄存器函数
{
    return IIC_WriteReg(handle, reg, val);    //写寄存器函数
}


void Init_9AX(void)
{
  iic1_write(FXAS21002_ADDR, 0x00,FXAS21002_CTRL_REG1);
  iic1_write(FXAS21002_ADDR, 0x00,FXAS21002_CTRL_REG0);
  delayms(100);  
  iic1_write(FXAS21002_ADDR, 0x03,FXAS21002_CTRL_REG1);
  
  //////FXOS8700///////////////////////////////////////////////////////////////////////////////////////////
  delayms(100);    
  uint8_t val;
  iic1_read(FXOS8700_ADDR, &vall ,FXOS8700_CTRL_REG1);   //读CTRL1寄存器
    
  iic1_write(FXOS8700_ADDR, val & (uint8_t)~ACTIVE_MASK,FXOS8700_CTRL_REG1); //使8700处于待机模式
  iic1_write(FXOS8700_ADDR, 0x03,FXAS21002_CTRL_REG1);
  iic1_write(FXOS8700_ADDR, 0x03,FXAS21002_CTRL_REG1);
  iic1_write(FXOS8700_ADDR, 0x03,FXAS21002_CTRL_REG1);
  iic1_write(FXOS8700_ADDR, 0x03,FXAS21002_CTRL_REG1);
  iic1_write(FXOS8700_ADDR, 0x03,FXAS21002_CTRL_REG1);
  iic1_write(FXOS8700_ADDR, 0x03,FXAS21002_CTRL_REG1);
  iic1_write(FXOS8700_ADDR, 0x03,FXAS21002_CTRL_REG1);
  iic1_write(FXOS8700_ADDR, 0x03,FXAS21002_CTRL_REG1);
  iic1_write(FXOS8700_ADDR, 0x03,FXAS21002_CTRL_REG1);
  
  IIC_WriteByteToSlave(FXOS8700_ADDR, FXOS8700_CTRL_REG1, val & (uint8_t)~ACTIVE_MASK);   //使8700处于待机模式
  IIC_WriteByteToSlave(FXOS8700_ADDR, F_SETUP_REG,F_MODE_DISABLED);    //关FIFO
  IIC_WriteByteToSlave(FXOS8700_ADDR, FXOS8700_M_CTRL_REG2, MOD_HIGH_RES);   //高分辨率模式
  IIC_WriteByteToSlave( FXOS8700_ADDR, M_CTRL_REG1, (M_RST_MASK | M_OSR_MASK | M_HMS_MASK));   //混合模式，加计和地磁计同时使用
  IIC_WriteByteToSlave(FXOS8700_ADDR, M_CTRL_REG2, M_HYB_AUTOINC_MASK); 
  IIC_WriteByteToSlave(FXOS8700_ADDR, XYZ_DATA_CFG_REG, FULL_SCALE_4G);       //加计 正负4g模式
  IIC_WriteByteToSlave(FXOS8700_ADDR, FXOS8700_CTRL_REG1, (HYB_DATA_RATE_200HZ | ACTIVE_MASK));       //设置数据输出频率 200hz 并且激活FX8700
  delayms(10);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】获取 九轴  原始数据
【软件版本】V1.0
【最后更新】2019年03月13日 
【函数名】
【返回值】无 
【参数值】指向存放九轴原始数据的地址 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Update9AX(short *ax,short *ay,short *az,short *gx,short *gy,short *gz, short *mx, short *my, short *mz)
{      
  uint8_t acc_buf[6]; 
  uint8_t mag_buf[6]; 
  uint8_t gyr_buf[6];
  IIC_ReadMultByteFromSlave(FXOS8700_ADDR,0x01,6,acc_buf);
  IIC_ReadMultByteFromSlave(FXOS8700_ADDR,0x33,6,mag_buf); 
  IIC_ReadMultByteFromSlave(FXAS21002_ADDR,0x01,6,gyr_buf);
  
  
  *ax = ((int16_t)((uint16_t)acc_buf[0]<<8 | (uint16_t)acc_buf[1]));  //加速度计14位的，低两位影响不大，直接按16位的用
  *ay = ((int16_t)((uint16_t)acc_buf[2]<<8 | (uint16_t)acc_buf[3]));
  *az = ((int16_t)((uint16_t)acc_buf[4]<<8 | (uint16_t)acc_buf[5]));

  *mx = (int16_t)((uint16_t)mag_buf[0]<<8 | (uint16_t)mag_buf[1]);
  *my = (int16_t)((uint16_t)mag_buf[2]<<8 | (uint16_t)mag_buf[3]);
  *mz = (int16_t)((uint16_t)mag_buf[4]<<8 | (uint16_t)mag_buf[5]);
  
  *gx = (int16_t)((uint16_t)gyr_buf[0]<<8 | (uint16_t)gyr_buf[1]);
  *gy = (int16_t)((uint16_t)gyr_buf[2]<<8 | (uint16_t)gyr_buf[3]);
  *gz = (int16_t)((uint16_t)gyr_buf[4]<<8 | (uint16_t)gyr_buf[5]);
 
}





/********************************************************************************************/

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】获取九轴原始数据
【软件版本】V1.0
【最后更新】2019年03月13日 
【函数名】
【返回值】无 
【参数值】无 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Sensor_ReadData(int16_t DAT[9])
{
    fxos_data_t fxos_data;
    if (FXOS_ReadSensorData(&g_fxosHandle, &fxos_data) != kStatus_Success)             //读取原始数据
    {
        printf("Failed to read acceleration data!\r\n");
    }

    DAT[0] = (int16_t)(((uint16_t)fxos_data.accelXMSB << 8) | fxos_data.accelXLSB);     //加速度计14位的，低两位影响不大，直接按16位的用
    DAT[1] = (int16_t)(((uint16_t)fxos_data.accelYMSB << 8) | fxos_data.accelYLSB);
    DAT[2] = (int16_t)(((uint16_t)fxos_data.accelZMSB << 8) | fxos_data.accelZLSB);
    DAT[3] = (int16_t)(((uint16_t)fxos_data.magXMSB << 8) | fxos_data.magXLSB);         //地磁计16位的
    DAT[4] = (int16_t)(((uint16_t)fxos_data.magYMSB << 8) | fxos_data.magYLSB);
    DAT[5] = (int16_t)(((uint16_t)fxos_data.magZMSB << 8) | fxos_data.magZLSB);

    
    fxos2100_data_t  fxos2100_data;
    if (FXOS2100_ReadSensorData(&g_fxosHandle, &fxos2100_data) != kStatus_Success)      //读取原始数据
    {
        printf("Failed to read acceleration data!\r\n");
    }
    DAT[6] = (int16_t)(((uint16_t)fxos2100_data.gyroXMSB << 8) | fxos2100_data.gyroXLSB);//角速度计 16位的
    DAT[7] = (int16_t)(((uint16_t)fxos2100_data.gyroYMSB << 8) | fxos2100_data.gyroYLSB);
    DAT[8] = (int16_t)(((uint16_t)fxos2100_data.gyroZMSB << 8) | fxos2100_data.gyroZLSB);
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】九轴模块测试例程
【软件版本】V1.0
【最后更新】2019年03月19日 
【函数名】
【返回值】无
【参数值】无 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

void Test_9AX(void)
{
    int16_t u16data[9];          //存放读取传感器 数据
    oled.init();

    oled.ops->word(15,0,"LQ 9AX Test");

    //LQ_init9AX();                //九轴初始化(使用硬件IIC)
    Init_9AX();                   //九轴初始化（使用模拟iic）
    
    char txt[16];
    for (;;)
    {       
      printf("\r\n\r\n ");

//      Sensor_ReadData(u16data); // Read sensor data  (使用硬件IIC)
      
      Update9AX(&u16data[0], &u16data[1], &u16data[2], &u16data[3], &u16data[4], &u16data[5], &u16data[6], &u16data[7], &u16data[8]);//获取九轴数据
     
      sprintf(txt,"AX:%5d ",(int16_t)u16data[0]); 
      oled.ops->word(0,2,txt);
      sprintf(txt,"AY:%5d ",(int16_t)u16data[1]); 
      oled.ops->word(0,3,txt); 
      sprintf(txt,"AZ:%5d ",(int16_t)u16data[2]); 
      oled.ops->word(0,4,txt);
      sprintf(txt,"MX:%5d ",(int16_t)u16data[3]);  
      oled.ops->word(0,5,txt);
      sprintf(txt,"MY:%5d ",(int16_t)u16data[4]); 
      oled.ops->word(0,6,txt);
      sprintf(txt,"MZ:%5d ",(int16_t)u16data[5]);
      oled.ops->word(0,7,txt);
      sprintf(txt,"GX:%5d ",(int16_t)u16data[6]);  
      oled.ops->word(60,5,txt);
      sprintf(txt,"GY:%5d ",(int16_t)u16data[7]);
      oled.ops->word(60,6,txt); 
      sprintf(txt,"GZ:%5d ",(int16_t)u16data[8]); 
      oled.ops->word(60,7,txt);

      delayms(100);//延时100毫秒   
    } /* End infinite loops */
}



