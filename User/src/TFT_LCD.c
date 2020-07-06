#include "stm32f10x.h"
#include "bit_IO.h"
#include "TFT_LCD.h"

uint16_t TFT_LCD_DeviceCode;


void TFT_LCD_Delay(uint32_t nCount)
{
	__IO uint16_t i;	 	
	for (i=0;i<nCount*100;i++);
}

uint16_t TFT_LCD_Read_Reg(uint16_t TFT_LCD_Reg)
{
	uint16_t temp;

	TFT_LCD_Write_Cmd(TFT_LCD_Reg);  //写入要读的寄存器号  
	GPIOE->CRH = (GPIOE->CRH & 0x00000000) | 0x44444444;  //将端口高8位配置成输入
	GPIOE->CRL = (GPIOE->CRL & 0x00000000) | 0x44444444;  //将端口低8位配置成输入
	TFT_LCD_CS = 0;
	TFT_LCD_RS = 1;
	TFT_LCD_RD = 0;
	temp = GPIOE->IDR&0xFFFF; 	//读取数据(读寄存器时,并不需要读2次)
	TFT_LCD_RD = 1;
	TFT_LCD_CS = 1;
	GPIOE->CRH = (GPIOE->CRH & 0x00000000) | 0x33333333;  //释放端口高8位为输出
	GPIOE->CRL = (GPIOE->CRL & 0x00000000) | 0x33333333;  //释放端口低8位为输出

	return temp;   
}



void TFT_LCD_Write_Reg(uint16_t TFT_LCD_Reg, uint16_t TFT_LCD_Dat)
{
	TFT_LCD_Write_Cmd(TFT_LCD_Reg);
	TFT_LCD_Write_Dat(TFT_LCD_Dat);
}

void TFT_LCD_Write_Cmd(uint16_t TFT_LCD_Cmd)
{
	TFT_LCD_CS = 0;
	TFT_LCD_RS = 0;
	GPIOE->ODR = TFT_LCD_Cmd;
	TFT_LCD_WR = 0;
	TFT_LCD_WR = 1;	
	TFT_LCD_CS = 1;
}

void TFT_LCD_Write_Dat(uint16_t TFT_LCD_Dat)
{
	TFT_LCD_CS = 0;
	TFT_LCD_RS = 1;
	GPIOE->ODR = TFT_LCD_Dat;
	TFT_LCD_WR = 0;
	TFT_LCD_WR = 1;	
	TFT_LCD_CS = 1;
}


void TFT_LCD_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);	
	
	/* 配置控制IO 连接到PD12.PD13.PD14.PD15 *********************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽输出方式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 输出IO口最大最速为50MHZ
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* 配置数据IO 连接到GPIOE *********************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽输出方式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 输出IO口最大最速为50MHZ
  	GPIO_Init(GPIOE, &GPIO_InitStructure);	


	TFT_LCD_Write_Reg(0x0000,0x0001);
	TFT_LCD_Delay(5); // LCD_Delay 50 ms 
	TFT_LCD_DeviceCode = TFT_LCD_Read_Reg(0x0000); 	
	//************* Start Initial Sequence **********//
	TFT_LCD_Write_Reg(0x00, 0x0001); // Start internal OSC.
	TFT_LCD_Write_Reg(0x01, 0x3B3F); // Driver output control, RL=0;REV=1;GD=1;BGR=0;SM=0;TB=1
	TFT_LCD_Write_Reg(0x02, 0x0600); // set 1 line inversion
	//************* Power control setup ************/
	TFT_LCD_Write_Reg(0x0C, 0x0007); // Adjust VCIX2 output voltage
	TFT_LCD_Write_Reg(0x0D, 0x0006); // Set amplitude magnification of VLCD63
	TFT_LCD_Write_Reg(0x0E, 0x3200); // Set alternating amplitude of VCOM
	TFT_LCD_Write_Reg(0x1E, 0x00BB); // Set VcomH voltage
	TFT_LCD_Write_Reg(0x03, 0x6A64); // Step-up factor/cycle setting
	//************ RAM position control **********/
	TFT_LCD_Write_Reg(0x0F, 0x0000); // Gate scan position start at G0.
	TFT_LCD_Write_Reg(0x44, 0xEF00); // Horizontal RAM address position
	TFT_LCD_Write_Reg(0x45, 0x0000); // Vertical RAM address start position
	TFT_LCD_Write_Reg(0x46, 0x013F); // Vertical RAM address end position
	// ----------- Adjust the Gamma Curve ----------//
	TFT_LCD_Write_Reg(0x30, 0x0000);
	TFT_LCD_Write_Reg(0x31, 0x0706);
	TFT_LCD_Write_Reg(0x32, 0x0206);
	TFT_LCD_Write_Reg(0x33, 0x0300);
	TFT_LCD_Write_Reg(0x34, 0x0002);
	TFT_LCD_Write_Reg(0x35, 0x0000);
	TFT_LCD_Write_Reg(0x36, 0x0707);
	TFT_LCD_Write_Reg(0x37, 0x0200);
	TFT_LCD_Write_Reg(0x3A, 0x0908);
	TFT_LCD_Write_Reg(0x3B, 0x0F0D);
	//************* Special command **************/
	TFT_LCD_Write_Reg(0x28, 0x0006); // Enable test command
	TFT_LCD_Write_Reg(0x2F, 0x12EB); // RAM speed tuning
	TFT_LCD_Write_Reg(0x26, 0x7000); // Internal Bandgap strength
	TFT_LCD_Write_Reg(0x20, 0xB0E3); // Internal Vcom strength
	TFT_LCD_Write_Reg(0x27, 0x0044); // Internal Vcomh/VcomL timing
	TFT_LCD_Write_Reg(0x2E, 0x7E45); // VCOM charge sharing time  
	//************* Turn On display ******************/
	TFT_LCD_Write_Reg(0x10, 0x0000); // Sleep mode off.
	TFT_LCD_Delay(8); // Wait 30mS
	TFT_LCD_Write_Reg(0x11, 0x6858); // Entry mode setup. 262K type B, take care on the data bus with 16it only
	TFT_LCD_Write_Reg(0x07, 0x0033); // Display ON   */		
}

void TFT_LCD_Set_Cursor(u8 Xpos, u16 Ypos)
{
	TFT_LCD_Write_Reg(0x004E, Xpos);
	TFT_LCD_Write_Reg(0X004F, Ypos);
}

void TFT_LCD_Set_Window(u16 START_X, u16 END_X, u16 START_Y, u16 END_Y)
{
	TFT_LCD_Write_Reg(0x44, START_Y | (END_Y - 1)<<8); // Horizontal RAM address position
	TFT_LCD_Write_Reg(0x45, START_X); // Vertical RAM address start position
	TFT_LCD_Write_Reg(0x46, END_X-1); // Vertical RAM address end position	
}


void TFT_LCD_Clear(uint16_t Color)
{
	uint32_t i;      
	TFT_LCD_Set_Cursor(0x00,0x0000); //设置光标位置
	TFT_LCD_Write_Cmd(0x22);    	//开始写入GRAM	 	  
	for(i = 0; i < 76800; i++)  		//320*240=76800
	{
		TFT_LCD_Write_Dat(Color);    
	}
}

void TFT_LCD_Show_Picture(u16 P_X, u8 P_Y, u16 P_W, u8 P_H, u8 *bitmap)
{
	uint32_t i; 
	uint32_t P_size = P_W * P_H;   
	u16 *bitmap_ptr = (u16 *)bitmap; 
	
	TFT_LCD_Set_Window(P_X, P_X + P_W, P_Y, P_Y + P_H); 
	TFT_LCD_Set_Cursor(P_Y, P_X + P_W - 1); //设置光标位置
	TFT_LCD_Write_Cmd(0x22);    	//开始写入GRAM	 	  
	for(i = 0; i < P_size; i++)
	{
		TFT_LCD_Write_Dat(*bitmap_ptr++);
	}	
}


