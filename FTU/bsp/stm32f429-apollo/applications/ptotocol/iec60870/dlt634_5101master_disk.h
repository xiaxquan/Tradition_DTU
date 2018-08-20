/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_5101master_disk.h
  * @brief:     The protocol of dlt634_5101master_disk.
  * @version:   V3.01
  * @author:    Mr.J
  * @date:      2015-10-25
  * @update:    
  */

#ifndef	_DLT634_5101MASTER_DISK_H_
#define _DLT634_5101MASTER_DISK_H_

/* INCLUDE FILES -------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* PUBLIC VARIABLE -----------------------------------------------------------*/

#define MASTER101_ID0 0
typedef enum
{
    DLT634_5101MASTER_DISK0 = 0,
    DLT634_5101MASTER_VOLUMES
}DLT634_5101_MASTER_DISK;

typedef enum
{
    EV_SMASTER_READY                    = 1<<0,  /*!< Startup finished. */
    EV_SMASTER_FRAME_RECEIVED           = 1<<1,  /*!< Frame received. */
    EV_SMASTER_EXECUTE                  = 1<<2,  /*!< Execute function. */
    EV_SMASTER_FRAME_SENT               = 1<<3,  /*!< Frame sent. */
    EV_SMASTER_ERROR_PROCESS            = 1<<4,  /*!< Frame error process. */
    EV_SMASTER_PROCESS_SUCESS           = 1<<5,  /*!< Request process success. */
    EV_SMASTER_ERROR_RESPOND_TIMEOUT    = 1<<6,  /*!< Request respond timeout. */
    EV_SMASTER_ERROR_RECEIVE_DATA       = 1<<7,  /*!< Request receive data error. */
    EV_SMASTER_ERROR_EXECUTE_FUNCTION   = 1<<8,  /*!< Request execute function error. */
}SMasterEventType;

typedef enum
{
    SM_MRE_NO_ERR,                  /*!< no error. */
    SM_MRE_ILL_ARG,                 /*!< illegal argument. */
    SM_MRE_REV_DATA,                /*!< receive data error. */
    SM_MRE_TIMEDOUT,                /*!< timeout error occurred. */
    SM_MRE_MASTER_BUSY,             /*!< master is busy now. */
    SM_MRE_EXE_FUN                  /*!< execute function error. */
} SMasterReqErrCode;

typedef struct{
	uint16_t slave;		/* 从机addr */
	uint8_t *fcb;
	uint8_t ackC;		/* 应答控制域 */
	uint8_t type;		/* 请求发送的帧类型 */
	uint8_t TI;			/* 当type = F3时 有用 */
	uint16_t COT;		/* 传送原因 */
	uint16_t asduAddr;	/* ASDU addr */
	uint8_t VSQ;		/* 可变结构限定词 */
	uint8_t *pbuff;		/* 实体数据buff */
	uint8_t rlen;		/* 实体数据长度 */
}ReqRevInfo;

typedef struct{
	uint16_t slave;		/* 从机addr */
	uint16_t asduAddr;	/* ASDU addr */
	uint8_t lastFCB;	/* 发送的控制码FCB位记录 */ 
	uint8_t type;		/* 请求发送的帧类型 */
	uint8_t TI;			/* 当type = F3时 有用 */
	uint16_t COT;		/* 传送原因 */
	uint8_t VSQ;		/* 可变结构限定词 */
	uint8_t *pbuff;	/* 实体数据buff */
	uint8_t slen;		/* 实体数据长度 */
	ReqRevInfo *rev;	/* 接收回调相关 */	
}ReqSendInfo;

/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
extern uint16_t DLT634_5101_MASTER_ReadData(uint8_t pdrv, uint8_t *pbuf, uint16_t count);
extern uint16_t DLT634_5101_MASTER_WriteData(uint8_t pdrv, uint8_t *pbuf, uint16_t count);
extern void DLT634_5101_MASTER_C_SC(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_MASTER_C_SR(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_MASTER_C_CS(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_MASTER_F_FR(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_MASTER_F_SR(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_MASTER_W_YXDATA(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_MASTER_W_YCDATA(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_MASTER_W_SOE(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_MASTER_W_FEvent(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_5101_MASTER_C_REPLY(uint8_t drvid,uint8_t *pbuf);
extern uint8_t DLT634_5101_MASTER_DATA_RECE(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_MasterTask(void);
extern int DLT634_5101_MASTER_INIT(void);

void SMasterRespondTimeOut(void);
void SMasterRequestScuuess(void);
SMasterReqErrCode SMasterWaitRequestFinish(void);
uint8_t SMasterEventGet(SMasterEventType *sEvent);
uint8_t SMasterEventPost(SMasterEventType sEvent);
uint8_t SMasterEventInit(void);
uint32_t GetTimer1Tick(void);
uint32_t GetTimer1IntervalTick(uint32_t beginTick);
/* 应用数据处理 */
int8_t DataResult_M_SP_NA_1(ReqRevInfo *revInfo,uint8_t * pBuff);
int8_t DataResult_M_DP_NA_1(ReqRevInfo *revInfo,uint8_t *pBuff);
int8_t DataResult_M_ME_NC_1(ReqRevInfo *revInfo,uint8_t * pBuff);
int8_t DataResult_C_IC_NA_1(ReqRevInfo *revInfo,uint8_t * pBuff);

int8_t DataResult_C_SC_NA_1(ReqRevInfo *revInfo,uint8_t *pBuff);
int8_t DataResult_C_SC_NB_1(ReqRevInfo *revInfo,uint8_t *pBuff);

/* 帧实体填充 */
uint8_t DataFill_MASTER_C_SC_NA_1(uint8_t pdrv, uint8_t *pBuff);
uint8_t DataFill_MASTER_C_TS_NA_1(uint8_t pdrv, uint8_t *pBuff);
uint8_t DataFill_MASTER_C_RP_NA_1(uint8_t pdrv, uint8_t *pBuff);
uint8_t DataFill_MASTER_C_CS_NA_1(uint8_t pdrv, uint8_t *pBuff);
uint8_t DataFill_MASTER_C_IC_NA_1(uint8_t pdrv, uint8_t *pBuff);

#endif /* END _DLT634_5101_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/
