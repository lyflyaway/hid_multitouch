#include "stm32f10x.h"
#include "bit_IO.h"
#include "Touch.h"

Touch_TypeDef Touch_Point;

void Touch_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE );  //��Ҫ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ��IO���ù��ܣ�ʹ���жϹ�����Ҫ������


	//������SPI���GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //ͨ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//������SPI���GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//Configure PC6 pin: TP_CS pin 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//Configure PC4 pin: TP_INT pin 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//��������
	GPIO_Init(GPIOC,&GPIO_InitStructure);



	#ifdef  VECT_TAB_RAM  
	  /* Set the Vector Table base location at 0x20000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	   // �����ж��� Ϊ2 
	  
	/* Enable the EXTI4_IRQn Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;	// �����ж���4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // ����ռ�����ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // ���ø����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // ʹ���ж���4
	NVIC_Init(&NVIC_InitStructure);							  // ��ʼ���ж���4
	
	
  	/* Connect PEN EXTI Line to Key Button GPIO Pin */
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);

  	/* Configure PEN EXTI Line to generate an interrupt on falling edge */  
  	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);

  	/* Generate software interrupt: simulate a falling edge applied on PEN EXTI line */
  	EXTI_GenerateSWInterrupt(EXTI_Line4);	
}



void TOUCH_Write_Cmd(uint8_t TOUCH_CMD)    
{  
	uint8_t i=0;   
	for(i=0; i<8; i++)  
	{ 	  
		if(TOUCH_CMD&0x80) TDIN = 1;  
		else TDIN = 0;   
		TOUCH_CMD<<=1;    
		TCLK = 0;//��������Ч	   	 
		TCLK = 1;      
	} 			    
} 

uint16_t TOUCH_Read_AD(uint8_t TOUCH_CMD)	  
{ 	 
	uint16_t i,f;	  
	uint16_t Result = 0; 
	TCLK = 0;	//������ʱ�� 	 
	TCS = 0; //ѡ��ADS7843	 
	TOUCH_Write_Cmd(TOUCH_CMD);//����������
	for(i=1000; i>0; i--) ;		//	delay_us(6);//ADS7846��ת��ʱ���Ϊ6us
	TCLK = 1;//��1��ʱ�ӣ����BUSY   	    
	TCLK = 0; 	 
	for(f=0; f<16; f++)  
	{ 				  
		Result<<=1; 	 
		TCLK = 0;//�½�����Ч  	    	   
		TCLK = 1;
		if(DOUT) Result++; 		 
	}  	
	Result>>=4;   //ֻ�и�12λ��Ч.
	TCS = 1;//�ͷ�ADS7843	 
	return(Result);   
}


void EXTI4_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_IMR_MR4) != RESET)
	{
		Touch_Point.Key_Sta = Touch_Down;//��������  	
    	/* Clear the Key Button EXTI line pending bit */
    	EXTI_ClearITPendingBit(EXTI_IMR_MR4);
	}
}





