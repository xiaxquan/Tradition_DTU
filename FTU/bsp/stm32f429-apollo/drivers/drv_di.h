/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_di.h
  * @brief:     The driver of di.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2017.09.05
  * @updata  : [2018-01-16][Sunxr][modify the driver]
  */
	
#ifndef __DRV_DI_H__
#define __DRV_DI_H__


/* INCLUDE FILES -------------------------------------------------------------*/	
#include "rtthread.h"
#include "stm32f4xx_hal.h"


/* DEFINE --------------------------------------------------------------------*/	
#ifdef  DEBUG
#define DI_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define DI_PRINTF(...)
#endif /* DEBUG */


/* STRUCT -------------------------------------------------------------------*/	
typedef struct TagDiStr
{  
    rt_uint32_t doubleCounter;
    rt_uint32_t deviceFalutCounter;
    rt_uint32_t disconnectCounter;	
    rt_uint8_t doubleState;
    rt_uint8_t doubleStateLast;   	
}DiStr;

/* DEFINE --------------------------------------------------------------------*/	

/* PUBLIC VARIABLES ----------------------------------------------------------*/
extern DiStr g_DiCollect;


/* PUBLIC FUNCTION  ----------------------------------------------------------*/
extern int rt_hw_di_init(void);
extern void rt_hw_di_check_task(rt_uint8_t clock);


#endif /* __DRV_DI_H__ */


/* END OF FILE ---------------------------------------------------------------*/

