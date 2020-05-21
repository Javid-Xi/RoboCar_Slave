/**
************************************************************
* @file         ws2812.c
* @brief        ws2812延时控制（已不用）
* @author       Javid
* @date         2020-05-16
* @version      1.0
***********************************************************/

#include "ws2812.h"
#include "delay.h"

/*************************************************
* Function: WS2812_Init
* Description: SPI2初始化	
* Parameter: none
* Return: none
* Note: 配置成主机模式，访问SD Card/W25Q64/NRF24L01
*************************************************/
void WS2812_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO口速度为50MHz

    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0,1

    GPIO_ResetBits(GPIOB, GPIO_Pin_7); //PB7 输出低

}

/*************************************************
* Function: ResetDataFlow
* Description: 复位，为下一次发送做准备	
* Parameter: none
* Return: none
* Note: 将DI置位为0后，延时约60us
*************************************************/
void ResetDataFlow(void)
{
    DI = 0;					//DI置为0后，延时50us以上，实现帧复位
    delay_us(60);
}

/*************************************************
* Function: SendOnePix(unsigned char *ptr)
* Description: 发送一个像素点的24bit数据	
* Parameter: 接收的参数是一个指针，此函数会将此指针指向的地址的连续的三个Byte的数据发送
* Return: none
* Note: 在主函数中直接调用此函数时，在整个帧发送开始前需要先执行 ResetDataFlow()
*		数据是按归零码的方式发送，速率为800KBPS
*************************************************/
void SendOnePix(unsigned char *ptr)
{
    unsigned char i, j;
    unsigned char temp;

    for(j = 0; j < 3; j++)
    {
        temp = ptr[j];
        for(i = 0; i < 8; i++)
        {
            if(temp & 0x80)		 //从高位开始发送
            {
                DI = 1;			 //发送“1”码
                delay_ns(7); //延时6*125ns=750ns
                DI = 0;
                delay_ns(3);//延时375ns
            }
            else				//发送“0”码
            {
                DI = 1;
                delay_ns(3);//延时375ns
                DI = 0;
                delay_ns(7);//延时6*125ns=750ns
            }
            temp = (temp << 1);		 //左移位
        }
    }
}

/*************************************************
* Function: SendOneFrame(unsigned char *ptr)
* Description: 发送一帧数据（即发送整个数组的数据）
* Parameter: 接收的参数是一个指针，此函数会将此指针指向的地址的整个数组的数据发送
* Return: none
* Note: 在主函数中直接调用此函数时，在整个帧发送开始前需要先执行 ResetDataFlow()
*		数据是按归零码的方式发送，速率为800KBPS
*************************************************/
void SendOneFrame(unsigned char *ptr)
{
    unsigned char k;

    ResetDataFlow();	//发送帧复位信号

    for(k = 0; k < SNUM; k++)	//发送一帧数据，SNUM是板子LED的个数
        SendOnePix(&ptr[(3 * k)]);

    ResetDataFlow();	 //发送帧复位信号
}

/*************************************************
* Function: SendSameColor(unsigned char *ptr,unsigned char cnt)
* Description: 相同颜色的点发送cnt次
* Parameter: 接收的参数是一个指针，指向像素点颜色数组，cnt传递发送个数
* Return: none
* Note: 在主函数中直接调用此函数时，在整个帧发送开始前需要先执行 ResetDataFlow()
*		数据是按归零码的方式发送，速率为800KBPS
*************************************************/
void SendSameColor(unsigned char *ptr, unsigned char cnt)
{
    unsigned char k;

    ResetDataFlow();				 //发送帧复位信号

    for(k = 0; k < cnt; k++)				 //发送一帧数据，SNUM是板子LED的个数
    {
        SendOnePix(&ptr[0]);
    }

    ResetDataFlow();				 //发送帧复位信号
}

/*************************************************
* Function: SendOneFrameFrom(unsigned char i,unsigned char *ptr)
* Description: 从指定的像素点开始发送一帧数据（即发送整个数组的数据）
* Parameter: 接收的参数是一个指针，此函数会将此指针指向的地址的整帧数据发送
*		     i:把数组的第0个像素数据发送到第i个像素点（第0个像素是板上标号为01的像素）
* Return: none
* Note: 即原本对应第一个像素的数据会发送到第i个像素点（LED）上
*************************************************/
void SendOneFrameFrom(unsigned char i, unsigned char *ptr)
{
    unsigned char k;

    ResetDataFlow();				 //发送帧复位信号

    for(k = (SNUM - i); k < SNUM; k++)		 //发送一帧数据
    {
        SendOnePix(&ptr[(3 * k)]);
    }
    for(k = 0; k < (SNUM - i); k++)
    {
        SendOnePix(&ptr[(3 * k)]);
    }

    ResetDataFlow();				 //发送帧复位信号
}

/*************************************************
* Function: SendOneFrameSince(unsigned char i,unsigned char *ptr)
* Description: 从指定的像素点开始发送一帧数据（即发送整个数组的数据）
* Parameter: 接收的参数是一个指针，此函数会将此指针指向的地址的整帧数据发送
*		     i:把数组的第0个像素数据发送到第i个像素点（第0个像素是板上标号为01的像素）
* Return: none
* Note: 即原本对应第i像素的数据会发送到第1个像素点（LED）上，
*		第i+1个像素点的数据发送到第2个像素上
*************************************************/
void SendOneFrameSince(unsigned char i, unsigned char *ptr)
{
    unsigned char k;

    ResetDataFlow();				 //发送帧复位信号

    for(k = i; k < SNUM; k++)				 //发送一帧数据
    {
        SendOnePix(&ptr[(3 * k)]);
    }
    for(k = 0; k < i; k++)
    {
        SendOnePix(&ptr[(3 * k)]);
    }
    ResetDataFlow();				 //发送帧复位信号
}
