/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshThread.h
  * @brief:     ʹ������ӿڣ�UDPЭ��ʵ��Զ�̵�¼������finsh���������
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */
  
#ifndef __NET_FINSH_THREAD_H__
#define __NET_FINSH_THREAD_H__

#include "rtthread.h"


#define NET_FINSH_THREAD_NAME                                           "net_finsh"
#define NET_FINSH_THREAD_PRIORITY                                       (13)
#define NET_FINSH_THREAD_STACK_SIZE                                     (4096)
#define NET_FINSH_THREAD_TIMESLICE                                      (20)

#define UDP_SERVE_THREAD_NAME                                           "udp_serve"
#define UDP_SERVE_THREAD_PRIORITY                                       (20)
#define UDP_SERVE_THREAD_STACK_SIZE                                     (4096)
#define UDP_SERVE_THREAD_TIMESLICE                                      (20)

#if RT_USING_NET_FINSH
uint8_t rt_NetFinsh_thread_start(void);
#endif

#if RT_USING_UDP_SERVE
uint8_t rt_UDP_CommunicateServe_thread_start(void);
#endif


#endif

