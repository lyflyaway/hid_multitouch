//ADC�ɼ�����ķ�ѹ

#include "stm32f10x.h"

//ADC_DR���ݼĴ���������ADCת�������ֵ��������ΪDMA�Ĵ���Դ��ַ�����ĵ�ַ����ADC1����Ļ���ַ(0x4001 2400) ����ADC���ݼĴ���(ADC_DR)�ĵ�ַƫ�� (0x4c)����õ��ġ�
#define ADC1_DR_Address ((u32)0x40012400+0x4c)

volatile u16 ADC_ConvertedValue;

void ADC1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; /*����ADCģʽ*/ 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; /*��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�*/ 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; /*��������ת��ģʽ������ͣ�ؽ���ADCת��*/ 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; /*��ʹ���ⲿ����ת��*/ 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; /*�ɼ������Ҷ���*/ 
	ADC_InitStructure.ADC_NbrOfChannel = 1; /*Ҫת����ͨ����Ŀ1*/ 
	ADC_Init(ADC1, &ADC_InitStructure); 
	
	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/ 
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*����ADC1��ͨ��11Ϊ55.5���������� */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5); 
	
	/* Enable ADC1 DMA */ 
	ADC_DMACmd(ADC1, ENABLE); 
	
	/* Enable ADC1 */ 
	ADC_Cmd(ADC1, ENABLE); 
	
	/*��λУ׼�Ĵ��� */ 
	ADC_ResetCalibration(ADC1); 
	
	/*�ȴ�У׼�Ĵ�����λ��� */ 
	while(ADC_GetResetCalibrationStatus(ADC1)); 
	
	/* ADCУ׼ */ 
	ADC_StartCalibration(ADC1); 
	/* �ȴ�У׼���*/ 
	while(ADC_GetCalibrationStatus(ADC1)); 
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 	
}

