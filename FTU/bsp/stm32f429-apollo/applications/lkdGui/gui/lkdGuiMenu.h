#ifndef __LKDGUIMENU_H
#define	__LKDGUIMENU_H

#include "gui_101_cmd.h"

#undef  NULL
#define NULL 0

/* lkd�˵��ڵ㶨�� */
typedef struct lkdMenuNode{
	uint16_t nodeId;/* �ڵ�� */
	uint8_t	*name;/* �ڵ����� */
	struct lkdMenuNode *pBro;/* ��һ���ֵܽڵ�� */
	struct lkdMenuNode *pSon;/* �ӽڵ�� */
	void *user;
}lkdMenuNode;

/* lkd�˵��ڵ㶨�� */
typedef struct {
	uint8_t	index;
	lkdCoord cy;
	lkdMenuNode *pNode;
}MenuStack;

typedef struct{
	lkdCoord x;/* �˵���ʼ���� */
	lkdCoord y;
	uint16_t wide;
	uint16_t hight;
	uint8_t ItemsWide;/* �˵���� */
	uint8_t Itemshigh;/* �˵���� */
	uint8_t index;		/* �˵���ǰitem */
	uint8_t count;		/* �˵��������� */
	uint8_t stackNum;	/* �˵�ջ��С ��MenuStackΪ��λ */
	MenuStack *stack;	/* �˵�ջ */
	lkdMenuNode *Root;/* �˵����ڵ� */
	
	/* ��ǰ�ڵ�滭��ʱ���� */
	lkdCoord cx;/* ��ʼ���� */
	lkdCoord cy;
	lkdMenuNode *pDraw;/* �滭�ڵ� */
	uint8_t nodeNum;/* ��ǰ���ڵ��µĽڵ��� */
	uint8_t cNum;
	
	void (* MenuDealWithFun)(void);/* �˵������� */
}lkdMenu;


void MenuTreeSNodeInsert(lkdMenuNode *fNode, lkdMenuNode *pNode);

/* �˵���ʼ�� */
void GuiMenuInit(lkdMenu *pM);
/* �˵�ѡ�нڵ������ƶ� */
void GuiMenuItemDownMove(lkdMenu *pM);
/* �˵�ѡ�нڵ������ƶ� */
void GuiMenuItemUpMove(lkdMenu *pM);
/* �˵���ǰ�ڵ�չ�� */
uint8_t GuiMenuCurrentNodeSonUnfold(lkdMenu *pM);
/* �˵���ǰ�ڵ����� */
uint8_t GuiMenuCurrentNodeHide(lkdMenu *pM);
/* �ػ�˵� */
void GuiMenuRedrawMenu(lkdMenu *pM);
/* ��ȡ��ǰ�ڵ�ID */
uint16_t GuiMenuGetCurrentNodeID(lkdMenu *pM);
/* ��ȡ��ǰ�ڵ�ָ�� */
lkdMenuNode *GuiMenuGetCurrentpNode(lkdMenu *pM);

#endif /* __LKDGUIMENU_H */

/* END */
