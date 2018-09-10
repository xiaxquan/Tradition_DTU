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


FifoHandle UDP_FinshFifoHandle;
static uint8_t UDP_FinshBuffer[UDP_DEMO_BUFSIZE];
static PointUint8 UDP_FinshBufferPack;


uint8_t udp_demo_recvbuf[UDP_DEMO_BUFSIZE];			//UDP�������ݻ�����



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
int8_t UDP_NetconnSendString(struct netconn* udpconn, uint8_t* sendString)
{
	static struct netbuf* sendNetbuf = NULL;
	void* pointer = NULL;
	int8_t error = 0;

	/*���д��μ��*/
	if((NULL == udpconn) || (NULL == sendString))
	{
		/*������������*/
		rt_kprintf("ERROR:%s-Line %d: Transfer Parameters Error!\r\n", __FILE__, __LINE__);
		return 1;
	}

	sendNetbuf = netbuf_new();
	if(NULL == sendNetbuf)
	{
		/*netbuf����ʧ��*/
		rt_kprintf("ERROR:%s-Line %d: Netbuf Creat Error!\r\n", __FILE__, __LINE__);
		return 2;
	}
	pointer = netbuf_alloc(sendNetbuf, strlen((char*)sendString));
	if(NULL == pointer)
	{
		/*��̬�ڴ�����ʧ��*/
		rt_kprintf("ERROR:%s-Line %d: Netbuf Alloc Error!\r\n", __FILE__, __LINE__);
		return 2;
	}
	
	memcpy(sendNetbuf->p->payload, (void*)sendString, strlen((char*)sendString));
	error = netconn_send(udpconn, sendNetbuf);
	if(0 != error)
	{
		/*��������ʧ��*/
		rt_kprintf("ERROR:%s-Line %d: Netbuf Send Error!\r\n", __FILE__, __LINE__);
		netbuf_delete(sendNetbuf);
		return error;
	}
	rt_kprintf("1:%s", (char*)sendString);
	rt_kprintf("2:%s", (char*)sendNetbuf->p->payload);
	netbuf_delete(sendNetbuf);      	//ɾ��buf
	
	return 0;
}


/**
  * @brief : ʹ��UDPЭ�飬netconn�ӿڽ�������PC�����ַ���
  * @param : udpconn netconn�ӿڵĽṹ��ָ��
  * @return: 0���ɹ�  1��ʧ�� 2�����δ���
  * @updata: 
  */
uint8_t UDP_NetconnReceiveString(struct netconn* udpconn)
{
	static struct netbuf* recvbuf = NULL;
	rt_base_t level;			/*�����жϵķ���ֵ*/
	struct pbuf* q = NULL;
	uint32_t data_len = 0;
	
	netconn_recv(udpconn, &recvbuf); //��������
	if(recvbuf != NULL)          //���յ�����
	{
		level = rt_hw_interrupt_disable(); //���ж�
		memset(udp_demo_recvbuf, 0, UDP_DEMO_BUFSIZE);  //���ݽ��ջ���������
		
		for(q=recvbuf->p; q!=NULL; q=q->next)  //����������pbuf����
		{
			//�ж�Ҫ������UDP_DEMO_RX_BUFSIZE�е������Ƿ����UDP_DEMO_RX_BUFSIZE��ʣ��ռ�,�������
			//�Ļ���ֻ����UDP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ�����,����Ļ����������е�����
			if(q->len > (UDP_DEMO_BUFSIZE-data_len))
			{
				memcpy((udp_demo_recvbuf + data_len), q->payload, (UDP_DEMO_BUFSIZE - data_len));//��������
				FinshStringEnqueue(&UDP_FinshFifoHandle, q->payload, (UDP_DEMO_BUFSIZE - data_len));
			}
			else
			{
				memcpy((udp_demo_recvbuf + data_len), q->payload, q->len);
				FinshStringEnqueue(&UDP_FinshFifoHandle, q->payload, q->len);
			}
			data_len += q->len;  	
			if(data_len > UDP_DEMO_BUFSIZE)
			{
				break; //����TCP�ͻ��˽�������,����
			}
		}
//		UDP_NetconnSendString(udpconn, udp_demo_recvbuf);
//		netconn_send(udpconn, recvbuf);
		rt_kprintf("1:%s", (char*)udp_demo_recvbuf);
		rt_kprintf("2:%s", (char*)recvbuf->p->payload);
		data_len = 0;  //������ɺ�data_lenҪ����
		rt_hw_interrupt_enable(level);  //���ж�
		netbuf_delete(recvbuf);      //ɾ��buf
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
	FifoInit(&UDP_FinshFifoHandle, &UDP_FinshBufferPack);
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

/*****************************File End**********************************/


