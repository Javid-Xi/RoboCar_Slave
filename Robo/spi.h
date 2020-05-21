#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

typedef struct {
 char G;
char R;
 char B;
 }_GRB;

extern _GRB SPI2Buff[8];

 				  	    													  
void SPI2_Init(void);			 //��ʼ��SPI��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
void write1pix(char g,char r,char b);
void write8pix(_GRB* buf);
void add_red(int cont,int div);

void RR(void);
void RL(void);
 
void LED_Circulate(void);//ѭ����˸
void ADD_COLOR(int G_div,int R_div,int B_div);//������
		 
#endif

