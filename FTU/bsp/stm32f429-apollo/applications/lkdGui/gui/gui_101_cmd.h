
#ifndef __GUI101CMD_H
#define	__GUI101CMD_H

#include "stdint.h"

/* ������y ������ x */
#define  GUI_LCM_XMAX	160
#define  GUI_LCM_YMAX	160
#define  GUIXMAX	GUI_LCM_XMAX
#define  GUIYMAX	GUI_LCM_YMAX

/* ������������ */
typedef int32_t lkdCoord;
/* ������ɫ���� */
typedef uint8_t lkdColour;

/* ������뷽ʽ */
typedef enum{
	FONT_LEFT,	/* ����� */
	FONT_MID,	/* ���� */
	FONT_RIGHT,	/* �Ҷ��� */
}FontFlag;

/* ������ */
typedef struct{
	int16_t x;
	int16_t y;
	uint16_t hight;
	uint16_t max;
	uint16_t lump;
}SCROLL;

/* ����λͼ�ṹ�� */
typedef struct{
	uint8_t number;
	uint8_t *bitmap;/* λͼ���� */
	uint16_t wide;/* λͼ�� */
	uint16_t high;/* λͼ�� */
	uint16_t beginx;/* λͼ��ʼ���� */
	uint16_t beginy;/* λͼ��ʼ���� */
}lkdBitmap;

#define PACKBUFFMAX 1024
typedef struct{
	uint8_t state;
	uint8_t *packBuff;
	uint16_t pIn;
	uint8_t cmdNum;
}Gui101CmdControl;

/* Hmi101֡����� */
typedef struct{
	uint8_t cmdNum;
	uint8_t *cmdBegin;
	union{
		uint16_t len16;
		uint8_t len8[2];
	}frameLen;
	uint16_t cmdOffset;
	uint16_t pContent;
}Hmi101FrameResult;


/* ����ǰ��ɫ/����ɫ */
extern lkdColour forecolor,backcolor;

void GuiBiasLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdCoord y1, lkdColour color);
void GuiHPointLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, uint8_t interval, lkdColour color);
void GuiRPointLine(lkdCoord x0, lkdCoord y0, lkdCoord y1,uint8_t interval, lkdColour color);
void GuiHLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdColour color);
void GuiRLine(lkdCoord x0, lkdCoord y0, lkdCoord y1, lkdColour color);
void GuiRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color);
void GuiFillRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color);
void GuiFont12(lkdCoord x, lkdCoord y, uint8_t *str);
void GuiFont12Align(lkdCoord x, lkdCoord y,uint16_t wide, FontFlag flag,uint8_t *str);
void GuiUpdateDisplayAll(void);
void GuiSetbackcolor(lkdColour colour);
void GuiSetForecolor(lkdColour colour);
void GuiExchangeColor(void);
void GuiAreaBitMap(lkdBitmap *bitMap,\
	lkdCoord x0, lkdCoord y0, lkdCoord endx, lkdCoord endy, uint8_t flag);
void OpenLcdDisplay(void);
void CloseLcdDisplay(void);
void GuiButton(int16_t x,int16_t y,uint8_t *keyName,uint8_t flag);
void GuiVScroll(SCROLL *pScroll);
void HmiCmd001Fill(uint8_t type, uint8_t num,uint8_t *pBuff);

/* �ɷ�����ɺ������� */
void Cmd101SendFinish(void);

void Hmi101Init(void);
void HmiStaticMemoryApply(void);
uint8_t Hmi101RunTimerOut(void);

extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);
extern uint8_t DLT634_HMI_SLAVE_C_REPLY(uint8_t drvid,uint8_t *pbuf);

extern void YaoxinDisplayInit(void);
#endif /* __GUI101CMD_H */

/* END */
