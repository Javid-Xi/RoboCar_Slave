#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//SPI驱动 代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

typedef struct {
    char G;
    char R;
    char B;
} _GRB;

extern _GRB SPI2Buff[8];

void SPI1_Init(void);
void SPI2_Init(void);			 //初始化SPI口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
void write1pix(char g, char r, char b);
void write8pix(_GRB* buf);

void RR(void);
void RL(void);

void LED_Circulate(void);//循环闪烁
void ADD_Color(uint8_t R_div, uint8_t G_div, uint8_t B_div);
void LED_Red_BLN(void);
void LED_Green_BLN(void);
void LED_Blue_BLN(void);
void LED_RGB_BLN(void);


#endif

