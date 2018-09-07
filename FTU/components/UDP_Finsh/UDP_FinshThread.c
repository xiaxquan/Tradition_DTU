/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshThread.c
  * @brief:     使用网络UDP实现远程登录开发板finsh的相关任务
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */


/*****************************include***********************************/
#include "lwip/api.h"
#include "UDP_FinshApp.h"
#include "UDP_FinshThread.h"
#include "tcp_server.h"
#include "rtconfig.h"
#include "rtthread.h"
#include ".\MultiThread\multi_thread.h"


/*****************************宏定义************************************/
#define UDP_DEMO_BUFSIZE		2000  //缓冲区大小


/****************************全局变量***********************************/
uint8_t udp_flag;  //UDP数据发送标志位
uint8_t udp_demo_recvbuf[UDP_DEMO_BUFSIZE];	//UDP接收数据缓冲区
uint8_t udp_demo_sendbuf[UDP_DEMO_BUFSIZE];	//UDP发送数据缓冲区

/***************************静态局部变量********************************/
#if RT_USING_UDP_FINSH 
static struct rt_thread *rt_thread_UDP_finsh;
static rt_uint8_t *rt_thread_UDP_finsh_stack;
#endif /* END RT_USING_UDP_FINSH */


/*****************************Function**********************************/

/**
  * @brief : udp finsh thread entry
  * @param : none
  * @return: none
  * @updata: 
  */  
#if RT_USING_UDP_FINSH
static void rt_udp_finsh_thread_entry(void *param)
{ 
	uint32_t cpu_sr;
	err_t err;
	static struct netconn* udpconn = NULL;
	static struct netbuf* recvbuf = NULL;
	static struct netbuf* sentbuf = NULL;
	struct ip_addr destipaddr;
	uint32_t data_len = 0;
	struct pbuf* q = NULL;
	rt_base_t level;
	struct lwip_dev lwipdev;
	do
	{
		rt_kprintf("UDP start\r\n");
		rt_thread_delay(1000);
		UDP_FinshIpSet(&lwipdev);
		
		LWIP_UNUSED_ARG(param);	
	//	UDP_CREAT:
		udpconn = netconn_new(NETCONN_UDP);  //创建一个UDP链接
		
		
		if(udpconn != NULL)  //创建UDP链接成功
		{
			udpconn->recv_timeout = 100;
			err = netconn_bind(udpconn, IP_ADDR_ANY, 10000); 
			IP4_ADDR(&destipaddr, lwipdev.remoteip[0], lwipdev.remoteip[1], lwipdev.remoteip[2], lwipdev.remoteip[3]); //构造目的IP地址
			netconn_connect(udpconn, &destipaddr, 2404); 	//连接到远端主机
			if(err == ERR_OK)//绑定完成
			{
				while(1)
				{
					if((udp_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //有数据要接收
					{
						sentbuf = netbuf_new();
						netbuf_alloc(sentbuf, strlen((char *)udp_demo_sendbuf));
						memcpy(sentbuf->p->payload, (void*)udp_demo_sendbuf, strlen((char*)udp_demo_sendbuf));
						err = netconn_send(udpconn, sentbuf);  	//将netbuf中的数据发送出去
						if(err != ERR_OK)
						{
							rt_kprintf("send failure\r\n");
							netbuf_delete(sentbuf);      //删除buf
						}
						udp_flag &= ~LWIP_SEND_DATA;	//清除数据发送标志
						netbuf_delete(sentbuf);      	//删除buf
					}
					
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
							}
							else
							{
								memcpy((udp_demo_recvbuf + data_len), q->payload, q->len);
							}
							data_len += q->len;  	
							if(data_len > UDP_DEMO_BUFSIZE)
							{
								break; //超出TCP客户端接收数组,跳出
							}
						}
						rt_hw_interrupt_enable(level);  //开中断
						data_len = 0;  //复制完成后data_len要清零
						rt_kprintf("%s\r\n", udp_demo_recvbuf);  //打印接收到的数据
						netbuf_delete(recvbuf);      //删除buf
					}
					else
					{
						rt_thread_delay(20);
					}
				}
			}
			else
			{
				rt_kprintf("UDP bind faliure\r\n");
			}
		}
		else
		{
			rt_kprintf("UDP connect failure\r\n");
	//		goto UDP_CREAT;
		}
	}
	while(1);
}
#endif


/**
  * @brief : Start udp finsh thread
  * @param : none
  * @return: none
  * @updata: 
  */
#if RT_USING_UDP_FINSH
static void udp_finsh_thread_start(void* param)
{
	rt_thread_t tid; 

    tid = rt_thread_create(UDP_FINSH_THREAD_NAME, 
                           rt_udp_finsh_thread_entry, 
                           param, 
                           UDP_FINSH_THREAD_STACK_SIZE, 
                           13, 
                           UDP_FINSH_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("UDP finsh thread start \r\n"); 
    }   
}
#endif


/**
  * @brief : 使用dp83848和UDP协议完成finsh远程登录的功能
  * @param : none
  * @return: 0:成功; 1:失败
  * @updata: 
  */
#if RT_USING_UDP_FINSH
uint8_t rt_UDP_FinshThread_start(void)
{
	
    udp_finsh_thread_start(RT_NULL);
	return RT_EOK;
}
INIT_APP_EXPORT(rt_UDP_FinshThread_start);
#endif


/*****************************File End**********************************/

