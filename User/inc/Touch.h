#ifndef __TOUCH_H
#define __TOUCH_H

#define Touch_Down 0x01
#define Touch_Up   0x00

/*������оƬ������������	*/   
#define TOUCH  PCin(4)   //PC4 INT
#define DOUT PAin(6)   //PA6  MISO
#define TDIN PAout(7)  //PA7  MOSI
#define TCLK PAout(5)  //PA5  SCLK
#define TCS  PCout(6)  //PC6  CS 

/* ADS7843/7846/UH7843/7846/XPT2046/TSC2046 ָ� */
#define TOUCH_CMD_Y 0X90  //0B10010000���ò�ַ�ʽ��X����
#define TOUCH_CMD_X	0XD0  //0B11010000���ò�ַ�ʽ��Y���� 

typedef struct 
{
	u16 X0;//ԭʼ����
	u16 Y0;
	u16 X; //����/�ݴ�����
	u16 Y;						   	    
	u8  Key_Sta;//�ʵ�״̬			  
	//������У׼����
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


