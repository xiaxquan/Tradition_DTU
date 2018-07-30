/**
  * @file   Gui101Cmd.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/11
  * @brief  Gui101CmdMaster
  */

#include "gui_101_cmd.h"
#include "gui_hmi_101.h"
#include "string.h"
#include "stdio.h"
#include "lkdGuiMenu.h"
#include "lkdGuiWindow.h"
#include <rtthread.h>
#include "hmiInOut.h"
#include "userVariable.h"
#include "gui_common.h"
#include "GUIdisplay.h"

/* hmi101�߳�ʹ�� */
static rt_thread_t Hmi101Thread;
static rt_uint8_t *Hmi101Threadstack;
static uint8_t Hmi101ThreadFlag = 0;
/* cmd101�����¼� */
struct rt_event Cmd101SendEvent;
/* cmd101���ƽṹ */
Gui101CmdControl cmd101;
/* ����ǰ��ɫ/����ɫ */
lkdColour forecolor = 1,backcolor;

/* �������� */
void Hmi101ThreadRunMontor(void);

/**
  *@brief  ����101ѹջ
  *@param  None
  *@retval None
  */
void EndCmd101Down(void)
{
	union{
		uint16_t len16;
		uint8_t len8[2];
	}frameLen;	
	frameLen.len16 = cmd101.pIn;
	cmd101.state = 2;
	cmd101.packBuff[0] = frameLen.len8[0];
	cmd101.packBuff[1] = frameLen.len8[1];
	cmd101.packBuff[2] = cmd101.cmdNum;
	DLT634_HMI_SLAVE_C_REPLY(0,cmd101.packBuff);
}

/**
  *@brief cmd101ѹջ���ƺ���
  *@param  x,y ����
  *@param  color ��ɫ
  *@retval None
  */
static void Cmd101DownControl(uint16_t len)
{
	if(cmd101.state == 0){//ջ��,����ѹջ
		cmd101.cmdNum = 0;
		cmd101.pIn = 3;
		cmd101.state = 1;
	}
	if(cmd101.state == 2){//���ڷ���,�����ȴ�
		rt_event_recv(&Cmd101SendEvent, (1 << 0), RT_EVENT_FLAG_OR |\
			RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);
		cmd101.cmdNum = 0;
		cmd101.pIn = 3;
		cmd101.state = 1;
	}
	if(cmd101.state == 1){//ѹջ��
		if(cmd101.pIn + len >= PACKBUFFMAX - 20){
			EndCmd101Down();
			rt_event_recv(&Cmd101SendEvent, (1 << 0), RT_EVENT_FLAG_OR |\
				RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);
			cmd101.cmdNum = 0;
			cmd101.pIn = 3;
			cmd101.state = 1;
		}
	}
}

/**
  *@brief cmd101������ɺ���
  *@param  x,y ����
  *@param  color ��ɫ
  *@retval None
  */
void Cmd101SendFinish(void)
{
	//��������¼�
	rt_event_send(&Cmd101SendEvent, (1 << 0));
}

/**
  *@brief  ��ʼ101ѹջ
  *@param  None
  *@retval None
  */
void BeginCmd101Down(void)
{
	if(cmd101.state != 0){
	}
	else{
		cmd101.pIn = 3;
	}
}

/**
  *@brief ��б��
  *@param  x,y ����
  *@param  color ��ɫ
  *@retval None
  */
void GuiBiasLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord x1, lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_BLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = 1;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}
/**
  *@brief ��ˮƽ����
  *@param  x0,y0 ��ʼ����
  *@param  x1 ��������
  *@param  interval ÿ��N�㻭һ����
  *@param  color ��ɫ
  *@retval None
  */
void GuiHPointLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord x1, uint8_t interval, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_HLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = interval;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}

/**
  *@brief ����ֱ����
  *@param  x0,y0 ��ʼ����
  *@param  y1 ��������
  *@param  interval ÿ��N�㻭һ����
  *@param  color ��ɫ
  *@retval None
  */
void  GuiRPointLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord y1,uint8_t interval, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_RLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = interval;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}
/**
  *@brief ��ˮƽ��
  *@param  x0,y0 ��ʼ����
  *@param  x1��������
  *@param  color ��ɫ
  *@retval None
  */
void GuiHLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_HLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = 1;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}

/**
  *@brief ����ֱ��
  *@param  x0,y0 ��ʼ����
  *@param  y1 ��������
  *@param  color ��ɫ
  *@retval None
  */
void  GuiRLine(lkdCoord x0, lkdCoord y0, lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_RLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = 1;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}

/**
  *@brief ������
  *@param  x0,y0 ��ʼ����
  *@param  x1,y1 ��������
  *@param  color ��ɫ
  *@retval None
  */
void GuiRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(8);
	cmd101.packBuff[cmd101.pIn + CMD101_CMD] = 101;
	cmd101.packBuff[cmd101.pIn + CMD101_TYPE] = C101TYPE_RECT;
	cmd101.packBuff[cmd101.pIn + CMD101_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD101_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD101_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD101_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD101_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD101_LEN] = 8;
	cmd101.pIn += 8;
	cmd101.cmdNum += 1;
}

/**
  *@brief ��������
  *@param  x0,y0 ��ʼ����
  *@param  x1,y1 ��������
  *@param  color ��ɫ
  *@retval None
  */
void GuiFillRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(8);
	cmd101.packBuff[cmd101.pIn + CMD101_CMD] = 101;
	cmd101.packBuff[cmd101.pIn + CMD101_TYPE] = C101TYPE_FILLRECT;
	cmd101.packBuff[cmd101.pIn + CMD101_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD101_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD101_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD101_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD101_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD101_LEN] = 8;
	cmd101.pIn += 8;
	cmd101.cmdNum += 1;
}

/**
  *@brief 12x12���庯��
  *@param  x,y ��ʼ����
  *@param  str �ַ���
  *@retval None
  */
void GuiFont12(lkdCoord x, lkdCoord y, uint8_t *str)
{
	uint8_t tstrlen;
	
	tstrlen = strlen((char *)str) + 1;
	Cmd101DownControl(tstrlen + 7);
	cmd101.packBuff[cmd101.pIn + CMD108_CMD] = 108;
	cmd101.packBuff[cmd101.pIn + CMD108_TYPE] = C108TYPE_DEFAULT;
	cmd101.packBuff[cmd101.pIn + CMD108_BX] = x;
	cmd101.packBuff[cmd101.pIn + CMD108_BY] = y;
	cmd101.packBuff[cmd101.pIn + CMD108_WIDE] = 160;
	cmd101.packBuff[cmd101.pIn + CMD108_HIGH] = 160;
	cmd101.packBuff[cmd101.pIn + CMD108_FlAG] = C108FLAG_FEEDLINE;
	memcpy(&cmd101.packBuff[cmd101.pIn + CMD108_CONTENT],str,tstrlen);
	cmd101.packBuff[cmd101.pIn + CMD108_LEN] = tstrlen + 7;
	cmd101.pIn += tstrlen + 7;
	cmd101.cmdNum += 1;
}

/**
  *@brief 12x12���������ʾ����
  *@param  x,y ��ʼ����
  *@param  wide ���
  *@param  flag �����־
  *@param  str �ַ���
  *@retval None
  */
void GuiFont12Align(lkdCoord x, lkdCoord y,uint16_t wide, FontFlag flag,uint8_t *str)
{
	uint8_t tstrlen;
	tstrlen = strlen((char *)str) + 1;
	Cmd101DownControl(tstrlen + 7);
	cmd101.packBuff[cmd101.pIn + CMD108_CMD] = 108;
	cmd101.packBuff[cmd101.pIn + CMD108_TYPE] = C108TYPE_DEFAULT;
	cmd101.packBuff[cmd101.pIn + CMD108_BX] = x;
	cmd101.packBuff[cmd101.pIn + CMD108_BY] = y;
	cmd101.packBuff[cmd101.pIn + CMD108_WIDE] = wide;
	cmd101.packBuff[cmd101.pIn + CMD108_HIGH] = 14;
	cmd101.packBuff[cmd101.pIn + CMD108_FlAG] = flag;
	memcpy(&cmd101.packBuff[cmd101.pIn + CMD108_CONTENT],str,tstrlen);
	cmd101.packBuff[cmd101.pIn + CMD108_LEN] = tstrlen + CMD108_CONTENT;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD108_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief ��ֱ������
  *@param  pScroll Scroll handle
  *@retval None
  */
void GuiVScroll(SCROLL *pScroll)
{
	Cmd101DownControl(8);
	cmd101.packBuff[cmd101.pIn + CMD105_CMD] = 105;
	cmd101.packBuff[cmd101.pIn + CMD105_TYPE] = C105TYPE_RSCROLL;
	cmd101.packBuff[cmd101.pIn + CMD105_BX] = pScroll->x;
	cmd101.packBuff[cmd101.pIn + CMD105_BY] = pScroll->y;
	cmd101.packBuff[cmd101.pIn + CMD105_SLEN] = pScroll->hight;
	cmd101.packBuff[cmd101.pIn + CMD105_SMAX] = pScroll->max;
	cmd101.packBuff[cmd101.pIn + CMD105_SLUMP] = pScroll->lump;
	cmd101.packBuff[cmd101.pIn + CMD105_LEN] = 8;
	cmd101.pIn += 8;
	cmd101.cmdNum += 1;
}

/**
  *@brief ����
  *@param  x��y ��ʼ����
  *@param  keyName ��������
  *@param  flag 0��ѡ�� 1ѡ��
  *@retval None
  */
void GuiButton(int16_t x,int16_t y,uint8_t *keyName,uint8_t flag)
{
	uint8_t tstrlen;
	tstrlen = strlen((char *)keyName) + 1;
	Cmd101DownControl(tstrlen + 7);
	cmd101.packBuff[cmd101.pIn + CMD106_CMD] = 106;
	cmd101.packBuff[cmd101.pIn + CMD106_TYPE] = C106TYPE_DEFAULT;
	cmd101.packBuff[cmd101.pIn + CMD106_BX] = x;
	cmd101.packBuff[cmd101.pIn + CMD106_BY] = y;
	cmd101.packBuff[cmd101.pIn + CMD106_FlAG] = flag;
	memcpy(&cmd101.packBuff[cmd101.pIn + CMD106_CONTENT],keyName,tstrlen);
	cmd101.packBuff[cmd101.pIn + CMD106_LEN] = tstrlen + CMD106_CONTENT;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD106_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief ����ʾ
  *@param  None
  *@retval None
  */
void CloseLcdDisplay(void)
{
	Cmd101DownControl(3);
	cmd101.packBuff[cmd101.pIn + CMD104_CMD] = 104;
	cmd101.packBuff[cmd101.pIn + CMD104_TYPE] = C104TYPE_OFFSCREEN;
	cmd101.packBuff[cmd101.pIn + CMD104_LEN] = 3;
	cmd101.pIn += 3;
	cmd101.cmdNum += 1;
	EndCmd101Down();
}

/**
  *@brief ����ʾ
  *@param  None
  *@retval None
  */
void OpenLcdDisplay(void)
{
	Cmd101DownControl(3);
	cmd101.packBuff[cmd101.pIn + CMD104_CMD] = 104;
	cmd101.packBuff[cmd101.pIn + CMD104_TYPE] = C104TYPE_ONSCREEN;
	cmd101.packBuff[cmd101.pIn + CMD104_LEN] = 3;
	cmd101.pIn += 3;
	cmd101.cmdNum += 1;
	EndCmd101Down();
}

/**
  *@brief ��ָ����Χ��λͼ
  *@param  bitMap λͼ����
  *@param  x0,y0 ��ʼ����
  *@param  endx,endy �յ�����
  *@param  flag 0 ������ʾ 1����
  *@retval None
  */
void GuiAreaBitMap(lkdBitmap *bitMap,\
	lkdCoord x0, lkdCoord y0, lkdCoord endx, lkdCoord endy, uint8_t flag)
{
	uint16_t tstrlen;
	
	if(bitMap->number < 1){
		if((bitMap->wide % 8) == 0){
			tstrlen = bitMap->high * (bitMap->wide / 8);
		}
		else{
			tstrlen = bitMap->high * (bitMap->wide / 8 + 1);
		}
		Cmd101DownControl(tstrlen + CMD107_CONTENT);
		memcpy(&cmd101.packBuff[cmd101.pIn + CMD107_CONTENT],bitMap->bitmap,tstrlen);
	}
	else{
		tstrlen = 1;
		Cmd101DownControl(tstrlen + CMD107_CONTENT);
		cmd101.packBuff[cmd101.pIn + CMD107_CONTENT] = '\0';
	}
	cmd101.packBuff[cmd101.pIn + CMD107_CMD] = 107;
	cmd101.packBuff[cmd101.pIn + CMD107_TYPE] = bitMap->number;
	cmd101.packBuff[cmd101.pIn + CMD107_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD107_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD107_EX] = endx;
	cmd101.packBuff[cmd101.pIn + CMD107_EY] = endy;
	cmd101.packBuff[cmd101.pIn + CMD107_MXLEN] = bitMap->wide;
	cmd101.packBuff[cmd101.pIn + CMD107_MYLEN] = bitMap->high;
	cmd101.packBuff[cmd101.pIn + CMD107_FlAG] = flag;
	
	cmd101.packBuff[cmd101.pIn + CMD107_LEN] = tstrlen + CMD107_CONTENT;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD107_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief ������ʾ
  *@param  None
  *@retval None
  */
void GuiUpdateDisplayAll(void)
{
	Cmd101DownControl(2);
	cmd101.packBuff[cmd101.pIn + CMD103_CMD] = 103;
	cmd101.packBuff[cmd101.pIn + CMD103_LEN] = 2;
	cmd101.pIn += 2;
	cmd101.cmdNum += 1;
	EndCmd101Down();
}

/**
  *@brief ��ȡǰ��ɫ
  *@param  None
  *@retval ����ǰ��ɫ
  */
lkdColour GuiGetForecolor(void)
{
	return forecolor;
}

/**
  *@brief ����ǰ��ɫ
  *@param  colour ǰ��ɫ
  *@retval None
  */
void GuiSetForecolor(lkdColour colour)
{
	Cmd101DownControl(4);
	forecolor = colour;
	cmd101.packBuff[cmd101.pIn + CMD109_CMD] = 109;
	cmd101.packBuff[cmd101.pIn + CMD109_TYPE] = C109TYPE_FORE;
	cmd101.packBuff[cmd101.pIn + CMD109_COLOUR] = forecolor;
	cmd101.packBuff[cmd101.pIn + CMD109_LEN] = 4;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD109_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief ��ȡ����ɫ
  *@param  None
  *@retval ���ر���ɫ
  */
lkdColour GuiGetbackcolor(void)
{
	return backcolor;
}
/**
  *@brief ���ñ���ɫ
  *@param  colour ����ɫ
  *@retval None
  */
void GuiSetbackcolor(lkdColour colour)
{
	Cmd101DownControl(4);
	backcolor = colour;
	cmd101.packBuff[cmd101.pIn + CMD109_CMD] = 109;
	cmd101.packBuff[cmd101.pIn + CMD109_TYPE] = C109TYPE_BACK;
	cmd101.packBuff[cmd101.pIn + CMD109_COLOUR] = backcolor;
	cmd101.packBuff[cmd101.pIn + CMD109_LEN] = 4;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD109_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief ����ǰ��ɫ�ͱ���ɫ
  *@param  None
  *@retval None
  */
void GuiExchangeColor(void)
{
	Cmd101DownControl(4);
	lkdColour tColour;
	tColour = backcolor;
	backcolor = forecolor;
	forecolor = tColour;
	cmd101.packBuff[cmd101.pIn + CMD109_CMD] = 109;
	cmd101.packBuff[cmd101.pIn + CMD109_TYPE] = C109TYPE_EXCHANGE;
	cmd101.packBuff[cmd101.pIn + CMD109_COLOUR] = forecolor;
	cmd101.packBuff[cmd101.pIn + CMD109_LEN] = 4;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD109_LEN];
	cmd101.cmdNum += 1;
}
/**
  *@brief cmd101�������
  *@param  type ����������� 0 ������ 1 ����
  *@param  num �������
  *@param  pBuff ��� type = 0 pBuff[i*2 + 0] = ����� pBuff[i*2 + 1] = ����Ŷ�Ӧ״̬ (i = num -1)
  *@param  pBuff ��� type = 1 pBuff[0] = ����� pBuff[1] = �鳤�� pBuff[2..] ��״̬
  *@retval None
  */
void HmiCmd001Fill(uint8_t type, uint8_t num,uint8_t *pBuff)
{
	Cmd101DownControl(4);
	cmd101.packBuff[cmd101.pIn + CMD001_CMD] = 1;
	cmd101.packBuff[cmd101.pIn + CMD001_NUM] = num;
	if(type == C001TYPE_DISCRETE){
		cmd101.packBuff[cmd101.pIn + CMD001_TYPE] = C001TYPE_DISCRETE;
		memcpy((char *)&cmd101.packBuff[cmd101.pIn + CMD001_NUMBER],(char *)pBuff,num*2);
		cmd101.packBuff[cmd101.pIn + CMD001_LEN] = 4 + num*2;
	}
	else if(type == C001TYPE_CONTINUOUS){
		uint8_t tNum = num / 8 + (num % 8 ? 1:0);
		cmd101.packBuff[cmd101.pIn + CMD001_TYPE] = C001TYPE_CONTINUOUS;
		cmd101.packBuff[cmd101.pIn + CMD001_NUMBER] = pBuff[0];
		memcpy((char *)&cmd101.packBuff[cmd101.pIn + CMD001_VALUE],(char *)&pBuff[1],tNum);
		cmd101.packBuff[cmd101.pIn + CMD001_LEN] = 5 + tNum;
	}
	else{
		return;
	}
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD001_LEN];
	cmd101.cmdNum += 1;
	EndCmd101Down();
}
/**
  *@brief 102���������
  *@param  pbuff ��������
  *@retval ���ݴ�С
  */
uint16_t HmiCmd002Fun(uint8_t *pbuff)
{
	uint8_t i;
	switch(pbuff[CMD002_TYPE])
	{
	case C002TYPE_DISCRETE://��������
		for(i = 0; i < pbuff[CMD002_NUM]; i++){
			KeyCmdResult(pbuff[CMD002_NUMBER + i*2], pbuff[CMD002_VALUE + i*2]);
		}break;
	case C002TYPE_CONTINUOUS://������
		for(i = 0; i < pbuff[CMD002_NUM]; i++){
			KeyCmdResult(pbuff[CMD002_NUMBER] + i, pbuff[CMD002_VALUE + i]);
		}break;
	default:break;
	}
	return pbuff[CMD002_LEN];
}

/**
  *@brief ģ���������
  *@param  pbuff ��������
  *@retval ���ݴ�С
  */
uint16_t HmiCmd003Fun(uint8_t *pbuff)
{
	uint8_t i;
	union{
		float tf;
		uint8_t t8[4];
	}analog;
	switch(pbuff[CMD003_TYPE])
	{
	case C003TYPE_DISCRETE://��������
		for(i = 0; i < pbuff[CMD003_NUM]; i++){
			analog.t8[0] = pbuff[CMD003_VALUE_LL + i*5];
			analog.t8[1] = pbuff[CMD003_VALUE_LH + i*5];
			analog.t8[2] = pbuff[CMD003_VALUE_HL + i*5];
			analog.t8[3] = pbuff[CMD003_VALUE_HH + i*5];
			AnalogCmdResult(pbuff[CMD003_NUMBER + i*5], analog.tf);
		}break;
	case C003TYPE_CONTINUOUS://������
		for(i = 0; i < pbuff[CMD003_NUM]; i++){
			analog.t8[0] = pbuff[CMD003_VALUE_LL + i*4];
			analog.t8[1] = pbuff[CMD003_VALUE_LH + i*4];
			analog.t8[2] = pbuff[CMD003_VALUE_HL + i*4];
			analog.t8[3] = pbuff[CMD003_VALUE_HH + i*4];
			AnalogCmdResult(pbuff[CMD003_NUMBER] + i, analog.tf);
		}break;
	default:break;
	}
	return pbuff[CMD003_LEN];
}
/**
  *@brief Hmi10Cmd���������
  *@param  pbuff ��������
  *@retval None
  */
void Hmi101CmdResult(Hmi101FrameResult *pFrame)
{
	/* ƫ��0Ϊ���� 1 Ϊ���� */
	switch(pFrame->cmdBegin[1])
	{
	case HmiCmd002:pFrame->pContent = HmiCmd002Fun(&pFrame->cmdBegin[0]);break;
	case HmiCmd003:pFrame->pContent = HmiCmd003Fun(&pFrame->cmdBegin[0]);break;
	default:pFrame->pContent = 0;break;
	}
	//GuiUpdateDisplayAll();
}
/**
  *@brief hmi101��ѯ����������
  *@param  pbuff ��������
  *@retval 0 �ɹ� 1 ʧ��
  */
uint8_t hmi101Scan(uint8_t *pBuff)
{
	Hmi101FrameResult frameIs;
	frameIs.frameLen.len8[0] = pBuff[H101CMD_LEN_L];
	frameIs.frameLen.len8[1] = pBuff[H101CMD_LEN_H];
	frameIs.cmdNum = pBuff[H101CMD_CMDNUM];
	if(frameIs.frameLen.len16 < 2 || frameIs.cmdNum < 1){
		return 1;
	}
	frameIs.pContent = 0;
	frameIs.cmdOffset = 0;
	while(frameIs.cmdNum){
		frameIs.cmdOffset += frameIs.pContent;
		if(frameIs.cmdOffset + H101CMD_CMD >= frameIs.frameLen.len16){
			return 1;
		}
		frameIs.cmdBegin = &pBuff[frameIs.cmdOffset + H101CMD_CMD];
		Hmi101CmdResult(&frameIs);
		frameIs.cmdNum --;
	}
	return 0;
}

static void Hmi101ThreadEntity(void *param)
{
	rt_err_t result;
	time_static_init();
	result = rt_event_init(&Cmd101SendEvent, "hmi101", RT_IPC_FLAG_PRIO);
	if (result != RT_EOK){  
	}
	cmd101.state = 0;
	userVariableDisplayInit();
	GUIDisplayInit();
	HmiInOutInit();
	for (;;){ 				
		GUIDisplayMian();
		LedChangeCheck();
		Hmi101ThreadRunMontor();
		rt_thread_delay(20);		
	}
}

/**
  *@brief hmi101�̵߳����ɱ��
  *@param  None
  *@retval None
  */
void HmiThreadDelete(void)
{
	rt_err_t result;
	result = rt_event_detach(&Cmd101SendEvent);
	if (result != RT_EOK){  
	}
	time_static_detach();
	result = rt_thread_detach(Hmi101Thread);
	if (result != RT_EOK){  
	}
}

/**
  *@brief hmi101��ʼ��
  *@param  None
  *@retval None
  */
void Hmi101Init(void)
{
	rt_err_t result;
	static uint8_t flag,mallocFlag;
	if(flag == 1){
		HmiThreadDelete();
		flag = 0;
	}
	if(flag == 0){
		if(mallocFlag == 0){//ֻ���ϵ�ʱ����һ��
			Hmi101Thread = (rt_thread_t)rt_malloc(sizeof(struct rt_thread));
			Hmi101Threadstack = (rt_uint8_t *)rt_malloc(HMICOM_THREAD_STACK_SIZE);
			userGUIBuff = (uint8_t *)rt_malloc(1024*4);
			cmd101.packBuff = (uint8_t *)rt_malloc(PACKBUFFMAX);
			mallocFlag = 1;
		}
		result = rt_thread_init(Hmi101Thread,HMICOM_THREAD_NAME,Hmi101ThreadEntity,
			RT_NULL,Hmi101Threadstack,HMICOM_THREAD_STACK_SIZE,HMICOM_THREAD_PRIORITY,HMICOM_THREAD_TIMESLICE);
		if(result == RT_EOK){
			rt_thread_startup(Hmi101Thread);
			flag = 1;
		}
	}
	Hmi101ThreadFlag = 0;//��ʼ��ʱ�����߳����б�־
}

/**
  *@brief Hmi�̵߳�ȫ��buff����
  *@param  None
  *@retval None
  */
void HmiStaticMemoryApply(void)
{
	
}

/**
  *@brief Hmi�̼߳���
  *@param  None
  *@retval None
  */
void Hmi101ThreadRunMontor(void)
{
	Hmi101ThreadFlag = 0;
}

/**
  *@brief Hmi�̳߳�ʱ����
  *@param  None
  *@retval 0 �߳����� 1 ��ʱ
  */
uint8_t Hmi101RunTimerOut(void)
{
	static uint32_t TimeOutTick;
	uint8_t errType;
	if(Hmi101ThreadFlag == 0){
		Hmi101ThreadFlag = 1;
		TimeOutTick = GetTimer1Tick();
		errType = 0;
	}
	else if(GetTimer1IntervalTick(TimeOutTick) > 10000){//10s
		errType = 1;
	}
	else{
		errType = 0;
	}
	
	return errType;
}

/* END */
