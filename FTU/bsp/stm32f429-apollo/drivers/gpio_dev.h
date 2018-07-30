/*
** ------------------------------- Copyright (c) -------------------------------                                     
**
**                           http://www.sojoline.com
**
** note:    Copyright SOJO. ALL RIGHTS RESERVED.
**          This software is provided under license and contains proprietary and
**          confidential material which is the property of Company Name tech. 
**    
** -------------------------------- FILE INFO ----------------------------------
** File name:               gpio_dev.h
** Descriptions:            The configration of GPIO. 
**
** -----------------------------------------------------------------------------
** Author:                  Mr.Lee
** Created date:            2017.07.12
** System:                  The new FTU system.
** Version:                 V1.01
** Descriptions:            The original version
**
** -----------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
** ---------------------------------------------------------------------------*/
#ifndef _GPIO_DEV_H_
#define _GPIO_DEV_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


#ifdef NORMAL_SWC

/* DEFINES -------------------------------------------------------------------*/
#define OUT_SET                   (GPIO_PIN_SET)
#define OUT_RESET                 (GPIO_PIN_RESET)

#define OUT_POS_24V               (0)
#define OUT_POS_CLOSE             (1)
#define OUT_POS_BACK              (2)
#define OUT_POS_CUT               (3)
#define OUT_POS_EXT_ALRAM         (4)
#define OUT_POS_LOCAL_CTRL        (5)
#define OUT_POS_VOL_ACTIVE        (6)

#define IN_NORMAL                 (GPIO_PIN_RESET)
#define IN_ERR                    (GPIO_PIN_SET)

#define IN_POS_HAND_CLOSE         (7)  // �ֶ���բ�ź�����λ
#define IN_POS_HAND_OPEN          (8)  // �ֶ���բ�ź�����λ
#define IN_POS_HAND_MODE          (9)  // �ֶ�/�͵ؿ����ź�����λ
#define IN_POS_ST_CLOSE           (10) // ���غ�բλ������
#define IN_POS_ST_CUT             (11) // ���ط�բλ������
#define IN_POS_ST_LOCAL_CTRL      (12) // Զ��/�͵ؿ����ź�����λ
#define IN_POS_ST_VOL_ACTIVE      (13) // ��ؼ����ź�����λ
#define IN_POS_ST_VOL_220         (14) // ���������ź�����λ
#define IN_POS_ST_VOL_LACK        (15) // ʧѹ�ź�����λ

#define LED_ON                    (GPIO_PIN_RESET)
#define LED_OFF                   (GPIO_PIN_SET)

#define LED_RUN_POS               (0) // ����
#define LED_FA_POS                (1) // error
#define LED_COM_POS               (2) // 101 ͨ��
#define LED_NET_POS               (3) // 104 ͨ��
#define LED_CUT_POS               (4) // �ٶ�
#define LED_ERT_POS               (5) // ����
#define LED_OVI_POS               (6) // ����
#define LED_RECOL_POS             (7) // �غ�բ

#define DIAL_GROUP_A              (0) // ���������ֵ
#define DIAL_GROUP_B              (1) // ������ʱ
#define DIAL_GROUP_C              (2) // ������ֵ
#define DIAL_GROUP_D              (3) // ������ʱ
#define DIAL_GROUP_E              (4) // �ٶϵ�����ֵ
#define DIAL_GROUP_F              (5) // ʱ����(low bit) / �غ�բ����(high bit)
#define DIAL_GROUP_G              (6)
#define DIAL_GROUP_H              (7)

#else /* not define NORMAL SWC */

#define OUT_SET                   (GPIO_PIN_SET)
#define OUT_RESET                 (GPIO_PIN_RESET)

#define IN_NORMAL                 (GPIO_PIN_RESET)
#define IN_ERR                    (GPIO_PIN_SET)

#define IN_POS_HAND_CLOSE         (7)  // �ⲿ�����ֶ����ź�
#define IN_POS_HAND_OPEN          (8)  // �ⲿ�����ֶ����ź�

#define IN_POS_ST_CLOSE           (10) // �ⲿ���뿪�غ�״̬
#define IN_POS_ST_CUT             (11) // �ⲿ���뿪�ط�״̬
#define IN_POS_HAND_MODE          (0)
#define IN_POS_ST_LOCAL_CTRL      (0)

#define OUT_POS_CLOSE             (1)  // �Զ����ƺ����
#define OUT_POS_CUT               (2)  // �Զ����Ʒ����

#define OUT_POS_RELAY_CLOSE       (3)  // �̵����ⲿ ��բ״̬--led
#define OUT_POS_RELAY_CUT         (4)  // �̵����ⲿ ��բ״̬--led
#define OUT_POS_RELAY_ALARM       (5)  // �̵����ⲿ �澯״̬--led

#define LED_ON                    (OUT_SET)   // �ⲿLED
#define LED_OFF                   (OUT_RESET) // �ⲿLED

#define LED_CLOSE                 (0x10)
#define LED_CUT                   (0x08)
#define LED_OVER_C                (0x04)
#define LED_RECLOSE               (0x02)
#define LED_ERROR                 (0x01)

#define LED_RUN_POS               (0)           // ����
#define LED_FA_POS                (LED_ERROR)   // error
#define LED_COM_POS               (0)           // 101ͨ��
#define LED_NET_POS               (0)           // 104ͨ��
#define LED_CUT_POS               (LED_OVER_C)  // �ٶ�
#define LED_ZERO_POS              (LED_OVER_C)  // ����
#define LED_OVI_POS               (LED_OVER_C)  // ����
#define LED_RECOL_POS             (LED_RECLOSE) // �غ�բ


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
//void EX_Led_On(int led);
//void EX_Led_Off(int led);


#endif /* END NORMAL_SWC */


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
//extern void Led_On(unsigned int led);
//extern void Led_Off(unsigned int led);
unsigned int rt_get_utc(void);
void rt_get_time(unsigned char *time);
void rt_set_time(unsigned char *time);


#endif /* END _GPIO_DEV_H_ */


/* END OF FILE ---------------------------------------------------------------*/

