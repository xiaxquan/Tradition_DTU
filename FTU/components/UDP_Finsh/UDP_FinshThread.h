/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshThread.h
  * @brief:     使用网络UDP实现远程登录开发板finsh的相关任务
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */
  
#ifndef __UDP_FINSH_THREAD_H__
#define __UDP_FINSH_THREAD_H__

#include "rtthread.h"


#define UDP_FINSH_THREAD_NAME											"UDP_finsh"
#define UDP_FINSH_THREAD_PRIORITY                                       (13)
#define UDP_FINSH_THREAD_STACK_SIZE                                     (4096)
#define UDP_FINSH_THREAD_TIMESLICE                                      (20)

#define UDP_PRINTF_THREAD_NAME                                          "UDP_printf"
#define UDP_PRINTF_THREAD_PRIORITY                                      (13)
#define UDP_PRINTF_THREAD_STACK_SIZE                                    (4096)
#define UDP_PRINTF_THREAD_TIMESLICE                                     (20)


#endif

