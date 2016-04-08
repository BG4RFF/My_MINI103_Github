/*****************************************************************************
 * �ļ���  ��Touch.c
 * ����    ��XPT2046�ײ�����������         
 * ʵ��ƽ̨��STM32F103VET6
 * ��汾  ��ST3.5.0
 * ����    ����־��
******************************************************************************/
#include "stm32f10x.h"
//#include "includes.h"
#include "touch.h"
#include "TFT_driver.h"	

unsigned int TP_X,TP_Y;	   //��ǰ��������

void Touch_Init(void)
{ 
	RCC->APB2ENR|=1<<2;  
    RCC->APB2ENR|=1<<3;  
    RCC->APB2ENR|=1<<4;  
	RCC->APB2ENR|=1<<5; 

	GPIOB->CRL&=0XF0FFFFFF;//PB6���ó�����	  
	GPIOB->CRL|=0X08000000; 	
	GPIOB->CRL&=0X0FFFFFFF;
	 
	GPIOB->CRL|=0X30000000;//PB7 �������  	 
    GPIOB->ODR|=1<<7;      //PB7�����
											  
	GPIOA->CRL&=0X0F0FFFFF;
	GPIOA->CRL|=0X30300000;//PA5��7�������
	GPIOA->ODR|=1<<5;      //PA5����� 
	GPIOA->ODR|=1<<7;      //PA5�����

	GPIOA->CRL&=0XF0FFFFFF;//PA6���ó�����	  
	GPIOA->CRL|=0X08000000; 
}
//******************************************************
void spistar(void)                                     //SPI��ʼ
{
	XPT2046_CS_1;
	XPT2046_CLK_1;
	XPT2046_DI_1;
	XPT2046_CLK_1;
}
//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPIд����
{
	unsigned char count=0;
	XPT2046_CLK_0;
	for(count=0;count<8;count++)
	{
		if(num & (0x80 >> count))
			XPT2046_DI_1;
		else
		    XPT2046_DI_0;
		XPT2046_CLK_0; //delay_us(3);                //��������Ч
		XPT2046_CLK_1; //delay_us(3);
	}
}
//**********************************************************
unsigned int ReadFromCharFrom7843(void)             //SPI ������
{
	unsigned char count=0;
	unsigned int Num=0;
	for(count=0;count<12;count++)
	{
		Num<<=1;
		XPT2046_CLK_1; //delay_us(3);                //�½�����Ч
		XPT2046_CLK_0; //delay_us(3);
		if(XPT2046_DO) Num++;
	}
	return(Num);
}

void AD7843(void)              //�ⲿ�ж�0 �������ܼ��̷���������
{
	XPT2046_CS_0;
	//delayms(1);                     //�жϺ���ʱ������������ʹ�ò������ݸ�׼ȷ
	//while(BUSY);                //���BUSY�źŲ���ʹ����ɾ������
	//delayms(1);
	WriteCharTo7843(0x90);        //�Ϳ����� 10010000 ���ò�ַ�ʽ��X���� ��ϸ����й�����
	//while(BUSY);               //���BUSY�źŲ���ʹ����ɾ������
	//delayms(1);
	XPT2046_CLK_1;//delay_us(3);
	XPT2046_CLK_0;//delay_us(3);
	TP_Y=ReadFromCharFrom7843();


	WriteCharTo7843(0xD0);       //�Ϳ����� 11010000 ���ò�ַ�ʽ��Y���� ��ϸ����й�����
	XPT2046_CLK_1; //delay_us(3);
	XPT2046_CLK_0; //delay_us(3);
	TP_X=ReadFromCharFrom7843();
	XPT2046_CS_1;
}

int  Get_Touch_x(void)
{
	int temp_x;
	spistar();
	XPT2046_CS_0;
	WriteCharTo7843(CMD_RDX);       //�Ϳ����� 11010000 ���ò�ַ�ʽ��Y���� ��ϸ����й�����
	XPT2046_CLK_1; //delay_us(3);
	XPT2046_CLK_0; //delay_us(3);
	temp_x = ReadFromCharFrom7843();
	XPT2046_CS_1;
	return temp_x;	
}

int  Get_Touch_y(void)
{
	int temp_y;
	spistar();
	XPT2046_CS_0;
	WriteCharTo7843(CMD_RDY);       //�Ϳ����� 11010000 ���ò�ַ�ʽ��Y���� ��ϸ����й�����
	XPT2046_CLK_1; //delay_us(3);
	XPT2046_CLK_0; //delay_us(3);
	temp_y = ReadFromCharFrom7843();
	XPT2046_CS_1;
	return temp_y;	
}
