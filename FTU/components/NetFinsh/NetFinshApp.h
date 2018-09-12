/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.h
  * @brief:     使用网络接口，UDP协议实现远程登录开发板finsh的应用函数
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */
  
#ifndef __NET_FINSH_APP_H__
#define __NET_FINSH_APP_H__

#include "common_data.h"
#include "lwip/netbuf.h"
#include "lwip/api.h"
#include "buffer.h"


#define NET_FINSH_BUFSIZE		  512		//finsh接收缓冲区大小
#define NET_PRINTF_BUFSIZE		2048	//打印输出缓冲区大小
#define UDP_SERVE_BUFSIZE		  512   //UDP通信服务缓冲区大小

extern struct netconn* g_NetFinshNetconn;
extern bool NetFinshFlag;
extern struct netconn* g_UDP_ServeNetconn;
extern bool UDP_ServeFlag;
extern FifoHandle* FinshReceiveFifoHandle;
extern FifoHandle* PrintfFifoHandle;
extern FifoHandle* UDP_ServeFifoHandle;


void NetFinshIpSet(struct lwip_dev* lwip);
int8_t UDP_NetconnSendString(struct netconn* udpNetconn, uint8_t* sendString);
uint8_t UDP_NetconnReceiveString(struct netconn* udpNetConn, FifoHandle* fifoHandle);
void NetFinshFifoFree(void);
void UDP_ServeFifoFree(void);
uint8_t FinshFifoInit(void);
uint8_t PrintfFifoInit(void);
uint8_t UDP_ServeFifoInit(void);
void FinshStringEnqueue(FifoHandle *handle, uint8_t* indata, uint32_t size);
char FinshCharDequeue(FifoHandle *handle);
char NetGetchar(void);
void NetFinsh_kprintf(const char *fmt, ...);


#endif

