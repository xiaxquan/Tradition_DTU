/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.h
  * @brief:     使用网络接口，UDP协议实现远程登录开发板finsh的应用函数
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


#define NET_FINSH_BUFSIZE		512		//finsh接收缓冲区大小
#define NET_PRINTF_BUFSIZE		2048	//打印输出缓冲区大小

extern struct netconn* g_NetFinshNetconn;
extern bool NET_FinshFlag;
extern FifoHandle* FinshReceiveFifoHandle;
extern FifoHandle* PrintfFifoHandle;


void NetFinshIpSet(struct lwip_dev* lwip);
int8_t UDP_NetconnSendString(struct netconn* udpNetconn, uint8_t* sendString);
uint8_t UDP_NetconnReceiveString(struct netconn* udpNetConn);
void NetFinshFifoFree(void);
uint8_t FinshFifoInit(void);
uint8_t PrintfFifoInit(void);
void FinshStringEnqueue(FifoHandle *handle, uint8_t* indata, uint32_t size);
char FinshCharDequeue(FifoHandle *handle);
char NetGetchar(void);
void NetFinsh_kprintf(const char *fmt, ...);

#endif

