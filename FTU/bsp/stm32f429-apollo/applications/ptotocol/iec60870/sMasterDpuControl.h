/**
  * @file    sMasterDpuControl.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/08/02
  * @brief   非平衡通信管理控制文件
  */

#include "stdint.h"

#ifndef __SMASTERDPUCONTROL_H   
#define __SMASTERDPUCONTROL_H

#ifndef NULL
#define NULL ((void *)0)
#endif

#define SM_LINKID 0
#define SM_SLAVEMAXNUM 16/* 从机最大数 */
#define SLAVE_RUNCLOSEWAITTIME 3000/* 从机在未连接状态下的循环调用时间 */
#define SLAVE_RUNSUSPENDWAITTIME 1000/* 从机在挂起状态下的循环调用时间 */
#define SLAVE_HAVEDATA_2_TIME 1000/* 从机在空闲状态下的二级数据循环调用时间 */

/* 从机事件标记 */
#define SLAVE_INIT 				0x08000000/* 初始化 */
#define SLAVE_INIT_REQUESTALL 	0x04000000/* 初始化总召唤 */
#define SLAVE_YAOKONG 			0x02000000/* 遥控 */
#define SLAVE_REQUESTALL 		0x01000000/* 初始化总召唤 */
#define SLAVE_HAVEDATA_1 		0x00800000/* 有一级数据请求召唤 */
#define SLAVE_CLOCKSYNC 		0x00008000/* 时钟同步 */
#define SLAVE_TESTCMD 			0x00004000/* 测试命令 */
#define SLAVE_RESET 			0x00002000/* 复位进程 */
#define SLAVE_HAVEDATA_2 		0x00001000/* 有二级数据请求召唤 */

#define DPU_SET_EVENT_FLAG(sInfo,flag) (sInfo->event |= flag)
#define DPU_RESET_EVENT_FLAG(sInfo,flag) (sInfo->event &= (~flag))
/* 从机运行枚举 */
enum RunFlagState{
	SLAVE_CLOSE,/* 未连接 */
	SLAVE_RUNINIT,/* 握手 */
	SLAVE_READY,/* 准备就绪,可以调用 */
	SLAVE_RUNING,/* 正在使用 */
	SLAVE_SUSPEND,/* 挂起 */
};

/* 从机切换状态枚举 */
enum SwitchFlagState{
	SLAVE_REQUESTSWITCH,/* 请求切换 */
	SLAVE_CANSWITCH,/* 可以强制切换 */
	SLAVE_NOTSWITCH,/* 不可切换 */
};

/* 从机信息 */
typedef struct SlaveInfo_{
	uint16_t addr; 	/* 从站地址 */
	uint16_t yxBaddr;/* 遥信开始地址 */
	uint16_t ycBaddr;
	uint16_t ykBaddr;
	uint8_t yxNum;
	uint8_t ycNum;
	uint8_t ykNum;
}SlaveInfo;

/* 从机信息 */
typedef struct DpuSlaveInfo_{
	uint16_t slave; 	/* 从站地址 */
	uint16_t asduAddr;	/* ASDU地址 */
	uint16_t yxBaddr;	/* 遥信开始地址 */
	uint16_t ycBaddr;
	uint16_t ykBaddr;
	uint16_t yxEaddr;	/* 遥信结束地址 */
	uint16_t ycEaddr;
	uint16_t ykEaddr;
	enum RunFlagState runflag;/* 运行状态,调度使用 */
	uint8_t lastFcb;
	uint8_t ackstatus;	/* 应答状态 */
	uint32_t timeConut;	/* 相对于上次运行的间隔时间 */
	uint32_t event;		/* 事件标记 */
	enum SwitchFlagState checkout;		/* 切换标志 */
	uint8_t sourceId;	/* 命令源 */
	uint32_t ykAllTick;
	uint32_t ykTick;
	uint16_t ykaddr;	/* 遥控地址 */
	uint8_t ykState;	/* 0 无 1 选择完成 */
	struct DpuSlaveInfo_ *next;			/* 连接下一个从机 */
}DpuSlaveInfo;


/* 从机表 */
typedef struct DpuSlaveList_{
	uint8_t num;	/* 从机数 */
	uint8_t count;	/* 计数器 */
	DpuSlaveInfo *head;/* 从机开始头 */
}DpuSlaveList;


typedef struct RequestSend_{
	uint16_t slave;	/* 从机地址 */
	uint8_t id;	/* 命令源id */
	uint8_t len;/* 命令长度 */
	uint32_t type;/* 命令类型 */
	uint8_t pbuff[250];/* 命令内容指针 */
	struct RequestSend_ *next;
}RequestSend;

typedef struct RequestSendList_{
	uint8_t num;
	struct RequestSend_ *head;
}RequestSendList;

/* 101初始化命令函数 */
void Master101InitCmdFun(DpuSlaveInfo *sInfo);
void SMasterDpuControlMain(void);
void SMasterDpuControlInit(void);
DpuSlaveInfo *GetSlaveInfoUseAddr(uint16_t sAddr);
#endif /* __SMASTERDPUCONTROL_H */

/* END */
