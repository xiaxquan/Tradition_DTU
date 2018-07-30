/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      commmon_config.h
  * @brief:     The commmon config of the project.
  * @version:   ver 1.1
  * @author:    Mr.J
  * @date:      2018-04-20
  * @update:    2018-04-20 [Sxr][make the code clean up]
  */
	
#ifndef __COMMON_CONFIG_H__
#define __COMMON_CONFIG_H__

/* 注释区------------------------------------------------------*/
#if 0      

/* 硬件板号 F01C-0701  F01C-0702   F01C-0102 */
#define LOGICLOCKINGMANUALREMOTECONTROL  // 逻辑闭锁手动遥控
#define GETPOWERLOSSVOLTAGELOCKCLOSE     // 得电合闸过程中失压闭锁合闸
#define SYNTHESIS_I0_ENABLE              // 合成零序电流使能
#define PRECONTROL_ENABLE                // 预控按键有效

#endif /* 注释区 */  



/* 地域区------------------------------------------------------*/
#define COMMON // 通用类
//#define JIANGMEN_0750_0 //江门大类方案


/* 配置区------------------------------------------------------*/
#ifdef COMMON

//#define SYNTHESIS_I0_ENABLE              // 合成零序电流使能
//#define LOGICLOCKINGMANUALREMOTECONTROL  // 逻辑闭锁手动遥控
#define TERMINAL_PRODUCT_SERIAL_NUMBER   "F01C.1.C0000.0758.180728"    // 产品序列号

#endif /* 配置区 COMMON */

#ifdef JIANGMEN_0750_0

#define LOGICLOCKINGMANUALREMOTECONTROL  // 逻辑闭锁手动遥控
//#define GETPOWERLOSSVOLTAGELOCKCLOSE     // 得电合闸过程中失压闭锁合闸
#define TERMINAL_PRODUCT_SERIAL_NUMBER   "F01C.1.C0000.0758.180728"    // 产品序列号

#endif /* 配置区 JIANGMEN_0750_0 */ 





#endif /* __COMMON_CONFIG_H__ */

/* END OF FILE ---------------------------------------------------------------*/
