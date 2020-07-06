#ifndef __TOUCH_H
#define __TOUCH_H

#define Touch_Down 0x01
#define Touch_Up   0x00

/*触摸屏芯片连接引脚配置	*/   
#define TOUCH  PCin(4)   //PC4 INT
#define DOUT PAin(6)   //PA6  MISO
#define TDIN PAout(7)  //PA7  MOSI
#define TCLK PAout(5)  //PA5  SCLK
#define TCS  PCout(6)  //PC6  CS 

/* ADS7843/7846/UH7843/7846/XPT2046/TSC2046 指令集 */
#define TOUCH_CMD_Y 0X90  //0B10010000即用差分方式读X坐标
#define TOUCH_CMD_X	0XD0  //0B11010000即用差分方式读Y坐标 

typedef struct 
{
	u16 X0;//原始坐标
	u16 Y0;
	u16 X; //最终/暂存坐标
	u16 Y;						   	    
	u8  Key_Sta;//笔的状态			  
	//触摸屏校准参数
	float xfac;
	float yfac;
	short xoff;
	short yoff;
}Touch_TypeDef;

extern Touch_TypeDef Touch_Point;


void Touch_Config(void);
void TOUCH_Write_Cmd(uint8_t TOUCH_CMD);
uint16_t TOUCH_Read_AD(uint8_t TOUCH_CMD);

#endif


