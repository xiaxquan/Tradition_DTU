/**
  * @file   GUIdisplay.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    03-06-2018
  * @brief   GUI显示
  */

#ifndef __GUIDISPLAY_H
#define __GUIDISPLAY_H

#include "lkdGuiMenu.h"
#include "gui_101_cmd.h"
#include "lkdGuiWindow.h"
#include "userVariable.h"

#define DZDISPLAYCOL 3		/* 定值显示列数 */
#define YaoCeDISPLAYCOL 3	/* 遥测显示列数 */
#define DISPLAYLISTROW 10	/* 可显示行数 */
#define DZDISPLAYROW   5    /* 定值显示行数 */
#define SOEDISPLAYROW 5		/* SOE显示行数 */
#define EVENTDISPLAYROW 1	/* 操作事件显示行数 */
#define CONSTPARDISPLAYROW 5/* 固有参数显示行数 */

#define YAOCE_UPDATATIME 1000 /* 遥测更新显示时间 1s */

#define MAX_PASSOWRD_LEVEL PassWord_NUM
#define MANUFACTURER_PASSWORD	"300444"

/* Lcd开关显示控制 */
#define LCDDISPLAYON	1
#define LCDDISPLAYOFF	0
#define LCDDISPLAYOFFTIME	(1000*60)/* ms */
/* 保存密码 */
struct PasswordSave{
	uint8_t passwordLevle;	/* 密码等级 */
	uint8_t password[MAX_PASSOWRD_LEVEL][6];	/* 保存所有密码 */
};


/* 返回状态 */
enum ReturnState{
	ISFAILTURE,	/* 失败 */
	ISTURE,		/* 成功 */
	ISWAITING,	/* 等待 */
	ISOVERTIME	/* 超时 */
};

/* 设置定值方法 */
enum DingZhiSetOrSee{
	NULL_DINGZHI,	/* NULL */
	SET_DINGZHI,	/* 修改定值 */
	SEE_DINGZHI,	/* 浏览定值 */
};

/* 消息窗口消息 */
struct Message{
	uint8_t *str;	/* 提示语 */
	uint8_t *flag;	/* 返回标志 */
};

/* 密码窗口消息 */
struct PassWordPipe{
	uint8_t passWordType; 	/* 密码类型 */
	uint8_t *returnFlag;	/* 返回标志 */
};

/* SOE显示 */
struct SOEDisplay{
	uint16_t allNum;
	int16_t pRead;
	uint8_t itemNum[4];	/* 当前条数 */
	uint8_t time[24];	/* 时间 */
	SoeContent pSoe;
};

/* 故障事件显示 */
struct EventDisplay{
	uint8_t pRead;
	uint8_t allNum;
	uint8_t itemNum[4];	/* 当前条数 */
	uint8_t time[24];		/* 时间 */
	uint8_t yaoceValue[16];/* 遥测值转化为字符串 */
	FeventContent pFevent;
};

/* 定值修改 */
struct DZModfiy{
	const DzhiDisplayInfo *info;	/* 定值结构体 */
	uint8_t flag;	/* 标志 */
	uint8_t itemIs;	/* 当前行 */
	uint8_t *str;	/* 要修改的字符串 */
};

enum RunStepType{
	STEP_NORMAL,
	STEP_UPDATAPRO,
	STEP_PASSWORD,
	STEP_MESSAGE,
	STEP_DZHIMODFIY,
	STEP_HOMEWIN,
	STEP_MAINMENU,
	STEP_DZHICHANGE,//定值切换
	STEP_ALLNUM,
};

/* 菜单节点ID */
enum userMenuId {
	MID_ROOT,
	MID_REALTIMEDATA,			/* 实时数据 */
	MID_DZHISCAN,				/* 定值查询 */
	MID_DEVICEEVENTRECORD,		/* 装置事件记录 */
	MID_CMDSEND,				/* 命令下发 */
	MID_DZHISET,				/* 定值设置 */
	MID_DEVICE_PARAMTER_SETTING,/* 装置参数设置 */
	MID_SYSTEM_PARAMTER_SETTING,/* 系统参数设置 */
	MID_VERSIONINFO,			/* 版本信息 */
	MID_ONE_TELEMETRY,			/* 一次遥测 */
	MID_SECOND_TELEMETRY,		/* 二次遥测 */
	MID_HARMONIC_COMPONENT,		/* 谐波分量 */
	MID_YAOXIN,					/* 遥信查询 */
	MID_SOE,					/* SOE查询 */
	MID_OPERATERECORD,			/* 操作记录 */
	MID_DEFAULTEVENT,			/* 故障事件 */
	MID_CHECKRECORD,			/* 自检记录 */
	MID_SERIAL,					/* 串口 */
	MID_INTERNET,				/* 网口 */
	MID_DEADEZONE,				/* 死区设置 */
	MID_ZERODRIFT,				/* 零漂设置 */
	MID_TIMEMODFIY,				/* 时间修改 */
	MID_BASICSET,				/* 基本设置 */
	MID_PASSWORDMODFIY			/* 修改密码 */
};

/* 定值表格绘制管理 */
struct DZItemsManage{
	uint8_t start_itemes;
	uint8_t current_items;
	uint8_t items_num;
	uint8_t **pText;
};

extern uint8_t *userGUIBuff;


/* 首页界面窗口 */
void HomeWindowFun(void *param);
/* 菜单窗口 */
void MenuWindowFun(void *param);
void YaoxinFun(void *param);//遥信
void YaoceWinFun(void *param);//遥测
void SoeCoWinFun(void *param);//soe/co处理
void FaultEventFun(void *param);//故障事件
void DingzhiWinFun(void *param);//定值通用显示
void CmdSendWinFun(void *param);// 命令下发
void VersionWinFun(void *param);//版本信息
void PasswordModfiyWinFun(void *param);//密码修改窗口
void TimeModfiyFun(void *param);//时间修改
void DZModfiyFun(void *param);//定值修改窗口函数 
void MessageFun(void *param);//消息窗口 
void PassWordFun(void *param);//密码窗口
void DzhiChangeFun(void *param);//定值切换窗口
/* 启动界面画图 */
void GUIStartInterface(void);
/* GUI主函数 */
void GUIDisplayMian(void);
/* GUI初始化 */
void GUIDisplayInit(void);

void MenuControlFun(void);
/* 设置节点模块 */
void SetDzhiSonNode(lkdMenuNode *pSon);
/* 定值变化回掉 */
void DzhiChangeCallBack(void);

#endif /* __GUIDISPLAY_H */
