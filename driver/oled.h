#ifndef _OLED_H
#define _OLED_H
#include "system.h"


/*------------------------            OLED硬件接口       ---------------------*/

/*-------------- 龙邱母版引脚 --------------*/
//IO复用定义
#define LCD_DC_PINMUX   IOMUXC_GPIO_B0_15_GPIO2_IO15
#define LCD_RST_PINMUX  IOMUXC_GPIO_B1_04_GPIO2_IO20
#define LCD_SDA_PINMUX  IOMUXC_GPIO_B1_05_GPIO2_IO21
#define LCD_SCL_PINMUX  IOMUXC_GPIO_B1_03_GPIO2_IO19
//使用的GPIO定义
#define LCD_DC_GPIO     GPIO2
#define LCD_RST_GPIO    GPIO2
#define LCD_SDA_GPIO    GPIO2
#define LCD_SCL_GPIO    GPIO2
//使用的管脚定义
#define LCD_DC_PIN      15
#define LCD_RST_PIN     20
#define LCD_SDA_PIN     21
#define LCD_SCL_PIN     19


/*-------------- 重新改引脚 --------------*/
/*  IO复用定义  */
//#define LCD_DC_PINMUX   IOMUXC_GPIO_B0_00_GPIO2_IO00
//#define LCD_RST_PINMUX  IOMUXC_GPIO_B0_03_GPIO2_IO03
//#define LCD_SDA_PINMUX  IOMUXC_GPIO_B0_10_GPIO2_IO10
//#define LCD_SCL_PINMUX  IOMUXC_GPIO_B0_13_GPIO2_IO13
///*  使用的GPIO定义  */
//#define LCD_DC_GPIO     GPIO2
//#define LCD_RST_GPIO    GPIO2
//#define LCD_SDA_GPIO    GPIO2
//#define LCD_SCL_GPIO    GPIO2
///* 使用的管脚定义  */
//#define LCD_DC_PIN      00
//#define LCD_RST_PIN     03
//#define LCD_SDA_PIN     10
//#define LCD_SCL_PIN     13


/*---------------------------- 其他位置不要改！！！！！ ----------------------*/


/* oled.c 中用到的宏定义 */
#define LCD_DC(n)   GPIO_PinWrite(LCD_DC_GPIO,  LCD_DC_PIN, n)
#define LCD_RST(n)	GPIO_PinWrite(LCD_RST_GPIO, LCD_RST_PIN, n)
#define LCD_SDA(n)	GPIO_PinWrite(LCD_SDA_GPIO, LCD_SDA_PIN, n)
#define LCD_SCL(n)  GPIO_PinWrite(LCD_SCL_GPIO, LCD_SCL_PIN, n)


/* 外部接口函数 */
void LCD_WrDat(unsigned char data);
void LCD_WrCmd(unsigned char cmd);
void LCD_Set_Pos(unsigned char x, unsigned char y);
void LCD_CLS(void);
void LCD_Init(void);
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_njust(void);
void LCD_Show_Frame94(void);
void LCD_PrintFloat(unsigned char x,unsigned char y,float num);

#endif

