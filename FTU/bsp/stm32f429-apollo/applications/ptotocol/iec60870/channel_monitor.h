#ifndef	_CHANNEL_MONITOR_H_
#define _CHANNEL_MONITOR_H_
#include "stm32f4xx_hal.h"



#define SerialPort1     1                   //����1
#define SerialPort2     2                   //����2
#define InternetAccess1 3                   //����1
#define InternetAccess2 4                   //����2

#define TransmittedData         0xAA    //������Ϊ��������          
#define ReceivedData            0x55    //������Ϊ��������
#define LinkAliveTime           6000    //5s
#define TestAliveTime           2000
#define DLT634_CHANNEL_MONITOR_VOLUMES  2

/* ͨ�����ӱ������δ�***************************************************************/
#define  _DLT634_ChannelToMonitor_SLAVE_STARTCODE11	             0x11					// ��ʼ��
#define  _DLT634_ChannelToMonitor_SLAVE_STARTCODE12              0x12                   // ֹͣ����
#define  _DLT634_ChannelToMonitor_SLAVE_ENDCODE66                0x66					// ��ֹ��

#define  _DLT634_ChannelToMonitor_SLAVE_FC_TEST                  0x01 					// ͨ����������
#define  _DLT634_ChannelToMonitor_SLAVE_FC_TEST_CON              0x02 					// ͨ���������Իظ�
#define  _DLT634_ChannelToMonitor_SLAVE_FC_ON                    0x09 					// ͨ����������
#define  _DLT634_ChannelToMonitor_SLAVE_FC_OFF                   0x0A 					// ͨ����������
#define  _DLT634_ChannelToMonitor_SLAVE_FC_RESPONSE              0x0B 					// ͨ������Ӧ��
#define  _DLT634_ChannelToMonitor_SLAVE_FC_OFF_RESPONSE          0x0C 					// ֹͣͨ������Ӧ��
#define  _DLT634_ChannelToMonitor_AddError                       0X0F                   // ��ַ����

#define _ChannelToMonitor1_START         	        0x01					//��������ͨ��������־
#define _ChannelToMonitor2_START         	        0x02					//��������ͨ��������־


#define AddrSize                                    2                       //��·��ַ����


#define DLT634_5101                                 101
#define DLT634_5104                                 104
    
    
typedef struct 
{
    enum
    {
        CRXDSTART = 0,	        											// Ѱ����ʼ��־״̬
        CRXDHEAD,	            											// Ѱ�ұ���ͷ
        CRXDCONTINUE             											// ���պ�������
    }CRxdStatus;
    uint8_t MonitorFlag[2];                                                       //������־
    uint16_t ByAddr;                                                    //��������ַ
    uint16_t Addr;                                                      //������ַ
	uint32_t LinkCounter;                                               //��·������
	uint32_t TestCounter; 
}ChannelToMonitor;                                                      //ͨ������

extern ChannelToMonitor channel_monitor;

int rt_channel_monitor_init(void);
int rt_channel_monitor_clock(uint8_t pdrv);
void rt_channel_monitor_task(void);
uint8_t DLT634_ChannelToMonitor_SLAVE_SearchMonitorFrame(uint8_t pdrv, uint8_t protocol, uint8_t *RxdBuf, uint16_t len);
void MonitoringDataTransmission(uint8_t *pbuf, uint16_t count, uint8_t ReceiveAndDispatch);
#endif


