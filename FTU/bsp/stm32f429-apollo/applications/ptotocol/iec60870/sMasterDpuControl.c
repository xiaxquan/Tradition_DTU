/**
  * @file    sMasterDpuControl.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/08/02
  * @brief   非平衡通信管理控制文件
  */

#include "sMasterDpuControl.h"
#include "dlt634_5101master_app.h"
#include "dlt634_5101master_disk.h"

/* 从机调用表 */
DpuSlaveList SlaveList;
static DpuSlaveInfo UserSlave[8];
/* 当前运行的从机 */
static DpuSlaveInfo *RunSlave;
static uint8_t SMasterAsduLen = 2;
/**
  *@brief  插入从机到列表中
  *@param  pSlave 从机
  *@retval - 1 出错 0 正确
  */
int8_t InsertSlaveToList(DpuSlaveInfo *pSlave)
{
	if(pSlave == NULL){
		return -1;
	}
	if(SlaveList.head == NULL){//表为空
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
  *@brief  从机初始化
  *@param  pSlave 从机
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
  *@brief  获取从机信息并初始化
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
  *@brief  总初始化
  *@param  None
  *@retval None
  */
void SMasterDpuControlInit(void)
{
	GetSlaveAndInit();
	RunSlave = NULL;
}

/**
  *@brief  查找当前最优的从机
  *@param  None
  *@retval NULL 没有可用主机 其他 主机指针
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
  *@brief  从机切换处理
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
  *@brief  sendInfo RevInfo 结构体初始化
  *@param  sInfo 从机信息
  *@param  pSend 发送信息结构体
  *@param  pRev  发送接收结构体
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
  *@brief  建立链路，初始化
  *@param  sInfo 从机信息
  *@retval None
  */
void SMasterInitCmdFun(DpuSlaveInfo *sInfo)
{
	ReqSendInfo tSend;
	ReqRevInfo tRev;
	ResetSendRevInfoDefault(sInfo,&tSend,&tRev);
	/* 建立链路 初始化 */
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
  *@brief  请求一级数据命令
  *@param  sInfo 从机信息
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
  *@brief  请求二级数据命令
  *@param  sInfo 从机信息
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
  *@brief  总召唤开始命令
  *@param  sInfo 从机信息
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
  *@brief  遥控命令
  *@param  sInfo 从机信息
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
  *@brief  时钟同步命令
  *@param  sInfo 从机信息
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
	//获取时钟，填充
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
  *@brief  时钟读取命令
  *@param  sInfo 从机信息
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
	//获取时钟，填充
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
  *@brief  心跳测试命令
  *@param  sInfo 从机信息
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
  *@brief  复位进程
  *@param  sInfo 从机信息
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
  *@brief  选择运行函数
  *@param  sInfo 从机信息
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
  *@brief  控制运行运行主函数
  *@param  sInfo 从机信息
  *@retval None
  */
void SMasterDpuControlMain(void)
{
	SlaveCheckoutDealWith();
	SlaveRunMain(RunSlave);
}

/* END */
