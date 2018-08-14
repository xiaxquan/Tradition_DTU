/**
  * @file    sMasterDpuControl.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/08/02
  * @brief   ��ƽ��ͨ�Ź�������ļ�
  */

#include "sMasterDpuControl.h"
#include "dlt634_5101master_app.h"
#include "dlt634_5101master_disk.h"

/* �ӻ����ñ� */
DpuSlaveList SlaveList;
static DpuSlaveInfo UserSlave[8];
/* ��ǰ���еĴӻ� */
static DpuSlaveInfo *RunSlave;
static uint8_t SMasterAsduLen = 2;
/**
  *@brief  ����ӻ����б���
  *@param  pSlave �ӻ�
  *@retval - 1 ���� 0 ��ȷ
  */
int8_t InsertSlaveToList(DpuSlaveInfo *pSlave)
{
	if(pSlave == NULL){
		return -1;
	}
	if(SlaveList.head == NULL){//��Ϊ��
		SlaveList.head = pSlave;
		SlaveList.num ++;
		return 0;
	}
	DpuSlaveInfo *tSlave = SlaveList.head;
	while(1){
		if(tSlave->next == NULL){
			tSlave->next = pSlave;
			SlaveList.num ++;
			break;
		}
		tSlave = tSlave->next;
	}
	return 0;
}


/**
  *@brief  �ӻ���ʼ��
  *@param  pSlave �ӻ�
  *@retval None
  */
void DpuSlaveInit(DpuSlaveInfo *pSlave,SlaveInfo *pInfo)
{
	pSlave->slave = pInfo->addr;
	pSlave->runflag = SLAVE_CLOSE;
	pSlave->next = NULL;
	pSlave->asduAddr = 0;
	InsertSlaveToList(pSlave);
}

/**
  *@brief  ��ȡ�ӻ���Ϣ����ʼ��
  *@param  None
  *@retval None
  */
void GetSlaveAndInit(void)
{
	for(uint8_t i = 0; i < 8; i ++){
		SlaveInfo tInfo;
		tInfo.addr = i + 20;
		DpuSlaveInit(&UserSlave[i],&tInfo);
	}
}

/**
  *@brief  �ܳ�ʼ��
  *@param  None
  *@retval None
  */
void SMasterDpuControlInit(void)
{
	GetSlaveAndInit();
	RunSlave = NULL;
}

/**
  *@brief  ���ҵ�ǰ���ŵĴӻ�
  *@param  None
  *@retval NULL û�п������� ���� ����ָ��
  */
DpuSlaveInfo *LookShouldRunSlave(void)
{
	uint32_t tEvent = 0;
	DpuSlaveInfo *tSInfo,*tSInfo2;
	tSInfo = SlaveList.head;
	tSInfo2 = tSInfo;
	
	if(tSInfo == NULL){
		return NULL;
	}
	for(uint8_t i = 0; i < SlaveList.num; i++){
		if(tSInfo->event > tEvent){
			tEvent = tSInfo->event;
			tSInfo2 = tSInfo;
		}
		tSInfo = tSInfo->next;
	}
	return tSInfo2;
}

static void CycleEventDealwith(void)
{
	DpuSlaveInfo *tSInfo = SlaveList.head;
	for(uint8_t i = 0; i < SlaveList.num; i ++){
		
	}
}
/**
  *@brief  �ӻ��л�����
  *@param  None
  *@retval None
  */
void SlaveCheckoutDealWith(void)
{
	if(RunSlave == NULL){
		RunSlave = LookShouldRunSlave();
		return;
	}
	if(RunSlave->checkout == SLAVE_REQUESTSWITCH){
		RunSlave = LookShouldRunSlave();
		if(RunSlave->event == 0 && RunSlave->runflag == SLAVE_CLOSE){
			RunSlave->event |= SLAVE_INIT;
		}
		return;
	}
}

/**
  *@brief  sendInfo RevInfo �ṹ���ʼ��
  *@param  sInfo �ӻ���Ϣ
  *@param  pSend ������Ϣ�ṹ��
  *@param  pRev  ���ͽ��սṹ��
  *@retval None
  */
void ResetSendRevInfoDefault(DpuSlaveInfo *sInfo,ReqSendInfo *pSend,ReqRevInfo *pRev)
{
	pSend->slave = sInfo->slave;
	pSend->lastFCB = sInfo->lastFcb;
	pSend->rev = pRev;
	pSend->pbuff = NULL;
	pSend->slen = 0;
	pSend->TI = 0;
	pRev->ackC = 0;
	pRev->fcb = &(sInfo->lastFcb);
	pRev->pbuff = NULL;
	pRev->rlen = 0;
}

/**
  *@brief  ������·����ʼ��
  *@param  sInfo �ӻ���Ϣ
  *@retval None
  */
void SMasterInitCmdFun(DpuSlaveInfo *sInfo)
{
	ReqSendInfo tSend;
	ReqRevInfo tRev;
	ResetSendRevInfoDefault(sInfo,&tSend,&tRev);
	/* ������· ��ʼ�� */
	tSend.type = _DLT634_5101MASTER_M_FUN9;
	if(SMasterSendFrame(SM_LINKID,&tSend) == SM_MRE_NO_ERR){
		tSend.type = _DLT634_5101MASTER_M_FUN0;
		if(SMasterSendFrame(SM_LINKID,&tSend) == SM_MRE_NO_ERR){
			tSend.type = _DLT634_5101MASTER_M_FUN10;
			if(SMasterSendFrame(SM_LINKID,&tSend) == SM_MRE_NO_ERR){
				DPU_RESET_EVENT_FLAG(sInfo,SLAVE_INIT);
				DPU_SET_EVENT_FLAG(sInfo,SLAVE_INIT_REQUESTALL);
				sInfo->runflag = SLAVE_READY;
			}
		}
	}
	if(sInfo->runflag != SLAVE_READY){
		sInfo->runflag = SLAVE_CLOSE;
	}
}

/**
  *@brief  ����һ����������
  *@param  sInfo �ӻ���Ϣ
  *@retval None
  */
void SMasterHaveData1CmdFun(DpuSlaveInfo *sInfo)
{
	SMasterReqErrCode eErrStatus;
	ReqSendInfo tSend;
	ReqRevInfo tRev;
	
	ResetSendRevInfoDefault(sInfo,&tSend,&tRev);
	tSend.type = _DLT634_5101MASTER_M_FUN10;
	eErrStatus = SMasterSendFrame(SM_LINKID,&tSend);
	if(eErrStatus == SM_MRE_NO_ERR){
		if((tRev.ackC & _DLT634_5101MASTER_ACD) == 0){
			DPU_RESET_EVENT_FLAG(sInfo,SLAVE_HAVEDATA_1);
			DPU_SET_EVENT_FLAG(sInfo,SLAVE_HAVEDATA_2);
		}
	}
	else if(eErrStatus == SM_MRE_TIMEDOUT){
		sInfo->runflag = SLAVE_CLOSE;
	}
}

/**
  *@brief  ���������������
  *@param  sInfo �ӻ���Ϣ
  *@retval None
  */
void SMasterHaveData2CmdFun(DpuSlaveInfo *sInfo)
{
	SMasterReqErrCode eErrStatus;
	ReqSendInfo tSend;
	ReqRevInfo tRev;
	ResetSendRevInfoDefault(sInfo,&tSend,&tRev);
	tSend.type = _DLT634_5101MASTER_M_FUN11;
	eErrStatus = SMasterSendFrame(SM_LINKID,&tSend);
	if(eErrStatus == SM_MRE_NO_ERR){
		if(tRev.ackC & _DLT634_5101MASTER_ACD){
			DPU_SET_EVENT_FLAG(sInfo,SLAVE_HAVEDATA_1);
		}
	}
	else if(eErrStatus == SM_MRE_TIMEDOUT){
		sInfo->runflag = SLAVE_CLOSE;
	}
}
/**
  *@brief  ���ٻ���ʼ����
  *@param  sInfo �ӻ���Ϣ
  *@retval None
  */
void SMasterStartReadAllCmdFun(DpuSlaveInfo *sInfo)
{
	SMasterReqErrCode eErrStatus;
	ReqSendInfo tSend;
	ReqRevInfo tRev;
	ResetSendRevInfoDefault(sInfo,&tSend,&tRev);
	tSend.type = _DLT634_5101MASTER_M_FUN3;
	tSend.TI = _DLT634_5101MASTER_C_IC_NA_1;
	eErrStatus = SMasterSendFrame(SM_LINKID,&tSend);
	if(eErrStatus == SM_MRE_NO_ERR){
		DPU_RESET_EVENT_FLAG(sInfo,SLAVE_INIT_REQUESTALL);
		DPU_RESET_EVENT_FLAG(sInfo,SLAVE_REQUESTALL);
		if(tRev.ackC & _DLT634_5101MASTER_ACD){
			DPU_SET_EVENT_FLAG(sInfo,SLAVE_HAVEDATA_1);
		}
	}
	else if(eErrStatus == SM_MRE_TIMEDOUT){
		sInfo->runflag = SLAVE_CLOSE;
	}
}

/**
  *@brief  ң������
  *@param  sInfo �ӻ���Ϣ
  *@retval None
  */
void SMasterYaokongCmdFun(DpuSlaveInfo *sInfo)
{
	SMasterReqErrCode eErrStatus;
	ReqSendInfo tSend;
	ReqRevInfo tRev;
	
	ResetSendRevInfoDefault(sInfo,&tSend,&tRev);
	//ResetSendRevInfoDefault(sInfo,&tSend,&tRev);
	//GetAndFillYaokongCmd(&tSend);
	
	//uint8_t ykCmd = tSend.pbuff[SMasterAsduLen + 2];

	tSend.type = _DLT634_5101MASTER_M_FUN3;
	eErrStatus =SMasterSendFrame(SM_LINKID,&tSend);
	if(eErrStatus == SM_MRE_NO_ERR){
		DPU_RESET_EVENT_FLAG(sInfo,SLAVE_YAOKONG);
		if(tRev.ackC & _DLT634_5101MASTER_ACD){
			DPU_SET_EVENT_FLAG(sInfo,SLAVE_HAVEDATA_1);
		}
	}
	else if(eErrStatus == SM_MRE_TIMEDOUT){
		sInfo->runflag = SLAVE_CLOSE;
	}
}

/**
  *@brief  ʱ��ͬ������
  *@param  sInfo �ӻ���Ϣ
  *@retval None
  */
void SMasterClockSyncCmdFun(DpuSlaveInfo *sInfo)
{
	SMasterReqErrCode eErrStatus;
	ReqSendInfo tSend;
	ReqRevInfo tRev;
	ResetSendRevInfoDefault(sInfo,&tSend,&tRev);
	tSend.type = _DLT634_5101MASTER_M_FUN3;
	tSend.TI = _DLT634_5101MASTER_C_CS_NA_1;
	tSend.COT = _DLT634_5101MASTER_COT_ACT;
	//��ȡʱ�ӣ����
	eErrStatus = SMasterSendFrame(SM_LINKID,&tSend);
	if(eErrStatus == SM_MRE_NO_ERR){
		DPU_RESET_EVENT_FLAG(sInfo,SLAVE_CLOCKSYNC);
		if(tRev.ackC & _DLT634_5101MASTER_ACD){
			DPU_SET_EVENT_FLAG(sInfo,SLAVE_HAVEDATA_1);
		}
	}
	else if(eErrStatus == SM_MRE_TIMEDOUT){
		sInfo->runflag = SLAVE_CLOSE;
	}
}

/**
  *@brief  ʱ�Ӷ�ȡ����
  *@param  sInfo �ӻ���Ϣ
  *@retval None
  */
void SMasterClockReadCmdFun(DpuSlaveInfo *sInfo)
{
	SMasterReqErrCode eErrStatus;
	ReqSendInfo tSend;
	ReqRevInfo tRev;
	ResetSendRevInfoDefault(sInfo,&tSend,&tRev);
	tSend.type = _DLT634_5101MASTER_M_FUN3;
	tSend.TI = _DLT634_5101MASTER_C_CS_NA_1;
	tSend.COT = _DLT634_5101MASTER_COT_REQ;
	//��ȡʱ�ӣ����
	eErrStatus = SMasterSendFrame(SM_LINKID,&tSend);
	if(eErrStatus == SM_MRE_NO_ERR){
		DPU_RESET_EVENT_FLAG(sInfo,SLAVE_CLOCKSYNC);
		if(tRev.ackC & _DLT634_5101MASTER_ACD){
			DPU_SET_EVENT_FLAG(sInfo,SLAVE_HAVEDATA_1);
		}
	}
	else if(eErrStatus == SM_MRE_TIMEDOUT){
		sInfo->runflag = SLAVE_CLOSE;
	}
}

/**
  *@brief  ������������
  *@param  sInfo �ӻ���Ϣ
  *@retval None
  */
void SMasterTestCmdFun(DpuSlaveInfo *sInfo)
{
	SMasterReqErrCode eErrStatus;
	ReqSendInfo tSend;
	ReqRevInfo tRev;
	ResetSendRevInfoDefault(sInfo,&tSend,&tRev);
	tSend.type = _DLT634_5101MASTER_M_FUN2;
	eErrStatus = SMasterSendFrame(SM_LINKID,&tSend);
	if(eErrStatus == SM_MRE_NO_ERR){
		DPU_RESET_EVENT_FLAG(sInfo,SLAVE_TESTCMD);
		if(tRev.ackC & _DLT634_5101MASTER_ACD){
			DPU_SET_EVENT_FLAG(sInfo,SLAVE_HAVEDATA_1);
		}
	}
	else if(eErrStatus == SM_MRE_TIMEDOUT){
		sInfo->runflag = SLAVE_CLOSE;
	}
}

/**
  *@brief  ��λ����
  *@param  sInfo �ӻ���Ϣ
  *@retval None
  */
void SMasterResetCmdFun(DpuSlaveInfo *sInfo)
{
	SMasterReqErrCode eErrStatus;
	ReqSendInfo tSend;
	ReqRevInfo tRev;
	ResetSendRevInfoDefault(sInfo,&tSend,&tRev);
	tSend.type = _DLT634_5101MASTER_M_FUN3;
	tSend.TI = _DLT634_5101MASTER_C_RP_NA_1;
	tSend.COT = _DLT634_5101MASTER_COT_ACT;
	eErrStatus = SMasterSendFrame(SM_LINKID,&tSend);
	if(eErrStatus == SM_MRE_NO_ERR){
		DPU_RESET_EVENT_FLAG(sInfo,SLAVE_TESTCMD);
		DPU_SET_EVENT_FLAG(sInfo,SLAVE_HAVEDATA_1);
	}
	else if(eErrStatus == SM_MRE_TIMEDOUT){
		sInfo->runflag = SLAVE_CLOSE;
	}
}

/**
  *@brief  ѡ�����к���
  *@param  sInfo �ӻ���Ϣ
  *@retval None
  */
void SlaveRunMain(DpuSlaveInfo *sInfo)
{
	uint32_t slaveEvent = sInfo->event;
	
	if(slaveEvent & SLAVE_INIT){
		SMasterInitCmdFun(sInfo);
	}
	else if(slaveEvent & SLAVE_INIT_REQUESTALL){
		SMasterStartReadAllCmdFun(sInfo);
	}
	else if(slaveEvent & SLAVE_YAOKONG){
		SMasterYaokongCmdFun(sInfo);
	}
	else if(slaveEvent & SLAVE_REQUESTALL){
		SMasterStartReadAllCmdFun(sInfo);
	}
	else if(slaveEvent & SLAVE_HAVEDATA_1){
		SMasterHaveData1CmdFun(sInfo);
	}
	else if(slaveEvent & SLAVE_CLOCKSYNC){
	}
	else if(slaveEvent & SLAVE_TESTCMD){
		SMasterTestCmdFun(sInfo);
	}
	else if(slaveEvent & SLAVE_RESET){
		SMasterTestCmdFun(sInfo);
	}
	else if(slaveEvent & SLAVE_HAVEDATA_2){
		SMasterHaveData2CmdFun(sInfo);
	}
}

/**
  *@brief  ������������������
  *@param  sInfo �ӻ���Ϣ
  *@retval None
  */
void SMasterDpuControlMain(void)
{
	SlaveCheckoutDealWith();
	SlaveRunMain(RunSlave);
}

/* END */
