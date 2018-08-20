/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      SMasterInfo.c
  * @brief:     The protocol of SMasterInfo.
  * @version:   V3.01
  * @author:    Mr.J
  * @date:      2017-05-16
  * @update:    [2018-01-22] [Chen][make the code clean up]
  */

/* INCLUDE FILES -------------------------------------------------------------*/
#include "dlt634_5101master_app.h"
#include "dlt634_5101master_disk.h"
#include "string.h"


DLT634_5101MASTER_PAD      DLT634_5101Master_Pad[_DLT634_5101MASTER_VOLUMES];
//DLT634_5101MASTER_APPINFO  DLT634_5101Master_App[_DLT634_5101MASTER_VOLUMES];
DLT634_5101MASTER_APPINFO  SMasterInfo[_DLT634_5101MASTER_VOLUMES];

/**
  *@brief  复位帧接收相关信息,(为接收下一帧数据准备)
  *@param  pdrv 
  *@retval None
  */
static void FrameReceiveInfoReset(uint8_t pdrv)
{
	/* 清除缓冲区无用数据 */
	DLT634_5101_MASTER_ReadData(pdrv,\
		&SMasterInfo[pdrv].RxdBuf[0],_DLT634_5101MASTER_LPDUSIZE);
	SMasterInfo[pdrv].rxCount = 0;
}

/**
  * @brief  发送帧给交互设备
  * @param  pdrv
  * @retval None
  */
static uint8_t FrameSendToDevice(uint8_t pdrv)
{
	FrameReceiveInfoReset(pdrv);
	DebugPrintf("\r\nsend: ");
	char tstr[4];
	for(uint8_t i = 0; i < SMasterInfo[pdrv].txLen; i++){
		sprintf(tstr,"%02X",SMasterInfo[pdrv].TxdBuf[i]);
		DebugPrintf("%s ",tstr);
	}
	DebugPrintf("\r\n");
    DLT634_5101_MASTER_WriteData(pdrv, &SMasterInfo[pdrv].TxdBuf[0],\
		SMasterInfo[pdrv].txLen);
	SMasterInfo[pdrv].linkStatus = LINK_WAITACK;
	/* 启动超时检测 */
	SMasterInfo[pdrv].otTick = GetTimer1Tick();
	SMasterInfo[pdrv].otsCheck = 1;
	SMasterInfo[pdrv].otflag = 0;
	return 0;
}

/**
  *@brief  链路接收超时处理
  *@param  pdrv 
  *@retval None
  */
static void LinkTimeOutResult(uint8_t pdrv)
{
	/* 是否启动超时检测 */
	if(SMasterInfo[pdrv].otsCheck != 1){
		return;
	}
	if(GetTimer1IntervalTick(SMasterInfo[pdrv].otTick) > DLT634_5101Master_Pad[pdrv].TimeOutValue){
		SMasterInfo[pdrv].otflag = 1;
	}
	if(SMasterInfo[pdrv].otflag == 1){//超时
		if((++SMasterInfo[pdrv].reSend) <= 2){
			SMasterInfo[pdrv].linkStatus = LINK_SEND;
			FrameSendToDevice(pdrv);//重发
		}
		else{//超时重发次数上限
			SMasterInfo[pdrv].otsCheck = 0;
			SMasterInfo[pdrv].otflag = 0;
			SMasterInfo[pdrv].reSend = 0;
			SMasterInfo[pdrv].linkStatus = LINK_IDLE;
			SMasterEventPost(EV_SMASTER_ERROR_RESPOND_TIMEOUT);
			DebugPrintf("通信超时\r\n");
		}
	}	
}

/**
  *@brief  获取接收帧的控制域
  *@param  pdrv 
  *@param  pBuff 帧起始地址
  *@retval 控制域值
  */
static uint8_t GetFrameSlaveControl(uint8_t pdrv,uint8_t *pBuff)
{
	uint8_t control;
	
	if(pBuff[0] == 0x10){
		control = pBuff[1];
	}
	else{
		control = pBuff[4];
	}
	return control;
}

/**
  *@brief  获取68帧的ASDU中的TI
  *@param  pdrv 
  *@param  pBuff 帧起始地址
  *@retval 标识类型值
  */
static uint8_t GetFrameSlaveAddr(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = 2;
	uint16_t addr;
	
	if(pBuff[0] == 0x68){//帧类别检测
		offset = 5;
	}
	addr = pBuff[offset];
	if(DLT634_5101Master_Pad[pdrv].LinkAddrSize == 2){
		addr = addr | (pBuff[offset + 1] << 8);
	}
	return addr;
}

/**
  *@brief  获取68帧的ASDU中的TI
  *@param  pdrv 
  *@param  pBuff 帧起始地址
  *@retval 标识类型值
  */
static uint8_t Get68FrameASDU_TI(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = DLT634_5101Master_Pad[pdrv].LinkAddrSize + 5;
	
	return pBuff[offset];
}

/**
  *@brief  获取68帧的ASDU中的VSQ
  *@param  pdrv 
  *@param  pBuff 帧起始地址
  *@retval vsq值
  */
static uint8_t Get68FrameASDU_VSQ(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = DLT634_5101Master_Pad[pdrv].LinkAddrSize + 6;
	
	return pBuff[offset];
}

/**
  *@brief  获取68帧的ASDU中的COT
  *@param  pdrv
  *@param  pBuff 帧起始地址
  *@retval COT值
  */
static uint8_t Get68FrameASDU_COT(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset;
	
	offset = DLT634_5101Master_Pad[pdrv].LinkAddrSize +\
		DLT634_5101Master_Pad[pdrv].ASDUCotSize + 7;
	
	return pBuff[offset];
}

/**
  *@brief  获取68帧的ASDU中的数据起始指针
  *@param  pdrv
  *@param  pBuff 帧起始地址
  *@retval 数据指针
  */
static uint8_t *Get68FrameASDU_pData(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = DLT634_5101Master_Pad[pdrv].LinkAddrSize +\
		DLT634_5101Master_Pad[pdrv].ASDUCotSize +\
		DLT634_5101Master_Pad[pdrv].ASDUAddrSize + 7;
	
	return &pBuff[offset];
}

/**
  *@brief  计算mod2校验
  *@param  pBuff 数据起始指针
  *@param  len 长度
  *@retval 校验和
  */
uint8_t ModCSCheckNum(uint8_t *pBuff,uint8_t len)
{
	uint8_t modCs = 0;
	
	for(uint8_t i = 0; i < len; i++){
		modCs += pBuff[i];
	}
	
	return modCs;
}

/**
  *@brief  计算帧的mod校验和
  *@param  pdrv
  *@param  pBuff 帧起始地址
  *@retval -1 校验失败 0 成功
  */
static int8_t FrameModCSCheck(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t len,offset,modCs = 0;
	if(pBuff[0] == 0x10){
		offset = 1;
		len = DLT634_5101Master_Pad[pdrv].LinkAddrSize + 1;
	}
	else if(pBuff[0] == 0x68){
		offset = 4;
		len = pBuff[1];
	}
	else{
		return -1;
	}
	/* 计算校验和 */
	modCs = ModCSCheckNum(&pBuff[offset],len);
	if(modCs == pBuff[offset + len]){//计算校验和与本身校验对比
		return 0;
	}
	return -1;
}

/**
  *@brief  帧有效性检测
  *@param  pdrv 
  *@param  pBuff 帧起始地址
  *@retval -1 无效 0 有效
  */
static int8_t FrameValidCheck(uint8_t pdrv, uint8_t *pBuff,uint8_t len)
{
	int8_t eStatus = -1;
	uint8_t addrLen = DLT634_5101Master_Pad[pdrv].LinkAddrSize;
	
	(void)len;
	
	DebugPrintf("\r\nRev: ");
	char tstr[4];
	for(uint8_t i = 0; i < len; i++){
		sprintf(tstr,"%02X",pBuff[i]);
		DebugPrintf("%s ",tstr);
	}
	DebugPrintf("\r\n");
	
	/* 从站地址匹配 */
	if(GetFrameSlaveAddr(pdrv,pBuff) != SMasterInfo[pdrv].slave){
		return eStatus;
	}
	
	/* 帧有效检测 */
	if(pBuff[0] == 0x10 && pBuff[addrLen + 3] == 0x16){
		if(FrameModCSCheck(pdrv, pBuff) == 0){//校验成功
			eStatus = 0;
		}
	}
	else if(pBuff[0] == 0x68 && pBuff[3] == 0x68 && pBuff[pBuff[1] + 5] == 0x16){
		if(pBuff[1] == pBuff[2]){//68帧的两个长度相等
			if(FrameModCSCheck(pdrv, pBuff) == 0){//校验成功
				eStatus = 0;
			}
		}
	}
	return eStatus;
}

/**
  *@brief  应用数据处理
  *@param  pdrv 
  *@retval None
  */
static void AppDataResult(uint8_t pdrv, uint8_t *pbuff)
{
	SMasterInfo[pdrv].rev->TI = Get68FrameASDU_TI(pdrv, pbuff);
	SMasterInfo[pdrv].rev->VSQ = Get68FrameASDU_VSQ(pdrv, pbuff);
	SMasterInfo[pdrv].rev->COT = Get68FrameASDU_COT(pdrv, pbuff);
	uint8_t *pDataBuff = Get68FrameASDU_pData(pdrv, pbuff);
	
	SMasterInfo[pdrv].rev->rlen = 0;/* 目前不用 */
	
	switch(SMasterInfo[pdrv].rev->TI){
/* + 监视方向的过程信息 */
		/* - 单点 双点 */
		case _DLT634_5101MASTER_M_SP_NA_1:
			DataResult_M_SP_NA_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		case _DLT634_5101MASTER_M_DP_NA_1:
			DataResult_M_DP_NA_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		/* - 测量值 归一化 标度化 短浮点数 */
		case _DLT634_5101MASTER_M_ME_NA_1:
			//DataResult_M_ME_NA_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		case _DLT634_5101MASTER_M_ME_NB_1:
			//DataResult_M_ME_NB_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		case _DLT634_5101MASTER_M_ME_NC_1:
			DataResult_M_ME_NC_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		/* - 带时标的单点 双点 故障事件 */
		case _DLT634_5101MASTER_M_SP_TB_1:
			//DataResult_M_SP_TB_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		case _DLT634_5101MASTER_M_DP_TB_1:break;
		case _DLT634_5101MASTER_M_FT_NA_1:break;
		/* - 累计量短浮点 带时标累计量短浮点 */
		case _DLT634_5101MASTER_M_IT_NB_1:break;
		case _DLT634_5101MASTER_M_IT_TC_1:break;
/* + 控制方向的过程信息 */	
		/* - 单点命令 双点命令 */
		case _DLT634_5101MASTER_C_SC_NA_1:
			DataResult_C_SC_NA_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		case _DLT634_5101MASTER_C_SC_NB_1:
			DataResult_C_SC_NB_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
/* + 监视方向的系统命令 */
		/* - 初始化结束 */
		case _DLT634_5101MASTER_M_EI_NA_1:break;
/* + 控制方向的系统命令 */
		/* - 总召唤 电能量召唤 时钟同步 测试命令 复位进程 */
		case _DLT634_5101MASTER_C_IC_NA_1:
			DataResult_C_IC_NA_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		case _DLT634_5101MASTER_C_CI_NA_1:break;
		case _DLT634_5101MASTER_C_CS_NA_1:break;
		case _DLT634_5101MASTER_C_TS_NA_1:break;
		case _DLT634_5101MASTER_C_RP_NA_1:break;
		/* -  切换定值区 读定值区号 读参数和定值 写参数和定值 */
		case _DLT634_5101MASTER_C_SR_NA_1:break;
		case _DLT634_5101MASTER_C_RR_NA_1:break;
		case _DLT634_5101MASTER_C_RS_NA_1:break;
		case _DLT634_5101MASTER_C_WS_NA_1:break;
		/* -  文件传输 软件升级 */
		case _DLT634_5101MASTER_F_FR_NA_1:break;
		case _DLT634_5101MASTER_F_SR_NA_1:break;
	}
}

/**
  *@brief  有效帧处理
  *@param  pdrv 
  *@param  pBuff 有效帧
  *@retval None
  */
static void ValidFrameResult(uint8_t pdrv, uint8_t *pBuff)
{
	int8_t estatus = 0;
	uint8_t control = GetFrameSlaveControl(pdrv,pBuff);
	/* 根据发送命令检测接收命令的有效性，处理相应结果 */
	if(SMasterInfo[pdrv].linkStatus != LINK_WAITACK){
		return;
	}
	SMasterInfo[pdrv].linkStatus = LINK_ACKOK;
	switch(SMasterInfo[pdrv].type){//根据发送判断处理
		case _DLT634_5101MASTER_M_FUN0:
		case _DLT634_5101MASTER_M_FUN2:
		case _DLT634_5101MASTER_M_FUN3:
			switch(control & _DLT634_5101MASTER_FUNCODE){
				case _DLT634_5101MASTER_S_FUN0:break;
				case _DLT634_5101MASTER_S_FUN1:break;
				default:estatus = - 1;break;
			}break;
		case _DLT634_5101MASTER_M_FUN8:
		case _DLT634_5101MASTER_M_FUN9:
			switch(control & _DLT634_5101MASTER_FUNCODE){
				case _DLT634_5101MASTER_S_FUN11:break;
				default:estatus = - 1;break;
			}break;
		case _DLT634_5101MASTER_M_FUN10:
		case _DLT634_5101MASTER_M_FUN11:
			switch(control & _DLT634_5101MASTER_FUNCODE){
				case _DLT634_5101MASTER_S_FUN8://用户数据处理
					AppDataResult(pdrv,pBuff);
					break;
				case _DLT634_5101MASTER_S_FUN9:break;
				default:estatus = - 1;break;
			}break;
		default:estatus = - 1;break;
	}
	/* 应答状态 */
	SMasterInfo[pdrv].rev->ackC = control;
	/* 释放链路 */
	SMasterInfo[pdrv].linkStatus = LINK_IDLE;
	if(estatus == 0){
		DebugPrintf("帧接收处理OK\r\n");
		SMasterEventPost(EV_SMASTER_PROCESS_SUCESS);
	}
	else{
		DebugPrintf("帧接收处理ERROR\r\n");
		SMasterEventPost(EV_SMASTER_ERROR_RECEIVE_DATA);
	}
}

/**
  *@brief  等待接收完整帧
  *@param  pdrv 
  *@retval None
  */
static void WaitReceiveFullFrame(uint8_t pdrv)
{
	DLT634_5101MASTER_APPINFO *thisApp = &SMasterInfo[pdrv];
	
	if(thisApp->linkStatus != LINK_WAITACK){//非等待应答状态不做处理
		return;
	}
	
	if(thisApp->rxCount > _DLT634_5101MASTER_LPDUSIZE){
		thisApp->rxCount = 0;/* 防止溢出 */
	}
	thisApp->rxCount += DLT634_5101_MASTER_ReadData(pdrv,\
	&(thisApp->RxdBuf[thisApp->rxCount]),_DLT634_5101MASTER_LPDUSIZE - thisApp->rxCount);
	
	if(thisApp->rxCount >= 5){
		if((thisApp->RxdBuf[0] == 0x10 && thisApp->rxCount >= DLT634_5101Master_Pad[pdrv].LinkAddrSize + 4)||\
		   (thisApp->RxdBuf[0] == 0x68 && thisApp->rxCount >= thisApp->RxdBuf[1] + 6))
		{
			int8_t eStatus = FrameValidCheck(pdrv, thisApp->RxdBuf,thisApp->rxCount);
			if(eStatus == 0){//有效帧处理
				SMasterInfo[pdrv].otsCheck = 0;
				ValidFrameResult(pdrv, thisApp->RxdBuf);
			}
			else{//帧异常处理
				/* 触发超时重发 */
//				SMasterInfo[pdrv].otflag = 1;
//				SMasterInfo[pdrv].otsCheck = 1;
				
			}
		}
	}
}

/**
  *@brief  68发送帧数据填充
  *@param  pdrv 
  *@param  pBuff 填充区起始指针：ASDU公共地址开始
  *@param  ti 标识类型
  *@param  ackflag(out) 应答状态
  *@retval SMasterReqErrCode 错误码
  */
static uint8_t SMaster68FrameDataFill(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t len = 0;
	switch(SMasterInfo[pdrv].TI){
/* + 监视方向的过程信息 */
		/* - 单点 双点 */
		case _DLT634_5101MASTER_M_SP_NA_1:break;
		case _DLT634_5101MASTER_M_DP_NA_1:break;
		/* - 测量值 归一化 标度化 短浮点数 */
		case _DLT634_5101MASTER_M_ME_NA_1:break;
		case _DLT634_5101MASTER_M_ME_NB_1:break;
		case _DLT634_5101MASTER_M_ME_NC_1:break;
		/* - 带时标的单点 双点 故障事件 */
		case _DLT634_5101MASTER_M_SP_TB_1:break;
		case _DLT634_5101MASTER_M_DP_TB_1:break;
		case _DLT634_5101MASTER_M_FT_NA_1:break;
		/* - 累计量短浮点 带时标累计量短浮点 */
		case _DLT634_5101MASTER_M_IT_NB_1:break;
		case _DLT634_5101MASTER_M_IT_TC_1:break;
/* + 控制方向的过程信息 */	
		/* - 单点命令 双点命令 */
		case _DLT634_5101MASTER_C_SC_NA_1:
		case _DLT634_5101MASTER_C_SC_NB_1:
			DataFill_MASTER_C_SC_NA_1(pdrv,pBuff);
			break;
/* + 监视方向的系统命令 */
		/* - 初始化结束 */
		case _DLT634_5101MASTER_M_EI_NA_1:break;
/* + 控制方向的系统命令 */
		/* - 总召唤 电能量召唤 时钟同步 测试命令 复位进程 */
		case _DLT634_5101MASTER_C_IC_NA_1:
			len = DataFill_MASTER_C_IC_NA_1(pdrv,pBuff);
			break;
		case _DLT634_5101MASTER_C_CI_NA_1:break;
		case _DLT634_5101MASTER_C_CS_NA_1:break;
		case _DLT634_5101MASTER_C_TS_NA_1:break;
		case _DLT634_5101MASTER_C_RP_NA_1:
			len = DataFill_MASTER_C_RP_NA_1(pdrv,pBuff);
			break;
		/* -  切换定值区 读定值区号 读参数和定值 写参数和定值 */
		case _DLT634_5101MASTER_C_SR_NA_1:break;
		case _DLT634_5101MASTER_C_RR_NA_1:break;
		case _DLT634_5101MASTER_C_RS_NA_1:break;
		case _DLT634_5101MASTER_C_WS_NA_1:break;
		/* -  文件传输 软件升级 */
		case _DLT634_5101MASTER_F_FR_NA_1:break;
		case _DLT634_5101MASTER_F_SR_NA_1:break;
	}
	return len;
}

static uint8_t FrameConrtolObtain(uint8_t pdrv,uint8_t type)
{
	uint8_t control = 0;
	/* 控制域计算 */
	control = _DLT634_5101MASTER_PRM | (type & _DLT634_5101MASTER_FUNCODE);
	if(SMasterInfo[pdrv].type == _DLT634_5101MASTER_M_FUN10 ||\
	   SMasterInfo[pdrv].type == _DLT634_5101MASTER_M_FUN11 ||\
	   SMasterInfo[pdrv].type == _DLT634_5101MASTER_M_FUN3)
	{
		if(SMasterInfo[pdrv].lastFCB  & _DLT634_5101MASTER_FCB){
			SMasterInfo[pdrv].lastFCB = 0;
		}
		else{
			SMasterInfo[pdrv].lastFCB = _DLT634_5101MASTER_FCB;
		}
		control |= SMasterInfo[pdrv].lastFCB;
		control |= _DLT634_5101MASTER_FCV;
	}
	return control;
}

static void SMaster68FramePacket(uint8_t pdrv, uint8_t type)
{
	uint8_t offset = 0,control,len;
	uint8_t *pframeBuff = &SMasterInfo[pdrv].TxdBuf[0];
	
	control = FrameConrtolObtain(pdrv,type);
	/* 封装帧 */
	pframeBuff[offset ++] = 0x68;
	pframeBuff[offset ++] = 0x00;//len
	pframeBuff[offset ++] = 0x00;
	pframeBuff[offset ++] = 0x68;
	pframeBuff[offset ++] = control;//control
	pframeBuff[offset ++] =(SMasterInfo[pdrv].slave & 0xFF);//addrL
	if(DLT634_5101Master_Pad[pdrv].LinkAddrSize == 2){
		pframeBuff[offset ++] = (uint8_t)(SMasterInfo[pdrv].slave >> 8);//addrH
	}
	len = SMaster68FrameDataFill(pdrv, &pframeBuff[offset]);
	offset += len;
	pframeBuff[1] = len + DLT634_5101Master_Pad[pdrv].LinkAddrSize + 1;//len
	pframeBuff[2] = pframeBuff[1];
	pframeBuff[offset ++] = ModCSCheckNum(&pframeBuff[4],pframeBuff[1]);//cs
	pframeBuff[offset ++] = 0x16;//end
	SMasterInfo[pdrv].txLen = offset;
}

static void SMaster10FramePacket(uint8_t pdrv,uint8_t type)
{
	uint8_t offset = 0,control;
	uint8_t *pframeBuff = &SMasterInfo[pdrv].TxdBuf[0];
	
	control = FrameConrtolObtain(pdrv,type);
	/* 封装帧 */
	pframeBuff[offset ++] = 0x10;
	pframeBuff[offset ++] = control;//control
	pframeBuff[offset ++] = (SMasterInfo[pdrv].slave & 0xFF);//addrL
	if(DLT634_5101Master_Pad[pdrv].LinkAddrSize == 2){
		pframeBuff[offset ++] = (uint8_t)(SMasterInfo[pdrv].slave >> 8);//addrH
	}	
	pframeBuff[offset ++] = ModCSCheckNum(&pframeBuff[1],\
		DLT634_5101Master_Pad[pdrv].LinkAddrSize + 1);//cs
	pframeBuff[offset ++] = 0x16;//end
	SMasterInfo[pdrv].txLen = offset;
}

/**
  *@brief  帧发送控制
  *@param  pdrv 
  *@retval None
  */
static void FrameSendControl(uint8_t pdrv)
{
	/* 是否有发送请求 */
	if(SMasterInfo[pdrv].linkStatus != LINK_REQUESTSEND){
		return;
	}
	
	SMasterInfo[pdrv].linkStatus = LINK_SEND;	
	
	if(SMasterInfo[pdrv].type == _DLT634_5101MASTER_M_FUN3){
		SMaster68FramePacket(pdrv, _DLT634_5101MASTER_M_FUN3);
	}
	else{
		SMaster10FramePacket(pdrv, SMasterInfo[pdrv].type);	
	}
	/* 发送等待接收 */
	FrameSendToDevice(pdrv);
	FrameReceiveInfoReset(pdrv);
}

/**
  *@brief  请求发送帧
  *@param  sInfo 发送相关信息
  *@retval SMasterReqErrCode 错误码
  */
SMasterReqErrCode SMasterSendFrame(uint8_t pdrv,ReqSendInfo *sInfo)
{
	SMasterReqErrCode eErrStatus = SM_MRE_NO_ERR;
	
	if(SMasterInfo[pdrv].linkStatus != LINK_IDLE){
		eErrStatus = SM_MRE_MASTER_BUSY;
	}
	else{
		SMasterInfo[pdrv].slave = sInfo->slave;
		SMasterInfo[pdrv].lastFCB = sInfo->lastFCB;
		SMasterInfo[pdrv].type = sInfo->type;
		SMasterInfo[pdrv].TI = sInfo->TI;
		SMasterInfo[pdrv].COT = sInfo->COT;
		SMasterInfo[pdrv].VSQ = sInfo->VSQ;
		SMasterInfo[pdrv].pbuff = sInfo->pbuff;
		SMasterInfo[pdrv].slen = sInfo->slen;
		SMasterInfo[pdrv].rev = sInfo->rev;
		SMasterInfo[pdrv].linkStatus = LINK_REQUESTSEND;
		SMasterInfo[pdrv].asduAddr = sInfo->asduAddr;
		/* 等待接收完成 */
		eErrStatus = SMasterWaitRequestFinish();
		*(sInfo->rev->fcb) = SMasterInfo[pdrv].lastFCB;
	}
	return eErrStatus;
}

/**
  *@brief  SMaster执行主函数
  *@param  pdrv 
  *@retval None
  */
void SMaster101Main(uint8_t pdrv)
{
	FrameSendControl(pdrv);
	WaitReceiveFullFrame(pdrv);
	LinkTimeOutResult(pdrv);
}

/* END */
