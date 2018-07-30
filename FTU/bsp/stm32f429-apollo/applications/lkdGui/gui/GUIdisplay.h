/**
  * @file   GUIdisplay.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    03-06-2018
  * @brief   GUI��ʾ
  */

#ifndef __GUIDISPLAY_H
#define __GUIDISPLAY_H

#include "lkdGuiMenu.h"
#include "gui_101_cmd.h"
#include "lkdGuiWindow.h"
#include "userVariable.h"

#define DZDISPLAYCOL 3		/* ��ֵ��ʾ���� */
#define YaoCeDISPLAYCOL 3	/* ң����ʾ���� */
#define DISPLAYLISTROW 10	/* ����ʾ���� */
#define DZDISPLAYROW   5    /* ��ֵ��ʾ���� */
#define SOEDISPLAYROW 5		/* SOE��ʾ���� */
#define EVENTDISPLAYROW 1	/* �����¼���ʾ���� */
#define CONSTPARDISPLAYROW 5/* ���в�����ʾ���� */

#define YAOCE_UPDATATIME 1000 /* ң�������ʾʱ�� 1s */

#define MAX_PASSOWRD_LEVEL PassWord_NUM
#define MANUFACTURER_PASSWORD	"300444"

/* Lcd������ʾ���� */
#define LCDDISPLAYON	1
#define LCDDISPLAYOFF	0
#define LCDDISPLAYOFFTIME	(1000*60)/* ms */
/* �������� */
struct PasswordSave{
	uint8_t passwordLevle;	/* ����ȼ� */
	uint8_t password[MAX_PASSOWRD_LEVEL][6];	/* ������������ */
};


/* ����״̬ */
enum ReturnState{
	ISFAILTURE,	/* ʧ�� */
	ISTURE,		/* �ɹ� */
	ISWAITING,	/* �ȴ� */
	ISOVERTIME	/* ��ʱ */
};

/* ���ö�ֵ���� */
enum DingZhiSetOrSee{
	NULL_DINGZHI,	/* NULL */
	SET_DINGZHI,	/* �޸Ķ�ֵ */
	SEE_DINGZHI,	/* �����ֵ */
};

/* ��Ϣ������Ϣ */
struct Message{
	uint8_t *str;	/* ��ʾ�� */
	uint8_t *flag;	/* ���ر�־ */
};

/* ���봰����Ϣ */
struct PassWordPipe{
	uint8_t passWordType; 	/* �������� */
	uint8_t *returnFlag;	/* ���ر�־ */
};

/* SOE��ʾ */
struct SOEDisplay{
	uint16_t allNum;
	int16_t pRead;
	uint8_t itemNum[4];	/* ��ǰ���� */
	uint8_t time[24];	/* ʱ�� */
	SoeContent pSoe;
};

/* �����¼���ʾ */
struct EventDisplay{
	uint8_t pRead;
	uint8_t allNum;
	uint8_t itemNum[4];	/* ��ǰ���� */
	uint8_t time[24];		/* ʱ�� */
	uint8_t yaoceValue[16];/* ң��ֵת��Ϊ�ַ��� */
	FeventContent pFevent;
};

/* ��ֵ�޸� */
struct DZModfiy{
	const DzhiDisplayInfo *info;	/* ��ֵ�ṹ�� */
	uint8_t flag;	/* ��־ */
	uint8_t itemIs;	/* ��ǰ�� */
	uint8_t *str;	/* Ҫ�޸ĵ��ַ��� */
};

enum RunStepType{
	STEP_NORMAL,
	STEP_UPDATAPRO,
	STEP_PASSWORD,
	STEP_MESSAGE,
	STEP_DZHIMODFIY,
	STEP_HOMEWIN,
	STEP_MAINMENU,
	STEP_DZHICHANGE,//��ֵ�л�
	STEP_ALLNUM,
};

/* �˵��ڵ�ID */
enum userMenuId {
	MID_ROOT,
	MID_REALTIMEDATA,			/* ʵʱ���� */
	MID_DZHISCAN,				/* ��ֵ��ѯ */
	MID_DEVICEEVENTRECORD,		/* װ���¼���¼ */
	MID_CMDSEND,				/* �����·� */
	MID_DZHISET,				/* ��ֵ���� */
	MID_DEVICE_PARAMTER_SETTING,/* װ�ò������� */
	MID_SYSTEM_PARAMTER_SETTING,/* ϵͳ�������� */
	MID_VERSIONINFO,			/* �汾��Ϣ */
	MID_ONE_TELEMETRY,			/* һ��ң�� */
	MID_SECOND_TELEMETRY,		/* ����ң�� */
	MID_HARMONIC_COMPONENT,		/* г������ */
	MID_YAOXIN,					/* ң�Ų�ѯ */
	MID_SOE,					/* SOE��ѯ */
	MID_OPERATERECORD,			/* ������¼ */
	MID_DEFAULTEVENT,			/* �����¼� */
	MID_CHECKRECORD,			/* �Լ��¼ */
	MID_SERIAL,					/* ���� */
	MID_INTERNET,				/* ���� */
	MID_DEADEZONE,				/* �������� */
	MID_ZERODRIFT,				/* ��Ư���� */
	MID_TIMEMODFIY,				/* ʱ���޸� */
	MID_BASICSET,				/* �������� */
	MID_PASSWORDMODFIY			/* �޸����� */
};

/* ��ֵ�����ƹ��� */
struct DZItemsManage{
	uint8_t start_itemes;
	uint8_t current_items;
	uint8_t items_num;
	uint8_t **pText;
};

extern uint8_t *userGUIBuff;


/* ��ҳ���洰�� */
void HomeWindowFun(void *param);
/* �˵����� */
void MenuWindowFun(void *param);
void YaoxinFun(void *param);//ң��
void YaoceWinFun(void *param);//ң��
void SoeCoWinFun(void *param);//soe/co����
void FaultEventFun(void *param);//�����¼�
void DingzhiWinFun(void *param);//��ֵͨ����ʾ
void CmdSendWinFun(void *param);// �����·�
void VersionWinFun(void *param);//�汾��Ϣ
void PasswordModfiyWinFun(void *param);//�����޸Ĵ���
void TimeModfiyFun(void *param);//ʱ���޸�
void DZModfiyFun(void *param);//��ֵ�޸Ĵ��ں��� 
void MessageFun(void *param);//��Ϣ���� 
void PassWordFun(void *param);//���봰��
void DzhiChangeFun(void *param);//��ֵ�л�����
/* �������滭ͼ */
void GUIStartInterface(void);
/* GUI������ */
void GUIDisplayMian(void);
/* GUI��ʼ�� */
void GUIDisplayInit(void);

void MenuControlFun(void);
/* ���ýڵ�ģ�� */
void SetDzhiSonNode(lkdMenuNode *pSon);
/* ��ֵ�仯�ص� */
void DzhiChangeCallBack(void);

#endif /* __GUIDISPLAY_H */
