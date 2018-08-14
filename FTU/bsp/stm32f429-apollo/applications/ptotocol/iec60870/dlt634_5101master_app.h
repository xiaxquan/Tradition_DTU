/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_5101master_app.h
  * @brief:     The protocol of dlt634_5101master_app.
  * @version:   V3.01
  * @author:    Mr.Lee
  * @date:      2015-10-25
  * @update:    
  */

#ifndef	_DLT634_5101MASTER_APP_H_
#define _DLT634_5101MASTER_APP_H_

/* INCLUDE FILES -------------------------------------------------------------*/
#include "integer.h"
#include "stdint.h"

#include "dlt634_5101master_config.h"
#include "sMasterDpuControl.h"
#include "dlt634_5101master_disk.h"

#ifndef  FALSE
#define  FALSE     0
#define  TRUE      !FALSE
#endif

#define DebugPrintf(format,...) printf(format,##__VA_ARGS__)

/* PUBLIC VARIABLE -----------------------------------------------------------*/
__packed typedef struct 
{
		uint8_t Milliseconds_L;   // D7-D0
		uint8_t Milliseconds_H;   // D7-D0
		uint8_t Minutes;					// D5-D0
		uint8_t Hours;						// D4-D0
		uint8_t DayofWeekMonth;		// WEEK(D7-D5)MONTH(D4-D0)
		uint8_t Months;						// D3-D0
		uint8_t Years;						// D6-D0
}_DLT634_5101MASTER_CP56Time2a_t;

__packed typedef struct 
{
		uint8_t Milliseconds_L;// D7-D0
		uint8_t Milliseconds_H;// D7-D0
		uint8_t Minutes;// D5-D0
		uint8_t Hours;// D4-D0
		uint8_t DayofWeekMonth;// WEEK(D7-D5)MONTH(D4-D0)
		uint8_t Months;// D3-D0
		uint8_t Years;// D6-D0
}PackCP56Tim2a;

__packed typedef struct 
{
	   __packed struct STATUS1
		{
		    uint8_t Length;
				__packed union
				{
					uint8_t priority;	
					uint8_t Lock_ID;
				}symbol;
		}status1;
		
    __packed struct HEAD1
		{
		    uint8_t TypeID;     
		    uint8_t VSQ;  
				uint8_t COT_L;
				uint8_t COT_H;
				uint8_t PubAddr_L;
				uint8_t PubAddr_H;
		}Head1;
		
		__packed union
		{
   		 uint8_t	buff[256-sizeof(struct STATUS1)-sizeof(struct HEAD1)];

   		 __packed  struct
			{
					uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;
			    uint8_t DatPackage_Type; //数据包类型
          uint8_t OPt_Identify;//操作标识
				  uint32_t Dir_ID;//文件ID
				  uint8_t DirName_Length;//文件名长度
				  uint8_t	buff[256];
			}C_210_READ_FILEDIR;	//读文件目录
			
//				  uint8_t	DirName_buff[256];
//				  uint8_t CallTag;//召唤标识
//				  uint8_t	Query_StartAddr[7];//查询起始地址
//				  uint8_t	Query_OverAddr[7];//查询结束地址
//				
//			}C_210_READ_FILEDIR;	//读文件目录

			
   		 __packed  struct
			{
					uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;
			    uint8_t DatPackage_Type; //数据包类型
          uint8_t OPt_Identify;//操作标识
				  uint8_t FileName_Length;//文件名长度
				  uint8_t	FileName_buff[256];
			}C_210_READFILE_ACTIVE;	//读文件激活	

   		 __packed  struct
			{
					uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;
			    uint8_t DatPackage_Type; //数据包类型
          uint8_t Opt_Identify;//操作标识
				  uint32_t File_ID;//文件ID
				  uint32_t DatSection_ID;
				  uint8_t  Result_Description;//结果描述符
			}C_210_READFILE_TRANSURE;//读文件数据传输确认

			
   		 __packed  struct
			{
					uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;
			    uint8_t QOI; 
			}C_100;
		__packed  struct
			{
					uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;
			    uint8_t QOI; 
			}C_101;
			
			 __packed struct
			{
			    uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;	
			    _DLT634_5101MASTER_CP56Time2a_t CP56Time2a;
			}C_103;
			
			 __packed struct
			{
			    uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;	
			    uint8_t QRP; 
			}C_105;
			
			 __packed struct
			{
			    uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;	
			    uint8_t COI; 
			}C_70;
			
			 __packed struct
			{
			    uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;
					uint8_t FBP_L;
					uint8_t FBP_H;				
			}C_104;
			
			__packed union
			{
					 __packed struct
					{
							 __packed struct
							{
						    uint8_t InfoAddr_L;
								uint8_t InfoAddr_H;	
						    uint8_t Value; 
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1))/3];
			  	}SQ0;
			  	
					 __packed struct
					{
							uint8_t InfoAddr_L;
							uint8_t InfoAddr_H;
							 __packed struct
							{
						    uint8_t Value; 
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1)-2)/1];
					}SQ1;
		  }C_1;
		
			__packed union
			{
					 __packed struct
					{
							 __packed struct
							{
						    uint8_t InfoAddr_L;
								uint8_t InfoAddr_H;	
						    uint8_t Value; 
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1))/3];
			  	}SQ0;
			  	
					 __packed struct
					{
						uint8_t InfoAddr_L;
						uint8_t InfoAddr_H;	
						 __packed struct
						{
					    uint8_t Value; 
				  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1)-2)/1];
					}SQ1;
		  }C_3;
		  
		  __packed union
			{
					 __packed struct
					{
							 __packed struct
							{
							    uint8_t InfoAddr_L;
									uint8_t InfoAddr_H;
							    uint8_t Value; 
							    _DLT634_5101MASTER_CP56Time2a_t CP56Time2a;
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1))/10];
			  	}SQ0;
			  	
					 __packed struct
					{
							uint8_t InfoAddr_L;
							uint8_t InfoAddr_H;	
							 __packed struct
							{
							    uint8_t Value; 
					    	  _DLT634_5101MASTER_CP56Time2a_t CP56Time2a;
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1)-2)/8];
					}SQ1;
		  }C_30;
		  
		  __packed union
			{
					 __packed struct
					{
							 __packed struct
							{
							    uint8_t InfoAddr_L;
									uint8_t InfoAddr_H;	
							    uint8_t Value; 
									_DLT634_5101MASTER_CP56Time2a_t CP56Time2a;
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1))/10];
			  	}SQ0;
			  	
					 __packed struct
					{
							uint8_t InfoAddr_L;
							uint8_t InfoAddr_H;	
							 __packed struct
							{
							    uint8_t Value; 
									_DLT634_5101MASTER_CP56Time2a_t CP56Time2a;
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1)-2)/8];
					}SQ1;
		  }C_31;
		  
		  __packed union
			{
					 __packed struct
					{
							 __packed struct
							{
							    uint8_t InfoAddr_L;
									uint8_t InfoAddr_H;
									uint64_t Value;
									uint8_t QDS;
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1))/7];
			  	}SQ0;
			  	
			  	 __packed struct
					{
							uint8_t InfoAddr_L;
							uint8_t InfoAddr_H;	
							 __packed struct
							{
									uint64_t Value;									
									uint8_t QDS;
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1)-2)/5];
			  	}SQ1;
			  	
		  }C_9;
		  
		  __packed union
			{
					 __packed struct
					{
							 __packed struct
							{
							    uint8_t InfoAddr_L;
									uint8_t InfoAddr_H;
									uint64_t Value;
									uint8_t QDS;
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1))/7];
			  	}SQ0;
			  	
			  	 __packed struct
					{
							uint8_t InfoAddr_L;
							uint8_t InfoAddr_H;
							 __packed struct
							{
									uint64_t Value_L;
									uint8_t QDS;
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1)-2)/5];
			  	}SQ1;
			  	
		  }C_11;
		  
		  __packed union
			{
					 __packed struct
					{
							 __packed struct
							{
							    uint8_t InfoAddr_L;
									uint8_t InfoAddr_H;
									uint64_t Value;	
									uint8_t QDS;
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1))/7];
			  	}SQ0;
			  	
			  	 __packed struct
					{
							uint8_t InfoAddr_L;
							uint8_t InfoAddr_H;
							 __packed struct
							{
							    uint64_t Value;	
									uint8_t QDS;
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1)-2)/5];
			  	}SQ1;
		  }C_13;
		  
		   __packed struct
			{
					uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;
					uint8_t SCO_L;
					uint8_t SCO_H;
			}C_45;	

			 __packed struct
			{
					uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;	
					uint8_t DCO_L;
					uint8_t DCO_H;
			}C_46;	
			
			  __packed struct
			{
					uint8_t Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1))]; 
		  }C_42;
		  
		    __packed struct
			{
					uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;	
					uint8_t SN_L;
					uint8_t SN_H;
		  }C_200;
		  
		  __packed union
			{
					 __packed struct
					{
							uint8_t InfoAddr_L;
							uint8_t InfoAddr_H;
			  	}Down;
			  	
			  	 __packed struct
					{
							uint8_t InfoAddr_L;
							uint8_t InfoAddr_H;	
							uint8_t SN1_L;
							uint8_t SN1_H;
							uint8_t SN2_L;
							uint8_t SN2_H;
							uint8_t SN3_L;
							uint8_t SN3_H;
			  	}Up;
		  }C_201;
		  
		  __packed union
			{
					 __packed struct
					{
							uint8_t SN1_L;
							uint8_t SN1_H;
							uint64_t Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1)-2)/4];		
			  	}Down;
			  	
			  	 __packed struct
					{
							uint8_t SN_L;
							uint8_t SN_H;	
							uint8_t PI_L;
							uint8_t PI_H;
							 __packed struct
							{
									uint8_t InfoAddr_L;
									uint8_t InfoAddr_H;
									uint8_t Tag;
									uint8_t Len;
									uint64_t Value;	
					  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1)-4)/8];
			  	}Up;
		  }C_202;
		  
		  __packed union
			{
					uint8_t SN_L;
					uint8_t SN_H;	
					uint8_t PI_L;
					uint8_t PI_H;
					 __packed struct
					{
							uint8_t InfoAddr_L;
							uint8_t InfoAddr_H;
							uint8_t Tag;
							uint8_t Len;
							uint64_t Value;	
			  	}Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1)-4)/8];
		  }C_203;
			
//			__packed struct
//			{
//					uint8_t Array[(256-sizeof(struct STATUS1)-sizeof(struct HEAD1)-4)/1];
//		  }C_210;
    }Data;
}DLT634_5101MASTER_PASDU;

/* 101主站状态枚举 */
enum LinkStatusType{
	LINK_IDLE,/* 空闲 */
	LINK_REQUESTSEND,/* 请求发送 */
	LINK_SEND,/* 处理发送 */
	LINK_WAITACK,/* 等待接收应答 */
	LINK_ACKOK,
};

typedef struct{
	uint16_t slave;		/* 从机addr */
	uint8_t *fcb;
	uint8_t ackC;		/* 应答控制域 */
	uint8_t type;		/* 请求发送的帧类型 */
	uint8_t TI;			/* 当type = F3时 有用 */
	uint16_t COT;		/* 传送原因 */
	//uint16_t asduAddr;	/* ASDU addr */
	uint8_t VSQ;		/* 可变结构限定词 */
	uint8_t *pbuff;		/* 实体数据buff */
	uint8_t rlen;		/* 实体数据长度 */
}ReqRevInfo;

typedef struct{
	uint16_t slave;		/* 从机addr */
	//uint16_t asduAddr;	/* ASDU addr */
	uint8_t lastFCB;	/* 发送的控制码FCB位记录 */ 
	uint8_t type;		/* 请求发送的帧类型 */
	uint8_t TI;			/* 当type = F3时 有用 */
	uint16_t COT;		/* 传送原因 */
	uint8_t VSQ;		/* 可变结构限定词 */
	uint8_t *pbuff;	/* 实体数据buff */
	uint8_t slen;		/* 实体数据长度 */
	ReqRevInfo *rev;	/* 接收回调相关 */	
}ReqSendInfo;

typedef struct 
{
	uint16_t slave;		/* 从机addr */
	enum LinkStatusType linkStatus;/* 链路当前的状态 */
	uint8_t lastFCB;	/* 发送的控制码FCB位记录 */ 
	uint8_t type;		/* 请求发送的帧类型 */
	uint8_t TI;			/* 当type = F3时 有用 */
	uint16_t COT;		/* 传送原因 */
	uint8_t VSQ;		/* 可变结构限定词 */
	uint8_t *pbuff;		/* 实体数据buff */
	uint8_t slen;		/* 实体数据长度 */
	ReqRevInfo *rev;	/* 接收回调相关 */	
	
	uint8_t otsCheck;	/* 启动超时检测 */
	uint8_t otflag;		/* 超时 */
	uint8_t reSend;		/* 重发计数 */
	uint32_t otTick;	/* 超时计数器 */
	
	/* 链路层发送接收 */
	uint8_t TxdBuf[_DLT634_5101MASTER_LPDUSIZE];
	uint8_t RxdBuf[_DLT634_5101MASTER_LPDUSIZE];
	uint8_t rxCount;
	uint8_t txLen;
	DLT634_5101MASTER_PASDU TX_Temp;
	
}DLT634_5101MASTER_APPINFO;

typedef struct 
{
	uint8_t Port;            //端口
	uint8_t BalanMode;	    	// 1平衡，0非平衡
	uint8_t LinkAddrSize;   //链路地址长度
	uint8_t ASDUCotSize;    //传送原因长度 
	uint8_t ASDUAddr;       //ASDU地址
	uint8_t ASDUAddrSize;    //ASDU地址长度
	uint64_t TimeOutValue;		//超时重发间隔
}DLT634_5101MASTER_PAD;

extern DLT634_5101MASTER_PAD      DLT634_5101Master_Pad[_DLT634_5101MASTER_VOLUMES];
extern DLT634_5101MASTER_APPINFO  SMasterInfo[_DLT634_5101MASTER_VOLUMES];


/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
void SMaster101Main(uint8_t pdrv);
SMasterReqErrCode SMasterSendFrame(uint8_t pdrv,ReqSendInfo *sInfo);
#endif /* END _DLT634_5101_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/


