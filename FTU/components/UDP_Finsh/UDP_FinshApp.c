/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.c
  * @brief:     使用网络UDP实现远程登录开发板finsh的应用函数
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


uint8_t udp_demo_recvbuf[UDP_DEMO_BUFSIZE];			//UDP接收数据缓冲区



/*****************************Function**********************************/

/**
  * @brief : udp finsh thread IP address set
  * @param : lwip lwip地址设置结构体指针
  * @return: none
  * @updata: 
  */
void UDP_FinshIpSet(struct lwip_dev* lwip)
{
	uint8_t i = 0;
	
	/*设置远端IP*/
	lwip->remoteip[0] = g_EthDP83848.remoteip[0];
	lwip->remoteip[1] = g_EthDP83848.remoteip[1];
	lwip->remoteip[2] = g_EthDP83848.remoteip[2];
	lwip->remoteip[3] = g_EthDP83848.remoteip[3];
	
	/*设置本地IP*/
	lwip->ip[0] = g_EthDP83848.ip[0];
	lwip->ip[1] = g_EthDP83848.ip[1];
	lwip->ip[2] = g_EthDP83848.ip[2];
	lwip->ip[3] = g_EthDP83848.ip[3];
	
	/*设置子网掩码*/
	lwip->netmask[0] = g_EthDP83848.netmask[0];
	lwip->netmask[1] = g_EthDP83848.netmask[1];
	lwip->netmask[2] = g_EthDP83848.netmask[2];
	lwip->netmask[3] = g_EthDP83848.netmask[3];
	
	/*设置网关*/
	lwip->gateway[0] = g_EthDP83848.gateway[0];
	lwip->gateway[1] = g_EthDP83848.gateway[1];
	lwip->gateway[2] = g_EthDP83848.gateway[2];
	lwip->gateway[3] = g_EthDP83848.gateway[3];
	
	/*设置MAC地址*/
	lwip->mac[0] = g_EthDP83848.mac[0];
	lwip->mac[1] = g_EthDP83848.mac[1];
	lwip->mac[2] = g_EthDP83848.mac[2];
	lwip->mac[3] = g_EthDP83848.mac[3];
	lwip->mac[4] = g_EthDP83848.mac[4];
	lwip->mac[5] = g_EthDP83848.mac[5];
	
	lwip->dhcpstatus = g_EthDP83848.dhcpstatus;
	
}


/**
  * @brief : 使用UDP协议，netconn接口向PC机发送信息
  * @param : udpconn netconn接口的结构体指针
  * @param : sendBuf 要发送的字符串
  * @return: 0：成功  1：传参错误  2：netbuf创建失败或动态内存申请失败  其他：发送错误
  * @updata: 
  */
int8_t UDP_NetconnSendString(struct netconn* udpconn, uint8_t* sendString)
{
	static struct netbuf* sendNetbuf = NULL;
	void* pointer = NULL;
	int8_t error = 0;

	/*进行传参检查*/
	if((NULL == udpconn) || (NULL == sendString))
	{
		/*所传参数错误*/
		rt_kprintf("ERROR:%s-Line %d: Transfer Parameters Error!\r\n", __FILE__, __LINE__);
		return 1;
	}

	sendNetbuf = netbuf_new();
	if(NULL == sendNetbuf)
	{
		/*netbuf创建失败*/
		rt_kprintf("ERROR:%s-Line %d: Netbuf Creat Error!\r\n", __FILE__, __LINE__);
		return 2;
	}
	pointer = netbuf_alloc(sendNetbuf, strlen((char*)sendString));
	if(NULL == pointer)
	{
		/*动态内存申请失败*/
		rt_kprintf("ERROR:%s-Line %d: Netbuf Alloc Error!\r\n", __FILE__, __LINE__);
		return 2;
	}
	
	memcpy(sendNetbuf->p->payload, (void*)sendString, strlen((char*)sendString));
	error = netconn_send(udpconn, sendNetbuf);
	if(0 != error)
	{
		/*发送数据失败*/
		rt_kprintf("ERROR:%s-Line %d: Netbuf Send Error!\r\n", __FILE__, __LINE__);
		netbuf_delete(sendNetbuf);
		return error;
	}
	rt_kprintf("1:%s", (char*)sendString);
	rt_kprintf("2:%s", (char*)sendNetbuf->p->payload);
	netbuf_delete(sendNetbuf);      	//删除buf
	
	return 0;
}


/**
  * @brief : 使用UDP协议，netconn接口接收来自PC机的字符串
  * @param : udpconn netconn接口的结构体指针
  * @return: 0：成功  1：失败 2：传参错误
  * @updata: 
  */
uint8_t UDP_NetconnReceiveString(struct netconn* udpconn)
{
	static struct netbuf* recvbuf = NULL;
	rt_base_t level;			/*开关中断的返回值*/
	struct pbuf* q = NULL;
	uint32_t data_len = 0;
	
	netconn_recv(udpconn, &recvbuf); //接收数据
	if(recvbuf != NULL)          //接收到数据
	{
		level = rt_hw_interrupt_disable(); //关中断
		memset(udp_demo_recvbuf, 0, UDP_DEMO_BUFSIZE);  //数据接收缓冲区清零
		
		for(q=recvbuf->p; q!=NULL; q=q->next)  //遍历完整个pbuf链表
		{
			//判断要拷贝到UDP_DEMO_RX_BUFSIZE中的数据是否大于UDP_DEMO_RX_BUFSIZE的剩余空间,如果大于
			//的话就只拷贝UDP_DEMO_RX_BUFSIZE中剩余长度的数据,否则的话机拷贝所有的数据
			if(q->len > (UDP_DEMO_BUFSIZE-data_len))
			{
				memcpy((udp_demo_recvbuf + data_len), q->payload, (UDP_DEMO_BUFSIZE - data_len));//拷贝数据
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
				break; //超出TCP客户端接收数组,跳出
			}
		}
//		UDP_NetconnSendString(udpconn, udp_demo_recvbuf);
//		netconn_send(udpconn, recvbuf);
		rt_kprintf("1:%s", (char*)udp_demo_recvbuf);
		rt_kprintf("2:%s", (char*)recvbuf->p->payload);
		data_len = 0;  //复制完成后data_len要清零
		rt_hw_interrupt_enable(level);  //开中断
		netbuf_delete(recvbuf);      //删除buf
	}
	return 0;
}


/**
  * @brief : 使用UDP协议的finsh的FIFO初始化
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
  * @brief : finsh接收到的多个字符入队
  * @param : handle FIFO句柄
  * @param : indata 要入队的字符串
  * @param : size 要入队的字节大小
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
  * @brief : finsh的FIFO出队
  * @param : handle FIFO句柄
  * @return: 出队的字符
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


