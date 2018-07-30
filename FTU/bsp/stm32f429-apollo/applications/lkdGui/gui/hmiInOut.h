/**
  * @file   hmiInout.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/18
  * @brief  hmi�������صȴ���
  */

#ifndef __HMIINOUT_H
#define	__HMIINOUT_H

#include "stdint.h"

/* ����״̬ */
#define USERKEY_DOWN	0
#define USERKEY_UP 		1

/* �˵�����ӳ�� */
#define	MENUKEY_UP      1
#define	MENUKEY_LEFT    3
#define	MENUKEY_DOWN    4
#define	MENUKEY_RIGHT   5
#define	MENUKEY_ESC     2
#define	MENUKEY_OK      9
/* ң�ذ���ӳ�� */
#define	YK_RESET        8
#define	YK_SHFITSWITCH  11
#define	YK_OPENSWITCH   12
#define	YK_CLOSESWITCH  13

/* ����ö�� */
enum KeyStatus{
	NoKey,
	UpKey,
	DownKey,
	LeftKey,
	RightKey,
	OkKey,
	CancelKey
};
/* �˵�����״̬ */
struct MenuKeyValue{
	uint8_t up:1;
	uint8_t down:1;
	uint8_t left:1;
	uint8_t right:1;
	uint8_t ok:1;
	uint8_t esc:1;
	enum KeyStatus keyIs;
};
/* ң�ذ���״̬ */
struct YKKeyValue{
	uint8_t open:1;
	uint8_t close:1;
	uint8_t shift:1;
	uint8_t reset:1;
	enum KeyStatus keyIs;
};

/* ����ӳ�� */
enum UserOutputNoMap{
	/* LED ӳ�� */
	LED_BEGIN_NO = 2,
	LED_END_NO = 50,
};

/* Ledʹ�ö��� */
enum UserLedDefine{
	ULED_NOENERGY,		/* δ���� */
	ULED_SWITCHOPEN,	/* ���ط�λ */
	ULED_REMOTE,		/* Զ�� */
	ULED_SWITCHCLOSE,	/* ���غ�λ */
	ULED_CLOSELOCK,		/* ��բ���� */
	ULED_OVERALARM,		/* �����澯 */
	ULED_OPENLOCK,		/* ��բ���� */
	ULED_ZEROALARM,		/* ����澯 */
	ULED_DEVICEFAULT,	/* װ�ù��� */
	ULED_SELFCHECK,		/* �Լ��쳣 */
	ULED_COMMUN,		/* ͨ�� */
	ULED_RECLOSELOCK,	/* �غϱ��� */
	ULED_BATTERYALARM,/* ��ظ澯 */
	ULED_BATTERYRELEASE,/* ��ػ */
	ULED_PROTECTOFF,	/* �������� */
	ULED_BATTERYLOWV,	/* ���Ƿѹ */
	ULED_LS,			/* ���ɿ��� */
	ULED_CB,			/* ��·�� */
	ULED_ALLNUM
};
enum UserLedState{
	ULED_OFF,
	ULED_ON,
};

/* ģ����ʹ�ö��� */
enum UserAnalogDefine{
	AI_TEMPERATURE = 1,
};

enum KeyStatus GetKeyStatus(void);
void SetKeyIsNoKey(void);
void KeyCmdResult(uint8_t keyNo, uint8_t state);
void AnalogCmdResult(uint8_t analogNo, float analogVal);
void LedChangeCheck(void);
void HmiInOutInit(void);

extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);
#endif /* __HMIINOUT_H */

/* END */
