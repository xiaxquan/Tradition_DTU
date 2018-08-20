/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_5101master_disk.c
  * @brief:     The protocol of dlt634_5101master_disk.
  * @version:   V3.01
  * @author:    Mr.J
  * @date:      2017-05-16
  * @update:    [2018-01-22] [Lexun][make the code clean up]
  */

/* INCLUDE FILES -------------------------------------------------------------*/
#include "dlt634_5101master_disk.h"
#include "dlt634_5101master_app.h"
#include "common_data.h"
#include "drv_do.h"
#include <rtthread.h>
#include <rtdevice.h>
#include "string.h"
#include "gui_common.h"

/* PRIVATE VARIABLES ----------------------------------------------------------*/
static rt_device_t dev[_DLT634_5101MASTER_VOLUMES];
static struct rt_event SMasterOsEvent;
/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/

///**
//  *@brief 获取开始计时数
//  *@param  None
//  *@retval beginTick  计时开始数，
//  */
//uint32_t GetTimer1Tick(void)
//{
//	//return timeCount;
//	return 0;
//}

///**
//  *@brief 获取间隔周期数
//  *@param  beginTick  计时开始数，由GetTimer1Tick()函数获得
//  *@retval 间隔周期数
//  */
//uint32_t GetTimer1IntervalTick(uint32_t beginTick)
//{
////	uint32_t temptick;
////	temptick = GetTimer1Tick();
////	if(beginTick <= temptick){
////		return (temptick - beginTick);
////	}
////	else{
////		return ((0xFFFFFFFF - beginTick)+temptick);
////	}
//	return 0;
//}

/**
  *@brief  获取系统时间
  *@param  pTim 时间接收指针
  *@retval None
  */
static void GetCurrentTim(PackCP56Tim2a *pTim)
{
	pTim->DayofWeekMonth = 10;
	pTim->Hours = 4;
	pTim->Milliseconds_H = 3;
	pTim->Milliseconds_L = 2;
	pTim->Minutes = 5;
	pTim->Months = 8;
	pTim->Years = 18;
}

/**
  *@brief  主机事件初始化
  *@param  None
  *@retval 0 ok
  */
uint8_t SMasterEventInit(void)
{
	rt_event_init(&SMasterOsEvent,"smaster event",RT_IPC_FLAG_PRIO);
	return 0;
}

/**
  *@brief  主机事件发送
  *@param  sEvent 事件
  *@retval 0 ok
  */
uint8_t SMasterEventPost(SMasterEventType sEvent)
{
	rt_event_send(&SMasterOsEvent, sEvent);
	return 0;
}

/**
  *@brief  主机事件接收
  *@param  sEvent 事件
  *@retval 0 ok
  */
uint8_t SMasterEventGet(SMasterEventType *eEvent)
{
	rt_uint32_t recvedEvent;
	rt_event_recv(&SMasterOsEvent,
            EV_SMASTER_READY | EV_SMASTER_FRAME_RECEIVED | EV_SMASTER_EXECUTE |
            EV_SMASTER_FRAME_SENT | EV_SMASTER_ERROR_PROCESS,
            RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER,
            &recvedEvent);
	/* the enum type couldn't convert to int type */
    switch (recvedEvent)
    {
    case EV_SMASTER_READY:
        *eEvent = EV_SMASTER_READY; break;
    case EV_SMASTER_FRAME_RECEIVED:
        *eEvent = EV_SMASTER_FRAME_RECEIVED;break;
    case EV_SMASTER_EXECUTE:
        *eEvent = EV_SMASTER_EXECUTE;break;
    case EV_SMASTER_FRAME_SENT:
        *eEvent = EV_SMASTER_FRAME_SENT;break;
    case EV_SMASTER_ERROR_PROCESS:
        *eEvent = EV_SMASTER_ERROR_PROCESS;break;
    }
    return 0;
}

/**
  *@brief  主机等待请求完成
  *@param  None
  *@retval SMasterReqErrCode错误码
  */
SMasterReqErrCode SMasterWaitRequestFinish(void)
{
	SMasterReqErrCode eErrStatus = SM_MRE_NO_ERR;
    rt_uint32_t recvedEvent;
    /* waiting for OS event */
    rt_event_recv(&SMasterOsEvent,
            EV_SMASTER_PROCESS_SUCESS | EV_SMASTER_ERROR_RESPOND_TIMEOUT
                    | EV_SMASTER_ERROR_RECEIVE_DATA
                    | EV_SMASTER_ERROR_EXECUTE_FUNCTION,
            RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER,
            &recvedEvent);
	switch (recvedEvent)
    {
		case EV_SMASTER_PROCESS_SUCESS:break;
		case EV_SMASTER_ERROR_RESPOND_TIMEOUT:
			eErrStatus = SM_MRE_TIMEDOUT;
			break;
		case EV_SMASTER_ERROR_RECEIVE_DATA:
			eErrStatus = SM_MRE_REV_DATA;
			break;
		case EV_SMASTER_ERROR_EXECUTE_FUNCTION:
			eErrStatus = SM_MRE_EXE_FUN;
			break;
    }
    return eErrStatus;
}

/**
  *@brief  主机请求成功
  *@param  None
  *@retval None
  */
void SMasterRequestScuuess(void) 
{
    rt_event_send(&SMasterOsEvent, EV_SMASTER_PROCESS_SUCESS);
}

/**
  *@brief  主机响应超时事件发送
  *@param  None
  *@retval None
  */
void SMasterRespondTimeOut(void) 
{
    rt_event_send(&SMasterOsEvent, EV_SMASTER_ERROR_RESPOND_TIMEOUT);
}

/**
  * @brief  read data function.
  * @param  [pdrv]  equipment serial number
  * @param  [pbuf]  storage array point
  * @param  [count] read number
  * @retval return length of reading data
  * @author Mr.J
  * @date   2017.8.3
  * @update 
  */
uint16_t DLT634_5101_MASTER_ReadData(uint8_t pdrv, uint8_t *pbuf, uint16_t count)
{
	uint16_t len = 0;  
	
	switch(pdrv)
	{
		case DLT634_5101MASTER_DISK0:
            len = rt_device_read(dev[pdrv],0,pbuf,count);
			break;
		default:
			break;
	}
	return(len);
}

/**
  * @brief  write data function.
  * @param  [pdrv]  equipment serial number
  * @param  [pbuf]  storage array point
  * @param  [count] write number
  * @retval return length of write data
  * @author Mr.J
  * @date   2017.8.3
  * @update 
  */
uint16_t DLT634_5101_MASTER_WriteData(uint8_t pdrv, uint8_t *pbuf, uint16_t count)
{
	switch(pdrv)
	{
		case DLT634_5101MASTER_DISK0:
            rt_device_write(dev[pdrv],0,pbuf,count); 
			break;
		default:
			break;
	}
	return(count);
}

/**
  * @brief  yk function.
  * @param  [pdrv]  equipment serial number
  * @param  [pbuf]  storage array point
  * @retval 
  * @author Mr.J
  * @date   2017.8.3
  * @update 
  */
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/SCO_L/SCO_H
void DLT634_5101_MASTER_C_SC(uint8_t pdrv, uint8_t *pbuf)//遥控
{
	switch(pdrv)
	{
		case DLT634_5101MASTER_DISK0:			
			break;
		
		default:
			break;
	}
}

/**
  * @brief  time synchronization function.
  * @param  [pdrv]  equipment serial number
  * @param  [pbuf]  storage array point
  * @retval 
  * @author Mr.J
  * @date   2017.8.3
  * @update 
  */
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/CP56Time2a
void DLT634_5101_MASTER_C_CS(uint8_t pdrv, uint8_t *pbuf)//对时
{
    struct CP56Time2a_t time;
    uint8_t num = 0;
    
    DBReadSystemTime(&time);
    
	switch(pdrv)
	{
		case DLT634_5101MASTER_DISK0:
			pbuf[num++] = 0x11;//LENTH
			pbuf[num++] = 0x00;
			pbuf[num++] = 0x67;
			pbuf[num++] = 0x01;
			pbuf[num++] = 0x06;
			pbuf[num++] = 0x00;
			pbuf[num++] = 0x01;
			pbuf[num++] = 0x00;
			pbuf[num++] = 0x00;
			pbuf[num++] = 0x00;
            //CP56Time2a
            memcpy(&pbuf[num],(uint8_t *)&time,sizeof(struct CP56Time2a_t));
			break;
		default:
			break;
	}
}

/**
  * @brief  parameter function.
  * @param  [pdrv]  equipment serial number
  * @param  [pbuf]  storage array point
  * @retval 
  * @author Mr.J
  * @date   2017.8.3
  * @update 
  */
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/SN_L/SN_H
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/SN1_L/SN1_H/SN2_L/SN2_H/SN3_L/SN3_H
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/SN1_L/SN1_H/Array
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/SN_L/SN_H/PI_L/PI_H//Array(InfoAddr_L/InfoAddr_H/Tag/Len/Value)
void DLT634_5101_MASTER_C_SR(uint8_t pdrv, uint8_t *pbuf)//定值参数
{ 
	switch(pdrv)
	{
		case DLT634_5101MASTER_DISK0:
			
			break;
		
		default:
			break;
	}
}

/**
  * @brief  file read&write function.
  * @param  [pdrv]  equipment serial number
  * @param  [pbuf]  storage array point
  * @retval 
  * @author Mr.J
  * @date   2017.8.3
  * @update 
  */
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/sty/Array
void DLT634_5101_MASTER_F_FR(uint8_t pdrv, uint8_t *pbuf)//文件读写
{ 
//	switch(pdrv)
//	{
//		case 0:
//			
//			break;
//		
//		default:
//			break;
//	}	
}

/**
  * @brief  software update function.
  * @param  [pdrv]  equipment serial number
  * @param  [pbuf]  storage array point
  * @retval 
  * @author Mr.J
  * @date   2017.8.3
  * @update 
  */
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/CTYPE
void DLT634_5101_MASTER_F_SR(uint8_t pdrv, uint8_t *pbuf)//软件升级
{ 
	switch(pdrv)
	{
		case DLT634_5101MASTER_DISK0:
			break;
		default:
			break;
	}
}

/**
  * @brief  reply of other function.
  * @param  [drvid]  application serial number
  * @param  [pbuf]  storage array point
  * @retval 
  * @author Mr.J
  * @date   2017.8.3
  * @update 
  */
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/
uint8_t DLT634_5101_MASTER_C_REPLY(uint8_t drvid,uint8_t *pbuf)//其他设备回复
{
	uint8_t res;
	
	return(res);
}

/**
  * @brief  receive data processing function.
  * @param  [pdrv]  equipment serial number
  * @param  [pbuf]  storage array point
  * @retval 
  * @author Chen
  * @date   2018.1.19
  * @update 
  */
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/
uint8_t DLT634_5101_MASTER_DATA_RECE(uint8_t pdrv, uint8_t *pbuf)
{
	uint8_t type_id;
	type_id = pbuf[2];//TypeID
	switch(type_id)
	{
		
		default:
			break;
	}
	return 0;
}

/**
  * @brief  parameter init function.
  * @param  [pdrv]  equipment serial number
  * @param  [pbuf]  storage array point
  * @retval 
  * @author Mr.J
  * @date   2017.8.3
  * @update 
  */
int DLT634_5101_MASTER_INIT(void)
{
	uint8_t pdrv=0;
	struct rt_serial_device *serial;
	
	DLT634_5101Master_Pad[pdrv].Port = MASTER101_ID0;          
	dev[pdrv] = rt_device_find(RT_USART6_NAME);
	serial = (struct rt_serial_device *)(dev[pdrv]);
	serial->config.baud_rate = BAUD_RATE_9600;
	serial->config.parity = PARITY_NONE;
	serial->config.data_bits = DATA_BITS_8;
	rt_device_open(dev[pdrv], RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);

	DLT634_5101Master_Pad[pdrv].BalanMode = 0;	//非平衡
	DLT634_5101Master_Pad[pdrv].LinkAddrSize = 2;//1:97,2:02
	DLT634_5101Master_Pad[pdrv].ASDUCotSize = 2;//1:97,2:02
	DLT634_5101Master_Pad[pdrv].ASDUAddr = 1;
	DLT634_5101Master_Pad[pdrv].ASDUAddrSize = 2;//1:97,2:02
	DLT634_5101Master_Pad[pdrv].TimeOutValue = 2000;//ms
	/* 链路状态空闲 */
	SMasterInfo[pdrv].linkStatus = LINK_IDLE;
	SMasterInfo[pdrv].lastFCB = 1;
	SMasterInfo[pdrv].otsCheck = 0;
	SMasterInfo[pdrv].reSend = 0;
	SMasterInfo[pdrv].slave = 0;
	SMasterInfo[pdrv].slen = 0;
	SMasterEventInit();	
	time_static_init();
	
	return (RT_EOK);
}


/**
  * @brief : task.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5101_MasterTask(void)
{
    static uint8_t disk0 = DLT634_5101MASTER_DISK0;
    
    SMaster101Main(disk0);
  
}
/* 数据处理 */

/**
  *@brief  单点信息处理
  *@param  slave 从机地址
  *@param  vsq 可变结构限定词
  *@param  pBuff ASDU公共地址开始的数据
  *@retval 0 ok
  */
int8_t DataResult_M_SP_NA_1(ReqRevInfo *revInfo,uint8_t *pBuff)
{
	DpuSlaveInfo *sInfo = GetSlaveInfoUseAddr(revInfo->slave);
	uint16_t addrOffset = 0x0001;
	
	struct COS_Str yxValue;
	uint8_t vsq;
	if(vsq < 128){//SQ = 0
		for(uint8_t i = 0; i < vsq; i += 3){
			yxValue.addr = (pBuff[i] + (pBuff[i + 1] << 8)) + sInfo->yxBaddr - addrOffset;
			if(yxValue.addr > sInfo->yxEaddr){
				continue;
			}
			yxValue.value = (pBuff[i + 2] + 1)&0x03;
			DBupdate_YX(&yxValue);
		}
	}
	else{
		uint8_t tNum = vsq - 128;
		uint8_t tAddr = (pBuff[0] + (pBuff[1] << 8)) + sInfo->yxBaddr - addrOffset;
		pBuff += 2;
		for(uint8_t i = 0; i < tNum; i ++){
			yxValue.addr = tAddr + i;
			if(yxValue.addr > sInfo->yxEaddr){
				break;
			}
			yxValue.value = (pBuff[i] + 1)&0x03;
			DBupdate_YX(&yxValue);
		}
	}
	return 0;
}

/**
  *@brief  双点信息处理
  *@param  slave 从机地址
  *@param  vsq 可变结构限定词
  *@param  pBuff ASDU公共地址开始的数据
  *@retval 0 ok
  */
int8_t DataResult_M_DP_NA_1(ReqRevInfo *revInfo,uint8_t *pBuff)
{
	DpuSlaveInfo *sInfo = GetSlaveInfoUseAddr(revInfo->slave);
	uint16_t addrOffset = 0x0001;
	
	struct COS_Str yxValue;
	uint8_t vsq;
	if(vsq < 128){//SQ = 0
		for(uint8_t i = 0; i < vsq; i += 3){
			yxValue.addr = (pBuff[i] + (pBuff[i + 1] << 8)) + sInfo->yxBaddr - addrOffset;
			if(yxValue.addr > sInfo->yxEaddr){
				continue;
			}
			yxValue.value = (pBuff[i + 2] + 1)&0x03;
			DBupdate_YX(&yxValue);
		}
	}
	else{
		uint8_t tNum = vsq - 128;
		uint8_t tAddr = (pBuff[0] + (pBuff[1] << 8)) + sInfo->yxBaddr - addrOffset;
		pBuff += 2;
		for(uint8_t i = 0; i < tNum; i ++){
			yxValue.addr = tAddr + i;
			if(yxValue.addr > sInfo->yxEaddr){
				break;
			}
			yxValue.value = (pBuff[i])&0x03;
			DBupdate_YX(&yxValue);
		}
	}
	return 0;
}
/**
  *@brief  遥测浮点数处理
  *@param  slave 从机地址
  *@param  vsq 可变结构限定词
  *@param  pBuff ASDU公共地址开始的数据
  *@retval 0 ok
  */
int8_t DataResult_M_ME_NC_1(ReqRevInfo *revInfo,uint8_t * pBuff)
{
	DpuSlaveInfo *sInfo = GetSlaveInfoUseAddr(revInfo->slave);
	uint16_t addrOffset = 0x4001;
	
	struct NVA_Str ycValue;
	uint8_t vsq;
	union{
		float f;
		uint8_t u8[4];
	}fU8;
	
	if(vsq < 128){//SQ = 0
		for(uint8_t i = 0; i < vsq; i += 7){
			ycValue.addr = (pBuff[i] + (pBuff[i + 1] << 8)) + sInfo->ycBaddr - addrOffset;
			if(ycValue.addr > sInfo->ycEaddr){
				continue;
			}		
			fU8.u8[0] = pBuff[i + 2];
			fU8.u8[1] = pBuff[i + 3];
			fU8.u8[2] = pBuff[i + 4];
			fU8.u8[3] = pBuff[i + 5];
			ycValue.value = fU8.f;
			DBupdate_YC(&ycValue);
		}
	}
	else{
		uint8_t tNum = vsq - 128;
		uint8_t tAddr = (pBuff[0] + (pBuff[1] << 8)) + sInfo->ycBaddr - addrOffset;
		pBuff += 2;
		for(uint8_t i = 0; i < tNum; i += 5){
			ycValue.addr = tAddr + i;
			if(ycValue.addr > sInfo->ycEaddr){
				break;
			}
			fU8.u8[0] = pBuff[i];
			fU8.u8[1] = pBuff[i + 1];
			fU8.u8[2] = pBuff[i + 2];
			fU8.u8[3] = pBuff[i + 3];
			ycValue.value = fU8.f;
			DBupdate_YC(&ycValue);
		}
	}
	return 0;
}
/**
  *@brief  总召唤处理
  *@param  slave 从机地址
  *@param  vsq 可变结构限定词
  *@param  pBuff ASDU公共地址开始的数据
  *@retval 0 ok
  */
int8_t DataResult_C_IC_NA_1(ReqRevInfo *revInfo,uint8_t * pBuff)
{
	return 0;
}

/**
  *@brief  单点命令处理
  *@param  slave 从机地址
  *@param  vsq 可变结构限定词
  *@param  pBuff ASDU公共地址开始的数据
  *@retval 0 ok
  */
int8_t DataResult_C_SC_NA_1(ReqRevInfo *revInfo,uint8_t *pBuff)
{
	uint8_t pSendBuff[12];
	DpuSlaveInfo *sInfo = GetSlaveInfoUseAddr(revInfo->slave);
	pSendBuff[0] = 11;
	pSendBuff[1] = 0;
	pSendBuff[2] = revInfo->TI;
	pSendBuff[3] = revInfo->VSQ;
	pSendBuff[4] = (uint8_t)revInfo->COT;
	pSendBuff[5] = (uint8_t)(revInfo->COT >> 8);
	pSendBuff[6] = (uint8_t)revInfo->asduAddr;
	pSendBuff[7] = (uint8_t)(revInfo->asduAddr >> 8);
	
	uint16_t ykAddr = pBuff[0] + (pBuff[1]<<8);
	if(ykAddr + sInfo->ykBaddr >= sInfo->ykEaddr){
		return  0;
	}
	ykAddr += sInfo->ykBaddr;
	
	pSendBuff[8] = pBuff[0];
	pSendBuff[9] = pBuff[1];
	pSendBuff[10] = pBuff[2];
	DBSend[sInfo->sourceId](sInfo->sourceId,pSendBuff);
	return 0;
}

/**
  *@brief  双点命令处理
  *@param  slave 从机地址
  *@param  vsq 可变结构限定词
  *@param  pBuff ASDU公共地址后的数据
  *@retval 0 ok
  */
int8_t DataResult_C_SC_NB_1(ReqRevInfo *revInfo,uint8_t *pBuff)
{
	DataResult_C_SC_NA_1(revInfo,pBuff);
	return 0;
}

/* -----------------数据填充 -------------- */

/**
  * @brief  总召唤数据填充.
  * @param  pdrv
  * @retval None
  */
uint8_t DataFill_MASTER_C_IC_NA_1(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = 0;
	
	pBuff[offset++] = _DLT634_5101MASTER_C_IC_NA_1;//TI
	pBuff[offset++] = 1;//VSQ
	pBuff[offset++] = (_DLT634_5101MASTER_COT_ACT & 0xFF);//COTL
	if(DLT634_5101Master_Pad[pdrv].ASDUCotSize == 2){
		pBuff[offset++] = (uint8_t)(_DLT634_5101MASTER_COT_ACT >> 8);//COTH
	}
	pBuff[offset++] = (DLT634_5101Master_Pad[pdrv].ASDUAddr & 0xFF);//ASDU_addrL
	if(DLT634_5101Master_Pad[pdrv].ASDUAddrSize == 2){
		pBuff[offset++] = (uint8_t)(DLT634_5101Master_Pad[pdrv].ASDUAddr >> 8);//ASDU_addrH
	}
	pBuff[offset++] = 0;//信息对象地址L
	pBuff[offset++] = 0;//信息对象地址H
	pBuff[offset++] = 20;//QOI
	
	return offset;
}

/**
  * @brief  时钟同步数据填充.
  * @param  pdrv
  * @retval None
  */
uint8_t DataFill_MASTER_C_CS_NA_1(uint8_t pdrv, uint8_t *pBuff)
{
	(void)pdrv;
	
	uint8_t offset = 0;
	
	pBuff[offset++] = _DLT634_5101MASTER_C_CS_NA_1;//TI
	pBuff[offset++] = 1;//VSQ
	pBuff[offset++] = (_DLT634_5101MASTER_COT_ACT & 0xFF);//COTL
	if(DLT634_5101Master_Pad[pdrv].ASDUCotSize == 2){
		pBuff[offset++] = (uint8_t)(_DLT634_5101MASTER_COT_ACT >> 8);//COTH
	}
	pBuff[offset++] = (DLT634_5101Master_Pad[pdrv].ASDUAddr & 0xFF);//ASDU_addrL
	if(DLT634_5101Master_Pad[pdrv].ASDUAddrSize == 2){
		pBuff[offset++] = (uint8_t)(DLT634_5101Master_Pad[pdrv].ASDUAddr >> 8);//ASDU_addrH
	}
	pBuff[offset++] = 0;//信息对象地址L
	pBuff[offset++] = 0;//信息对象地址H
	PackCP56Tim2a *ptim = (PackCP56Tim2a *)&pBuff[offset];
	offset += sizeof(PackCP56Tim2a);
	GetCurrentTim(ptim);
	
	return offset;
}
/**
  * @brief  复位进程数据填充.
  * @param  pdrv
  * @param  pBuff 数据填充区
  * @retval 填充长度
  */
uint8_t DataFill_MASTER_C_RP_NA_1(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = 0;
	
	pBuff[offset++] = _DLT634_5101MASTER_C_RP_NA_1;//TI
	pBuff[offset++] = 1;//VSQ
	pBuff[offset++] = (_DLT634_5101MASTER_COT_ACT & 0xFF);//COTL
	if(DLT634_5101Master_Pad[pdrv].ASDUCotSize == 2){
		pBuff[offset++] = (uint8_t)(_DLT634_5101MASTER_COT_ACT >> 8);//COTH
	}
	pBuff[offset++] = (DLT634_5101Master_Pad[pdrv].ASDUAddr & 0xFF);//ASDU_addrL
	if(DLT634_5101Master_Pad[pdrv].ASDUAddrSize == 2){
		pBuff[offset++] = (uint8_t)(DLT634_5101Master_Pad[pdrv].ASDUAddr >> 8);//ASDU_addrH
	}
	pBuff[offset++] = 0;//信息对象地址L
	pBuff[offset++] = 0;//信息对象地址H
	pBuff[offset++] = 1;//QOI
	
	return offset;
}

/**
  * @brief  测试命令数据填充.
  * @param  pdrv
  * @param  pBuff 数据填充区
  * @retval 填充长度
  */
uint8_t DataFill_MASTER_C_TS_NA_1(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = 0;
	
	pBuff[offset++] = _DLT634_5101MASTER_C_TS_NA_1;//TI
	pBuff[offset++] = 1;//VSQ
	pBuff[offset++] = (_DLT634_5101MASTER_COT_ACT & 0xFF);//COTL
	if(DLT634_5101Master_Pad[pdrv].ASDUCotSize == 2){
		pBuff[offset++] = (uint8_t)(_DLT634_5101MASTER_COT_ACT >> 8);//COTH
	}
	pBuff[offset++] = (DLT634_5101Master_Pad[pdrv].ASDUAddr & 0xFF);//ASDU_addrL
	if(DLT634_5101Master_Pad[pdrv].ASDUAddrSize == 2){
		pBuff[offset++] = (uint8_t)(DLT634_5101Master_Pad[pdrv].ASDUAddr >> 8);//ASDU_addrH
	}
	pBuff[offset++] = 0;//信息对象地址L
	pBuff[offset++] = 0;//信息对象地址H
	pBuff[offset++] = 0x55;//FBPL
	pBuff[offset++] = 0xAA;//FBPH
	
	return offset;
}

/**
  * @brief  遥控单命令数据填充.
  * @param  pdrv
  * @param  pBuff 数据填充区
  * @retval 填充长度
  */
uint8_t DataFill_MASTER_C_SC_NA_1(uint8_t pdrv, uint8_t *pBuff)
{
	uint8_t offset = 0;

	pBuff[offset++] = SMasterInfo[pdrv].TI;//TI
	pBuff[offset++] = 1;//VSQ
	pBuff[offset++] = (SMasterInfo[pdrv].COT & 0xFF);//COTL
	if(DLT634_5101Master_Pad[pdrv].ASDUCotSize == 2){
		pBuff[offset++] = (uint8_t)(SMasterInfo[pdrv].COT >> 8);//COTH
	}
	pBuff[offset++] = (SMasterInfo[pdrv].asduAddr & 0xFF);//ASDU_addrL
	if(DLT634_5101Master_Pad[pdrv].ASDUAddrSize == 2){
		pBuff[offset++] = (uint8_t)(SMasterInfo[pdrv].asduAddr >> 8);//ASDU_addrH
	}
	pBuff[offset++] = SMasterInfo[pdrv].pbuff[0];//信息对象地址L
	pBuff[offset++] = SMasterInfo[pdrv].pbuff[1];//信息对象地址H
	pBuff[offset++] = SMasterInfo[pdrv].pbuff[2];//SCO
	
	return offset;
}

/* END */

