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
#include "common_data.h"
#include <rtthread.h>

/* �ӻ����ñ� */
DpuSlaveList SlaveList;
static DpuSlaveInfo UserSlave[SM_SLAVEMAXNUM];
/* ��ǰ���еĴӻ� */
static DpuSlaveInfo *RunSlave;

static RequestSendList RsendList;

uint8_t reqSendBUff[256];
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
  *@brief  ��ȡ�ӻ���Ϣ����ʼ��
  *@param  None
  *@retval None
  */
void GetSlaveAndInit(void)
{
	struct ConfigurationSetModbase *slavecfg = &g_ConfigurationSetModDB;
	uint8_t slaveNum = SM_SLAVEMAXNUM;
	if(slavecfg->ModMaxNum < SM_SLAVEMAXNUM){
		slaveNum = slavecfg->ModMaxNum;
	}
	for(uint8_t i = 0; i < slaveNum; i ++){
		UserSlave[i].slave = slavecfg->ModAddr[i];
		UserSlave[i].yxBaddr = slavecfg->ModYxAddr[i];
		UserSlave[i].ycBaddr = slavecfg->ModYcAddr[i];
		UserSlave[i].ykBaddr = slavecfg->ModYkAddr[i];
		/* ������������ַ */
		if(slavecfg->ModYxNum[i] > 0){
			UserSlave[i].yxEaddr = slavecfg->ModYxNum[i] + UserSlave[i].yxBaddr - 1;
		}
		else{
			UserSlave[i].yxEaddr = UserSlave[i].yxBaddr;
		}
		if(slavecfg->ModYcNum[i] > 0){
			UserSlave[i].ycEaddr = slavecfg->ModYcNum[i] + UserSlave[i].ycBaddr - 1;
		}
		else{
			UserSlave[i].ycEaddr = UserSlave[i].ycBaddr;
		}
		if(slavecfg->ModYkNum[i] > 0){
			UserSlave[i].ykEaddr = slavecfg->ModYkNum[i] + UserSlave[i].ykBaddr - 1;
		}
		else{
			UserSlave[i].ykEaddr = UserSlave[i].ykBaddr;
		}
		
		UserSlave[i].asduAddr = 1;
		UserSlave[i].runflag = SLAVE_CLOSE;
		UserSlave[i].lastFcb = 0;
		UserSlave[i].event = 0;
		UserSlave[i].sourceId = 0;
		UserSlave[i].ykState = 0;
		UserSlave[i].next = NULL;
		InsertSlaveToList(&UserSlave[i]);
	}
}

/**
  *@brief  ����������������б�
  *@param  pReSend ��Ϣ�ṹ��
  *@retval None
  */
void InsertReSendToList(RequestSend *pReSend)
{
	RequestSend *tpSend = RsendList.head;

	while(1){
		if(tpSend == NULL){
			tpSend = pReSend;
			RsendList.num ++;
			break;
		}
		tpSend = tpSend->next;
	}
}

/**
  *@brief  Ѱ����Ӧ�ķ�������
  *@param  sAddr �ӻ���ַ
  *@param  type ����
  *@retval ��Ϣ�ṹ�� NULL δ�ҵ�
  */
RequestSend *LookReSendFromList(uint16_t sAddr, uint32_t type)
{
	RequestSend *tpSend;
	
	for(tpSend = RsendList.head; tpSend != NULL; tpSend = tpSend->next){
		if(tpSend->slave == sAddr && tpSend->type == type){
			return tpSend;
		}
	}
	return NULL;
}

/**
  *@brief  ɾ����Ӧ�ķ�������
  *@param  reqInfo ��Ϣ�ṹ��
  *@retval None
  */
void DeleteReSendFromList(RequestSend *reqInfo)
{
	RequestSend *tpSend,*tpSend2;
	
	tpSend = RsendList.head;
	
	if(tpSend == reqInfo){
		tpSend = reqInfo->next;
		return;
	}
	
	while(1)
	{
		if(tpSend != NULL){
			tpSend2 = tpSend->next;
			if(tpSend2 == reqInfo){
				tpSend->next = reqInfo->next;
				rt_free(reqInfo);
				break;
			}
		}
		else{
			break;
		}
		tpSend = tpSend2;
	}
}

/**
  *@brief  �ôӻ���ַ��ȡ�ӻ���Ϣ
  *@param  sAddr �ӻ���ַ
  *@retval �ӻ���Ϣ NULL δ�ҵ�
  */
DpuSlaveInfo *GetSlaveInfoUseAddr(uint16_t sAddr)
{
	
	DpuSlaveInfo *tSlave = SlaveList.head;
	while(tSlave != NULL)
	{
		if(tSlave->slave == sAddr){
			return tSlave;
		}
		tSlave = tSlave->next;
	}
	return NULL;
}

/**
  *@brief  �ôӻ���ַ��ȡ�ӻ���Ϣ
  *@param  sAddr �ӻ���ַ
  *@retval �ӻ���Ϣ NULL δ�ҵ�
  */
static int8_t YaokongResult(RequestSend *pReSend)
{
	int8_t errorStatus = 0;
	uint8_t offset = 6;
	DpuSlaveInfo *tSlave = SlaveList.head;
	uint8_t slaveNum = SlaveList.num;
	
	/* ��ȡ��ַ */
	uint16_t ykAddr = pReSend->pbuff[offset] + (pReSend->pbuff[offset + 1]<<8);
	
	for(uint8_t i = 0; i < slaveNum; i ++){
		/* ���Ҵ˵�ַ��Ӧ�Ĵӻ� */
		if((tSlave->ykBaddr <= ykAddr) && (tSlave->ykEaddr > ykAddr)){
			if(tSlave->runflag == SLAVE_CLOSE || (tSlave->event & SLAVE_YAOKONG)){
				errorStatus = 1;/* ��ǰ������ִ��ң�� */
			}
			else{
				pReSend->slave = tSlave->slave;
				/* ң�ص�ַӳ�� */
				ykAddr = ykAddr - tSlave->ykBaddr + 1;
				pReSend->pbuff[offset] = (uint8_t)ykAddr;
				pReSend->pbuff[offset + 1] = (uint8_t)(ykAddr<<8);
				DPU_SET_EVENT_FLAG(tSlave,SLAVE_YAOKONG);
				errorStatus = 0;
			}
			break;
		}
		if(tSlave->next != NULL){
			tSlave = tSlave->next;
		}
		else{
			errorStatus = -1;
			break;
		}
	}
	
	if(errorStatus != 0){
		pReSend->pbuff[5] = 47;
		pReSend->pbuff[6] = 0;
		DBSend[pReSend->id](pReSend->id,pReSend->pbuff);
	}
	return errorStatus;
}

/**
  *@brief  ����������ص�
  *@param  drvid 
  *@param  drvid
  *@retval 1 ok  0 false
  */
static uint8_t RequestSendCmd(uint8_t drvid,uint8_t *pbuf)
{ //LENTH/Lock_ID/TI
	RequestSend *pReSend;
	
	if(pbuf[0] > 250){
		return 0;
	}
	
	pReSend = rt_malloc(sizeof(RequestSend));
	if(pReSend == NULL){
		return 0;
	}
	pReSend->id = drvid;
	pReSend->next = NULL;
	pReSend->len = pbuf[0];
	rt_memcpy(pReSend->pbuff,&pbuf[0],pbuf[0]);
	
	switch(pbuf[2]){
/* + ���Ʒ���Ĺ�����Ϣ */	
		/* - �������� ˫������ */
		case _DLT634_5101MASTER_C_SC_NA_1:
		case _DLT634_5101MASTER_C_SC_NB_1:
			pReSend->type = SLAVE_YAOKONG;
			if(YaokongResult(pReSend) != 0){
				rt_free(pReSend);
			}
			else{
				InsertReSendToList(pReSend);
			}
			break;
/* + ���Ʒ����ϵͳ���� */
		/* - ���ٻ� �������ٻ� ʱ��ͬ�� �������� ��λ���� */
		case _DLT634_5101MASTER_C_IC_NA_1:break;
		case _DLT634_5101MASTER_C_CI_NA_1:break;
		case _DLT634_5101MASTER_C_CS_NA_1:break;
		case _DLT634_5101MASTER_C_TS_NA_1:break;
		case _DLT634_5101MASTER_C_RP_NA_1:break;
		/* -  �л���ֵ�� ����ֵ���� �������Ͷ�ֵ д�����Ͷ�ֵ */
		case _DLT634_5101MASTER_C_SR_NA_1:break;
		case _DLT634_5101MASTER_C_RR_NA_1:break;
		case _DLT634_5101MASTER_C_RS_NA_1:break;
		case _DLT634_5101MASTER_C_WS_NA_1:break;
		/* -  �ļ����� ������� */
		case _DLT634_5101MASTER_F_FR_NA_1:break;
		case _DLT634_5101MASTER_F_SR_NA_1:break;
	}
	return 1;
}

/**
  *@brief  ��ȡң������
  *@param  sInfo 
  *@param  pSend
  *@retval 0 ok  
  */
int8_t GetAndFillYaokongCmd(DpuSlaveInfo *sInfo,ReqSendInfo *pSend)
{
	int8_t errorStatus = 0;
	
	RequestSend *reqInfo = LookReSendFromList(sInfo->slave,SLAVE_YAOKONG);
	if(reqInfo == NULL){
		return -1;
	}
	
	pSend->TI = reqInfo->pbuff[2];
	pSend->VSQ = reqInfo->pbuff[3];
	pSend->COT = reqInfo->pbuff[4] + (reqInfo->pbuff[5]<<8);
	pSend->asduAddr = reqInfo->pbuff[6] + (reqInfo->pbuff[7]<<8);
	
	rt_memcpy(pSend->pbuff,&(reqInfo->pbuff[8]),3);
	DeleteReSendFromList(reqInfo);
	return errorStatus;
}

/**
  *@brief  �ܳ�ʼ��
  *@param  None
  *@retval None
  */
void SMasterDpuControlInit(void)
{
	GetSlaveAndInit();
	RsendList.num = 0;
	RsendList.head = NULL;
	(DBSend[SMASTER101_ID0]) = RequestSendCmd;
	
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

void CycleEventDealwith(void)
{
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
		DPU_SET_EVENT_FLAG(RunSlave,SLAVE_INIT);
	}
	else if(RunSlave->next != NULL){
		RunSlave = RunSlave->next;
	}
	else{
		RunSlave = SlaveList.head;
	}
	if(RunSlave->runflag == SLAVE_CLOSE){
		if(GetTimer1IntervalTick(RunSlave->timeConut) > 15000){
			RunSlave->timeConut = GetTimer1Tick();
			DPU_SET_EVENT_FLAG(RunSlave,SLAVE_INIT);
		}
	}
//	if(RunSlave->checkout == SLAVE_REQUESTSWITCH){
//		RunSlave = LookShouldRunSlave();
//		if(RunSlave->event == 0 && RunSlave->runflag == SLAVE_CLOSE){
//			RunSlave->event |= SLAVE_INIT;
//		}
//		return;
//	}
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
	pSend->pbuff = reqSendBUff;
	pSend->slen = 0;
	pSend->TI = 0;
	pRev->ackC = 0;
	pRev->fcb = &(sInfo->lastFcb);
	pRev->pbuff = reqSendBUff;
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
		/* ����������û����� */
		if((tRev.ackC & _DLT634_5101MASTER_FUNCODE) == _DLT634_5101MASTER_M_FUN9){
			//DPU_RESET_EVENT_FLAG(sInfo,SLAVE_HAVEDATA_2);
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
	if(GetAndFillYaokongCmd(sInfo,&tSend) != 0){
		return;
	}
	tSend.type = _DLT634_5101MASTER_M_FUN3;
	eErrStatus = SMasterSendFrame(SM_LINKID,&tSend);
	if(eErrStatus == SM_MRE_NO_ERR){
		if((tRev.ackC & _DLT634_5101MASTER_FUNCODE) == 0){//ȷ���Ͽ�
			DPU_SET_EVENT_FLAG(sInfo,SLAVE_HAVEDATA_1);
		}
		else{
			//����ʧ��
		}
		DPU_RESET_EVENT_FLAG(sInfo,SLAVE_YAOKONG);
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
