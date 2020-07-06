#include "stm32f10x.h"
#include "CAN.h"



/* ���жϴ��������з��� */
__IO uint32_t CAN_return = 0;


void CAN_Config(void)	//CAN 1
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
}



/* 
* ��������CAN_Polling 
* ���� ������ CAN �Ĺ���ģʽΪ �ػ�ģʽ 
* ���� ���� 
* ��� : -PASSED �ɹ� 
* -FAILED ʧ�� 
* ���� ���ڲ����� 
*/ 
CANTestResult CAN_Polling(void) 
{ 
	CAN_InitTypeDef CAN_InitStructure; 
	CAN_FilterInitTypeDef CAN_FilterInitStructure; 
	CanTxMsg TxMessage; 
	CanRxMsg RxMessage; 
	uint32_t i = 0; 
	uint8_t TransmitMailbox = 0; 

	/* CAN register init */ 
	CAN_DeInit(CAN1); 
	CAN_StructInit(&CAN_InitStructure); 

	/* CAN cell init */ 
	CAN_InitStructure.CAN_TTCM=DISABLE; 
	CAN_InitStructure.CAN_ABOM=DISABLE; 
	CAN_InitStructure.CAN_AWUM=DISABLE; 
	CAN_InitStructure.CAN_NART=DISABLE; 
	CAN_InitStructure.CAN_RFLM=DISABLE; 
	CAN_InitStructure.CAN_TXFP=DISABLE; 
	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack; // �ػ�ģʽ 
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; 
	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq; 
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq; 
	CAN_InitStructure.CAN_Prescaler=5; // ��Ƶϵ��Ϊ5 
	CAN_Init(CAN1, &CAN_InitStructure); // ��ʼ��CAN 

	/* CAN filter init */ 
	CAN_FilterInitStructure.CAN_FilterNumber=0; 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000; 
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000; 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000; 
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000; 
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0; 
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; 
	CAN_FilterInit(&CAN_FilterInitStructure); 

	/* transmit */ 
	TxMessage.StdId=0x11; // �趨��׼��ʶ����11λ����չ��Ϊ29λ�� 
	TxMessage.RTR=CAN_RTR_DATA; // ������Ϣ��֡����Ϊ����֡������Զ��֡�� 
	TxMessage.IDE=CAN_ID_STD; // ��Ϣ��־��ʵ���׼��ʶ�� 
	TxMessage.DLC=2; // ������֡��һ֡8λ 
	TxMessage.Data[0]=0xCA; // ��һ֡���� 
	TxMessage.Data[1]=0xFE; // �ڶ�֡���� 

	TransmitMailbox=CAN_Transmit(CAN1, &TxMessage); 
	i = 0; 
	// ���ڼ����Ϣ�����Ƿ����� 
	while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK) && (i != 0xFF)) 
	{ 
		i++; 
	} 

	i = 0; 
	// ��鷵�صĹҺŵ���Ϣ��Ŀ 
	while((CAN_MessagePending(CAN1, CAN_FIFO0) < 1) && (i != 0xFF)) 
	{ 
		i++; 
	} 
	/* receive */ 
	RxMessage.StdId=0x00; 
	RxMessage.IDE=CAN_ID_STD; 
	RxMessage.DLC=0; 
	RxMessage.Data[0]=0x00; 
	RxMessage.Data[1]=0x00; 
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); 

	if (RxMessage.StdId!=0x11) 
	{ 
		return FAILED; 
	} 
	
	if (RxMessage.IDE!=CAN_ID_STD) 
	{ 
		return FAILED; 
	} 

	if (RxMessage.DLC!=2) 
	{ 
		return FAILED; 
	}
	 
	if ((RxMessage.Data[0]<<8 | RxMessage.Data[1])!=0xCAFE) 
	{ 
		return FAILED; 
	} 
	//printf("receive data:0X%X,0X%X",RxMessage.Data[0], RxMessage.Data[1]); 
	return PASSED; /* Test Passed */ 
} 


CANTestResult CAN_Interrupt(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	CanTxMsg TxMessage;
	uint32_t i = 0;
	
	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM=DISABLE;           // ʱ�䴥��ͨ�Ž�ֹ
	CAN_InitStructure.CAN_ABOM=DISABLE;	          // �����˳������ж���λ��0���˳�
	CAN_InitStructure.CAN_AWUM=DISABLE;	          // �Զ�����ģʽ������sleep
	CAN_InitStructure.CAN_NART=DISABLE;	          // �Զ����´��ͱ��ƣ�֪�����ͳɹ�
	CAN_InitStructure.CAN_RFLM=DISABLE;	          // FIFOû���������±��ĸ��Ǿɱ���
	CAN_InitStructure.CAN_TXFP=DISABLE;           // ���ͱ������ȼ�ȷ������־��
	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack; // �ػ�ģʽ
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;        // 1tq��BS1��BS2��ֵ���������й�
	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler=1;					  // ��Ƶϵ��Ϊ1
	CAN_Init(CAN1, &CAN_InitStructure);						// ��ʼ��CAN1
	
	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber=1;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* CAN FIFO0 message pending interrupt enable */ 
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); //�ж�ʹ��
	
	/* transmit 1 message */
	TxMessage.StdId=0x00;					 // ��׼��ʶ��Ϊ0
	TxMessage.ExtId=0x1234;				 // ������չ��ʾ����29λ��
	TxMessage.IDE=CAN_ID_EXT;			 // ʹ����չ��ʶ��
	TxMessage.RTR=CAN_RTR_DATA;		 // ��Ϣ����Ϊ����֡��һ֡8λ
	TxMessage.DLC=2;							 // ������֡��Ϣ
	TxMessage.Data[0]=0xDE;				 // ��һ֡��Ϣ
	TxMessage.Data[1]=0xCA;				 // �ڶ�֡��Ϣ
	CAN_Transmit(CAN1, &TxMessage);
	
	/* initialize the value that will be returned */
	CAN_return = 0xFF;
	   
	/* receive message with interrupt handling */
	i=0;
	while((CAN_return == 0xFF) && (i < 0xFFF))
	{
		i++;
	}
	
	if (i == 0xFFF)
	{
		CAN_return=0;  
	}
	
	/* disable interrupt handling */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
	
	return (CANTestResult)CAN_return;
}




/*
 * ��������USB_LP_CAN1_RX0_IRQHandler
 * ����  ��USB�жϺ�CAN�����жϷ������USB��CAN����I/O������ֻ�õ�CAN���жϡ�
 * ����  ����
 * ���  : ��	 
 * ����  ����
 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	
	RxMessage.StdId=0x00;
	RxMessage.ExtId=0x00;
	RxMessage.IDE=0;
	RxMessage.DLC=0;
	RxMessage.FMI=0;
	RxMessage.Data[0]=0x00;
	RxMessage.Data[1]=0x00;
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	
	if((RxMessage.ExtId==0x1234) && (RxMessage.IDE==CAN_ID_EXT)
	 && (RxMessage.DLC==2) && ((RxMessage.Data[1]|RxMessage.Data[0]<<8)==0xDECA))
	{
		CAN_return = 1; 
	}
	else
	{
		CAN_return = 0; 
	}
}


