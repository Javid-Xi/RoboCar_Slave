/**
************************************************************
* @file         spi.c
* @brief        spi MOSI信号线控制WS2812
* @author       Javid
* @date         2020-05-16
* @version      1.0
* @Note			JTAG SWD设置
***********************************************************/

#include "spi.h"
#include "delay.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"

////写入颜色的buffer
_GRB SPI2Buff[8] = {{255, 0, 0}, {255, 165, 0}, {255, 255, 0}, {0, 255, 0}, {0, 127, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}};
//_GRB SPI2Buff[8] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
//spi MOSI信号线高低电平时间控制
#define code1 0xf8 //写1
#define code0 0xc0 //写0

/*************************************************
* Function: SPI1_Init
* Description: SPI2初始化
* Parameter: none
* Return: none
* Note: 配置成主机模式，访问SD Card/W25Q64/NRF24L01
*************************************************/
void SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
//    DMA_InitTypeDef DMA_InitStructure;

    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE ); //PORTB时钟使能
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI2时钟使能
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
    SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_Cmd(SPI1, ENABLE); //使能SPI外设

//    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

//    DMA_InitStructure.DMA_BufferSize = 3 * 8 * 24;
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//    //DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)SPI2Buff[0];
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;;
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
//    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

//    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
//    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
//    DMA_Cmd(DMA1_Channel5, ENABLE);

//    SPI2_ReadWriteByte(0xff);//启动传输

}

/*************************************************
* Function: SPI2_Init
* Description: SPI2初始化
* Parameter: none
* Return: none
* Note: 配置成主机模式，访问SD Card/W25Q64/NRF24L01
*************************************************/
void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
//    DMA_InitTypeDef DMA_InitStructure;

    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE ); //PORTB时钟使能
    RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

    GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); //PB13/14/15上拉

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
    SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_Cmd(SPI2, ENABLE); //使能SPI外设

//    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

//    DMA_InitStructure.DMA_BufferSize = 3 * 8 * 24;
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//    //DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)SPI2Buff[0];
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;;
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
//    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

//    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
//    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
//    DMA_Cmd(DMA1_Channel5, ENABLE);

//    SPI2_ReadWriteByte(0xff);//启动传输

}

/*************************************************
* Function: SPI2_SetSpeed
* Description: SPI 速度设置函数
* Parameter: SPI_BaudRatePrescaler:
*			 SPI_BaudRatePrescaler_2   2分频
*			 SPI_BaudRatePrescaler_8   8分频
*			 SPI_BaudRatePrescaler_16  16分频
*			 SPI_BaudRatePrescaler_256 256分频
* Return: none
*************************************************/
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPI2->CR1 &= 0XFFC7;
    SPI2->CR1 |= SPI_BaudRatePrescaler;	//设置SPI2速度
    SPI_Cmd(SPI2, ENABLE);

}

/*************************************************
* Function: SPI2_SetSpeed
* Description: SPIx 读写一个字节
* Parameter: TxData:要写入的字节
* Return: 读取到的字节
*************************************************/
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
        retry++;
        if(retry > 200)return 0;
    }
    SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
    retry = 0;
    return 1;
}

/*************************************************
* Function: write1pix
* Description: 写一个灯点数据
* Parameter: r,g,b数值
* Return: none
*************************************************/
void write1pix(char g, char r, char b)
{
    int i;
    for(i = 0; i < 8; i++) {

        if(g & 0x80) {
            SPI2_ReadWriteByte(code1);
        } else {
            SPI2_ReadWriteByte(code0);
        }
        g = g << 1;
    }
    for(i = 0; i < 8; i++) {
        if(r & 0x80) {
            SPI2_ReadWriteByte(code1);
        } else {
            SPI2_ReadWriteByte(code0);
        }
        r = r << 1;
    }
    for(i = 0; i < 8; i++) {
        if(b & 0x80) {
            SPI2_ReadWriteByte(code1);
        } else {
            SPI2_ReadWriteByte(code0);
        }
        b = b << 1;
    }
}

/*************************************************
* Function: write8pix
* Description: 写8个灯点数据
* Parameter: RGB数组
* Return: none
*************************************************/
void write8pix(_GRB* buf)
{
    int i, j;
    for(i = 0; i < 100; i++) {
        while(!SPI2_ReadWriteByte(0X0));
    }
    for(j = 0; j < 8; j++) {
        write1pix(buf->G, buf->R , buf->B);
        buf++;
    }
}

/*************************************************
* Function: RL
* Description: SPI2Buff左循环移位
* Parameter: none
* Return: none
*************************************************/
void RL(void)
{
    int i;
    _GRB tem = SPI2Buff[0];

    for(i = 1; i < 8; i++)
        SPI2Buff[i - 1] = SPI2Buff[i];

    SPI2Buff[7] = tem;
}

/*************************************************
* Function: RR
* Description: SPI2Buff右循环移位
* Parameter: none
* Return: none
*************************************************/
void RR(void)
{
    int i;
    _GRB tem = SPI2Buff[7];

    for(i = 7; i > 0; i--)
        SPI2Buff[i] = SPI2Buff[i - 1];

    SPI2Buff[0] = tem;
}

/*************************************************
* Function: LED_Circulate
* Description: LED灯环循环
* Parameter: none
* Return: none
*************************************************/
void LED_Circulate(void)
{
    static u8 cout = 0;
    write8pix(SPI2Buff);
    delay_ms(100);
    if(cout < 100)
    {
        RL();
        cout++;
    }
    else
    {
        RR();
        cout++;
        if(cout == 200)
            cout = 0;

    }
}

/*************************************************
* Function: ADD_COLOR
* Description: 写入颜色
* Parameter: RGB三色值
* Return: none
*************************************************/
void ADD_Color(uint8_t R_div, uint8_t G_div, uint8_t B_div)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
        SPI2Buff[i].R += R_div;

    for(i = 0; i < 8; i++)
        SPI2Buff[i].G += G_div;

    for(i = 0; i < 8; i++)
        SPI2Buff[i].B += B_div;
}

/*************************************************
* Function: LED_Red_BLN
* Description: 红色呼吸灯
* Parameter: none
* Return: none
*************************************************/
void LED_Red_BLN(void)
{
    uint8_t i,j;
    for(j = 0; j <= 255; j++)
    {
        for(i = 0; i < 8; i++)
        SPI2Buff[i].R += j;
		
		write8pix(SPI2Buff);
        delay_ms(100);
    }
//	delay_ms(500);
    for(j = 255; j > 0; j--) 
	{
         for(i = 0; i < 8; i++)
        SPI2Buff[i].R += j;
		
		write8pix(SPI2Buff);
        delay_ms(100);
    }
}

/*************************************************
* Function: LED_Green_BLN
* Description: 绿色呼吸灯
* Parameter: none
* Return: none
*************************************************/
void LED_Green_BLN(void)
{
    uint8_t i;
    for(i = 0; i <= 255; i++)
    {
        ADD_Color(0, i, 0);
		write8pix(SPI2Buff);
        delay_ms(50);
    }
    for(i = 255; i > 0; i--) {
        ADD_Color(0, i, 0);
		write8pix(SPI2Buff);
        delay_ms(50);
    }
}

/*************************************************
* Function: LED_Blue_BLN
* Description: 蓝色呼吸灯
* Parameter: none
* Return: none
*************************************************/
void LED_Blue_BLN(void)
{
    uint8_t i;
    for(i = 0; i <= 255; i++)
    {
        ADD_Color(0, 0, i);
		write8pix(SPI2Buff);
        delay_ms(100);
    }
    for(i = 255; i > 0; i--) {
        ADD_Color(0, 0, i);
		write8pix(SPI2Buff);
        delay_ms(100);
    }
}


/*************************************************
* Function: LED_RGB_BLN
* Description: 三色呼吸灯
* Parameter: none
* Return: none
*************************************************/
void LED_RGB_BLN(void)
{
    LED_Red_BLN();
    LED_Green_BLN();
    LED_Blue_BLN();
}



