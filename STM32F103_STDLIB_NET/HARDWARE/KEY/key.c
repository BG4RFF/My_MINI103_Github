/*****************************************************************************
 * �ļ���  ��KEY.c
 * ����    �������ײ�����������         
 * ʵ��ƽ̨��STM32F103VET6
 * ��汾  ��ST3.5.0
 * ����    ����־��
******************************************************************************/
#include "stm32f10x.h"
#include "key.h"
/*******************************************
 * ��������KEY_Init
 * ����  ��������ʼ������
 * ����1 : KEY1-->GPIOD12 �������� 
 * ����2 ��KEY2-->GPIOC0  ��������
 * ע��  ����
*********************************************/								    
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ��KEY1-->GPIOD12 KEY2-->GPIOC0  ��������
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTD,PORTCʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;//PD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOD12

	//��ʼ��KEY2-->GPIOC0  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC.0

}
/*******************************************
 * ������ ��KEY_Scan
 * ����   ��������ʼ������
 * ����1  : KEY1-->GPIOD12 �������� 
 * ����2  ��KEY2-->GPIOC0  ��������
 * return �����ذ���ֵ
 * ˵��	  ��1��KEY1����
		    2��KEY2����
 * ע��	  ���˺�������Ӧ���ȼ�,KEY1>KEY2
*********************************************/
u8 KEY_Scan(void)
{   
	int i;
	if(KEY1==0||KEY2==0)
	{
		//OSTimeDlyHMSM(0, 0, 0, 10);//ȥ���� 
		for(i=0;i<0xffffff;i++);
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
	}   
 	return 0;// �ް�������
}
