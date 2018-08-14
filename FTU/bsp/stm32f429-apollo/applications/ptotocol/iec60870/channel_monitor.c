#include "channel_monitor.h"
#include "string.h"
#include "common_data.h"
#include "dlt634_5101slave_app.h"
#include "dlt634_5101slave_disk.h"
#include "dlt634_5104slave_disk.h"
#include "tcp_server.h"
#include "drv_w5500_socket.h"
#include "lwip/sockets.h"


static rt_device_t dev[_DLT634_5101SLAVE_VOLUMES];
ChannelToMonitor channel_monitor;

static uint8_t DLT634_ChannelToMonitor_SLAVE_CKS(uint8_t pdrv ,uint8_t *pBuf)
{
    uint16_t sum = 0;
    uint16_t j =0;

    if (*(pBuf) == _DLT634_ChannelToMonitor_SLAVE_STARTCODE11)
    {
        for (j = 0; j < (AddrSize + 1); j++)
        {
            sum += *(pBuf + 1 + j);
        }
    }
    return (uint8_t)(sum&0xff);
}
/**
  * @brief : IECУ��
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_ChannelToMonitor_SLAVE_CheckFram11Valid(uint8_t pdrv, uint8_t *RxdBuf)
{
    if (RxdBuf[2 + AddrSize] != (DLT634_ChannelToMonitor_SLAVE_CKS(pdrv, RxdBuf)&0xFF))	                    // �̶�֡�����ݵ�У��ͼ��
    {
        return (0);
    }
    if (RxdBuf[3 + AddrSize] != _DLT634_ChannelToMonitor_SLAVE_ENDCODE66)                                // �̶�֡�����ݵĽ���֡���
    {
        return (0);
    }
    return (1);
}

/**
  * @brief : ����ͨ������Ӧ��
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_ChannelToMonitor_SLAVE_SlaveFrame11Response(uint8_t pdrv, uint8_t protocol)
{
    uint8_t pBuf[6];
	if (channel_monitor.ByAddr)                                                              //������ͨ����ַ��Ϊ��
    {
        pBuf[0] = _DLT634_ChannelToMonitor_SLAVE_STARTCODE11;                                            //��ʼ��
        pBuf[1] = _DLT634_ChannelToMonitor_SLAVE_FC_RESPONSE;                                            //�����򣬻ش���Ӧ
		pBuf[2] = channel_monitor.ByAddr & 0xff;
        pBuf[3] = (channel_monitor.ByAddr>>8) & 0xff;

        pBuf[2+AddrSize] = DLT634_ChannelToMonitor_SLAVE_CKS(pdrv, pBuf);                               //֡У���
        pBuf[3+AddrSize] = _DLT634_ChannelToMonitor_SLAVE_ENDCODE66;                                     //��ֹ��
        if(protocol == DLT634_5101)
        {
            DLT634_5101_SLAVE_WriteData(pdrv, pBuf, 6);                                       //Ӧ����
        }
        if(protocol == DLT634_5104)
        {
			switch (pdrv)
			{
				case 0:
					lwip_send(1, pBuf, 6, 0);
					break;
				case 1:
					w5500_send(0, pBuf, 6);
					break;
				default:
					break;
			}
        } 
    }        
}

/**
  * @brief : ֹͣͨ������Ӧ��
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_ChannelToMonitor_SLAVE_SlaveFrame12Response(uint8_t pdrv, uint8_t protocol)
{
    uint8_t pBuf[6];
	
	if (channel_monitor.ByAddr)                                                              //������ͨ����ַ��Ϊ��
    {
        pBuf[0] = _DLT634_ChannelToMonitor_SLAVE_STARTCODE11;                                            //��ʼ��
        pBuf[1] = _DLT634_ChannelToMonitor_SLAVE_FC_OFF_RESPONSE;                                            //�����򣬻ش���Ӧ
		pBuf[2] = channel_monitor.ByAddr & 0xff;
        pBuf[3] = (channel_monitor.ByAddr>>8) & 0xff;

        pBuf[2+AddrSize] = DLT634_ChannelToMonitor_SLAVE_CKS(pdrv, pBuf);                               //֡У���
        pBuf[3+AddrSize] = _DLT634_ChannelToMonitor_SLAVE_ENDCODE66;                                     //��ֹ��
        if(protocol == DLT634_5101)
        {
            DLT634_5101_SLAVE_WriteData(pdrv, pBuf, 6);                                   //Ӧ����
        }
        if(protocol == DLT634_5104)
        {
			switch (pdrv)
			{
				case 0:
					lwip_send(1, pBuf, 6, 0);
					break;
				case 1:
					w5500_send(0, pBuf, 6);
					break;
				default:
					break;
			}
        }                                               
    }        
}

/**
  * @brief : ��������֡����
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_ChannelToMonitor_SLAVE_SlaveFrame01Response(uint8_t pdrv, uint8_t protocol)
{
    uint8_t pBuf[6];
	
	if (channel_monitor.ByAddr)                                                              //������ͨ����ַ��Ϊ��
    {
        pBuf[0] = _DLT634_ChannelToMonitor_SLAVE_STARTCODE11;                                            //��ʼ��
        pBuf[1] = _DLT634_ChannelToMonitor_SLAVE_FC_TEST;                                            //�����򣬻ش���Ӧ
		pBuf[2] = channel_monitor.ByAddr & 0xff;
        pBuf[3] = (channel_monitor.ByAddr>>8) & 0xff;

        pBuf[2+AddrSize] = DLT634_ChannelToMonitor_SLAVE_CKS(pdrv, pBuf);                               //֡У���
        pBuf[3+AddrSize] = _DLT634_ChannelToMonitor_SLAVE_ENDCODE66;                                     //��ֹ��
        if(protocol == DLT634_5101)
        {
            DLT634_5101_SLAVE_WriteData(pdrv, pBuf, 6);                                    //Ӧ����
        }
        if(protocol == DLT634_5104)
        {
			switch (pdrv)
			{
				case 0:
					lwip_send(1, pBuf, 6, 0);
					break;
				case 1:
					w5500_send(0, pBuf, 6);
					break;
				default:
					break;
			}
        }                                               
    }        
}

/**
  * @brief : ������ַ����
  * @param : [pdrv]
  * @return: [none]
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_ChannelToMonitor_AddError(uint8_t pdrv, uint8_t protocol)
{
    uint8_t pBuf[6];
	
	if (channel_monitor.ByAddr)                                                              //������ͨ����ַ��Ϊ��
    {
        pBuf[0] = _DLT634_ChannelToMonitor_SLAVE_STARTCODE11;                                            //��ʼ��
        pBuf[1] = _DLT634_ChannelToMonitor_AddError;                                            //�����򣬻ش���Ӧ
		pBuf[2] = channel_monitor.ByAddr & 0xff;
        pBuf[3] = (channel_monitor.ByAddr>>8) & 0xff;
        
        pBuf[2+AddrSize] = DLT634_ChannelToMonitor_SLAVE_CKS(pdrv, pBuf);                               //֡У���
        pBuf[3+AddrSize] = _DLT634_ChannelToMonitor_SLAVE_ENDCODE66;                                     //��ֹ��

        if(protocol == DLT634_5101)
        {
            DLT634_5101_SLAVE_WriteData(pdrv, pBuf, 6);                                     //Ӧ����
        }

        if(protocol == DLT634_5104)
        {
			switch (pdrv)
			{
				case 0:
					lwip_send(1, pBuf, 6, 0);
					break;
				case 1:
					w5500_send(0, pBuf, 6);
					break;
				default:
					break;
			}
        }                                               
    }        
}

/**
  * @brief : ��������
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_ChannelToMonitor_SLAVE_DecodeFrame11(uint8_t pdrv, uint8_t protocol, uint8_t *RxdBuf)
{
	channel_monitor.ByAddr = (RxdBuf[3]<<8)|RxdBuf[2];                              //�����˿ڵ�ַ
	if(channel_monitor.ByAddr == channel_monitor.Addr)
	{
		DLT634_ChannelToMonitor_AddError(pdrv, protocol);
		memset(RxdBuf, 0, 256);
		return 0;
	}	
	
    if(protocol == DLT634_5101)                                                             //101����
    {
        if(pdrv == DLT634_5101SLAVE_DISK0)
        {
            channel_monitor.Addr = SerialPort1;
        }
        if(pdrv == DLT634_5101SLAVE_DISK1)
        {
            channel_monitor.Addr = SerialPort2;
        }
		
		switch(RxdBuf[1])
		{
		    case _DLT634_ChannelToMonitor_SLAVE_FC_ON: 
				DLT634_ChannelToMonitor_SLAVE_SlaveFrame11Response(pdrv, protocol);                          //Ӧ��ظ�
				channel_monitor.MonitorFlag[pdrv] |= _ChannelToMonitor1_START;              //������������
				memset(RxdBuf, 0, 256);
				return 1;			
		    case _DLT634_ChannelToMonitor_SLAVE_FC_OFF: 	
				channel_monitor.MonitorFlag[pdrv] &= ~_ChannelToMonitor1_START;                 //���ڼ����ر�
				DLT634_ChannelToMonitor_SLAVE_SlaveFrame12Response(pdrv, protocol);                              //Ӧ��ظ�
				memset(RxdBuf, 0, 256);
				return 0;				
		    case _DLT634_ChannelToMonitor_SLAVE_FC_TEST: 
				channel_monitor.LinkCounter = 0;
			    return 1;
		    default: 
				channel_monitor.MonitorFlag[pdrv] &= ~_ChannelToMonitor1_START;
				channel_monitor.ByAddr = 0;
				return 0;							
		}
    }
    
    else if(protocol == DLT634_5104)                                                        //104����
    {
        if(pdrv == DLT634_5101SLAVE_DISK0)
        {
             channel_monitor.Addr = InternetAccess1;
        }
        if(pdrv == DLT634_5101SLAVE_DISK1)
        {
             channel_monitor.Addr = InternetAccess2;
        }
		
		switch(RxdBuf[1])
		{
		    case _DLT634_ChannelToMonitor_SLAVE_FC_ON: 
				DLT634_ChannelToMonitor_SLAVE_SlaveFrame11Response(pdrv, protocol);                          //Ӧ��ظ�
				channel_monitor.MonitorFlag[pdrv] |= _ChannelToMonitor2_START;              //������������
				memset(RxdBuf, 0, 256);
				return 1;			
		    case _DLT634_ChannelToMonitor_SLAVE_FC_OFF: 
				channel_monitor.MonitorFlag[pdrv] &= ~_ChannelToMonitor2_START;                 			//���ڼ����ر�
				DLT634_ChannelToMonitor_SLAVE_SlaveFrame12Response(pdrv, protocol);                              //Ӧ��ظ�
				memset(RxdBuf, 0, 256);
				return 0;				
		    case _DLT634_ChannelToMonitor_SLAVE_FC_TEST: 
				channel_monitor.LinkCounter = 0;
			    return 1;
		    default: 
				channel_monitor.MonitorFlag[pdrv] &= ~_ChannelToMonitor2_START;
				channel_monitor.ByAddr = 0;
				return 0;							
		}		
    }
    else
    {
        return 0;
    }
}

uint8_t RxdBuf[2][256];

/**
  * @brief : ��������
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_ChannelToMonitor_SLAVE_SearchMonitorFrame(uint8_t pdrv, uint8_t protocol, uint8_t *CRxdBuf, uint16_t len)
{
    uint8_t stop = 0;
    uint16_t MonitorRxdHead = 0;
    uint16_t MonitorRxdTail = len;
    uint16_t RxdLength=0;
	
	memcpy(RxdBuf[pdrv], CRxdBuf, 256);
	
    while ((MonitorRxdHead < MonitorRxdTail) && (!stop))
    {
        if (channel_monitor.CRxdStatus == CRXDSTART)								        // Ѱ����ʼ��־״̬
        {
            while (	(RxdBuf[pdrv][MonitorRxdHead]!= _DLT634_ChannelToMonitor_SLAVE_STARTCODE11)&&(MonitorRxdHead < MonitorRxdTail))
            {
                MonitorRxdHead++;									
            }
            if (MonitorRxdHead < MonitorRxdTail) 						                    // �ҵ������ַ������������뻺�������롣
            {
                channel_monitor.CRxdStatus = CRXDHEAD;				                        //Ѱ�ұ�ͷ
                if (MonitorRxdHead != 0)
                {	
                    memcpy(RxdBuf[pdrv], RxdBuf[pdrv]+MonitorRxdHead, MonitorRxdTail - MonitorRxdHead); //Ŀ�ĵ�ַ��ԭ��ַ������
                    MonitorRxdTail -= MonitorRxdHead;
                    MonitorRxdHead = 0;
                }
            }
        }
        RxdLength = MonitorRxdTail - MonitorRxdHead;                                        // ����һ֡���ݳ���
        if (channel_monitor.CRxdStatus == CRXDHEAD) 									    // ���Ĺ�һ֡����
        {
            if (RxdLength >= 4 + AddrSize)                                                  //һ֡��6�ֽ�
            {
                if (!DLT634_ChannelToMonitor_SLAVE_CheckFram11Valid(pdrv, RxdBuf[pdrv]))                      //У��
                {
                    MonitorRxdHead++;
                    channel_monitor.CRxdStatus = CRXDSTART;				                    // Ѱ����ʼ��־״̬
                }
                else
                {
                    channel_monitor.CRxdStatus = CRXDCONTINUE;					            // ���պ�������
                }
            }
            else
            {
                stop = 1;
            }
        }
        if (channel_monitor.CRxdStatus == CRXDCONTINUE)										//�պ�������
        {
            if (MonitorRxdHead > 0)
            {
                memcpy(RxdBuf[pdrv], RxdBuf[pdrv]+MonitorRxdHead, MonitorRxdTail - MonitorRxdHead);
                MonitorRxdTail -= MonitorRxdHead;
                MonitorRxdHead = 0;
            }
            MonitorRxdHead += 4 + AddrSize;
            channel_monitor.CRxdStatus = CRXDSTART; 
            return (DLT634_ChannelToMonitor_SLAVE_DecodeFrame11(pdrv, protocol, RxdBuf[pdrv]));
        }
    }
        return (0);                        //���Ľ���  
}

/**
  * @brief : ������ݷ���
  * @param : [pbuf]�������ݵ�ַ
  * @param : [count]�����ֽڳ���
  * @param : [ReceiveAndDispatch]���ջ��Ƿ���
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void MonitoringDataTransmission(uint8_t *pbuf, uint16_t count, uint8_t ReceiveAndDispatch)
{
    uint8_t Buf[2];					//�������ݱ�־
	Buf[0] = 0xAA;
	Buf[1] = 0x55;
	
	channel_monitor.TestCounter = 0;
	
    switch(channel_monitor.Addr)
    {
        case SerialPort1:                                                                   //����1
			if(ReceiveAndDispatch == TransmittedData)
			{
				rt_device_write(dev[DLT634_5101SLAVE_DISK0], 0,  Buf, 2); 
			}
            rt_device_write(dev[DLT634_5101SLAVE_DISK0], 0, pbuf, count); 
            break;
        case SerialPort2:                                                                   //����2
			if(ReceiveAndDispatch == TransmittedData)
			{
				rt_device_write(dev[DLT634_5101SLAVE_DISK1], 0, Buf, 2); 
			}
            rt_device_write(dev[DLT634_5101SLAVE_DISK1], 0, pbuf, count); 
            break;
        case InternetAccess1:                                                               //����1
			if(ReceiveAndDispatch == TransmittedData)
			{
				lwip_send(1, Buf, 2, 0);
			}
            lwip_send(1, pbuf, count, 0);
            break;
        case InternetAccess2:                                                               //����2
			if(ReceiveAndDispatch == TransmittedData)
			{
				w5500_send(0, Buf, 2);
			}
            w5500_send(0, pbuf, count);
            break;
    }
}

/**
  * @brief : channel_monitor_clock
  * @param : [none]
  * @return: [1]
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
int rt_channel_monitor_clock(uint8_t pdrv)
{
	if (channel_monitor.MonitorFlag[pdrv] != 0)
	{
		if (++channel_monitor.LinkCounter >= LinkAliveTime)
		{
            DLT634_5104_SLAVE_StopLink(pdrv);
			channel_monitor.LinkCounter = 0;
			channel_monitor.MonitorFlag[pdrv] = 0;
			channel_monitor.ByAddr = 0;
		}	
		if (++channel_monitor.TestCounter >= TestAliveTime)
		{
			channel_monitor.TestCounter = 0;
			if (channel_monitor.MonitorFlag[pdrv] == _ChannelToMonitor1_START)
			{
				DLT634_ChannelToMonitor_SLAVE_SlaveFrame01Response(pdrv, DLT634_5101);
			}
			if (channel_monitor.MonitorFlag[pdrv] == _ChannelToMonitor2_START)
			{
				DLT634_ChannelToMonitor_SLAVE_SlaveFrame01Response(pdrv, DLT634_5104);
			}
		}
	}
    
    return 1;
}

/**
  * @brief : task.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void rt_channel_monitor_task(void)
{
    uint8_t pdrv;
    
    for (pdrv = 0; pdrv < DLT634_CHANNEL_MONITOR_VOLUMES; pdrv++)
    {
        rt_channel_monitor_clock(pdrv);
    }
}

/**
  * @brief : channel_monitor_init
  * @param : [none]
  * @return: [1]
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
int rt_channel_monitor_init(void)
{
    dev[DLT634_5101SLAVE_DISK0] = rt_device_find(RT_UART4_NAME); 
	
	if ((uint16_t)g_Parameter[UART_PORT] == 0)
	{    
	    dev[DLT634_5101SLAVE_DISK1] = rt_device_find(RT_USART1_NAME);
	}
	else
	{
	    dev[DLT634_5101SLAVE_DISK1] = rt_device_find(RT_USART1_NAME);
	}
	
    rt_device_open(dev[DLT634_5101SLAVE_DISK0], RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    rt_device_open(dev[DLT634_5101SLAVE_DISK1], RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);  
    return 1;
}
//INIT_APP_EXPORT(rt_channel_monitor_init);


