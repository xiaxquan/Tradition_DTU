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
  *@brief  ��λ֡���������Ϣ,(Ϊ������һ֡����׼��)
  *@param  pdrv 
  *@retval None
  */
static void FrameReceiveInfoReset(uint8_t pdrv)
{
	/* ����������������� */
	DLT634_5101_MASTER_ReadData(pdrv,\
		&SMasterInfo[pdrv].RxdBuf[0],_DLT634_5101MASTER_LPDUSIZE);
	SMasterInfo[pdrv].rxCount = 0;
}

/**
  * @brief  ����֡�������豸
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
	/* ������ʱ��� */
	SMasterInfo[pdrv].otTick = GetTimer1Tick();
	SMasterInfo[pdrv].otsCheck = 1;
	SMasterInfo[pdrv].otflag = 0;
	return 0;
}

/**
  *@brief  ��·���ճ�ʱ����
  *@param  pdrv 
  *@retval None
  */
static void LinkTimeOutResult(uint8_t pdrv)
{
	/* �Ƿ�������ʱ��� */
	if(SMasterInfo[pdrv].otsCheck != 1){
		return;
	}
	if(GetTimer1IntervalTick(SMasterInfo[pdrv].otTick) > DLT634_5101Master_Pad[pdrv].TimeOutValue){
		SMasterInfo[pdrv].otflag = 1;
	}
	if(SMasterInfo[pdrv].otflag == 1){//��ʱ
		if((++SMasterInfo[pdrv].reSend) <= 2){
			SMasterInfo[pdrv].linkStatus = LINK_SEND;
			FrameSendToDevice(pdrv);//�ط�
		}
		else{//��ʱ�ط���������
			SMasterInfo[pdrv].otsCheck = 0;
			SMasterInfo[pdrv].otflag = 0;
			SMasterInfo[pdrv].reSend = 0;
			SMasterInfo[pdrv].linkStatus = LINK_IDLE;
			SMasterEventPost(EV_SMASTER_ERROR_RESPOND_TIMEOUT);
			DebugPrintf("ͨ�ų�ʱ\r\n");
		}
	}	
}

/**
  *@brief  ��ȡ����֡�Ŀ�����
  *@param  pdrv 
  *@param  pBuff ֡��ʼ��ַ
  *@retval ������ֵ
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
  *@brief  ��ȡ68֡��ASDU�е�TI
  *@param  pdrv 
  *@param  pBuff ֡��ʼ��ַ
  *@retval ��ʶ����ֵ
  */
static uint8_t GetFrameSlaveAddr(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = 2;
	uint16_t addr;
	
	if(pBuff[0] == 0x68){//֡�����
		offset = 5;
	}
	addr = pBuff[offset];
	if(DLT634_5101Master_Pad[pdrv].LinkAddrSize == 2){
		addr = addr | (pBuff[offset + 1] << 8);
	}
	return addr;
}

/**
  *@brief  ��ȡ68֡��ASDU�е�TI
  *@param  pdrv 
  *@param  pBuff ֡��ʼ��ַ
  *@retval ��ʶ����ֵ
  */
static uint8_t Get68FrameASDU_TI(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = DLT634_5101Master_Pad[pdrv].LinkAddrSize + 5;
	
	return pBuff[offset];
}

/**
  *@brief  ��ȡ68֡��ASDU�е�VSQ
  *@param  pdrv 
  *@param  pBuff ֡��ʼ��ַ
  *@retval vsqֵ
  */
static uint8_t Get68FrameASDU_VSQ(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = DLT634_5101Master_Pad[pdrv].LinkAddrSize + 6;
	
	return pBuff[offset];
}

/**
  *@brief  ��ȡ68֡��ASDU�е�COT
  *@param  pdrv
  *@param  pBuff ֡��ʼ��ַ
  *@retval COTֵ
  */
static uint8_t Get68FrameASDU_COT(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset;
	
	offset = DLT634_5101Master_Pad[pdrv].LinkAddrSize +\
		DLT634_5101Master_Pad[pdrv].ASDUCotSize + 7;
	
	return pBuff[offset];
}

/**
  *@brief  ��ȡ68֡��ASDU�е�������ʼָ��
  *@param  pdrv
  *@param  pBuff ֡��ʼ��ַ
  *@retval ����ָ��
  */
static uint8_t *Get68FrameASDU_pData(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = DLT634_5101Master_Pad[pdrv].LinkAddrSize +\
		DLT634_5101Master_Pad[pdrv].ASDUCotSize +\
		DLT634_5101Master_Pad[pdrv].ASDUAddrSize + 7;
	
	return &pBuff[offset];
}

/**
  *@brief  ����mod2У��
  *@param  pBuff ������ʼָ��
  *@param  len ����
  *@retval У���
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
  *@brief  ����֡��modУ���
  *@param  pdrv
  *@param  pBuff ֡��ʼ��ַ
  *@retval -1 У��ʧ�� 0 �ɹ�
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
	/* ����У��� */
	modCs = ModCSCheckNum(&pBuff[offset],len);
	if(modCs == pBuff[offset + len]){//����У����뱾��У��Ա�
		return 0;
	}
	return -1;
}

/**
  *@brief  ֡��Ч�Լ��
  *@param  pdrv 
  *@param  pBuff ֡��ʼ��ַ
  *@retval -1 ��Ч 0 ��Ч
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
	
	/* ��վ��ַƥ�� */
	if(GetFrameSlaveAddr(pdrv,pBuff) != SMasterInfo[pdrv].slave){
		return eStatus;
	}
	
	/* ֡��Ч��� */
	if(pBuff[0] == 0x10 && pBuff[addrLen + 3] == 0x16){
		if(FrameModCSCheck(pdrv, pBuff) == 0){//У��ɹ�
			eStatus = 0;
		}
	}
	else if(pBuff[0] == 0x68 && pBuff[3] == 0x68 && pBuff[pBuff[1] + 5] == 0x16){
		if(pBuff[1] == pBuff[2]){//68֡�������������
			if(FrameModCSCheck(pdrv, pBuff) == 0){//У��ɹ�
				eStatus = 0;
			}
		}
	}
	return eStatus;
}

/**
  *@brief  Ӧ�����ݴ���
  *@param  pdrv 
  *@retval None
  */
static void AppDataResult(uint8_t pdrv, uint8_t *pbuff)
{
	SMasterInfo[pdrv].rev->TI = Get68FrameASDU_TI(pdrv, pbuff);
	SMasterInfo[pdrv].rev->VSQ = Get68FrameASDU_VSQ(pdrv, pbuff);
	SMasterInfo[pdrv].rev->COT = Get68FrameASDU_COT(pdrv, pbuff);
	uint8_t *pDataBuff = Get68FrameASDU_pData(pdrv, pbuff);
	
	SMasterInfo[pdrv].rev->rlen = 0;/* Ŀǰ���� */
	
	switch(SMasterInfo[pdrv].rev->TI){
/* + ���ӷ���Ĺ�����Ϣ */
		/* - ���� ˫�� */
		case _DLT634_5101MASTER_M_SP_NA_1:
			DataResult_M_SP_NA_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		case _DLT634_5101MASTER_M_DP_NA_1:
			DataResult_M_DP_NA_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		/* - ����ֵ ��һ�� ��Ȼ� �̸����� */
		case _DLT634_5101MASTER_M_ME_NA_1:
			//DataResult_M_ME_NA_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		case _DLT634_5101MASTER_M_ME_NB_1:
			//DataResult_M_ME_NB_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		case _DLT634_5101MASTER_M_ME_NC_1:
			DataResult_M_ME_NC_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		/* - ��ʱ��ĵ��� ˫�� �����¼� */
		case _DLT634_5101MASTER_M_SP_TB_1:
			//DataResult_M_SP_TB_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		case _DLT634_5101MASTER_M_DP_TB_1:break;
		case _DLT634_5101MASTER_M_FT_NA_1:break;
		/* - �ۼ����̸��� ��ʱ���ۼ����̸��� */
		case _DLT634_5101MASTER_M_IT_NB_1:break;
		case _DLT634_5101MASTER_M_IT_TC_1:break;
/* + ���Ʒ���Ĺ�����Ϣ */	
		/* - �������� ˫������ */
		case _DLT634_5101MASTER_C_SC_NA_1:
			DataResult_C_SC_NA_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
		case _DLT634_5101MASTER_C_SC_NB_1:
			DataResult_C_SC_NB_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
/* + ���ӷ����ϵͳ���� */
		/* - ��ʼ������ */
		case _DLT634_5101MASTER_M_EI_NA_1:break;
/* + ���Ʒ����ϵͳ���� */
		/* - ���ٻ� �������ٻ� ʱ��ͬ�� �������� ��λ���� */
		case _DLT634_5101MASTER_C_IC_NA_1:
			DataResult_C_IC_NA_1(SMasterInfo[pdrv].rev,pDataBuff);
			break;
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
}

/**
  *@brief  ��Ч֡����
  *@param  pdrv 
  *@param  pBuff ��Ч֡
  *@retval None
  */
static void ValidFrameResult(uint8_t pdrv, uint8_t *pBuff)
{
	int8_t estatus = 0;
	uint8_t control = GetFrameSlaveControl(pdrv,pBuff);
	/* ���ݷ�������������������Ч�ԣ�������Ӧ��� */
	if(SMasterInfo[pdrv].linkStatus != LINK_WAITACK){
		return;
	}
	SMasterInfo[pdrv].linkStatus = LINK_ACKOK;
	switch(SMasterInfo[pdrv].type){//���ݷ����жϴ���
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
				case _DLT634_5101MASTER_S_FUN8://�û����ݴ���
					AppDataResult(pdrv,pBuff);
					break;
				case _DLT634_5101MASTER_S_FUN9:break;
				default:estatus = - 1;break;
			}break;
		default:estatus = - 1;break;
	}
	/* Ӧ��״̬ */
	SMasterInfo[pdrv].rev->ackC = control;
	/* �ͷ���· */
	SMasterInfo[pdrv].linkStatus = LINK_IDLE;
	if(estatus == 0){
		DebugPrintf("֡���մ���OK\r\n");
		SMasterEventPost(EV_SMASTER_PROCESS_SUCESS);
	}
	else{
		DebugPrintf("֡���մ���ERROR\r\n");
		SMasterEventPost(EV_SMASTER_ERROR_RECEIVE_DATA);
	}
}

/**
  *@brief  �ȴ���������֡
  *@param  pdrv 
  *@retval None
  */
static void WaitReceiveFullFrame(uint8_t pdrv)
{
	DLT634_5101MASTER_APPINFO *thisApp = &SMasterInfo[pdrv];
	
	if(thisApp->linkStatus != LINK_WAITACK){//�ǵȴ�Ӧ��״̬��������
		return;
	}
	
	if(thisApp->rxCount > _DLT634_5101MASTER_LPDUSIZE){
		thisApp->rxCount = 0;/* ��ֹ��� */
	}
	thisApp->rxCount += DLT634_5101_MASTER_ReadData(pdrv,\
	&(thisApp->RxdBuf[thisApp->rxCount]),_DLT634_5101MASTER_LPDUSIZE - thisApp->rxCount);
	
	if(thisApp->rxCount >= 5){
		if((thisApp->RxdBuf[0] == 0x10 && thisApp->rxCount >= DLT634_5101Master_Pad[pdrv].LinkAddrSize + 4)||\
		   (thisApp->RxdBuf[0] == 0x68 && thisApp->rxCount >= thisApp->RxdBuf[1] + 6))
		{
			int8_t eStatus = FrameValidCheck(pdrv, thisApp->RxdBuf,thisApp->rxCount);
			if(eStatus == 0){//��Ч֡����
				SMasterInfo[pdrv].otsCheck = 0;
				ValidFrameResult(pdrv, thisApp->RxdBuf);
			}
			else{//֡�쳣����
				/* ������ʱ�ط� */
//				SMasterInfo[pdrv].otflag = 1;
//				SMasterInfo[pdrv].otsCheck = 1;
				
			}
		}
	}
}

/**
  *@brief  68����֡�������
  *@param  pdrv 
  *@param  pBuff �������ʼָ�룺ASDU������ַ��ʼ
  *@param  ti ��ʶ����
  *@param  ackflag(out) Ӧ��״̬
  *@retval SMasterReqErrCode ������
  */
static uint8_t SMaster68FrameDataFill(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t len = 0;
	switch(SMasterInfo[pdrv].TI){
/* + ���ӷ���Ĺ�����Ϣ */
		/* - ���� ˫�� */
		case _DLT634_5101MASTER_M_SP_NA_1:break;
		case _DLT634_5101MASTER_M_DP_NA_1:break;
		/* - ����ֵ ��һ�� ��Ȼ� �̸����� */
		case _DLT634_5101MASTER_M_ME_NA_1:break;
		case _DLT634_5101MASTER_M_ME_NB_1:break;
		case _DLT634_5101MASTER_M_ME_NC_1:break;
		/* - ��ʱ��ĵ��� ˫�� �����¼� */
		case _DLT634_5101MASTER_M_SP_TB_1:break;
		case _DLT634_5101MASTER_M_DP_TB_1:break;
		case _DLT634_5101MASTER_M_FT_NA_1:break;
		/* - �ۼ����̸��� ��ʱ���ۼ����̸��� */
		case _DLT634_5101MASTER_M_IT_NB_1:break;
		case _DLT634_5101MASTER_M_IT_TC_1:break;
/* + ���Ʒ���Ĺ�����Ϣ */	
		/* - �������� ˫������ */
		case _DLT634_5101MASTER_C_SC_NA_1:
		case _DLT634_5101MASTER_C_SC_NB_1:
			DataFill_MASTER_C_SC_NA_1(pdrv,pBuff);
			break;
/* + ���ӷ����ϵͳ���� */
		/* - ��ʼ������ */
		case _DLT634_5101MASTER_M_EI_NA_1:break;
/* + ���Ʒ����ϵͳ���� */
		/* - ���ٻ� �������ٻ� ʱ��ͬ�� �������� ��λ���� */
		case _DLT634_5101MASTER_C_IC_NA_1:
			len = DataFill_MASTER_C_IC_NA_1(pdrv,pBuff);
			break;
		case _DLT634_5101MASTER_C_CI_NA_1:break;
		case _DLT634_5101MASTER_C_CS_NA_1:break;
		case _DLT634_5101MASTER_C_TS_NA_1:break;
		case _DLT634_5101MASTER_C_RP_NA_1:
			len = DataFill_MASTER_C_RP_NA_1(pdrv,pBuff);
			break;
		/* -  �л���ֵ�� ����ֵ���� �������Ͷ�ֵ д�����Ͷ�ֵ */
		case _DLT634_5101MASTER_C_SR_NA_1:break;
		case _DLT634_5101MASTER_C_RR_NA_1:break;
		case _DLT634_5101MASTER_C_RS_NA_1:break;
		case _DLT634_5101MASTER_C_WS_NA_1:break;
		/* -  �ļ����� ������� */
		case _DLT634_5101MASTER_F_FR_NA_1:break;
		case _DLT634_5101MASTER_F_SR_NA_1:break;
	}
	return len;
}

static uint8_t FrameConrtolObtain(uint8_t pdrv,uint8_t type)
{
	uint8_t control = 0;
	/* ��������� */
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
	/* ��װ֡ */
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
	/* ��װ֡ */
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
  *@brief  ֡���Ϳ���
  *@param  pdrv 
  *@retval None
  */
static void FrameSendControl(uint8_t pdrv)
{
	/* �Ƿ��з������� */
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
	/* ���͵ȴ����� */
	FrameSendToDevice(pdrv);
	FrameReceiveInfoReset(pdrv);
}

/**
  *@brief  ������֡
  *@param  sInfo ���������Ϣ
  *@retval SMasterReqErrCode ������
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
		/* �ȴ�������� */
		eErrStatus = SMasterWaitRequestFinish();
		*(sInfo->rev->fcb) = SMasterInfo[pdrv].lastFCB;
	}
	return eErrStatus;
}

/**
  *@brief  SMasterִ��������
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
