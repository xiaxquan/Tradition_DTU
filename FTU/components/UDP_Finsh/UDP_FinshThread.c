/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshThread.c
  * @brief:     使用网络接口，UDP协议实现远程登录开发板finsh的相关任务
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
#define PRINT_BUFFER_SIZE		128		//打印输出的缓冲区大小





/*****************************Function**********************************/

/**
  * @brief : udp finsh thread entry
  * @param : none
  * @return: none
  * @updata: 
  */  
#if RT_USING_NET_FINSH
static void rt_udp_finsh_thread_entry(void *param)
{
	err_t err = 0;
	uint8_t ret = 0;
	struct ip_addr destipAddr;
	rt_base_t level;
	struct lwip_dev lwipDev;
	uint8_t i = 0;
	uint8_t printBuffer[PRINT_BUFFER_SIZE] = {0};
	
	ret = FinshFifoInit();		/*初始化fifo*/
	if(!ret)
	{
		ret = PrintfFifoInit();
	}
	
	while(!ret)
	{
		rt_thread_delay(1000);
		NetFinshIpSet(&lwipDev);
		
		LWIP_UNUSED_ARG(param);	

		g_NetFinshNetconn = netconn_new(NETCONN_UDP);  //创建一个UDP链接
		
		
		if(g_NetFinshNetconn != NULL)  //创建UDP链接成功
		{
			g_NetFinshNetconn->recv_timeout = 100;
			err = netconn_bind(g_NetFinshNetconn, IP_ADDR_ANY, LOCAL_PORT); 
			IP4_ADDR(&destipAddr, lwipDev.remoteip[0], lwipDev.remoteip[1], lwipDev.remoteip[2], lwipDev.remoteip[3]); //构造目的IP地址
			netconn_connect(g_NetFinshNetconn, &destipAddr, REMOTE_PORT); 	//连接到远端主机
			if(err == ERR_OK)//绑定完成
			{
				/*UDP链接已经创建，之后可以使用网口的打印函数了*/
				NET_FinshFlag = true;
				rt_kprintf("Net finsh Init Success\r\n");
				
				while(1)
				{
					/*等待接收，将接收到的字符入队*/
					UDP_NetconnReceiveString(g_NetFinshNetconn);

					if(true == NET_FinshFlag)
					{
						memset(printBuffer, 0, PRINT_BUFFER_SIZE);
						for(i=0; (i<PRINT_BUFFER_SIZE) && (PrintfFifoHandle->fifo.count); i++)
						{
							printBuffer[i] = FinshCharDequeue(PrintfFifoHandle);
						}
						if(0 != i)
						{
							UDP_NetconnSendString(g_NetFinshNetconn, printBuffer);
						}
						
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
		}
		NET_FinshFlag = false;
	}
	
	NetFinshFifoFree();		/*释放接收和发送的队列所用到的动态分配的内存*/
}
#endif


/**
  * @brief : Start udp finsh thread
  * @param : none
  * @return: none
  * @updata: 
  */
#if RT_USING_NET_FINSH
static void net_finsh_thread_start(void* param)
{
	rt_thread_t tid; 

    tid = rt_thread_create(NET_FINSH_THREAD_NAME, 
                           rt_udp_finsh_thread_entry, 
                           param, 
                           NET_FINSH_THREAD_STACK_SIZE, 
                           NET_FINSH_THREAD_PRIORITY, 
                           NET_FINSH_THREAD_TIMESLICE);

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
#if RT_USING_NET_FINSH
uint8_t rt_NetFinshThread_start(void)
{
    net_finsh_thread_start(RT_NULL);
	
	return RT_EOK;
}
INIT_APP_EXPORT(rt_NetFinshThread_start);
#endif





/*****************************File End**********************************/

