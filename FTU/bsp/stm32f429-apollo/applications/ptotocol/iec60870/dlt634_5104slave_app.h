/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_5104slave_app.h
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    Mr.Sun
  * @date:      2017.12.19
  * @update:    
  */

#ifndef	_DLT634_5104SLAVE_LINK_H_
#define _DLT634_5104SLAVE_LINK_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "dlt634_5104slave_config.h"
#include "stm32f4xx_hal.h"

#ifndef  FALSE
#define  FALSE     0
#define  TRUE      !FALSE
#endif

/* PUBLIC VARIABLE -----------------------------------------------------------*/

/* DEFINE --------------------------------------------------------------------*/

/* ��·�����״̬ */

/* Struct  -------------------------------------------------------------------*/
#pragma pack(push,1)
typedef struct 
{
    uint8_t Milliseconds_L;   // D7-D0
    uint8_t Milliseconds_H;   // D7-D0
    uint8_t Minutes;					// D5-D0
    uint8_t Hours;						// D4-D0
    uint8_t DayofWeekMonth;		// WEEK(D7-D5)MONTH(D4-D0)
    uint8_t Months;						// D3-D0
    uint8_t Years;						// D6-D0
}_DLT634_5104SLAVE_CP56Time2a_t;

typedef struct 
{
    struct STATUS
    {
        uint8_t Length;
        union 
        {
            uint8_t priority;	
            uint8_t Lock_ID;
        }symbol;
    }status;

    struct HEAD
    {
        uint8_t TypeID;     
        uint8_t VSQ;  
        uint8_t COT_L;
        uint8_t COT_H;
        uint8_t PubAddr_L;
        uint8_t PubAddr_H;
    }Head;

    union 
    {
        uint8_t buff[256-sizeof(struct STATUS)-sizeof(struct HEAD)];
        
        struct 
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_M;
            uint8_t InfoAddr_H;
            uint8_t QOI; 
        }C_100;

        struct 
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_M;
            uint8_t InfoAddr_H;	
            _DLT634_5104SLAVE_CP56Time2a_t CP56Time2a;
        }C_103;

        struct 
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_M;
            uint8_t InfoAddr_H;	
            uint8_t QRP; 
        }C_105;

        struct 
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_M;
            uint8_t InfoAddr_H;	
            uint8_t COI; 
        }C_70;

        struct 
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_M;
            uint8_t InfoAddr_H;
            uint8_t FBP_L;
            uint8_t FBP_H;				
        }C_104;

        union 
        {
            struct 
            {
                struct 
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_M;
                    uint8_t InfoAddr_H;	
                    uint8_t Value; 
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/4];
            }SQ0;
            struct 
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_M;
                uint8_t InfoAddr_H;
                struct 
                {
                    uint8_t Value; 
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-3)/1];
            }SQ1;
        }C_1;

        union 
        {
            struct 
            {
                struct 
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_M;
                    uint8_t InfoAddr_H;	
                    uint8_t Value; 
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/4];
            }SQ0;
            struct 
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_M; 
                uint8_t InfoAddr_H;	
                struct 
                {
                    uint8_t Value; 
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-3)/1];
            }SQ1;
        }C_3;

        union 
        {
            struct 
            {
                struct 
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_M;
                    uint8_t InfoAddr_H;
                    uint8_t Value; 
                    _DLT634_5104SLAVE_CP56Time2a_t CP56Time2a;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/11];
            }SQ0;
            struct 
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_M;
                uint8_t InfoAddr_H;	
                struct 
                {
                    uint8_t Value; 
                    _DLT634_5104SLAVE_CP56Time2a_t CP56Time2a;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-3)/8];
            }SQ1;
        }C_30;

        union 
        {
            struct 
            {
                struct 
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_M;
                    uint8_t InfoAddr_H;	
                    uint8_t Value; 
                    _DLT634_5104SLAVE_CP56Time2a_t CP56Time2a;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/11];
            }SQ0;
            struct 
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_M;
                uint8_t InfoAddr_H;	
                struct 
                {
                    uint8_t Value; 
                    _DLT634_5104SLAVE_CP56Time2a_t CP56Time2a;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-3)/8];
            }SQ1;
        }C_31;

        union 
        {
            struct 
            {
                struct 
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_M;
                    uint8_t InfoAddr_H;
                    uint16_t Value;
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/8];
            }SQ0;
            struct 
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_M;
                uint8_t InfoAddr_H;	
                struct 
                {
                    uint16_t Value;									
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-3)/5];
            }SQ1;
        }C_9;

        union 
        {
            struct 
            {
                struct 
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_M;
                    uint8_t InfoAddr_H;
                    uint16_t Value;
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/8];
            }SQ0;
            struct 
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_M;
                uint8_t InfoAddr_H;
                struct 
                {
                    uint16_t Value;
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-3)/5];
            }SQ1;
        }C_11;

        union 
        {
            struct 
            {
                struct 
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_M;
                    uint8_t InfoAddr_H;
                    uint32_t Value;	
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/8];
            }SQ0;

            struct 
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_M;
                uint8_t InfoAddr_H;
                struct 
                {
                    uint32_t Value;	
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-3)/5];
            }SQ1;
        }C_13;

        struct 
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_M;
            uint8_t InfoAddr_H;
            uint8_t SCO_L;
            uint8_t SCO_H;
        }C_45;	

        struct 
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_M;
            uint8_t InfoAddr_H;	
            uint8_t DCO_L;
            uint8_t DCO_H;
        }C_46;	

        struct 
        {
            uint8_t Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))]; 
        }C_42;

        struct 
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_M;
            uint8_t InfoAddr_H;	
            uint8_t SN_L;
            uint8_t SN_H;
        }C_200;

        union 
        {
            struct 
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_M;
                uint8_t InfoAddr_H;
            }Down;

            struct 
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_M;
                uint8_t InfoAddr_H;	
                uint8_t SN1_L;
                uint8_t SN1_H;
                uint8_t SN2_L;
                uint8_t SN2_H;
                uint8_t SN3_L;
                uint8_t SN3_H;
            }Up;
        }C_201;

        union 
        {
            struct 
            {
                uint8_t SN1_L;
                uint8_t SN1_H;
                uint16_t Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-2)/4];		
            }Down;

            struct 
            {
                uint8_t SN_L;
                uint8_t SN_H;	
                uint8_t PI_L;
                uint8_t PI_H;
                struct 
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_M;
                    uint8_t InfoAddr_H;
                    uint8_t Tag;
                    uint8_t Len;
                    uint32_t Value;	
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-4)/9];
            }Up;
        }C_202;

        union 
        {
            uint8_t SN_L;
            uint8_t SN_H;	
            uint8_t PI_L;
            uint8_t PI_H;
            struct 
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_M;
                uint8_t InfoAddr_H;
                uint8_t Tag;
                uint8_t Len;
                uint32_t Value;	
            }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-4)/9];
        }C_203;

        struct 
        {
            uint8_t Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))]; 
        }C_210;
    }Data;
}DLT634_5104SLAVE_PASDU;

 struct DLT634_5104SLAVE_PASDU_104
{
     struct PASDUHead_104
    {   
        uint8_t	TypeID; // ���ͱ�ʶ
        uint8_t	VSQ;    // �ɱ�ṹ�޶���
     
        // ����ԭ��
        uint16_t COT;

        // Ӧ�÷������ݵ�Ԫ������ַ
        uint16_t PubAddr;

        // ��Ϣ�����ַ
        uint16_t InfoAddr;
    }Head;
    uint8_t Data[DLT634_5104SLAVE_ASDUSIZE - sizeof(struct PASDUHead_104)];
};

struct DLT634_5104SLAVE_SendData
{
    uint8_t TxLen;
    uint8_t TxData[DLT634_5104SLAVE_ASDUSIZE];	
};  
		
/* �鴦��ṹ -----------------------------------------------------------------*/ 
struct DLT634_5104SLAVE_APPINFO
{	
    uint8_t Connect; // ��������״̬	

    uint8_t  YXTypeID;           // ң�����ͱ�ʶ
    uint8_t  YCTypeID;           // ң�����ͱ�ʶ	
	
	uint8_t  SDataEn;            // ���ݷ���ʹ��
    int16_t  CurSData;          // δ��ȷ�ϵ�I֡���ݻ�����λ�ñ�ʶ
    int16_t  SDataIn;           // δ��ȷ�ϵ�I֡���ݻ�������ָ��
	int16_t  SDataOut;           // δ��ȷ�ϵ�I֡���ݻ�������ָ��
	
  	uint8_t  RxdBuf[2*DLT634_5104SLAVE_APDUSIZE]; // ���ջ�����
    uint8_t  TxdBuf[2*DLT634_5104SLAVE_APDUSIZE]; // ���ͻ�����	

  	uint32_t  Data1Flag; 
    uint32_t  AllDataFlag; // ����״̬��־
    uint32_t  AppNextFlag; // �������ݱ�־

  	uint8_t TxLen;     

  	struct DLT634_5104SLAVE_PASDU_104 TxMsg;  // Ӧ�ò㷢������ָ��
//    struct DLT634_5104SLAVE_PASDU_104 RxMsg; // Ӧ�ò��������ָ�� 		
	
    struct DLT634_5104SLAVE_SendData SData[DLT634_5104SLAVE_K + 1]; // δ��ȷ�ϵ�I֡����    

    struct PGroupTrn
    {		
        uint16_t InfoAddr;
        uint8_t TypeID;
        uint8_t COT;
        uint8_t GroupNo;
    }GroupTrn;// ���鴦��ṹ��
	
    DLT634_5104SLAVE_PASDU *StoreDATA1IN;              // �����ͻ�����1��������ָ��
    DLT634_5104SLAVE_PASDU *StoreDATA1OUT;             // �����ͻ�����1�����ݳ�ָ��
    DLT634_5104SLAVE_PASDU StoreDATA1Buf[DLT634_5104SLAVE_STOREDATA1NUM];     	// 1�����ݴ����ͻ�����
    DLT634_5104SLAVE_PASDU *StoreDATA2IN;              // �����ͻ�����1��������ָ��
    DLT634_5104SLAVE_PASDU *StoreDATA2OUT;             // �����ͻ�����1�����ݳ�ָ��
    DLT634_5104SLAVE_PASDU StoreDATA2Buf[DLT634_5104SLAVE_STOREDATA2NUM];     	// 2�����ݴ����ͻ�����
    DLT634_5104SLAVE_PASDU ST_Temp;              // �洢���ݴ���

    DLT634_5104SLAVE_PASDU TX_Temp;              //�������ݴ���
    DLT634_5104SLAVE_PASDU RX_Temp;              //�������ݴ���  

    uint16_t AllData_yx_sendaddr;
    uint16_t AllData_yc_sendaddr;    
};

struct DLT634_5104SLAVE_PAPCI
{
    uint8_t	StartCode;
    uint8_t	Length;
    uint16_t NS;
    uint16_t NR;
};

struct DLT634_5104SLAVE_LINKINFO
{
    uint8_t Connect; // ��������״̬
    uint8_t DataType[DLT634_5104SLAVE_K]; // �����������ͣ�I��ʽ֡
    uint8_t StopSendFlag; //ֹͣ���ͱ�־(δ��ȷ�ϵ�I��ʽ֡�ﵽK����ֹͣ����)		

    uint8_t RxdBuf[2*DLT634_5104SLAVE_APDUSIZE]; // ���ջ�����
    uint8_t TxdBuf[2*DLT634_5104SLAVE_APDUSIZE]; // ���ͻ�����		

    uint16_t NR; // ���հ�����
    uint16_t NS; // ���Ͱ�����
    uint16_t NRACK; // �ϴ�ȷ�ϵĽ������к�

    uint16_t PeerNoAckNum; // �Է�δ����ȷ�ϵ�I��ʽ֡��Ŀ	  

    uint16_t RxdHead; // �������ݵ�ͷָ��
    uint16_t RxdTail;		
    uint16_t RxdLength;

    uint16_t FrameHead; // ��֡ʱ����ͷ�ڷ��ͻ�������λ��
    uint16_t TxdHead;
    uint16_t TxdTail;

    // ��������
    uint16_t Tick0;
    uint16_t Tick1;
    uint16_t Tick2;
    uint16_t Tick3;

    uint32_t  ClockCounter;  // �����������ڿ���
		
    enum DLRXDSTATUS
    {
            RXDSTART = 0,	                               // Ѱ����ʼ��־״̬
            RXDHEAD,	                                   // Ѱ�ұ���ͷ
            RXDCONTINUE                                  // ���պ�������
    }RxdStatus;// ����״̬
        
    struct PTick
    {
        uint16_t TValue0;	
        uint16_t TValue1[DLT634_5104SLAVE_K];
        uint16_t TValue2;
        uint16_t TValue3;
        uint8_t	Flag; // ��λʹ��
        uint8_t	FlagBak; // ����
    }Tick;				
};

struct DLT634_5104SLAVE_AppMsg
{
    uint16_t Cmd;
    uint16_t ConNum;
    uint8_t Len;
    uint8_t* pData;
};

struct DLT634_5104SLAVE_PAD
{
    uint8_t   Port;            //�˿�

    uint8_t   CotSize;      // ����ԭ���ֽڳ���
    uint8_t   PubAddrSize;  // ��·������ַ�ֽڳ���
    uint8_t   LinkAddrSize; // ��·��ַռ���ֽ�  
    uint8_t   InfoAddrSize; // ��Ϣ���ַ�ֽڳ���

    uint8_t  YCNum;        // ң��������
    uint8_t  YXNum;        // ң��������
    uint8_t  YKNum;        // ң��������
    uint8_t  DDNum;        // ���������

    uint16_t  LinkAddress;  // ��·��ַ,��·��ַ���빫����ַ��ͬ 	  
    uint16_t  PubAddress;   // ������ַ��������·��ַ��ͬ������ͬһ��·�ϵ��豸(��վ)��ַ

    uint16_t  YX_AllNum;    // ң������
    uint16_t  YX_FirstAddr; // ң���׵�ַ
    uint16_t  YX_FrameNum;	// ң��ÿ������
    uint16_t  YC_AllNum;		// ң������
    uint16_t  YC_FirstAddr; // ң���׵�ַ
    uint16_t  YC_FrameNum;  // ң��ÿ������

    uint32_t  TickValue[4]; // IEC104 4����ʱʱ��
    uint32_t   ClockTimers;  // ���ó�������������ִ��һ�Σ�������������
};
#pragma pack(pop)
/* PUBLIC VARIABLES ----------------------------------------------------------*/
extern struct DLT634_5104SLAVE_PAD DLT634_5104Slave_Pad[DLT634_5104SLAVE_VOLUMES]; // DL/T634_5101��Լ����

/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
extern uint8_t DLT634_5104_SLAVE_Clock(uint8_t port);
extern uint8_t DLT634_5104_SLAVE_AppInit(uint8_t port);
extern void DLT634_5104_SLAVE_ParaInit(uint8_t port);
extern uint8_t DLT634_5104_SLAVE_Reply(uint8_t pdrv, uint8_t *pbuf);

#endif /* END _DLT634_5104SLAVE_LINK_H_*/

/* END OF FILE ---------------------------------------------------------------*/


