#ifndef __LKDGUIMENU_H
#define	__LKDGUIMENU_H

#include "gui_101_cmd.h"

#undef  NULL
#define NULL 0

/* lkd菜单节点定义 */
typedef struct lkdMenuNode{
	uint16_t nodeId;/* 节点号 */
	uint8_t	*name;/* 节点名称 */
	struct lkdMenuNode *pBro;/* 下一个兄弟节点号 */
	struct lkdMenuNode *pSon;/* 子节点号 */
	void *user;
}lkdMenuNode;

/* lkd菜单节点定义 */
typedef struct {
	uint8_t	index;
	lkdCoord cy;
	lkdMenuNode *pNode;
}MenuStack;

typedef struct{
	lkdCoord x;/* 菜单起始坐标 */
	lkdCoord y;
	uint16_t wide;
	uint16_t hight;
	uint8_t ItemsWide;/* 菜单项宽 */
	uint8_t Itemshigh;/* 菜单项高 */
	uint8_t index;		/* 菜单当前item */
	uint8_t count;		/* 菜单级数计数 */
	uint8_t stackNum;	/* 菜单栈大小 以MenuStack为单位 */
	MenuStack *stack;	/* 菜单栈 */
	lkdMenuNode *Root;/* 菜单根节点 */
	
	/* 当前节点绘画临时保存 */
	lkdCoord cx;/* 起始坐标 */
	lkdCoord cy;
	lkdMenuNode *pDraw;/* 绘画节点 */
	uint8_t nodeNum;/* 当前父节点下的节点数 */
	uint8_t cNum;
	
	void (* MenuDealWithFun)(void);/* 菜单处理函数 */
}lkdMenu;


void MenuTreeSNodeInsert(lkdMenuNode *fNode, lkdMenuNode *pNode);

/* 菜单初始化 */
void GuiMenuInit(lkdMenu *pM);
/* 菜单选中节点向下移动 */
void GuiMenuItemDownMove(lkdMenu *pM);
/* 菜单选中节点向上移动 */
void GuiMenuItemUpMove(lkdMenu *pM);
/* 菜单当前节点展开 */
uint8_t GuiMenuCurrentNodeSonUnfold(lkdMenu *pM);
/* 菜单当前节点隐藏 */
uint8_t GuiMenuCurrentNodeHide(lkdMenu *pM);
/* 重绘菜单 */
void GuiMenuRedrawMenu(lkdMenu *pM);
/* 获取当前节点ID */
uint16_t GuiMenuGetCurrentNodeID(lkdMenu *pM);
/* 获取当前节点指针 */
lkdMenuNode *GuiMenuGetCurrentpNode(lkdMenu *pM);

#endif /* __LKDGUIMENU_H */

/* END */
