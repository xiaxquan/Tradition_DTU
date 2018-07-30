/** ----------------------------------------------------------------------------
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      bsp_cfg.h
  * @brief:     The configuration file of board surport packege. 
  * @version:   V1.0.0 
  * @author:    LEX
  * @date:      2017.11.23
  * @update:    [YYYY-MM-DD][NAME][DESCRIPTION]
 ** --------------------------------------------------------------------------*/
#ifndef  _BSP_CFG_H_
#define  _BSP_CFG_H_ 


/* DEFINE --------------------------------------------------------------------*/


/* BOOT VERSION --------------------------------------------------------------*/
#define  BOOTVersion                                       ("3.01")


/* TASK STACK NAMES CONFIGURATION --------------------------------------------*/
#define  LWIP_TASK_TCPIP_THREAD_NAME                       ("Taks of tcpip_thread")
#define  LWIP_TASK_DHCP_NAME                               ("Task of Shell")
#define  LWIP_TASK_DP83848_NAME                            ("Task of DP83848 input thread")
#define  LWIP_TASK_W5500_NAME                              ("Task of W5500 input thread")
#define  APP_TASK_START_NAME                               ("Task of Start")
#define  APP_TASK_WATCH_NAME                               ("Task of watch")
#define  APP_TASK_DLT634_5104_NAME                         ("Task of DL/T634.5104-2009")
#define  APP_TASK_TCP_SERVER_2404_NAME                     ("Task of TCP SERVER(2404)")
#define  APP_TASK_TCP_SERVER_8000_NAME                     ("Task of TCP SERVER(8000)")
#define  APP_TASK_DLT634_5101_MASTER_NAME                  ("Task of DL/T634.5101-2002 MASTER")
#define  APP_TASK_DLT634_5101_LINELOSS_NAME                ("Task of DL/T634.5101-2002 LINELOSS")
#define  APP_TASK_DLT634_5101_SLAVE_NAME                   ("Task of DL/T634.5101-2002 SLAVE")
#define  APP_TASK_DLT634_5104_SLAVE_NAME                   ("Task of DL/T634.5104-2002 SLAVE")
#define  APP_TASK_SDM_MASTER_NAME                          ("Task of SDM MASTER")
#define  APP_TASK_SDM_SLAVE_NAME                           ("Task of SDM SLAVE")
#define  APP_TASK_MODBUS_NAME                              ("Task of MODBUS")
#define  APP_TASK_SHELL_NAME                               ("Task of Shell")


/* TASK NAMES CONFIGURATION --------------------------------------------------*/
#define  INIT_TASK_NAME                                    "init"


/* DEVICE NAMES CONFIGURATION ------------------------------------------------*/
#define  RS232_DEVICE_NAME                                 "rs232_device"
#define  RS485_DEVICE_NAME                                 "rs485_device"
#define  PIN_DEVICE_NAME                                   "pin_device"
#define  IO_DEVICE_NAME                                    "io_device"
#define  FRAM_DEVICE_NAME                                  "fm25l04_device"
#define  RTC_DEVICE_NAME                                   "rtc"
#define  LED_DEVICE_NAME                                   "led_device"
#define  LCD_DEVICE_NAME                                   "lcd_device"
#define  WATCHDOG_DEVICE_NAME                              "watchdog_device"
#define  SOE_DEVICE_NAME                                   "soe_device"
#define  ADC_DEVICE_NAME                                   "ad7606_device"
#define  DIAL_DEVICE_NAME                                  "dial_device"
#define  W5500_DEVICE_NAME                                 "w5500_device"


/* SEMAPHORE NAMES CONFIGURATION ---------------------------------------------*/
#define  RS232_RECEIVE_SEM_NAME                            "uart2_recv_sem"
#define  RS485_RECEIVE_SEM_NAME                            "uart3_recv_sem"


/* TASK STACK SIZE CONFIGURATION ---------------------------------------------*/ 
#define  INIT_TASK_STACK_SIZE                              (2048) // ���ó�ʼ�����������ջ��С


/* TASK PRIORITIES CONFIGURATION ---------------------------------------------*/ 
#define	 INIT_TASK_PRIORITY                                (RT_THREAD_PRIORITY_MAX / 3) // ���ó�ʼ�������������ȼ�[32/3]


/* TASK STACK SIZE CONFIGURATION ---------------------------------------------*/
#define  INIT_TASK_TIME_SLICE                              (20) // ���ó�ʼ����������ʱ��Ƭ


/* INTERRUPT PRIORITIES CONFIGURATION ----------------------------------------*/
/* ���ȼ�:��1����ռ���ȼ���PreemptionPriority��ռ�ø���λ��
**        ��2�������ȼ���SubPriority�� ռ�õ���λ��
** ---------------------------------------------------------------------------*/
#define  INT_PREEMPPRIO_SIZE                               (4U) // ��ռ���ȼ���PreemptionPriority��ռ�ø���λ
#define  INT_SUBPRIO_SIZE                                  (4U) // �����ȼ���SubPriority��ռ�õ���λ
#define  INT_TICK_PRIO                                     ((0x00<<4)|0x00) // �δ�ʱ���ж����ȼ�
#define  INT_ETHERNET_PRIO									               ((0x00<<4)|0x01) // ��̫���ж����ȼ����
#define  INT_USART1_PRIO									                 ((0x03<<4)|0x03) // RS485/RS232�ⲿͨ�Žӿ�
#define  INT_USART2_PRIO									                 ((0x03<<4)|0x01) // RS232�ڲ�ͨ�Žӿڣ������CPUͨ��
#define  INT_USART3_PRIO									                 ((0x03<<4)|0x04) // RS232�ⲿά���ӿ�
#define  INT_UART4_PRIO									                   ((0x03<<4)|0x04) // RS485/RS232�ⲿͨ�Žӿ�
#define  INT_UART5_PRIO									                   ((0x03<<4)|0x01) // δ��
#define  INT_USART6_PRIO									                 ((0x03<<4)|0x02) // RS232�ڲ�ͨ�Žӿڣ�����CPUͨ��
#define  INT_UART7_PRIO									                   ((0x03<<4)|0x07) // δ��
#define  INT_UART8_PRIO									                   ((0x03<<4)|0x08) // δ��
#define  INT_EXTI9_5_PRIO									                 ((0x05<<4)|0x01) // ����ң��ģ��
#define  INT_EXTI15_10_PRIO									               ((0x05<<4)|0x02) // ����ң��ģ��
#define  INT_IIC1_PRIO                                     ((0x01<<2)|0x06) // δ��
#define  INT_SPI1_PRIO									                   ((0x01<<2)|0x02) // δ��
#define  INT_SPI2_PRIO									                   ((0x01<<2)|0x02) // FLASH�ӿ�
#define  INT_SPI3_PRIO									                   ((0x01<<2)|0x02) // δ��
#define  INT_SPI4_PRIO									                   ((0x01<<2)|0x02) // δ��
#define  INT_SPI5_PRIO									                   ((0x01<<4)|0x01) // W5500�ӿ�
#define  INT_SPI6_PRIO									                   ((0x01<<2)|0x02) // δ��
#define  INT_IO_SPI7_PRIO                                  ((0x01<<2)|0x02) // FRAM
#define  INT_IO_SPI8_PRIO									                 ((0x01<<2)|0x02) // 2.4G����ģ��
#define  INT_TIM0_PRIO									                   ((0x01<<2)|0x04) // δ��
#define  INT_TIM1_PRIO									                   ((0x01<<2)|0x05) // δ��


/* DRIVER CONFIGURATION ------------------------------------------------------*/


/* APPLICATION PROTOCOL CONFIGURATION ----------------------------------------*/
#define  APP_TASK_FA_CFG                                   (0) // �͵طֲ�ʽFA���ã���δʹ��
#define  APP_TASK_DNP30_SEC_CFG                            (0) // DNP��վ���ã���δʹ��
#define	 APP_TASK_DLT634_5101_SLAVE_CFG                    (1) // DL/T634.5101��վ����
#define	 APP_TASK_DLT634_5101_MASTER_CFG                   (1) // DL/T634.5101��վ����
#define  APP_TASK_DLT634_5101_LINELOSS_CFG                 (1) // DL/T634.5101����ģ������
#define	 APP_TASK_DLT634_5104_SLAVE_CFG                    (1) // DL/T634.5104��վ����
#define	 APP_TASK_DLT645_MASTER_CFG                        (0) // DL/T645��վ���ã���δʹ��
#define  APP_TASK_MODBUS_MASTER_CFG                        (0) // MODBUSЭ����վ���ã���δʹ��
#define  APP_TASK_SDM_MASTER_CFG                           (0) // �ڲ���ԼSDM(V3.01)Masterģʽ���ã�������FTU_V3.01,������CPUͨ�ţ�
#define  APP_TASK_SDM_SLAVE_CFG                            (0) // �ڲ���ԼSDM(V3.01)Slaveģʽ���ã�������FTU_V3.01,����ʾCPU��ά���ӿ�ͨ�ţ�
#define  APP_TASK_WATCH_CFG                                (1) // ������������

/* APPLICATION PROTOCOL CONFIGURATION ----------------------------------------*/


#endif /* END _BSP_CFG_H_ */


/* END OF FILE ---------------------------------------------------------------*/



