/*  <The 14th National University Students Intelligent Car Race.>
 *  Copyright (C) <2019>  < github.com/Meng2015 >
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

#ifndef _OLED_H
#define _OLED_H
#include "system.h"


/*------------------------            OLED硬件接口       ---------------------*/

#ifdef  MY_PCB
/*  IO复用定义  */
#define LCD_DC_PINMUX   IOMUXC_GPIO_B0_15_GPIO2_IO15
#define LCD_RST_PINMUX  IOMUXC_GPIO_B1_04_GPIO2_IO20
#define LCD_SDA_PINMUX  IOMUXC_GPIO_B1_05_GPIO2_IO21
#define LCD_SCL_PINMUX  IOMUXC_GPIO_B1_03_GPIO2_IO19

#define LCD_DC_GPIO     GPIO2
#define LCD_RST_GPIO    GPIO2
#define LCD_SDA_GPIO    GPIO2
#define LCD_SCL_GPIO    GPIO2

#define LCD_DC_PIN      15
#define LCD_RST_PIN     20
#define LCD_SDA_PIN     21
#define LCD_SCL_PIN     19


/*-------------- 龙邱母版引脚 --------------*/
#else

#define LCD_DC_PINMUX   IOMUXC_GPIO_B0_15_GPIO2_IO15
#define LCD_RST_PINMUX  IOMUXC_GPIO_B1_04_GPIO2_IO20
#define LCD_SDA_PINMUX  IOMUXC_GPIO_B1_05_GPIO2_IO21
#define LCD_SCL_PINMUX  IOMUXC_GPIO_B1_03_GPIO2_IO19

#define LCD_DC_GPIO     GPIO2
#define LCD_RST_GPIO    GPIO2
#define LCD_SDA_GPIO    GPIO2
#define LCD_SCL_GPIO    GPIO2

#define LCD_DC_PIN      15
#define LCD_RST_PIN     20
#define LCD_SDA_PIN     21
#define LCD_SCL_PIN     19

#endif

/*-------------- 重新改引脚 --------------*/



/*---------------------------- 其他位置不要改！！！！！ ----------------------*/

#define OLED_WIDTH 132
#define OLED_HIGH 64


/* oled.c 中用到的宏定义 */
#define LCD_DC(n)   GPIO_PinWrite(LCD_DC_GPIO,  LCD_DC_PIN, n)
#define LCD_RST(n)	GPIO_PinWrite(LCD_RST_GPIO, LCD_RST_PIN, n)
#define LCD_SDA(n)	GPIO_PinWrite(LCD_SDA_GPIO, LCD_SDA_PIN, n)
#define LCD_SCL(n)  GPIO_PinWrite(LCD_SCL_GPIO, LCD_SCL_PIN, n)


typedef struct _oled_operations oled_operations_t;  

struct _oled_operations
{
    void (*data)(uint8_t data);
    void (*cmd)(uint8_t cmd);
    void (*position)(uint8_t x,uint8_t y);
    void (*clear)(void);
    void (*word)(uint8_t x,uint8_t y,unsigned char ch[]);
    void (*logo)(void);
};


typedef struct _oled_device oled_device_t;

struct _oled_device
{
    void (*init)(void);
    const oled_operations_t *ops;
};





extern const oled_device_t oled;



/* 外部接口函数 */

void LCD_WrCmd(unsigned char cmd);
void LCD_Set_Pos(unsigned char x, unsigned char y);

void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_njust(void);
void LCD_Show_Frame94(void);
void LCD_PrintFloat(unsigned char x,unsigned char y,float num);

#endif

