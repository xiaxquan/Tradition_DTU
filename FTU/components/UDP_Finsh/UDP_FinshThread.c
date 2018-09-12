/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshThread.c
  * @brief:     ʹ������ӿڣ�UDPЭ��ʵ��Զ�̵�¼������finsh���������
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


/*****************************�궨��************************************/
#define REMOTE_PORT				2404	//Զ�˶˿�
#define LOCAL_PORT				10000	//���ض˿�
#define PRINT_BUFFER_SIZE		128		//��ӡ����Ļ�������С





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
	
	ret = FinshFifoInit();		/*��ʼ��fifo*/
	if(!ret)
	{
		ret = PrintfFifoInit();
	}
	
	while(!ret)
	{
		rt_thread_delay(1000);
		NetFinshIpSet(&lwipDev);
		
		LWIP_UNUSED_ARG(param);	

		g_NetFinshNetconn = netconn_new(NETCONN_UDP);  //����һ��UDP����
		
		
		if(g_NetFinshNetconn != NULL)  //����UDP���ӳɹ�
		{
			g_NetFinshNetconn->recv_timeout = 100;
			err = netconn_bind(g_NetFinshNetconn, IP_ADDR_ANY, LOCAL_PORT); 
			IP4_ADDR(&destipAddr, lwipDev.remoteip[0], lwipDev.remoteip[1], lwipDev.remoteip[2], lwipDev.remoteip[3]); //����Ŀ��IP��ַ
			netconn_connect(g_NetFinshNetconn, &destipAddr, REMOTE_PORT); 	//���ӵ�Զ������
			if(err == ERR_OK)//�����
			{
				/*UDP�����Ѿ�������֮�����ʹ�����ڵĴ�ӡ������*/
				NET_FinshFlag = true;
				rt_kprintf("Net finsh Init Success\r\n");
				
				while(1)
				{
					/*�ȴ����գ������յ����ַ����*/
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
	
	NetFinshFifoFree();		/*�ͷŽ��պͷ��͵Ķ������õ��Ķ�̬������ڴ�*/
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
  * @brief : ʹ��dp83848��UDPЭ�����finshԶ�̵�¼�Ĺ���
  * @param : none
  * @return: 0:�ɹ�; 1:ʧ��
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

