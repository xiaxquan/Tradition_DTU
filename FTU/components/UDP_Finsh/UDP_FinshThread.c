/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshThread.c
  * @brief:     ʹ������UDPʵ��Զ�̵�¼������finsh���������
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


/*****************************�궨��************************************/
#define UDP_DEMO_BUFSIZE		2000  //��������С


/****************************ȫ�ֱ���***********************************/
uint8_t udp_flag;  //UDP���ݷ��ͱ�־λ
uint8_t udp_demo_recvbuf[UDP_DEMO_BUFSIZE];	//UDP�������ݻ�����
uint8_t udp_demo_sendbuf[UDP_DEMO_BUFSIZE];	//UDP�������ݻ�����

/***************************��̬�ֲ�����********************************/
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
		udpconn = netconn_new(NETCONN_UDP);  //����һ��UDP����
		
		
		if(udpconn != NULL)  //����UDP���ӳɹ�
		{
			udpconn->recv_timeout = 100;
			err = netconn_bind(udpconn, IP_ADDR_ANY, 10000); 
			IP4_ADDR(&destipaddr, lwipdev.remoteip[0], lwipdev.remoteip[1], lwipdev.remoteip[2], lwipdev.remoteip[3]); //����Ŀ��IP��ַ
			netconn_connect(udpconn, &destipaddr, 2404); 	//���ӵ�Զ������
			if(err == ERR_OK)//�����
			{
				while(1)
				{
					if((udp_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //������Ҫ����
					{
						sentbuf = netbuf_new();
						netbuf_alloc(sentbuf, strlen((char *)udp_demo_sendbuf));
						memcpy(sentbuf->p->payload, (void*)udp_demo_sendbuf, strlen((char*)udp_demo_sendbuf));
						err = netconn_send(udpconn, sentbuf);  	//��netbuf�е����ݷ��ͳ�ȥ
						if(err != ERR_OK)
						{
							rt_kprintf("send failure\r\n");
							netbuf_delete(sentbuf);      //ɾ��buf
						}
						udp_flag &= ~LWIP_SEND_DATA;	//������ݷ��ͱ�־
						netbuf_delete(sentbuf);      	//ɾ��buf
					}
					
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
							}
							else
							{
								memcpy((udp_demo_recvbuf + data_len), q->payload, q->len);
							}
							data_len += q->len;  	
							if(data_len > UDP_DEMO_BUFSIZE)
							{
								break; //����TCP�ͻ��˽�������,����
							}
						}
						rt_hw_interrupt_enable(level);  //���ж�
						data_len = 0;  //������ɺ�data_lenҪ����
						rt_kprintf("%s\r\n", udp_demo_recvbuf);  //��ӡ���յ�������
						netbuf_delete(recvbuf);      //ɾ��buf
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
  * @brief : ʹ��dp83848��UDPЭ�����finshԶ�̵�¼�Ĺ���
  * @param : none
  * @return: 0:�ɹ�; 1:ʧ��
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

