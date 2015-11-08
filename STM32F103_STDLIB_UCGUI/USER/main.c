/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "MyBoard_BSP.h"
#include "GUI.h"
#include "DIALOG.h"
#include "GUIDEMO.h"

/* Private variables ---------------------------------------------------------*/
uint8_t GUI_Initialized   = 0;

extern WM_HWIN CreateMy_Board_GUI(void);
/**
 ******************************************************************************
 �������ƣ�My_Board_BSP_Init
 �������: ��
 �����������
 ��    ����ʵ�ֿ������ʼ��
 *****************************************************************************/
int main(void)
{
	GUI_Initialized = 1;
	
	My_Board_BSP_Init();
	GUI_Init();
	
	CreateMy_Board_GUI();

}

