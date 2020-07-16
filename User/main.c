#include <stdio.h>
#include "stm32f10x.h"
#include "bit_IO.h"
#include "LED.h"
#include "USART.h"
#include "ADC.h"
//#include "ChipTempreture.h"
#include "PWM.h"
#include "TFT_LCD.h"
#include "Touch.h"
#include "CAN.h"



__IO uint8_t PrevXferComplete = 1;
extern unsigned char gImage_pic[];
//extern volatile u16 ADC_ConvertedValue;



void Delay(vu32 nCount)
{
	u32 x;
	for(; nCount != 0; nCount--);
		for(x=0x2FFFFF; x != 0; x--);
}


extern void USB_Init(void);
void Joystick_Send(void);

int main(void)
{
    uint8_t counter = 0;
//	float ADC_ConvertedValueLocal;
//	__IO u16 Current_Temp;
//	__IO u16 V25 = 0x6E2;		//temperature CON
//	__IO u16 Avg_Slope = 0x05;	//temperature CON
//	u16 a,b,c,d;				//TOUCH VAR
	
	
//	LED_Config();
//	ADC1_Config();	
//	ADC_ChipTempreture_Config();
	
//	USART1_Config();
	
//	TIM3_PWM_Config();
	
//	TFT_LCD_Config();
//	TFT_LCD_Clear(WHITE);
//	TFT_LCD_Show_Picture(2, 2, 310, 232, gImage_pic);

//	Touch_Config();
//	Touch_Point.Key_Sta = Touch_Up;
	
//	CAN_Config();
//	printf("CAN test result loopback = %d, interrupt = %d ", CAN_Polling(), CAN_Interrupt());

    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_9);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USB_Init();
	
	while(1)
	{
	    if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))
	    {
	        if (counter++ >= 100)
	        {
	            counter = 0;
                Joystick_Send();
//                while (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8));
	        }
	    }
	    else
	    {
	        counter = 0;
	    }
//	    GPIO_ResetBits(GPIOC, GPIO_Pin_9);
//        Delay(0xFFFFF);	
//	    GPIO_SetBits(GPIOC, GPIO_Pin_9);
//	    Delay(0xFFFFF);	

	    
//    	Delay(0xFFFFF);	
//    	GPIO_SetBits(GPIOD, GPIO_Pin_8);
//    	GPIO_ResetBits(GPIOD,GPIO_Pin_11);
//
//    	Delay(0xFFFFF);	
//    	GPIO_SetBits(GPIOD, GPIO_Pin_9);
//    	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
//		
//    	Delay(0xFFFFF);	
//    	GPIO_SetBits(GPIOD, GPIO_Pin_10);
//    	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
//		
//    	Delay(0xFFFFF);	
//    	GPIO_SetBits(GPIOD, GPIO_Pin_11);
//    	GPIO_ResetBits(GPIOD,GPIO_Pin_10);
		
//		ADC_ConvertedValueLocal = (float) ADC_ConvertedValue*3.3/4096;
//		printf("\r\n %f V \r\n", ADC_ConvertedValueLocal);
//		//USART1_printf(USART1,"\r\n %f V \r\n", ADC_ConvertedValueLocal);
		
//		Current_Temp = (unsigned short int)((V25 - ADC_ConvertedValue) / Avg_Slope + 25);
//		printf("The current temperature = %03d C\r\n", Current_Temp);



//		if(Touch_Point.Key_Sta == Touch_Down)
//		{
//			EXTI->IMR&=~(1<<4); //关闭line4上的中断
//			do
//			{
//				a = TOUCH_Read_AD(TOUCH_CMD_X);
//				b = TOUCH_Read_AD(TOUCH_CMD_Y);
//				if(a<100 || b<100) continue;
//				
//				c = TOUCH_Read_AD(TOUCH_CMD_X);
//				d = TOUCH_Read_AD(TOUCH_CMD_Y);
//				if(c<100 || d<100) continue;
//				
//				if(((a>=c && a<c+50) || (c>=a && c<a+50)) && ((b>=d && b<d+50) || (d>=b && d<b+50)))
//				{
//					printf("%d %d \r\n", (a+c)/2, (b+d)/2);
//					break;
//				}
//			}
//			while(TOUCH == 0);
//			Touch_Point.Key_Sta = Touch_Up;	
//			EXTI->IMR|=1<<4;   //开启line4上的中断		
//		}
		
		
		
		
	}
}


