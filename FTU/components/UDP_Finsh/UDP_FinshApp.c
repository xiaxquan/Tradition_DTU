/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.c
  * @brief:     ʹ������ӿڣ�UDPЭ��ʵ��Զ�̵�¼������finsh��Ӧ�ú���
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */
 

/*****************************include***********************************/
#include "UDP_FinshApp.h"
#include <string.h>
#include "board.h"
#include "rtdef.h"
#include <stdbool.h>

/****************************ȫ�ֱ���***********************************/
struct netconn* g_NetFinshNetconn = NULL;
bool NET_FinshFlag = false;

/*����FIFO*/
FifoHandle* FinshReceiveFifoHandle = NULL;
static uint8_t* FinshBuffer = NULL;
static PointUint8* FinshBufferPack = NULL;

/*��ӡFIFO*/
FifoHandle* PrintfFifoHandle = NULL;
static uint8_t* PrintfBuffer = NULL;
static PointUint8* PrintfBufferPack = NULL;


/*****************************Function**********************************/

/**
  * @brief : udp finsh thread IP address set
  * @param : lwip lwip��ַ���ýṹ��ָ��
  * @return: none
  * @updata: 
  */
void NetFinshIpSet(struct lwip_dev* lwip)
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
			if(q->len > NET_FINSH_BUFSIZE)
			{
				FinshStringEnqueue(FinshReceiveFifoHandle, q->payload, NET_FINSH_BUFSIZE);
			}
			else
			{
				FinshStringEnqueue(FinshReceiveFifoHandle, q->payload, q->len);
			}	
			if(dataLenth > NET_FINSH_BUFSIZE)
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
  * @brief : ʹ������ʵ��finsh�еķ��ͺͽ��ն��ж�̬�ڴ��ͷ�
  * @param : none
  * @return: none
  * @updata: 
  */
void NetFinshFifoFree(void)
{
	if(NULL != FinshReceiveFifoHandle)
	{
		rt_free(FinshReceiveFifoHandle);
		FinshReceiveFifoHandle = NULL;
	}
	if(NULL != FinshBuffer)
	{
		rt_free(FinshBuffer);
		FinshBuffer = NULL;
	}
	if(NULL != FinshBufferPack)
	{
		rt_free(FinshBufferPack);
		FinshBufferPack = NULL;
	}
	
	if(NULL != PrintfFifoHandle)
	{
		rt_free(PrintfFifoHandle);
		PrintfFifoHandle = NULL;
	}
	if(NULL != PrintfBuffer)
	{
		rt_free(PrintfBuffer);
		PrintfBuffer = NULL;
	}
	if(NULL != PrintfBufferPack)
	{
		rt_free(PrintfBufferPack);
		PrintfBufferPack = NULL;
	}

}


/**
  * @brief : ʹ��UDPЭ���finsh��FIFO��ʼ��
  * @param : none
  * @return: 0:�ɹ�    1:ʧ��
  * @updata: 
  */
uint8_t FinshFifoInit(void)
{
	uint8_t err = 0;
	FinshReceiveFifoHandle = rt_malloc(sizeof(FifoHandle));
	if(NULL == FinshReceiveFifoHandle)
	{
		rt_kprintf("%s:[%s] #-%d: FinshReceiveFifoHandle Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}
	FinshBuffer = rt_malloc(sizeof(uint8_t) * NET_FINSH_BUFSIZE);
	if(NULL == FinshReceiveFifoHandle)
	{
		rt_kprintf("%s:[%s] #-%d: FinshBuffer Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}
	FinshBufferPack = rt_malloc(sizeof(PointUint8));
	if(NULL == FinshReceiveFifoHandle)
	{
		rt_kprintf("%s:[%s] #-%d: FinshBufferPack Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}

	if(0 == err)
	{
		FinshBufferPack->len = NET_FINSH_BUFSIZE;
		FinshBufferPack->pData = FinshBuffer;
		FifoInit(FinshReceiveFifoHandle, FinshBufferPack);
	}
	else
	{
		NetFinshFifoFree();
	}

	return err;
}


/**
  * @brief : ʹ��UDPЭ���printf��FIFO��ʼ��
  * @param : none
  * @return: 0:�ɹ�    1:ʧ��
  * @updata: 
  */
uint8_t PrintfFifoInit(void)
{
	uint8_t err = 0;
	PrintfFifoHandle = rt_malloc(sizeof(FifoHandle));
	if(NULL == PrintfFifoHandle)
	{
		rt_kprintf("%s:[%s] #-%d: PrintfFifoHandle Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}
	PrintfBuffer = rt_malloc(sizeof(uint8_t) * NET_PRINTF_BUFSIZE);
	if(NULL == PrintfBuffer)
	{
		rt_kprintf("%s:[%s] #-%d: PrintfBuffer Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}
	PrintfBufferPack = rt_malloc(sizeof(PointUint8));
	if(NULL == PrintfBufferPack)
	{
		rt_kprintf("%s:[%s] #-%d: PrintfBufferPack Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}

	if(0 == err)
	{
		PrintfBufferPack->len = NET_PRINTF_BUFSIZE;
		PrintfBufferPack->pData = PrintfBuffer;
		FifoInit(PrintfFifoHandle, PrintfBufferPack);
	}
	else
	{
		NetFinshFifoFree();
	}

	return err;
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
char NetGetchar(void)
{
	return FinshCharDequeue(FinshReceiveFifoHandle);
}

/**
  * @brief : ʹ��UDP�Ĵ�ӡ����
  * @param : ��ӡ����Ϣ
  * @return: none
  * @updata: 
  */
void NetFinsh_kprintf(const char *fmt, ...)
{
	va_list args;
    rt_size_t length;
    char rt_log_buf[RT_CONSOLEBUF_SIZE] = {0};

    va_start(args, fmt);
	
	length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1)
        length = RT_CONSOLEBUF_SIZE - 1;

	if(true == NET_FinshFlag)		/*UDP_FinshFlagΪ1˵��UDP��ӡ�Ѿ���ʼ��������ʹ����*/
	{
		FinshStringEnqueue(PrintfFifoHandle, (uint8_t*)rt_log_buf, length);
	}
	else
	{
		#if defined(RT_USING_DEVICE) && defined(RT_USING_CONSOLE)
			static rt_device_t _console_device = RT_NULL;
		#endif

		_console_device = rt_console_get_device();			/*��ȡ��ǰ����̨����Ĵ����豸��*/
		rt_uint16_t old_flag = _console_device->open_flag;

        _console_device->open_flag |= RT_DEVICE_FLAG_STREAM;
        rt_device_write(_console_device, 0, rt_log_buf, length);
        _console_device->open_flag = old_flag;
		
	}
	
	va_end(args);
}

/*****************************File End**********************************/


