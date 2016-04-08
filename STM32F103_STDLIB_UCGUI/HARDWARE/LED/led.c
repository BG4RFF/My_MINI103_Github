/*****************************************************************************
 * �ļ���  ��LED.c
 * ����    ��LED�ײ�����������         
 * ʵ��ƽ̨��STM32F103VET6
 * ��汾  ��ST3.5.0
 * ����    ����־��
******************************************************************************/
#include "stm32f10x.h"
#include "led.h"

/*******************************************
 * ��������LED_Init
 * ����  ����ʼ��PE5��PE6Ϊ�����.��ʹ���������ڵ�ʱ��
 * ����  : �� 
 * ���  ����
 * ����  ����
 * ע��  ����
*********************************************/
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;	 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOE,GPIO_Pin_5|GPIO_Pin_6);						 //PB.5 �����
}
 
/*******************************************
 * ��������LED_Delay
 * ����  ����LED��ʼ��ʱ���ʵ�����ʱ
 * ����  : �� 
 * ���  ����
 * ����  ����
 * ע��  ����
*********************************************/ 
void LED_Delay(int count)  // /* X1ms */
{
  int i,j;
  for(i=0;i<count;i++)
    for(j=0;j<100;j++);
}
