/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_5101master_config.h
  * @brief:     The protocol of dlt634_5101master_config.
  * @version:   V3.01
  * @author:    Mr.J
  * @date:      2015-10-25
  * @update:    
  */
 
#ifndef	_DLT634_5101MASTER_CONFIG_H_
#define _DLT634_5101MASTER_CONFIG_H_

/* PUBLIC VARIABLE -----------------------------------------------------------*/
#define _DLT634_5101MASTER_VOLUMES	1
/* Number of volumes (logical drives) to be used.*/
#define _DLT634_5101MASTER_LPDUSIZE  256
#define _DLT634_5101MASTER_STOREDATA1NUM	20
#define _DLT634_5101MASTER_TimeOutClock	    10
#define  _DLT634_5101MASTER_NUMOF_MAXRETRY                0x03 // Ĭ��DL/T634.5101_2002��Լ��·����������Ӵ���

//LINK_FLAG
#define  _DLT634_5101MASTER_REQSTATUS                   0x00000000		//������·
#define  _DLT634_5101MASTER_RECONFIRM                   0x00000002		//��λ
#define  _DLT634_5101MASTER_REQSTDATA1               	0x00000004		//����1������
#define  _DLT634_5101MASTER_REQSTDATA2               	0x00000008		//����2������
#define  _DLT634_5101MASTER_SYNCLOCK                	0x00000010		//ʱ��ͬ��
#define  _DLT634_5101MASTER_CALLTOTAL                	0x00000020		//վ���ٻ�
#define  _DLT634_5101MASTER_CALLELEC                	0x00000040		//�������ٻ�
#define  _DLT634_5101MASTER_FREEZELEC                	0x00000080		//����������
#define  _DLT634_5101MASTER_CONSIGLE                	0x00000100		//��������
#define  _DLT634_5101MASTER_CONDOBLE                	0x00000200		//˫������
#define  _DLT634_5101MASTER_CONTEST                		0x00000400		//��������
#define  _DLT634_5101MASTER_VALUECHG                	0x00000800		//�л���ֵ��
#define  _DLT634_5101MASTER_VALUEREAD               	0x00001000		//����ֵ����
#define  _DLT634_5101MASTER_PARAMREAD                	0x00002000		//�������Ͷ�ֵ
#define  _DLT634_5101MASTER_PARAMWRT                	0x00004000		//д�����Ͷ�ֵ
#define  _DLT634_5101MASTER_FILETRANS                	0x00008000		//�ļ�����
#define  _DLT634_5101MASTER_SOFTUP                		0x00010000		//�������
#define  _DLT634_5101MASTER_PROFIRM               		0x00020000		//��λ��������
#define  _DLT634_5101MASTER_FILEDEAL              		0x00040000		//�ļ�����
#define  _DLT634_5101MASTER_FILE_TRANSURE             0x00080000		//�ļ����ݴ���ȷ��
#define  _DLT634_5101MASTER_FILE_READACTIVE           0x00100000		//�ļ���ȡ����
//EqStatus
#define  _DLT634_5101MASTER_LINKIN                		0x00		//������·
#define  _DLT634_5101MASTER_LINKEND                		0x01		//��·���
//APP_FLAG
#define  _DLT634_5101MASTER_CALLALLDATA                0x00000001
#define  _DLT634_5101MASTER_LINKTEST                   0x00000002
#define  _DLT634_5101MASTER_READTIME                   0x00000004

/*************************************************************************************************************************************/
//DL/T634.5101_2002��Լ

/* ��·��֡���δ�***************************************************************/
#define  _DLT634_5101MASTER_STARTCODE10	                     0x10	// ��ʼ��
#define  _DLT634_5101MASTER_STARTCODE68	                     0x68	// ��ʼ��
#define  _DLT634_5101MASTER_ENDCODE                           0x16	// ��ֹ��

/* ��·���������************************************************************/
//ǰ4λ
#define  _DLT634_5101MASTER_FUNCODE                       0x0F // ������
#define  _DLT634_5101MASTER_FCV                           0x10 // ֡������Чλ 
#define  _DLT634_5101MASTER_DFC                           0x10 // ����������
#define  _DLT634_5101MASTER_FCB                           0x20 // ֡����λ
#define  _DLT634_5101MASTER_ACD                           0x20 // Ҫ�����λ 
#define  _DLT634_5101MASTER_PRM                           0x40 // ��������λ
#define  _DLT634_5101MASTER_DIR                           0x80 // ���䷽��λ

//��·����վ����վ���书���붨��
#define   _DLT634_5101MASTER_M_FUN0             	0           // ��λԶ����·
#define   _DLT634_5101MASTER_M_FUN2              2           // ������·���� 
#define   _DLT634_5101MASTER_M_FUN3              3	          // ����ȷ������
#define   _DLT634_5101MASTER_M_FUN4             	4	          // ���Ͳ�ȷ������
#define   _DLT634_5101MASTER_M_FUN8              8	          // ������Ӧȷ������״̬
#define   _DLT634_5101MASTER_M_FUN9             	9	          // �ٻ���·״̬
#define   _DLT634_5101MASTER_M_FUN10             10	        // �ٻ�1���û����� 
#define   _DLT634_5101MASTER_M_FUN11             11          // �ٻ�2���û�����

//*��·����վ����վ���书���붨�� 
#define   _DLT634_5101MASTER_S_FUN0              0            // ȷ��
#define   _DLT634_5101MASTER_S_FUN1              1            // ȷ����·æδ���ձ���
#define   _DLT634_5101MASTER_S_FUN8              8            // ��������Ӧ����֡
#define   _DLT634_5101MASTER_S_FUN9              9            // �����ٻ�������
#define   _DLT634_5101MASTER_S_FUN11             11           // ��Ӧ��·״̬��ش�����֡
 
/* TI���ͱ�ʶ ********************************************************************************/ 
//���ӷ��������Ϣ
#define   _DLT634_5101MASTER_M_SP_NA_1              1            // ������Ϣ
#define   _DLT634_5101MASTER_M_DP_NA_1              3            // ˫����Ϣ
#define   _DLT634_5101MASTER_M_ME_NA_1              9            // ����ֵ����һ��ֵ
#define   _DLT634_5101MASTER_M_ME_NB_1              11           // ����ֵ����Ȼ�ֵ
#define   _DLT634_5101MASTER_M_ME_NC_1              13           // ����ֵ���̸�����
#define   _DLT634_5101MASTER_M_SP_TB_1              30           // ��ʱ��ĵ�����Ϣ
#define   _DLT634_5101MASTER_M_DP_TB_1              31           // ��ʱ���˫����Ϣ
#define   _DLT634_5101MASTER_M_FT_NA_1              42           // �����¼���Ϣ
#define   _DLT634_5101MASTER_M_IT_NB_1              206          // �ۼ������̸�����
#define   _DLT634_5101MASTER_M_IT_TC_1              207          // ��ʱ���ۼ������̸�����

//���Ʒ��������Ϣ
#define   _DLT634_5101MASTER_C_SC_NA_1              45            // ��������
#define   _DLT634_5101MASTER_C_SC_NB_1              46            // ˫������

//���ӷ����ϵͳ����
#define   _DLT634_5101MASTER_M_EI_NA_1              70            // ��ʼ������

//���Ʒ����ϵͳ����
#define   _DLT634_5101MASTER_C_IC_NA_1              100            // վ���ٻ�����
#define   _DLT634_5101MASTER_C_CI_NA_1              101            // �������ٻ�����
#define   _DLT634_5101MASTER_C_CS_NA_1              103            // ʱ��ͬ������
#define   _DLT634_5101MASTER_C_TS_NA_1              104            // ��������
#define   _DLT634_5101MASTER_C_RP_NA_1              105            // ��λ��������
#define   _DLT634_5101MASTER_C_SR_NA_1              200            // �л���ֵ��
#define   _DLT634_5101MASTER_C_RR_NA_1              201            // ����ֵ����
#define   _DLT634_5101MASTER_C_RS_NA_1              202            // �������Ͷ�ֵ
#define   _DLT634_5101MASTER_C_WS_NA_1              203            // д�����Ͷ�ֵ
#define   _DLT634_5101MASTER_F_FR_NA_1              210            // �ļ�����
#define   _DLT634_5101MASTER_F_SR_NA_1              211            // �������

/* COT����ԭ�� ********************************************************************************/ 
#define   _DLT634_5101MASTER_COT_CYC              1            // ����ѭ��
#define   _DLT634_5101MASTER_COT_BACK             2            // ����ɨ��
#define   _DLT634_5101MASTER_COT_SPONT            3            // ͻ��
#define   _DLT634_5101MASTER_COT_INIT             4            // ��ʼ��
#define   _DLT634_5101MASTER_COT_REQ              5            // ���������
#define   _DLT634_5101MASTER_COT_ACT              6            // ����
#define   _DLT634_5101MASTER_COT_ACTCON           7            // ����ȷ��
#define   _DLT634_5101MASTER_COT_DEACT            8            // ֹͣ����
#define   _DLT634_5101MASTER_COT_DEACTCON         9            // ֹͣ����ȷ��
#define   _DLT634_5101MASTER_COT_ACCTTERM         10            // ������ֹ
#define   _DLT634_5101MASTER_COT_FILE             13            // �ļ�����
#define   _DLT634_5101MASTER_COT_INTROGEN         20            // ��Ӧվ�ٻ�
#define   _DLT634_5101MASTER_COT_REQCOGEN         37            // ��Ӧ�����ٻ�

#endif /* END _DLT634_5101_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/
