//ADC采集电阻的分压

#include "stm32f10x.h"

//ADC_DR数据寄存器保存了ADC转换后的数值，以它作为DMA的传输源地址。它的地址是由ADC1外设的基地址(0x4001 2400) 加上ADC数据寄存器(ADC_DR)的地址偏移 (0x4c)计算得到的。
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
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; /*独立ADC模式*/ 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; /*禁止扫描模式，扫描模式用于多通道采集*/ 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; /*开启连续转换模式，即不停地进行ADC转换*/ 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; /*不使用外部触发转换*/ 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; /*采集数据右对齐*/ 
	ADC_InitStructure.ADC_NbrOfChannel = 1; /*要转换的通道数目1*/ 
	ADC_Init(ADC1, &ADC_InitStructure); 
	
	/*配置ADC时钟，为PCLK2的8分频，即9MHz*/ 
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*配置ADC1的通道11为55.5个采样周期 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5); 
	
	/* Enable ADC1 DMA */ 
	ADC_DMACmd(ADC1, ENABLE); 
	
	/* Enable ADC1 */ 
	ADC_Cmd(ADC1, ENABLE); 
	
	/*复位校准寄存器 */ 
	ADC_ResetCalibration(ADC1); 
	
	/*等待校准寄存器复位完成 */ 
	while(ADC_GetResetCalibrationStatus(ADC1)); 
	
	/* ADC校准 */ 
	ADC_StartCalibration(ADC1); 
	/* 等待校准完成*/ 
	while(ADC_GetCalibrationStatus(ADC1)); 
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 	
}

