#include <debug-uart.h>
#include <stm32f10x.h>
#include "stdio.h"

void  
dbg_setup_uart_default()  
{  
  USART_InitTypeDef USART_InitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;  
     
  //ʹ��GPIOAʱ��  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_USART1 ,ENABLE);  
   
  //PA9 TX1 �����������  
  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_9;  
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;  
  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP;  
  GPIO_Init(GPIOA,&GPIO_InitStructure);  
  //PA10 RX1 ��������  
  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_10;  
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;     
  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;  
  GPIO_Init(GPIOA,&GPIO_InitStructure);  
   
   USART_InitStructure.USART_BaudRate = 115200;						//����115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);							//���ô��ڲ�������					//ʹ�ܷ��ͻ�����ж�   

  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);	
}  

int fputc(int ch,FILE *f)  
{  
  USART_SendData(USART1,(uint8_t)ch);  
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)== RESET );  
  return ch;  
} 
