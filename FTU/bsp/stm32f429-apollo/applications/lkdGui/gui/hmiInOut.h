/**
  * @file   hmiInout.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/18
  * @brief  hmi按键开关等处理
  */

#ifndef __HMIINOUT_H
#define	__HMIINOUT_H

#include "stdint.h"

/* 按键状态 */
#define USERKEY_DOWN	0
#define USERKEY_UP 		1

/* 菜单按键映射 */
#define	MENUKEY_UP      1
#define	MENUKEY_LEFT    3
#define	MENUKEY_DOWN    4
#define	MENUKEY_RIGHT   5
#define	MENUKEY_ESC     2
#define	MENUKEY_OK      9
/* 遥控按键映射 */
#define	YK_RESET        8
#define	YK_SHFITSWITCH  11
#define	YK_OPENSWITCH   12
#define	YK_CLOSESWITCH  13

/* 按键枚举 */
enum KeyStatus{
	NoKey,
	UpKey,
	DownKey,
	LeftKey,
	RightKey,
	OkKey,
	CancelKey
};
/* 菜单按键状态 */
struct MenuKeyValue{
	uint8_t up:1;
	uint8_t down:1;
	uint8_t left:1;
	uint8_t right:1;
	uint8_t ok:1;
	uint8_t esc:1;
	enum KeyStatus keyIs;
};
/* 遥控按键状态 */
struct YKKeyValue{
	uint8_t open:1;
	uint8_t close:1;
	uint8_t shift:1;
	uint8_t reset:1;
	enum KeyStatus keyIs;
};

/* 开出映射 */
enum UserOutputNoMap{
	/* LED 映射 */
	LED_BEGIN_NO = 2,
	LED_END_NO = 50,
};

/* Led使用定义 */
enum UserLedDefine{
	ULED_NOENERGY,		/* 未储能 */
	ULED_SWITCHOPEN,	/* 开关分位 */
	ULED_REMOTE,		/* 远方 */
	ULED_SWITCHCLOSE,	/* 开关合位 */
	ULED_CLOSELOCK,		/* 合闸闭锁 */
	ULED_OVERALARM,		/* 过流告警 */
	ULED_OPENLOCK,		/* 分闸闭锁 */
	ULED_ZEROALARM,		/* 零序告警 */
	ULED_DEVICEFAULT,	/* 装置故障 */
	ULED_SELFCHECK,		/* 自检异常 */
	ULED_COMMUN,		/* 通信 */
	ULED_RECLOSELOCK,	/* 重合闭锁 */
	ULED_BATTERYALARM,/* 电池告警 */
	ULED_BATTERYRELEASE,/* 电池活化 */
	ULED_PROTECTOFF,	/* 保护闭锁 */
	ULED_BATTERYLOWV,	/* 电池欠压 */
	ULED_LS,			/* 负荷开关 */
	ULED_CB,			/* 断路器 */
	ULED_ALLNUM
};
enum UserLedState{
	ULED_OFF,
	ULED_ON,
};

/* 模拟量使用定义 */
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
