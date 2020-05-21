/**
************************************************************
* @file         ws2812.c
* @brief        ws2812��ʱ���ƣ��Ѳ��ã�
* @author       Javid
* @date         2020-05-16
* @version      1.0
***********************************************************/

#include "ws2812.h"
#include "delay.h"

/*************************************************
* Function: WS2812_Init
* Description: SPI2��ʼ��	
* Parameter: none
* Return: none
* Note: ���ó�����ģʽ������SD Card/W25Q64/NRF24L01
*************************************************/
void WS2812_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz

    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.0,1

    GPIO_ResetBits(GPIOB, GPIO_Pin_7); //PB7 �����

}

/*************************************************
* Function: ResetDataFlow
* Description: ��λ��Ϊ��һ�η�����׼��	
* Parameter: none
* Return: none
* Note: ��DI��λΪ0����ʱԼ60us
*************************************************/
void ResetDataFlow(void)
{
    DI = 0;					//DI��Ϊ0����ʱ50us���ϣ�ʵ��֡��λ
    delay_us(60);
}

/*************************************************
* Function: SendOnePix(unsigned char *ptr)
* Description: ����һ�����ص��24bit����	
* Parameter: ���յĲ�����һ��ָ�룬�˺����Ὣ��ָ��ָ��ĵ�ַ������������Byte�����ݷ���
* Return: none
* Note: ����������ֱ�ӵ��ô˺���ʱ��������֡���Ϳ�ʼǰ��Ҫ��ִ�� ResetDataFlow()
*		�����ǰ�������ķ�ʽ���ͣ�����Ϊ800KBPS
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
            if(temp & 0x80)		 //�Ӹ�λ��ʼ����
            {
                DI = 1;			 //���͡�1����
                delay_ns(7); //��ʱ6*125ns=750ns
                DI = 0;
                delay_ns(3);//��ʱ375ns
            }
            else				//���͡�0����
            {
                DI = 1;
                delay_ns(3);//��ʱ375ns
                DI = 0;
                delay_ns(7);//��ʱ6*125ns=750ns
            }
            temp = (temp << 1);		 //����λ
        }
    }
}

/*************************************************
* Function: SendOneFrame(unsigned char *ptr)
* Description: ����һ֡���ݣ�������������������ݣ�
* Parameter: ���յĲ�����һ��ָ�룬�˺����Ὣ��ָ��ָ��ĵ�ַ��������������ݷ���
* Return: none
* Note: ����������ֱ�ӵ��ô˺���ʱ��������֡���Ϳ�ʼǰ��Ҫ��ִ�� ResetDataFlow()
*		�����ǰ�������ķ�ʽ���ͣ�����Ϊ800KBPS
*************************************************/
void SendOneFrame(unsigned char *ptr)
{
    unsigned char k;

    ResetDataFlow();	//����֡��λ�ź�

    for(k = 0; k < SNUM; k++)	//����һ֡���ݣ�SNUM�ǰ���LED�ĸ���
        SendOnePix(&ptr[(3 * k)]);

    ResetDataFlow();	 //����֡��λ�ź�
}

/*************************************************
* Function: SendSameColor(unsigned char *ptr,unsigned char cnt)
* Description: ��ͬ��ɫ�ĵ㷢��cnt��
* Parameter: ���յĲ�����һ��ָ�룬ָ�����ص���ɫ���飬cnt���ݷ��͸���
* Return: none
* Note: ����������ֱ�ӵ��ô˺���ʱ��������֡���Ϳ�ʼǰ��Ҫ��ִ�� ResetDataFlow()
*		�����ǰ�������ķ�ʽ���ͣ�����Ϊ800KBPS
*************************************************/
void SendSameColor(unsigned char *ptr, unsigned char cnt)
{
    unsigned char k;

    ResetDataFlow();				 //����֡��λ�ź�

    for(k = 0; k < cnt; k++)				 //����һ֡���ݣ�SNUM�ǰ���LED�ĸ���
    {
        SendOnePix(&ptr[0]);
    }

    ResetDataFlow();				 //����֡��λ�ź�
}

/*************************************************
* Function: SendOneFrameFrom(unsigned char i,unsigned char *ptr)
* Description: ��ָ�������ص㿪ʼ����һ֡���ݣ�������������������ݣ�
* Parameter: ���յĲ�����һ��ָ�룬�˺����Ὣ��ָ��ָ��ĵ�ַ����֡���ݷ���
*		     i:������ĵ�0���������ݷ��͵���i�����ص㣨��0�������ǰ��ϱ��Ϊ01�����أ�
* Return: none
* Note: ��ԭ����Ӧ��һ�����ص����ݻᷢ�͵���i�����ص㣨LED����
*************************************************/
void SendOneFrameFrom(unsigned char i, unsigned char *ptr)
{
    unsigned char k;

    ResetDataFlow();				 //����֡��λ�ź�

    for(k = (SNUM - i); k < SNUM; k++)		 //����һ֡����
    {
        SendOnePix(&ptr[(3 * k)]);
    }
    for(k = 0; k < (SNUM - i); k++)
    {
        SendOnePix(&ptr[(3 * k)]);
    }

    ResetDataFlow();				 //����֡��λ�ź�
}

/*************************************************
* Function: SendOneFrameSince(unsigned char i,unsigned char *ptr)
* Description: ��ָ�������ص㿪ʼ����һ֡���ݣ�������������������ݣ�
* Parameter: ���յĲ�����һ��ָ�룬�˺����Ὣ��ָ��ָ��ĵ�ַ����֡���ݷ���
*		     i:������ĵ�0���������ݷ��͵���i�����ص㣨��0�������ǰ��ϱ��Ϊ01�����أ�
* Return: none
* Note: ��ԭ����Ӧ��i���ص����ݻᷢ�͵���1�����ص㣨LED���ϣ�
*		��i+1�����ص�����ݷ��͵���2��������
*************************************************/
void SendOneFrameSince(unsigned char i, unsigned char *ptr)
{
    unsigned char k;

    ResetDataFlow();				 //����֡��λ�ź�

    for(k = i; k < SNUM; k++)				 //����һ֡����
    {
        SendOnePix(&ptr[(3 * k)]);
    }
    for(k = 0; k < i; k++)
    {
        SendOnePix(&ptr[(3 * k)]);
    }
    ResetDataFlow();				 //����֡��λ�ź�
}
