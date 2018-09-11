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
#include ".\MultiThread\multi_thread.h"


/*****************************宏定义************************************/
#define REMOTE_PORT				2404	//远端端口
#define LOCAL_PORT				10000	//本地端口


/****************************全局变量***********************************/



/***************************静态全局变量********************************/
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
	err_t err;
	struct ip_addr destipAddr;
	rt_base_t level;
	struct lwip_dev lwipDev;
	
	UDP_FinshFifoInit();		/*初始化fifo*/

	
	do
	{
		rt_kprintf("UDP start\r\n");
		rt_thread_delay(1000);
		UDP_FinshIpSet(&lwipDev);
		
		LWIP_UNUSED_ARG(param);	

		g_UDP_Netconn = netconn_new(NETCONN_UDP);  //创建一个UDP链接
		
		
		if(g_UDP_Netconn != NULL)  //创建UDP链接成功
		{
			g_UDP_Netconn->recv_timeout = 100;
			err = netconn_bind(g_UDP_Netconn, IP_ADDR_ANY, LOCAL_PORT); 
			IP4_ADDR(&destipAddr, lwipDev.remoteip[0], lwipDev.remoteip[1], lwipDev.remoteip[2], lwipDev.remoteip[3]); //构造目的IP地址
			netconn_connect(g_UDP_Netconn, &destipAddr, REMOTE_PORT); 	//连接到远端主机
			if(err == ERR_OK)//绑定完成
			{
				while(1)
				{
					UDP_NetconnReceiveString(g_UDP_Netconn);
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

