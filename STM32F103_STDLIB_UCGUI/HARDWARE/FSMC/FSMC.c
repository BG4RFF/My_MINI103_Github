/*****************************************************************************
 * �ļ���  ��FSMC.c
 * ����    ��FSMC�ײ�����������         
 * ʵ��ƽ̨��STM32F103VET6
 * ��汾  ��ST3.5.0
 * ����    ����־��
******************************************************************************/
#include "FSMC.h"
#include "core_cm3.h"
/***********************************************************************************************
								��ص�FSMC��Ӧ�ĺ궨��
**********************************************************************************************/
/*ѡ��LCDָ���Ĵ���*/
#define LCD_WR_REG(index)       ((*(__IO u16 *) (Bank1_LCD_C)) = ((u16)index))
/*��LCD�е�index�Ĵ�������Ϊvalֵ*/
#define LCD_WR_CMD(index,val)  do{(*(__IO u16 *) (Bank1_LCD_C)) = ((u16)index);	(*(__IO u16 *) (Bank1_LCD_D)) = ((u16)(val));}while(0)
/*��LCD GRAMд������*/
#define LCD_WR_Data(val)        ((*(__IO u16 *) (Bank1_LCD_D)) = ((u16)(val)))	
#define LCD_S6D04H0_CMD(index)       LCD_WR_REG(index)
#define LCD_S6D04H0_Parameter(val)	 LCD_WR_Data(val)
								  
/**********************************************************************************************
								���ȫ�ֱ���
***********************************************************************************************/
/*****************************************************************************
��������־λ :display_direction
ȡֵ��	     0��GRAMָ��ɨ�跽��Ϊ����ģʽ
	        1��GRAMָ��ɨ�跽��Ϊ����ģʽ
˵����      �ñ�־λ�����÷����û�ʶ��ǰ�����Ǵ�������ģʽ
*******************************************************************************/  
volatile u8 display_direction = 0;
/*****************************************************************************
FSMC��ʼ���ṹ�� :display_direction
*******************************************************************************/ 	 
FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
/*******************************************
 * ��������FSMC_Delay
 * ����  ����FSMC��ʼ��ʱ���ʵ�����ʱ
 * ����  : �� 
 * ���  ����
 * ����  ����
 * ע��  ����
*********************************************/ 
void FSMC_Delay(int count)  // /* X1ms */
{
  int i,j;
  for(i=0;i<count;i++)
    for(j=0;j<100;j++);
}
/*******************************************
 * ��������LCD_GPIO_Config
 * ����  ������FSMC����LCD��I/O
 * ����  : �� 
 * ���  ����
 * ����  ����
 * ע��  ����
*********************************************/  
void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* ʹ��FSMCʱ��*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |RCC_APB2Periph_AFIO, ENABLE);
    /* ����LCD��λ���ƹܽ�*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 
    GPIO_Init(GPIOE, &GPIO_InitStructure);  		   
    
    /* ����FSMC���Ӧ��������,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10*/	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* ����FSMC���Ӧ�Ŀ�����
	 * PD4-FSMC_NOE  :LCD-RD
     * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
     * PD11-FSMC_A16 :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
     
	 
	  
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);
    GPIO_ResetBits(GPIOE, GPIO_Pin_1);			//RESET=0
    GPIO_SetBits(GPIOD, GPIO_Pin_4);		    //RD=1
    GPIO_SetBits(GPIOD, GPIO_Pin_5);			//WR=1
	GPIO_SetBits(GPIOD, GPIO_Pin_11); 
}

/*******************************************
 * ��������LCD_FSMC_Config
 * ����  ��LCD  FSMC ģʽ����
 * ����  : �� 
 * ���  ����
 * ����  ����
 * ע��  ����
*********************************************/ 
void LCD_FSMC_Config(void)
{
	FSMC_NORSRAMTimingInitTypeDef  readTiming;
	FSMC_NORSRAMTimingInitTypeDef  WriteTiming;
	 
	WriteTiming.FSMC_AddressSetupTime = 0x02;	 //��ַ����ʱ��
    WriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
    WriteTiming.FSMC_DataSetupTime = 0x05;		 //���ݽ���ʱ��
    WriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    WriteTiming.FSMC_CLKDivision = 0x00;
    WriteTiming.FSMC_DataLatency = 0x00;
    WriteTiming.FSMC_AccessMode = FSMC_AccessMode_B;	 // һ��ʹ��ģʽB������LCD
	   
    readTiming.FSMC_AddressSetupTime = 0x02;	 //��ַ����ʱ��
    readTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
    readTiming.FSMC_DataSetupTime = 0x05;		 //���ݽ���ʱ��
    readTiming.FSMC_BusTurnAroundDuration = 0x00;
    readTiming.FSMC_CLKDivision = 0x02;
    readTiming.FSMC_DataLatency = 0x00;
    readTiming.FSMC_AccessMode = FSMC_AccessMode_B;	 // һ��ʹ��ģʽB������LCD
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readTiming;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &WriteTiming; 
   
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);   
    /* ʹ�� FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE); 

}

/**********************************
 * ��������LCD_Rst
 * ����  ��LCD �����λ
 * ����  : �� 
 * ���  ����
 * ����  ����
 * ע��  ����
*************************************/
void LCD_Rst(void)
{			
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	 //�͵�ƽ��λ
    FSMC_Delay(100); 					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
    FSMC_Delay(120); 	
}

/********************************
 * ��������LCD_RD_REG
 * ����  ���� S6D04H0 RAM ����
 * ����  : �� 
 * ���  ����ȡ������,16bit *
 * ����  ����
 * ע��  ����
*************************************/
u16 LCD_RD_data(void)
{
//	u16 R=0, B=0 ;
//	LCD_S6D04H0_CMD(0x2e);	           /*LCD GRAM READ COMMAND--DATASHEET PAGE116*/
//	(*(__IO u16 *) (Bank1_LCD_D)); //FSMC_Delay(1); /*FIRST READ OUT DUMMY DATA*/
//	R =(*(__IO u16 *) (Bank1_LCD_D)); //FSMC_Delay(1);/*READ OUT RED DATA  */
//	B =(*(__IO u16 *) (Bank1_LCD_D)); //FSMC_Delay(1); /*READ OUT BLUE DATA*/
//    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);   /* ʹ�� FSMC Bank1_SRAM Bank */
//	return ((B&0xf800)|((R&0x00fc)<<3)|((R&0xf800)>>10)) ;
	u16 D1=0, D2=0;
	LCD_S6D04H0_CMD(0x2e);	          /*LCD GRAM READ COMMAND--DATASHEET PAGE116*/			//RS=1
	(*(__IO u16 *) (Bank1_LCD_D)); //delay_us(1); /*FIRST READ OUT DUMMY DATA*/	
	(*(__IO u16 *) (Bank1_LCD_D)); //delay_us(1);/*READ OUT RED DATA  */	
	D1 =(*(__IO u16 *) (Bank1_LCD_D)); //delay_us(1); /*READ OUT BLUE DATA*/
	D2 =(*(__IO u16 *) (Bank1_LCD_D));

    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);

	return ((D1&0xf800)|((D2&0xfc00)>>5)|((D1&0x00f8)>>3));
}

/**********************************
 * ��������Set_direction
 * ����  ������S6D04H0GRAMָ��ɨ�跽��
 * ����  : 0�� ����ɨ��
 		   1�� ����ɨ��
 * ���  ����
 * ����  ����
 * ע��  ����
*************************************/
void Set_direction(u8 option)
{

    switch(option)
    {
    case 0:
	{	    /*����*/
        LCD_S6D04H0_CMD(0x36); 
		LCD_S6D04H0_Parameter(0x68);    //����
		LCD_S6D04H0_CMD(0X2A); 
		LCD_S6D04H0_Parameter(0x00);	//start 
		LCD_S6D04H0_Parameter(0x00);
		LCD_S6D04H0_Parameter(0x01);	//end
		LCD_S6D04H0_Parameter(0x3F);
		
		LCD_S6D04H0_CMD(0X2B); 
		LCD_S6D04H0_Parameter(0x00);   //start
		LCD_S6D04H0_Parameter(0x00);
		LCD_S6D04H0_Parameter(0x00);   //end
		LCD_S6D04H0_Parameter(0xEF);
        display_direction = 0;		
        }break;
    case 1:
        {		/*����*/
        LCD_S6D04H0_CMD(0x36); 
	    LCD_S6D04H0_Parameter(0x48);	//���� 
	    LCD_S6D04H0_CMD(0X2A); 
		LCD_S6D04H0_Parameter(0x00);
		LCD_S6D04H0_Parameter(0x00);
		LCD_S6D04H0_Parameter(0x00);
		LCD_S6D04H0_Parameter(0xEF);	
		
		LCD_S6D04H0_CMD(0X2B); 
		LCD_S6D04H0_Parameter(0x00);
		LCD_S6D04H0_Parameter(0x00);
		LCD_S6D04H0_Parameter(0x01);
		LCD_S6D04H0_Parameter(0x3F);
        display_direction = 1;
        }break;
        
    }

}




/**********************************
 * ��������LCD_open_windows
 * ����  ������(��x,yΪ������㣬��Ϊlen,��Ϊwid)
 * ����  : -x    �������
           -y	   �������
           -len  ������ 
           -wid ������
 * ���  ����
 * ����  ����
 * ע��  ����
*************************************/
void LCD_open_windows(u16 x,u16 y,u16 len,u16 wid)
{                    

    if(display_direction == 0)		/*����Ǻ���ѡ��*/
    {

		LCD_S6D04H0_CMD(0X2A); 
		LCD_S6D04H0_Parameter(x>>8);	//start 
		LCD_S6D04H0_Parameter(x-((x>>8)<<8));
		LCD_S6D04H0_Parameter((x+len-1)>>8);	//end
		LCD_S6D04H0_Parameter((x+len-1)-(((x+len-1)>>8)<<8));
		
		LCD_S6D04H0_CMD(0X2B); 
		LCD_S6D04H0_Parameter(y>>8);   //start
		LCD_S6D04H0_Parameter(y-((y>>8)<<8));
		LCD_S6D04H0_Parameter((y+wid-1)>>8);   //end
		LCD_S6D04H0_Parameter((y+wid-1)-(((y+wid-1)>>8)<<8));
     
    }
    else
    {
		LCD_S6D04H0_CMD(0X2B); 
		LCD_S6D04H0_Parameter(x>>8);
		LCD_S6D04H0_Parameter(x-((x>>8)<<8));
		LCD_S6D04H0_Parameter((x+len-1)>>8);
		LCD_S6D04H0_Parameter((x+len-1)-(((x+len-1)>>8)<<8));

	    LCD_S6D04H0_CMD(0X2A); 
		LCD_S6D04H0_Parameter(y>>8);
		LCD_S6D04H0_Parameter(y-((y>>8)<<8));
		LCD_S6D04H0_Parameter((y+wid-1)>>8);
		LCD_S6D04H0_Parameter((y+wid-1)-(((y+wid-1)>>8)<<8));	
        
    }

      LCD_S6D04H0_CMD(0x2c);     
}


/**********************************
 * ��������Lcd_data_start
 * ����  ������S6D04H0��ʾ
 * ����  : ��
 * ���  ����
 * ����  ����
 * ע��  ����
*************************************/
void Lcd_data_start(void)
{
     LCD_WR_REG(0x2C);//��ʼд
}
/**********************************
 * ��������Lcd_init_conf
 * ����  ��S6D04H0 LCD�Ĵ�����ʼ����
 * ����  : ��
 * ���  ����
 * ����  ����
 * ע��  ����
*************************************/
void Lcd_init_conf(void)
{
	/**********************	���³�ʼ������ ÿ�������´��в�ͬ�����Ĳ���**************************/
  
  LCD_S6D04H0_CMD(0XF0);  //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 220ҳ	 //LCD_S6D04H0_CMD���� ��������  LCD_S6D04H0_Parameter���� ���ݸ������µĲ���
  LCD_S6D04H0_Parameter(0X5A);          
  LCD_S6D04H0_Parameter(0X5A);          
  
  //APON CONTROL                         
  LCD_S6D04H0_CMD(0XF3);  //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 227ҳ
  LCD_S6D04H0_Parameter(0X00); 
  LCD_S6D04H0_Parameter(0X00);      
  LCD_S6D04H0_Parameter(0X00);   
  LCD_S6D04H0_Parameter(0X00);           
  
  //SLEEP OUT
  LCD_S6D04H0_CMD(0X11);  //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 165ҳ       
  FSMC_Delay(10); 
  
  //POWER CONTROL	   VC = "0111"
  LCD_S6D04H0_CMD(0XF4);   //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 228ҳ
  LCD_S6D04H0_Parameter(0X07); 
  LCD_S6D04H0_Parameter(0X00); 
  LCD_S6D04H0_Parameter(0X00); 
  LCD_S6D04H0_Parameter(0X00);  
  LCD_S6D04H0_Parameter(0X21);             
  LCD_S6D04H0_Parameter(0X47);       
  LCD_S6D04H0_Parameter(0X01);       
  LCD_S6D04H0_Parameter(0X02);     
  LCD_S6D04H0_Parameter(0X3F); 
  LCD_S6D04H0_Parameter(0X66);   
  LCD_S6D04H0_Parameter(0X05); 
  
  //POWER CONTROL	   VC = "0111"
  LCD_S6D04H0_CMD(0XF4); //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 228ҳ   ����20������
  LCD_S6D04H0_Parameter(0X07); 
  LCD_S6D04H0_Parameter(0X00); 
  LCD_S6D04H0_Parameter(0X00); 
  LCD_S6D04H0_Parameter(0X00);  
  LCD_S6D04H0_Parameter(0X21);             
  LCD_S6D04H0_Parameter(0X47);       
  LCD_S6D04H0_Parameter(0X04);  
  LCD_S6D04H0_Parameter(0X02);       
  LCD_S6D04H0_Parameter(0X2A);  
  LCD_S6D04H0_Parameter(0X7d);       
  LCD_S6D04H0_Parameter(0X05);  
  
  LCD_S6D04H0_CMD(0XF2);  //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 221ҳ
  LCD_S6D04H0_Parameter(0X28);             
  LCD_S6D04H0_Parameter(0X60);       
  LCD_S6D04H0_Parameter(0X03);  
  LCD_S6D04H0_Parameter(0X04);       
  LCD_S6D04H0_Parameter(0X04);  
  LCD_S6D04H0_Parameter(0X00);       
  LCD_S6D04H0_Parameter(0X00);
  LCD_S6D04H0_Parameter(0X15);             
  LCD_S6D04H0_Parameter(0X48);       
  LCD_S6D04H0_Parameter(0X00);  
  LCD_S6D04H0_Parameter(0X07);       
  LCD_S6D04H0_Parameter(0X01);  
  
  LCD_S6D04H0_CMD(0XF5);  //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 235ҳ
  LCD_S6D04H0_Parameter(0X00);             
  LCD_S6D04H0_Parameter(0X1d);       
  LCD_S6D04H0_Parameter(0X4f);  
  LCD_S6D04H0_Parameter(0X00);       
  LCD_S6D04H0_Parameter(0X00); 
  LCD_S6D04H0_Parameter(0X06);  
  
  
  //POWER CONTROL	
  LCD_S6D04H0_CMD(0XF3); 	 //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 227ҳ
  LCD_S6D04H0_Parameter(0X00);      
  LCD_S6D04H0_Parameter(0X03); 
  LCD_S6D04H0_Parameter(0X00);          
  LCD_S6D04H0_Parameter(0X00);         
  LCD_S6D04H0_Parameter(0X00);      
  FSMC_Delay(5);  
  
  //POWER CONTROL	
  LCD_S6D04H0_CMD(0XF3); 	//�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 227ҳ
  LCD_S6D04H0_Parameter(0X00);                 
  LCD_S6D04H0_Parameter(0X0F); 
  LCD_S6D04H0_Parameter(0X09); 
  LCD_S6D04H0_Parameter(0X00);
  LCD_S6D04H0_Parameter(0X00);
  FSMC_Delay(5);  
  
  LCD_S6D04H0_CMD(0XF3); 	//�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 227ҳ
  LCD_S6D04H0_Parameter(0X00);                  
  LCD_S6D04H0_Parameter(0X0F); 
  LCD_S6D04H0_Parameter(0X01); 
  FSMC_Delay(10);
  
  //POWER CONTROL	
  LCD_S6D04H0_CMD(0XF3);   //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 227ҳ
  LCD_S6D04H0_Parameter(0X00);                  
  LCD_S6D04H0_Parameter(0X1F); 
  FSMC_Delay(20);
  
  LCD_S6D04H0_CMD(0XF3);  //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 227ҳ
  LCD_S6D04H0_Parameter(0X00);                  
  LCD_S6D04H0_Parameter(0X3F);    
  
  FSMC_Delay(10);
  LCD_S6D04H0_CMD(0XF3); 	//�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 227ҳ
  LCD_S6D04H0_Parameter(0X00);                  
  LCD_S6D04H0_Parameter(0X3F);
  LCD_S6D04H0_Parameter(0X03);
  
  
  LCD_S6D04H0_CMD(0XF3);    //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 227ҳ
  LCD_S6D04H0_Parameter(0X00);              
  LCD_S6D04H0_Parameter(0X7F); 
  FSMC_Delay(30);          
  
  LCD_S6D04H0_CMD(0XF3);  //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 227ҳ
  LCD_S6D04H0_Parameter(0X00);                  
  LCD_S6D04H0_Parameter(0XFF);
  
  FSMC_Delay(20);
  
  LCD_S6D04H0_CMD(0XF3);   //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 227ҳ
  LCD_S6D04H0_Parameter(0X00);                 
  LCD_S6D04H0_Parameter(0XFF);   
  LCD_S6D04H0_Parameter(0X1F); 
  LCD_S6D04H0_Parameter(0X00);   
  LCD_S6D04H0_Parameter(0X02); 
  FSMC_Delay(20);
  LCD_S6D04H0_CMD(0XF3);  //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 227ҳ
  LCD_S6D04H0_Parameter(0X01); 
  
  LCD_S6D04H0_CMD(0XF0); 
  LCD_S6D04H0_Parameter(0Xa5); 
  LCD_S6D04H0_Parameter(0Xa5); 
 
  LCD_S6D04H0_CMD(0X36);   //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 189ҳ
  LCD_S6D04H0_Parameter(0X48); 
  
  LCD_S6D04H0_CMD(0X3a);   //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 195ҳ
  LCD_S6D04H0_Parameter(0X55);   //16bit (66 18bit)
 
  FSMC_Delay(120); 
  
  LCD_S6D04H0_CMD(0x29);	 //��ʾ��		 //�ο�S6D04H0����IC�ֲ�.pdf �ĵ��� 173ҳ
  LCD_S6D04H0_CMD(0x2c);	//д���ݵ�RAM��	
}

