/**
  * @file    hmi101.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/10
  * @brief   
  */

#ifndef __HMI101_H
#define	__HMI101_H

#include "stdint.h"

#define HMIREVBUFF_MAX 1024
struct Hmi101Rev{
	uint8_t revBuff[HMIREVBUFF_MAX];
	uint16_t pIn;
	uint16_t pout;
};

/* Hmi101֡���� */
enum Hmi101Frame{
	H101CMD_LEN_L,
	H101CMD_LEN_H,
	H101CMD_CMDNUM,
	H101CMD_CMD,
	/* ��������о������� */
};

/* �������� */
enum HmiInputCmd_001{
	CMD001_LEN,		/* ����� */
	CMD001_CMD,		/* �����ʶ */
	CMD001_TYPE,	/* 0 ������ 1 ���� */
	CMD001_NUM,		/* �������� */
	CMD001_NUMBER,/* ����� */
	CMD001_VALUE,	/* ��Ӧֵ */
	/* ��������� ȫ��Ϊֵ */
	/* ���������  CMD100_NUMBER0 CMD100_VALUE0 CMD100_NUMBER1 CMD100_VALUE1 ... */
};
enum Cmd001Type{
	C001TYPE_DISCRETE,	/* ������ */
	C001TYPE_CONTINUOUS,/* ���� */
};
/* �������� */
enum HmiInputCmd_002{
	CMD002_LEN,		/* ����� */
	CMD002_CMD,		/* �����ʶ */
	CMD002_TYPE,	/* 0 ������ 1 ���� */
	CMD002_NUM,		/* �������� */
	CMD002_NUMBER,/* ������ */
	CMD002_VALUE,	/* ��Ӧֵ */
	/* ��������� ȫ��Ϊֵ */
	/* ���������  CMD002_NUMBER0 CMD002_VALUE0 CMD002_NUMBER1 CMD002_VALUE1 ... */
};
enum Cmd002Type{
	C002TYPE_DISCRETE,	/* ������ */
	C002TYPE_CONTINUOUS,/* ���� */
};

/* ģ�������� */
enum HmiAnalogCmd_003{
	CMD003_LEN,		/* ����� */
	CMD003_CMD,		/* �����ʶ */
	CMD003_TYPE,	/* 0 ������ 1 ���� */
	CMD003_NUM,		/* ģ�������� */
	CMD003_NUMBER,/* ģ������ */
	CMD003_VALUE_LL,	/* ��Ӧֵ */
	CMD003_VALUE_LH,
	CMD003_VALUE_HL,
	CMD003_VALUE_HH,
	/* ��������� ȫ��Ϊֵ */
	/* ���������  CMD002_NUMBER0 CMD002_VALUE0 CMD002_NUMBER1 CMD002_VALUE1 ... */
};
enum Cmd003Type{
	C003TYPE_DISCRETE,	/* ������ */
	C003TYPE_CONTINUOUS,/* ���� */
};

/* �������� */
enum HmiGuiCmd_100{
	CMD100_LEN,		/* ����� */
	CMD100_CMD,		/* �����ʶ */
	CMD100_TYPE,	/* �������� 0ˮƽ�� 1��ֱ�� 2 б�� */
	CMD100_BX,		/* ��ʼ����x */
	CMD100_BY,		/* ��ʼ����y */
	CMD100_EX,		/* ��������x */
	CMD100_EY,		/* ��������y */
	CMD100_POINT,	/* ���߼�� */
	CMD100_COLOUR,/* ��ɫ */
};
enum Cmd100Type{
	C100TYPE_HLINE,	/* ˮƽ�� */
	C100TYPE_RLINE,	/* ��ֱ�� */
	C100TYPE_BLINE,	/* б�� */
};
/* ���������� */
enum HmiGuiCmd_101{
	CMD101_LEN,		/* ����� */
	CMD101_CMD,		/* �����ʶ */
	CMD101_TYPE,	/* �������� 0 ���� 1 ������ */
	CMD101_BX,		/* ��ʼ����x */
	CMD101_BY,		/* ��ʼ����y */
	CMD101_EX,		/* ��������x */
	CMD101_EY,		/* ��������y */
	CMD101_COLOUR,/* ��ɫ */
};
enum Cmd101Type{
	C101TYPE_RECT,			/* ���� */
	C101TYPE_FILLRECT,	/* ������ */
};
/* ���������� */
enum HmiGuiCmd_102{
	CMD102_LEN,		/* ����� */
	CMD102_CMD,		/* �����ʶ */
	CMD102_TYPE,	/* �������� 0 �Զ��崰�� 1 ���봰�� 2 ��Ϣ����  */
	CMD102_BX,		/* ��ʼ����x */
	CMD102_BY,		/* ��ʼ����y */
	CMD102_EX,		/* ��������x */
	CMD102_EY,		/* ��������y */
	CMD102_CONTENT,/* ��������->�ַ���,\0���� */
};
enum Cmd102Type{
	C102TYPE_DEFAULT,		/* �Զ��崰�� */
	C102TYPE_PASSWORD,	/* ���봰�� */
	C102TYPE_MESSAGE,		/* ��Ϣ���� */
};
/* ˢ��LCD���� */
enum HmiGuiCmd_103{
	CMD103_LEN,		/* ����� */
	CMD103_CMD,		/* �����ʶ */
};

/* ����LCD���� */
enum HmiGuiCmd_104{
	CMD104_LEN,		/* ����� */
	CMD104_CMD,		/* �����ʶ */
	CMD104_TYPE,	/* �������� 0 �ر��� 1 ������ 2 ����ʾ 3 ����ʾ */
};
enum Cmd104Type{
	C104TYPE_OFFBLA,		/* �ر��� */
	C104TYPE_ONBLA,			/* ������ */
	C104TYPE_OFFSCREEN,	/* ����ʾ */
	C104TYPE_ONSCREEN,	/* ����ʾ */
};
/* ������������ */
enum HmiGuiCmd_105{
	CMD105_LEN,		/* ����� */
	CMD105_CMD,		/* �����ʶ */
	CMD105_TYPE,	/* ���������� 0ˮƽ������ 1��ֱ������ */
	CMD105_BX,		/* ��ʼ����x */
	CMD105_BY,		/* ��ʼ����y */
	CMD105_SLEN,	/* ���������� */
	CMD105_SMAX,	/* ���������ֵ */
	CMD105_SLUMP,	/* ��ǰֵ */
};
enum Cmd105Type{
	C105TYPE_HSCROLL,	/* ˮƽ������ */
	C105TYPE_RSCROLL,	/* ��ֱ������ */
};
/* ����ť���� */
enum HmiGuiCmd_106{
	CMD106_LEN,		/* ����� */
	CMD106_CMD,		/* �����ʶ */
	CMD106_TYPE,	/* ��ť����  0 �Զ��� 1 ȷ�� 2 ȡ�� */
	CMD106_BX,		/* ��ʼ����x */
	CMD106_BY,		/* ��ʼ����y */
	CMD106_FlAG,	/* ��ť״̬  0 ��ѡ�� 1 ѡ�� */
	CMD106_CONTENT,/* ��ť����->�ַ���,\0���� */
};
enum Cmd106Type{
	C106TYPE_DEFAULT,	/* �Զ��� */
	C106TYPE_OK,			/* ȷ�� */
	C106TYPE_ESC,			/* ȡ�� */
};
enum Cmd106Flag{
	C106FLAG_NOTHIS,	/* ��ѡ�� */
	C106FLAG_ISTHIS,	/* ѡ�� */
};
/* ����ͼƬ���� */
enum HmiGuiCmd_107{
	CMD107_LEN,		/* ����� */
	CMD107_CMD,		/* �����ʶ */
	CMD107_TYPE,	/* ͼƬ����  0 �Զ��� 1 ˫��ͼ�� 2 ������ͼƬ */
	CMD107_BX,		/* ��ʼ����x */
	CMD107_BY,		/* ��ʼ����y */
	CMD107_EX,		/* ��������x */
	CMD107_EY,		/* ��������y */
	CMD107_MXLEN,	/* �����ش�С */
	CMD107_MYLEN,	/* �����ش�С */
	CMD107_FlAG,	/* ͼƬ��־  0 ������ʾ 1 ���� */
	CMD107_CONTENT,/* ͼƬ���� */
};
enum Cmd107Type{
	C107TYPE_DEFAULT,/* �Զ��� */
	C107TYPE_SOJO,/* SOJO */
	C107TYPE_WAIT,/* ������ */
};
enum Cmd107Flag{
	C107FLAG_NORMAL,	/* ������ʾ */
	C107FLAG_INVERSE,	/* ���� */
};

/* д������ */
enum HmiGuiCmd_108{
	CMD108_LEN,		/* ����� */
	CMD108_CMD,		/* �����ʶ */
	CMD108_TYPE,	/* �������� 0 Ĭ������ */
	CMD108_BX,		/* ��ʼ����x */
	CMD108_BY,		/* ��ʼ����y */
	CMD108_WIDE,	/* ��� */
	CMD108_HIGH,	/* �߶� */
	CMD108_FlAG,	/* �����־  0 ����� 1 ���� 2 �Ҷ��� 3 �ɻ���д */
	CMD108_CONTENT,/* �������� GB2312 \0 end*/
};
enum Cmd108Type{
	C108TYPE_DEFAULT,/* Ĭ������ */
};
enum Cmd108Flag{
	C108FLAG_LEFT,			/* ����� */
	C108FLAG_MID,				/* ���� */
	C108FLAG_RIGHT,			/* �Ҷ��� */
	C108FLAG_FEEDLINE,	/* �ɻ���д */
};

/* ������ɫ���� */
enum HmiGuiCmd_109{
	CMD109_LEN,		/* ����� */
	CMD109_CMD,		/* �����ʶ */
	CMD109_TYPE,	/* �������� 0 ǰ��ɫ 1����ɫ 2 ������ɫ */
	CMD109_COLOUR,/* ��ɫ */
};
enum Cmd109Type{
	C109TYPE_FORE,		/* ǰ��ɫ */
	C109TYPE_BACK,		/* ����ɫ */
	C109TYPE_EXCHANGE,/* ������ɫ */
};

enum HmiGuiCmdTab{
	HmiCmd002 = 2,
	HmiCmd003 = 3,
	GuiCmd100 = 100,
	GuiCmd101,
	GuiCmd102,
	GuiCmd103,
	GuiCmd104,
	GuiCmd105,
	GuiCmd106,
	GuiCmd107,
	GuiCmd108,
	GuiCmd109,
	GuiCmd110,
};


extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);

#endif /* END __HMI101_H */
