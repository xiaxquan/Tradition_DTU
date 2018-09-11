/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.h
  * @brief:     使用网络UDP实现远程登录开发板finsh的应用函数
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */
  
#ifndef __UDP_FINSH_APP_H__
#define __UDP_FINSH_APP_H__

#include "common_data.h"
#include "lwip/netbuf.h"
#include "lwip/api.h"
#include "buffer.h"


#define UDP_DEMO_BUFSIZE		512	//缓冲区大小


extern struct netconn* g_UDP_Netconn;
extern bool UDP_FinshFlag;
extern FifoHandle UDP_FinshReceiveFifoHandle;
extern FifoHandle UDP_FinshPrintfFifoHandle;


void UDP_FinshIpSet(struct lwip_dev* lwip);
int8_t UDP_NetconnSendString(struct netconn* udpNetconn, uint8_t* sendString);
uint8_t UDP_NetconnReceiveString(struct netconn* udpNetConn);
void UDP_FinshFifoInit(void);
void UDP_PrintfFifoInit(void);
void FinshStringEnqueue(FifoHandle *handle, uint8_t* indata, uint32_t size);
char FinshCharDequeue(FifoHandle *handle);
char UDP_getchar(void);
void UDP_finsh_kprintf(const char *fmt, ...);

#endif

