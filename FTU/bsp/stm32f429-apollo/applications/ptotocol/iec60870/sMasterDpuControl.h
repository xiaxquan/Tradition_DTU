/**
  * @file    sMasterDpuControl.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/08/02
  * @brief   ��ƽ��ͨ�Ź�������ļ�
  */

#include "stdint.h"

#ifndef __SMASTERDPUCONTROL_H   
#define __SMASTERDPUCONTROL_H

#ifndef NULL
#define NULL ((void *)0)
#endif

#define SM_LINKID 0

#define SLAVE_RUNCLOSEWAITTIME 3000/* �ӻ���δ����״̬�µ�ѭ������ʱ�� */
#define SLAVE_RUNSUSPENDWAITTIME 1000/* �ӻ��ڹ���״̬�µ�ѭ������ʱ�� */
#define SLAVE_HAVEDATA_2_TIME 1000/* �ӻ��ڿ���״̬�µĶ�������ѭ������ʱ�� */

/* �ӻ��¼���� */
#define SLAVE_INIT 				0x08000000/* ��ʼ�� */
#define SLAVE_INIT_REQUESTALL 	0x04000000/* ��ʼ�����ٻ� */
#define SLAVE_YAOKONG 			0x02000000/* ң�� */
#define SLAVE_REQUESTALL 		0x01000000/* ��ʼ�����ٻ� */
#define SLAVE_HAVEDATA_1 		0x00800000/* ��һ�����������ٻ� */
#define SLAVE_CLOCKSYNC 		0x00008000/* ʱ��ͬ�� */
#define SLAVE_TESTCMD 			0x00004000/* �������� */
#define SLAVE_RESET 			0x00002000/* ��λ���� */
#define SLAVE_HAVEDATA_2 		0x00001000/* �ж������������ٻ� */

#define DPU_SET_EVENT_FLAG(sInfo,flag) (sInfo->event |= flag)
#define DPU_RESET_EVENT_FLAG(sInfo,flag) (sInfo->event &= (~flag))
/* �ӻ�����ö�� */
enum RunFlagState{
	SLAVE_CLOSE,/* δ���� */
	SLAVE_RUNINIT,/* ���� */
	SLAVE_READY,/* ׼������,���Ե��� */
	SLAVE_RUNING,/* ����ʹ�� */
	SLAVE_SUSPEND,/* ���� */
};

/* �ӻ��л�״̬ö�� */
enum SwitchFlagState{
	SLAVE_REQUESTSWITCH,/* �����л� */
	SLAVE_CANSWITCH,/* ����ǿ���л� */
	SLAVE_NOTSWITCH,/* �����л� */
};

/* �ӻ���Ϣ */
typedef struct SlaveInfo_{
	uint16_t addr; 	/* ��վ��ַ */
}SlaveInfo;

/* �ӻ���Ϣ */
typedef struct DpuSlaveInfo_{
	uint16_t slave; 	/* ��վ��ַ */
	uint16_t asduAddr;	/* ASDU��ַ */
	enum RunFlagState runflag;/* ����״̬,����ʹ�� */
	uint8_t lastFcb;
	uint8_t ackstatus;	/* Ӧ��״̬ */
	uint32_t timeConut;	/* ������ϴ����еļ��ʱ�� */
	uint32_t event;		/* �¼���� */
	enum SwitchFlagState checkout;		/* �л���־ */
	uint8_t sourceId;	/* ����Դ */
	uint16_t ykaddr;	/* ң�ص�ַ */
	uint8_t ykState;	/* 0 �� 1 ѡ����� */
	struct DpuSlaveInfo_ *next;			/* ������һ���ӻ� */
}DpuSlaveInfo;


/* �ӻ��� */
typedef struct DpuSlaveList_{
	uint8_t num;/* �ӻ��� */
	DpuSlaveInfo *head;/* �ӻ���ʼͷ */
}DpuSlaveList;

/* 101��ʼ������� */
void Master101InitCmdFun(DpuSlaveInfo *sInfo);
void SMasterDpuControlMain(void);
void SMasterDpuControlInit(void);
#endif /* __SMASTERDPUCONTROL_H */

/* END */
