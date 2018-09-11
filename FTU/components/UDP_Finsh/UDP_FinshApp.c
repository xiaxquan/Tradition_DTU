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


/****************************全局变量***********************************/
struct netconn* g_UDP_Netconn = NULL;
bool UDP_FinshFlag = 0;

/*接收FIFO*/
FifoHandle UDP_FinshReceiveFifoHandle;
static uint8_t UDP_FinshBuffer[UDP_DEMO_BUFSIZE];
static PointUint8 UDP_FinshBufferPack;

/*打印FIFO*/
FifoHandle UDP_FinshSendFifoHandle;
static uint8_t UDP_PrintfBuffer[UDP_DEMO_BUFSIZE];
static PointUint8 UDP_PrintfBufferPack;


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
  * @brief : 使用UDP协议，netconn接口接收来自PC机的字符串
  * @param : udpconn netconn接口的结构体指针
  * @return: 0：成功  1：失败 2：传参错误
  * @updata: 
  */
uint8_t UDP_NetconnReceiveString(struct netconn* udpNetConn)
{
	static struct netbuf* recvNetBuf = NULL;
	rt_base_t level;			/*开关中断的返回值*/
	struct pbuf* q = NULL;
	uint32_t dataLenth = 0;
	
	netconn_recv(udpNetConn, &recvNetBuf); //接收数据
	if(recvNetBuf != NULL)          //接收到数据
	{
		level = rt_hw_interrupt_disable(); //关中断
		
		for(q=recvNetBuf->p; q!=NULL; q=q->next)  //遍历完整个pbuf链表
		{
			//判断要拷贝到UDP_DEMO_RX_BUFSIZE中的数据是否大于UDP_DEMO_RX_BUFSIZE的剩余空间,如果大于
			//的话就只拷贝UDP_DEMO_RX_BUFSIZE中剩余长度的数据,否则的话机拷贝所有的数据
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
				break; //超出TCP客户端接收数组,跳出
			}
		}
		rt_hw_interrupt_enable(level);  //开中断
		netbuf_delete(recvNetBuf);      //删除buf
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
	FifoInit(&UDP_FinshReceiveFifoHandle, &UDP_FinshBufferPack);
}


/**
  * @brief : 使用UDP协议的printf的FIFO初始化
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


/**
  * @brief : 使用UDP获取字符
  * @param : none
  * @return: 出队的字符
  * @updata: 
  */
char UDP_getchar(void)
{
	return FinshCharDequeue(&UDP_FinshReceiveFifoHandle);
}

/**
  * @brief : 使用UDP的打印函数
  * @param : 打印的信息
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
	
	if(UDP_FinshFlag)		/*UDP_FinshFlag为1说明UDP打印已经初始化，可以使用了*/
	{
//		UDP_NetconnSendString(g_UDP_Netconn, (void*)rt_log_buf);
		/*需加入互斥操作*/
		
//		FinshStringEnqueue(&UDP_FinshSendFifoHandle, (uint8_t*)rt_log_buf, length);
		
	}
	else
	{
		rt_hw_console_output(rt_log_buf);
	}
	
	va_end(args);
}

/*****************************File End**********************************/


