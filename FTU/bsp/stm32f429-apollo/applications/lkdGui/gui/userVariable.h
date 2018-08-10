#ifndef  __USERVARIABLE_H
#define  __USERVARIABLE_H

#include "stdint.h"
#include "point_table_config.h"

/* ʱ����ʾ */
typedef struct {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t s;
	uint16_t ms;
}SystemTimeDisplay;

/* ң����ʾ�ṹ�� */
typedef struct{
	uint8_t *pBuff;	/* ����ң�������� */
	uint8_t Num;		/* ���������� */
	struct tagTelesignalCfg *pRoot; /* ң��ʵ��ָ�� */
}YaoxinDisplayInfo;

/* ң����ʾ�ṹ�� */
typedef struct{
	uint8_t *pBuff;	/* ����ң�������� */
	uint8_t num;		/* ���������� */
	struct tagTelemetryCfg *pRoot; /* ң��ʵ��ָ�� */
}YaoceDisplayInfo;

/* ��ֵ��ʾ�ṹ�� */
typedef struct{
	uint8_t *pBuff;	/* ���涨ֵ������ */
	uint8_t num;		/* ���������� */
	void(* SaveModify)(uint16_t addr);/* ��ֵ�޸ı���ָ�뺯�� */
	struct tagValueParaCfg *pRoot; /* ��ֵʵ��ָ�� */
}DzhiDisplayInfo;

/* Soe��ʾ�ṹ�� */
typedef struct{
	uint8_t highlight;
	uint8_t *pName;
	uint8_t *pContent;
	SystemTimeDisplay time;
}SoeContent;

/* Soe��ʾ�ṹ�� */
typedef struct{
	uint16_t num;		/* soe���� */
	uint16_t in;
	uint16_t (* GetNum)(void);
	uint8_t (* CheckUpdata)(void);
	uint8_t (* GetNoContent)(uint16_t soeNo,SoeContent *pSoe);
}SoeDisplayInfo;

/* �����¼���ʾ�ṹ�� */
typedef struct{
	uint8_t *pName;
	uint8_t *pContent;
	uint8_t yaoceNum;
	uint8_t *pYaoceName[16];
	uint8_t *pYaoceUnit[16];
	float pYaoceValue[16];
	SystemTimeDisplay time;
}FeventContent;

/* �����¼���ʾ�ṹ�� */
typedef struct{
	uint16_t num;		/* �����¼����� */
	uint16_t in;
}FeventDisplayInfo;

/* �����·����� */
struct HmiCmdSend{
	uint8_t *name;
	uint8_t cmd;
};
/* �����·���Ϣ */
typedef struct{
	uint8_t itemsNum;
	struct HmiCmdSend *pHmiCmd;
	void (* cmdfun)(uint8_t cmdIs);
}HmiCmdSendInfo;

/* �汾��Ϣ */
typedef struct{
	uint8_t num;
	struct tagInherentParaCfg *pItems;
}VersionInfo;

/* ������� */
struct PassWordMessage{
	uint8_t passWord[8];
	uint8_t *name;
};

/* ң����Ϣ�ṹ */
extern YaoxinDisplayInfo yxInfo;
/* ң����Ϣ�ṹ */
extern YaoceDisplayInfo yceInfo[3];
/* ��ֵ��Ϣ�ṹ */
extern DzhiDisplayInfo dzhi0Info[PARAMETER_MENU_ALLNUM];
extern DzhiDisplayInfo dzhi1Info[FIXED_VALUE_MENU_ALLNUM];
/* Soe��ʾ��Ϣ */
extern SoeDisplayInfo soeInfo;
/* Co��ʾ��Ϣ */
extern SoeDisplayInfo coInfo;
/* ulog��ʾ��Ϣ */
extern SoeDisplayInfo ulogInfo;
/* �����·��ṹ�� */
extern HmiCmdSendInfo hcmdInfo;
/* �汾��Ϣ�ṹ�� */
extern VersionInfo versInfo;
/* ������� */
#define PassWord_NUM 2
extern struct PassWordMessage PassWordItems[PassWord_NUM];

/* ��ȡ��ʾʱ�� */
void GetDisplayTime(SystemTimeDisplay *tim);
/* �޸���ʾʱ�� */
void SetDisplayTime(SystemTimeDisplay *tim);
/* ��ʾ��Ϣӳ���ʼ�� */
void userVariableDisplayInit(void);
/* ��ȡSOE���� */
uint8_t GetSoeNoContent(uint16_t soeNo,SoeContent *pSoe);
/* ���SOE���޸��� */
uint8_t CheckSoeUpdata(void);
/* ��ȡSOE���� */
uint16_t GetSoeNum(void);
/* ��ȡCO���� */
uint16_t GetCoNum(void);
/* ��ȡulog���� */
uint16_t GetUlogNum(void);
/* ��ȡ�����¼����� */
uint16_t GetFeventNum(void);
/* ��ȡ�����¼����� */
uint8_t GetFeventNoContent(uint16_t feventNo,FeventContent *pEvent);
/* �������¼����޸��� */
uint8_t CheckFeventUpdata(void);
/* �Ƿ���³����� */
uint8_t CheckUpdataProgram(void);

void UserDefineDzhiInit(void);

#endif

/* END */
