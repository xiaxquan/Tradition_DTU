#ifndef _MAIN_PROTECT_CONNECT_H_
#define _MAIN_PROTECT_CONNECT_H_

#include "stm32f4xx_hal.h"

enum
{
	MAINPRO_DEV0,				// 
	MAINPRO_DEVMAXNUM
};

extern void MainProtectCtrlInit(void);
extern void MainProtectCtrlClock(void);
extern void MainProtectCtrlReset(uint8_t pdrv);
#endif
