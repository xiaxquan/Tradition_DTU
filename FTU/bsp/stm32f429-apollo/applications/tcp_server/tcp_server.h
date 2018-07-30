/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      tcp_server.h
  * @brief:     The tcp server application.
  * @version:   V1.0.1 
  * @author:    LEX
  * @date:      2017.12.06
  * @update:    [YYYY-MM-DD] [����������][�������]
  */
#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


/* DEFINE --------------------------------------------------------------------*/
#define  TCPSERVER2404_SOCKET0                      0 // ����SOCKET0���ڽ���TCP����ˣ��˿ں�Ϊ2404
#define  TCP_SERVER_PORT_2404                       2404 // ����DL/T634.5104��Լ����tcp server�Ķ˿�

#define  TCP_SERVER_2404_RX_BUFSIZE                 (256*14) // ����tcp server���������ݳ���
#define  TCP_SERVER_2404_TX_BUFSIZE                 512 // ����tcp server��������ݳ���

#define  LWIP_SEND_DATA                             0X80 // ����TCP server�����ݷ���
#define  LWIP_TCP_SERVER_LINKUP                     0X40 // ����TCP server��������״̬
#define  LWIP_TCP_SERVER_DISCONNECT                 0X20 // ����TCP server�Ͽ�����״̬


/* PUBLIC FUNCTION -----------------------------------------------------------*/
extern void rt_w5500_tcpserver_thread_entry(void *param); // TCP�����������߳� 
extern uint8_t w5500_tcpserver_linkstate(void); // �ж���·״̬
extern void w5500_tcpserver_disconnect(void); // �����Ͽ���·
extern uint16_t w5500_tcpserver_getc(uint8_t* str, uint16_t length);
extern uint16_t w5500_tcpserver_putc(uint8_t* str, uint16_t length);

extern void rt_dp83848_tcpserver_thread_entry(void *param); // TCP�����������߳� 
extern uint8_t dp83848_tcpserver_linkstate(void); // �ж���·״̬
extern void dp83848_tcpserver_disconnect(void); // �����Ͽ���·
extern uint16_t dp83848_tcpserver_getc(uint8_t* str, uint16_t length);
extern uint16_t dp83848_tcpserver_putc(uint8_t* str, uint16_t length);


#endif /* END __TCP_SERVER_H__ */


/* END OF FILE ---------------------------------------------------------------*/


