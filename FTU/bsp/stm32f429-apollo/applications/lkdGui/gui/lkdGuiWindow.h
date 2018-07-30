
#ifndef __LKDGUIWINDOW_H
#define	__LKDGUIWINDOW_H

#include "stdint.h"

#undef  NULL
#define NULL 0

/* ���� */
struct WINDOW_
{
	int16_t x;// ��ʼ����
	int16_t y;// ��ʼ����
	int16_t wide;// ���
	int16_t hight;// �߶�
	uint8_t *title;// ���������ָ��(NULLΪ����ʾ)
	void(*WindowFunction)(void *param);// ���ڶ�Ӧ�ķ������
	void * param;//����
};
typedef  struct WINDOW_ WINDOW;


/* ���ڹ��� */
struct WindowManage{
	uint8_t pWin;
	WINDOW *userWindow[12];	
};

#define LIST_CONTENT_ROWHIGHT 14//�����и߶�
#define MAXLIST_COL 4
#define LIST_USESCROLL_H 	0x01
#define LIST_USESCROLL_V 	0x02
#define LIST_USEBORDER_H 	0x04
struct ColSet_{
	uint8_t *colWide;
	uint8_t *colFlag;	
};
union DrawFlag_{
	uint8_t all;
	struct{
		uint8_t useHScroll:1;//ˮƽ������
		uint8_t useVScroll:1;//��ֱ������
		uint8_t useBorder:1;//ʹ�ñ߿�
		uint8_t useHighLight:1;//��ǰ�и���
	}bit;
};

struct LIST_{
	int16_t x;
	int16_t y;
	uint16_t wide;
	uint16_t hight;
	uint8_t row;
	uint8_t col;//��
	uint8_t drawRow;
	uint8_t currentRow;
	uint8_t flag;
	uint8_t **content;
	struct ColSet_ *colSet;
};
typedef struct ColSet_ ListColSet;
typedef struct LIST_ LIST;


void userGUITopWindowHide(void);
void userGUIWindowAdd(WINDOW *pWindow);
void userGUITopWindowDisplay(void);
void userGUITopWindowRedraw(void);
void WinManageInit(void);


void DrawList(LIST *plist);

#endif /* __LKDGUIMENU_H */

/* END */
