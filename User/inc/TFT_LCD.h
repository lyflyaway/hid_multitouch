#ifndef __TFT_LCD_H
#define __TFT_LCD_H

#define TFT_LCD_CS  PDout(12)
#define TFT_LCD_RS	PDout(13)
#define TFT_LCD_WR  PDout(14)
#define TFT_LCD_RD	PDout(15)


#define   BLACK        0x0000                    /* 黑色： 0, 0, 0               */
#define   NAVY         0x000F                    /* 深蓝色： 0, 0, 128           */
#define   DGREEN       0x03E0                    /* 深绿色： 0, 128, 0           */
#define   DCYAN        0x03EF                    /* 深青色： 0, 128, 128         */
#define   MAROON       0x7800                    /* 深红色：128, 0, 0            */
#define   PURPLE       0x780F                    /* 紫色： 128, 0, 128           */
#define   OLIVE        0x7BE0                    /* 橄榄绿：128, 128, 0          */
#define   LGRAY        0xC618                    /* 灰白色：192, 192, 192        */
#define   DGRAY        0x7BEF                    /* 深灰色：128, 128, 128        */
#define   BLUE         0x001F                    /* 蓝色： 0, 0, 255             */
#define   GREEN        0x07E0                 	 /* 绿色： 0, 255, 0             */
#define   CYAN         0x07FF                    /* 青色： 0, 255, 255           */
#define   RED          0xF800                    /* 红色： 255, 0, 0             */
#define   MAGENTA      0xF81F                    /* 品红： 255, 0, 255           */
#define   YELLOW       0xFFE0                    /* 黄色： 255, 255, 0           */
#define   WHITE        0xFFFF                    /* 白色： 255, 255, 255         */


uint16_t TFT_LCD_Read_Reg(uint16_t TFT_LCD_Reg);
void TFT_LCD_Write_Reg(uint16_t TFT_LCD_Reg, uint16_t TFT_LCD_Dat);
void TFT_LCD_Write_Cmd(uint16_t TFT_LCD_Cmd);
void TFT_LCD_Write_Dat(uint16_t TFT_LCD_Dat);
void TFT_LCD_Config(void);
void TFT_LCD_Delay(uint32_t nCount);
void TFT_LCD_Set_Cursor(u8 Xpos, u16 Ypos);
void TFT_LCD_Set_Window(u16 START_X, u16 END_X, u16 START_Y, u16 END_Y);
void TFT_LCD_Clear(uint16_t Color);
void TFT_LCD_Show_Picture(u16 P_X, u8 P_Y, u16 P_W, u8 P_H, u8 *bitmap);


#endif


