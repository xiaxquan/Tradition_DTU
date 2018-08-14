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
			    uint8_t DatPackage_Type; //���ݰ�����
          uint8_t OPt_Identify;//������ʶ
				  uint32_t Dir_ID;//�ļ�ID
				  uint8_t DirName_Length;//�ļ�������
				  uint8_t	buff[256];
			}C_210_READ_FILEDIR;	//���ļ�Ŀ¼
			
//				  uint8_t	DirName_buff[256];
//				  uint8_t CallTag;//�ٻ���ʶ
//				  uint8_t	Query_StartAddr[7];//��ѯ��ʼ��ַ
//				  uint8_t	Query_OverAddr[7];//��ѯ������ַ
//				
//			}C_210_READ_FILEDIR;	//���ļ�Ŀ¼

			
   		 __packed  struct
			{
					uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;
			    uint8_t DatPackage_Type; //���ݰ�����
          uint8_t OPt_Identify;//������ʶ
				  uint8_t FileName_Length;//�ļ�������
				  uint8_t	FileName_buff[256];
			}C_210_READFILE_ACTIVE;	//���ļ�����	

   		 __packed  struct
			{
					uint8_t InfoAddr_L;
					uint8_t InfoAddr_H;
			    uint8_t DatPackage_Type; //���ݰ�����
          uint8_t Opt_Identify;//������ʶ
				  uint32_t File_ID;//�ļ�ID
				  uint32_t DatSection_ID;
				  uint8_t  Result_Description;//���������
			}C_210_READFILE_TRANSURE;//���ļ����ݴ���ȷ��

			
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

/* 101��վ״̬ö�� */
enum LinkStatusType{
	LINK_IDLE,/* ���� */
	LINK_REQUESTSEND,/* ������ */
	LINK_SEND,/* ������ */
	LINK_WAITACK,/* �ȴ�����Ӧ�� */
	LINK_ACKOK,
};

typedef struct{
	uint16_t slave;		/* �ӻ�addr */
	uint8_t *fcb;
	uint8_t ackC;		/* Ӧ������� */
	uint8_t type;		/* �����͵�֡���� */
	uint8_t TI;			/* ��type = F3ʱ ���� */
	uint16_t COT;		/* ����ԭ�� */
	//uint16_t asduAddr;	/* ASDU addr */
	uint8_t VSQ;		/* �ɱ�ṹ�޶��� */
	uint8_t *pbuff;		/* ʵ������buff */
	uint8_t rlen;		/* ʵ�����ݳ��� */
}ReqRevInfo;

typedef struct{
	uint16_t slave;		/* �ӻ�addr */
	//uint16_t asduAddr;	/* ASDU addr */
	uint8_t lastFCB;	/* ���͵Ŀ�����FCBλ��¼ */ 
	uint8_t type;		/* �����͵�֡���� */
	uint8_t TI;			/* ��type = F3ʱ ���� */
	uint16_t COT;		/* ����ԭ�� */
	uint8_t VSQ;		/* �ɱ�ṹ�޶��� */
	uint8_t *pbuff;	/* ʵ������buff */
	uint8_t slen;		/* ʵ�����ݳ��� */
	ReqRevInfo *rev;	/* ���ջص���� */	
}ReqSendInfo;

typedef struct 
{
	uint16_t slave;		/* �ӻ�addr */
	enum LinkStatusType linkStatus;/* ��·��ǰ��״̬ */
	uint8_t lastFCB;	/* ���͵Ŀ�����FCBλ��¼ */ 
	uint8_t type;		/* �����͵�֡���� */
	uint8_t TI;			/* ��type = F3ʱ ���� */
	uint16_t COT;		/* ����ԭ�� */
	uint8_t VSQ;		/* �ɱ�ṹ�޶��� */
	uint8_t *pbuff;		/* ʵ������buff */
	uint8_t slen;		/* ʵ�����ݳ��� */
	ReqRevInfo *rev;	/* ���ջص���� */	
	
	uint8_t otsCheck;	/* ������ʱ��� */
	uint8_t otflag;		/* ��ʱ */
	uint8_t reSend;		/* �ط����� */
	uint32_t otTick;	/* ��ʱ������ */
	
	/* ��·�㷢�ͽ��� */
	uint8_t TxdBuf[_DLT634_5101MASTER_LPDUSIZE];
	uint8_t RxdBuf[_DLT634_5101MASTER_LPDUSIZE];
	uint8_t rxCount;
	uint8_t txLen;
	DLT634_5101MASTER_PASDU TX_Temp;
	
}DLT634_5101MASTER_APPINFO;

typedef struct 
{
	uint8_t Port;            //�˿�
	uint8_t BalanMode;	    	// 1ƽ�⣬0��ƽ��
	uint8_t LinkAddrSize;   //��·��ַ����
	uint8_t ASDUCotSize;    //����ԭ�򳤶� 
	uint8_t ASDUAddr;       //ASDU��ַ
	uint8_t ASDUAddrSize;    //ASDU��ַ����
	uint64_t TimeOutValue;		//��ʱ�ط����
}DLT634_5101MASTER_PAD;

extern DLT634_5101MASTER_PAD      DLT634_5101Master_Pad[_DLT634_5101MASTER_VOLUMES];
extern DLT634_5101MASTER_APPINFO  SMasterInfo[_DLT634_5101MASTER_VOLUMES];


/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
void SMaster101Main(uint8_t pdrv);
SMasterReqErrCode SMasterSendFrame(uint8_t pdrv,ReqSendInfo *sInfo);
#endif /* END _DLT634_5101_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/


