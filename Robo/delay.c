/**
************************************************************
* @file         delay.c
* @brief        延时
* @author       Javid
* @date         2019-02-20
* @version      1.0
*
***********************************************************/
#include "delay.h"

static u8  fac_ns = 0;							//us延时倍乘数
static u8  fac_us = 0;							//us延时倍乘数
static u16 fac_ms = 0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数

/*************************************************
* Function: delay_init
* Description: 初始化延迟函数	
* Parameter: freq_khz:PWM输出频率，范围1~20,单位KHz
* Return: none
* Note: SYSTICK的时钟固定为HCLK时钟的1/8
*		SYSCLK:系统时钟
*************************************************/
void delay_init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	fac_ns = SystemCoreClock / 1000000;				//为系统时钟SYSCLK
    fac_us = SystemCoreClock / 8000000;				//为系统时钟SYSCLK的1/8 这里即等于9
    fac_ms = (u16)fac_us * 1000;					//非OS下,代表每个ms需要的systick时钟数
}

/*************************************************
* Function: delay_ns
* Description: 延时纳秒	
* Parameter: nus
* Return: none
* Note: 参数范围1-8 1为125ns以此类推
*************************************************/
void delay_ns(u8 nns)
{
    u32 temp;
    SysTick->LOAD = nns * fac_ns; 					//时间加载
    SysTick->VAL = 0x00;        					//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//开始倒数
    do
    {
        temp = SysTick->CTRL;
    } while((temp & 0x01) && !(temp & (1 << 16)));		//等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
    SysTick->VAL = 0X00;      					 //清空计数器
}

/*************************************************
* Function: delay_us
* Description: 延时微秒	
* Parameter: nus
* Return: none
*************************************************/	    
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us; 					//时间加载
    SysTick->VAL = 0x00;        					//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//开始倒数
    do
    {
        temp = SysTick->CTRL;
    } while((temp & 0x01) && !(temp & (1 << 16)));		//等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
    SysTick->VAL = 0X00;      					 //清空计数器
}

/*************************************************
* Function: delay_ms
* Description: 延时毫秒
* Parameter: nms
* Return: none
* Note:	注意nms的范围
*		SysTick->LOAD为24位寄存器,所以,最大延时为:
*		nms<=0xffffff*8*1000/SYSCLK
*		SYSCLK单位为Hz,nms单位为ms
*		对72M条件下,nms<=1864 
*************************************************/	
void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms;				//时间加载(SysTick->LOAD为24bit)
    SysTick->VAL = 0x00;							//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//开始倒数
    do
    {
        temp = SysTick->CTRL;
    } while((temp & 0x01) && !(temp & (1 << 16)));		//等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
    SysTick->VAL = 0X00;       					//清空计数器
}








































