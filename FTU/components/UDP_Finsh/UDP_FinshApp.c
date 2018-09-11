/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.c
  * @brief:     ʹ������UDPʵ��Զ�̵�¼������finsh��Ӧ�ú���
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */
 

/*****************************include***********************************/
#include "UDP_FinshApp.h"
#include <string.h>


/****************************ȫ�ֱ���***********************************/
struct netconn* g_UDP_Netconn = NULL;
bool UDP_FinshFlag = 0;

/*����FIFO*/
FifoHandle UDP_FinshReceiveFifoHandle;
static uint8_t UDP_FinshBuffer[UDP_DEMO_BUFSIZE];
static PointUint8 UDP_FinshBufferPack;

/*��ӡFIFO*/
FifoHandle UDP_FinshSendFifoHandle;
static uint8_t UDP_PrintfBuffer[UDP_DEMO_BUFSIZE];
static PointUint8 UDP_PrintfBufferPack;


/*****************************Function**********************************/

/**
  * @brief : udp finsh thread IP address set
  * @param : lwip lwip��ַ���ýṹ��ָ��
  * @return: none
  * @updata: 
  */
void UDP_FinshIpSet(struct lwip_dev* lwip)
{
	uint8_t i = 0;
	
	/*����Զ��IP*/
	lwip->remoteip[0] = g_EthDP83848.remoteip[0];
	lwip->remoteip[1] = g_EthDP83848.remoteip[1];
	lwip->remoteip[2] = g_EthDP83848.remoteip[2];
	lwip->remoteip[3] = g_EthDP83848.remoteip[3];
	
	/*���ñ���IP*/
	lwip->ip[0] = g_EthDP83848.ip[0];
	lwip->ip[1] = g_EthDP83848.ip[1];
	lwip->ip[2] = g_EthDP83848.ip[2];
	lwip->ip[3] = g_EthDP83848.ip[3];
	
	/*������������*/
	lwip->netmask[0] = g_EthDP83848.netmask[0];
	lwip->netmask[1] = g_EthDP83848.netmask[1];
	lwip->netmask[2] = g_EthDP83848.netmask[2];
	lwip->netmask[3] = g_EthDP83848.netmask[3];
	
	/*��������*/
	lwip->gateway[0] = g_EthDP83848.gateway[0];
	lwip->gateway[1] = g_EthDP83848.gateway[1];
	lwip->gateway[2] = g_EthDP83848.gateway[2];
	lwip->gateway[3] = g_EthDP83848.gateway[3];
	
	/*����MAC��ַ*/
	lwip->mac[0] = g_EthDP83848.mac[0];
	lwip->mac[1] = g_EthDP83848.mac[1];
	lwip->mac[2] = g_EthDP83848.mac[2];
	lwip->mac[3] = g_EthDP83848.mac[3];
	lwip->mac[4] = g_EthDP83848.mac[4];
	lwip->mac[5] = g_EthDP83848.mac[5];
	
	lwip->dhcpstatus = g_EthDP83848.dhcpstatus;
	
}


/**
  * @brief : ʹ��UDPЭ�飬netconn�ӿ���PC��������Ϣ
  * @param : udpconn netconn�ӿڵĽṹ��ָ��
  * @param : sendBuf Ҫ���͵��ַ���
  * @return: 0���ɹ�  1�����δ���  2��netbuf����ʧ�ܻ�̬�ڴ�����ʧ��  ���������ʹ���
  * @updata: 
  */
int8_t UDP_NetconnSendString(struct netconn* udpNetconn, uint8_t* sendString)
{
	struct netbuf* sendNetbuf = NULL;
	void* pointer = NULL;
	int8_t error = 0;

	sendNetbuf = netbuf_new();
	netbuf_alloc(sendNetbuf,strlen((char *)sendString));
	memcpy(sendNetbuf->p->payload,(void*)sendString,strlen((char*)sendString));
	error = netconn_send(udpNetconn, sendNetbuf);
	if(error != ERR_OK)
	{
		rt_kprintf("send error\r\n");
		netbuf_delete(sendNetbuf);
	}
	netbuf_delete(sendNetbuf);

	
	return 0;
}


/**
  * @brief : ʹ��UDPЭ�飬netconn�ӿڽ�������PC�����ַ���
  * @param : udpconn netconn�ӿڵĽṹ��ָ��
  * @return: 0���ɹ�  1��ʧ�� 2�����δ���
  * @updata: 
  */
uint8_t UDP_NetconnReceiveString(struct netconn* udpNetConn)
{
	static struct netbuf* recvNetBuf = NULL;
	rt_base_t level;			/*�����жϵķ���ֵ*/
	struct pbuf* q = NULL;
	uint32_t dataLenth = 0;
	
	netconn_recv(udpNetConn, &recvNetBuf); //��������
	if(recvNetBuf != NULL)          //���յ�����
	{
		level = rt_hw_interrupt_disable(); //���ж�
		
		for(q=recvNetBuf->p; q!=NULL; q=q->next)  //����������pbuf����
		{
			//�ж�Ҫ������UDP_DEMO_RX_BUFSIZE�е������Ƿ����UDP_DEMO_RX_BUFSIZE��ʣ��ռ�,�������
			//�Ļ���ֻ����UDP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ�����,����Ļ����������е�����
			if(q->len > UDP_DEMO_BUFSIZE)
			{
				FinshStringEnqueue(&UDP_FinshReceiveFifoHandle, q->payload, UDP_DEMO_BUFSIZE);
			}
			else
			{
				FinshStringEnqueue(&UDP_FinshReceiveFifoHandle, q->payload, q->len);
			}	
			if(dataLenth > UDP_DEMO_BUFSIZE)
			{
				break; //����TCP�ͻ��˽�������,����
			}
		}
		rt_hw_interrupt_enable(level);  //���ж�
		netbuf_delete(recvNetBuf);      //ɾ��buf
	}
	return 0;
}


/**
  * @brief : ʹ��UDPЭ���finsh��FIFO��ʼ��
  * @param : none
  * @return: none
  * @updata: 
  */
void UDP_FinshFifoInit(void)
{
	UDP_FinshBufferPack.len = UDP_DEMO_BUFSIZE;
	UDP_FinshBufferPack.pData = UDP_FinshBuffer;
	FifoInit(&UDP_FinshReceiveFifoHandle, &UDP_FinshBufferPack);
}


/**
  * @brief : ʹ��UDPЭ���printf��FIFO��ʼ��
  * @param : none
  * @return: none
  * @updata: 
  */
void UDP_PrintfFifoInit(void)
{
	UDP_PrintfBufferPack.len = UDP_DEMO_BUFSIZE;
	UDP_PrintfBufferPack.pData = UDP_PrintfBuffer;
	FifoInit(&UDP_FinshSendFifoHandle, &UDP_PrintfBufferPack);
}


/**
  * @brief : finsh���յ��Ķ���ַ����
  * @param : handle FIFO���
  * @param : indata Ҫ��ӵ��ַ���
  * @param : size Ҫ��ӵ��ֽڴ�С
  * @return: none
  * @updata: 
  */
void FinshStringEnqueue(FifoHandle *handle, uint8_t* indata, uint32_t size)
{
	uint32_t i = 0;
	for(i=0; i<size; i++)
	{
		handle->Enqueue(handle, indata[i]);
	}
}


/**
  * @brief : finsh��FIFO����
  * @param : handle FIFO���
  * @return: ���ӵ��ַ�
  * @updata: 
  */
char FinshCharDequeue(FifoHandle *handle)
{
	uint8_t dedata = 0;
	uint8_t ret = 0;
	ret = handle->Dequeue(handle, &dedata);
	if(false == ret)
	{
		return 0;
	}
	else if(true == ret)
	{
		return dedata;
	}
}


/**
  * @brief : ʹ��UDP��ȡ�ַ�
  * @param : none
  * @return: ���ӵ��ַ�
  * @updata: 
  */
char UDP_getchar(void)
{
	return FinshCharDequeue(&UDP_FinshReceiveFifoHandle);
}

/**
  * @brief : ʹ��UDP�Ĵ�ӡ����
  * @param : ��ӡ����Ϣ
  * @return: none
  * @updata: 
  */
void UDP_finsh_kprintf(const char *fmt, ...)
{
	va_list args;
    rt_size_t length;
    static char rt_log_buf[RT_CONSOLEBUF_SIZE];

    va_start(args, fmt);
	
	length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1)
        length = RT_CONSOLEBUF_SIZE - 1;
	
	if(UDP_FinshFlag)		/*UDP_FinshFlagΪ1˵��UDP��ӡ�Ѿ���ʼ��������ʹ����*/
	{
//		UDP_NetconnSendString(g_UDP_Netconn, (void*)rt_log_buf);
		/*����뻥�����*/
		
//		FinshStringEnqueue(&UDP_FinshSendFifoHandle, (uint8_t*)rt_log_buf, length);
		
	}
	else
	{
		rt_hw_console_output(rt_log_buf);
	}
	
	va_end(args);
}

/*****************************File End**********************************/


