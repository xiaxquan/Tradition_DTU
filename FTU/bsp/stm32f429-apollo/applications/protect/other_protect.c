/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      other_protect.c
  * @brief:     The driver of other_protect.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2017-11-28
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include <math.h>
//#include <arm_math.h>
//#include "drv_usart.h"
//#include "drv_fm25vxx.h"
#include "string.h"
#include "drv_do.h"
#include "drv_timer.h"
#include "drv_sd2405.h"
#include "common_data.h"
#include "other_protect.h"
#include "drv_fm25vxx.h"

/* PRIVATE VARIABLES ---------------------------------------------------------*/
static rt_device_t device_fram = RT_NULL; 
static List s_ListTimers[DB_DEV_MAX_NUM];//定时器链表链表//0x80000000使能位

static struct OverLimit OverLimitUab;
static struct OverLimit OverLimitUbc;
static struct OverLimit OverLimitUac;
static struct OverLimit OverLimitUAB;
static struct OverLimit OverLimitUBC;
static struct OverLimit OverLimitIa;
static struct OverLimit OverLimitIb;
static struct OverLimit OverLimitIc;
static struct OverLimit OverLimitU0;
static struct OverLimit OverLimitI0;
static struct OverLimit OverLimitDC_U;
static struct OverLimit OverLimitDC_I;

/* 各种记录存储计数器 */
static struct MemoryCounterStr MemoryCounter;

static struct CommunicatLockStr CommunicatLock; // 通讯互锁

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @Description: 系统运行监测任务
  * @param:  clock-调用周期
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void SysMonitorTask(void)
{	
//	rt_uint8_t a_all_sta = 0;
//    rt_uint8_t s_rlt = 0;
	
//    rt_device_control(device_fram, FM_MONITOR, &s_rlt); 
//	 	 
//    if (s_rlt == 1) // FRAM监测  定值区
//    {
//        a_all_sta |= MONI_FRAM;
//    }
//    else
//    {
//        a_all_sta &= ~MONI_FRAM;
//    }

//	if (g_TelesignalDB.Str.devicePowerDown == ON)
//    {
//        a_all_sta |= MONI_DO;
//    }
//    else
//    {
//        a_all_sta &= ~MONI_DO;
//    }
		
//	if (a_all_sta == 0)
//	{
//	    DBWriteSOE(g_TelesignalAddr.selfCheckAbnomal, OFF);
//	}
//	else
//	{
//	    DBWriteSOE(g_TelesignalAddr.selfCheckAbnomal, ON);
//	}
}

/**
  * @Description: 申请系统计数器.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void ApplyForCounter(rt_uint8_t pdrv, rt_uint32_t **dev)
{
    rt_uint32_t *s_timer;
        
    s_timer = rt_malloc(sizeof(rt_uint32_t));
    
    *dev = s_timer;
    *s_timer = 0;
    
    list_ins_next(&s_ListTimers[pdrv],NULL,s_timer); 
}

/**
  * @Description: 遥测越限判断函数
  * @param:  overlimit-越限结构体
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void TelemetryOverLimit(struct OverLimit *overlimit)
{
    if (!g_FixedValueP[OVERLIMIT_SWITCH] || !*overlimit->funSwitch)
    {
		DBWriteSOE(overlimit->soeAddrUp, OFF);
		DBWriteSOE(overlimit->soeAddrDown, OFF);
        return;
    }

    /* 上限 */
    if (*overlimit->telemetry > *overlimit->uplimit)
    {
        if (*overlimit->stateUp == OFF)
        {
            if(!(*overlimit->counterUp & DB_COUNTER_EN))
            {
                *overlimit->counterUp = DB_COUNTER_EN;//启动时钟
            }

            if ((*overlimit->counterUp & DB_COUNTER) > (rt_uint32_t)((*overlimit->delay)*1000))
            {
                *overlimit->counterUp = 0;
                DBWriteSOE(overlimit->soeAddrUp, ON);
            }
        }
    }
    else
    {
        *overlimit->counterUp = 0;
        if (*overlimit->stateUp == ON && *overlimit->telemetry <= (*overlimit->uplimit * (*overlimit->uplimitFactor)))
        {
            if(!(*overlimit->counterUpReverse & DB_COUNTER_EN))
            {
                *overlimit->counterUpReverse = DB_COUNTER_EN;//启动时钟
            }

            if ((*overlimit->counterUpReverse & DB_COUNTER) > (rt_uint32_t)((*overlimit->delay)*1000))
            {
                *overlimit->counterUpReverse = 0;
                DBWriteSOE(overlimit->soeAddrUp, OFF);
            }			
        }
    }

    /* 下限 */
    if (*overlimit->telemetry < *overlimit->downlimit)
    {
		*overlimit->counterDownReverse = 0;
        if (*overlimit->stateDown == OFF)
        {
            if(!(*overlimit->counterDown & DB_COUNTER_EN))
            {
                *overlimit->counterDown = DB_COUNTER_EN;//启动时钟
            }

            if ((*overlimit->counterDown & DB_COUNTER) > (rt_uint32_t)((*overlimit->delay)*1000))
            {
                *overlimit->counterDown = 0;
                DBWriteSOE(overlimit->soeAddrDown, ON);
            }
        }
    }
    else
    {
        *overlimit->counterDown = 0;
        if (*overlimit->stateDown == ON && *overlimit->telemetry >= (*overlimit->downlimit * (*overlimit->downlimitFactor)))
        {
            if(!(*overlimit->counterDownReverse & DB_COUNTER_EN))
            {
                *overlimit->counterDownReverse = DB_COUNTER_EN;//启动时钟
            }

            if ((*overlimit->counterDownReverse & DB_COUNTER) > (rt_uint32_t)((*overlimit->delay)*1000))
            {
                *overlimit->counterDownReverse = 0;
                DBWriteSOE(overlimit->soeAddrDown, OFF);
            }			
        }
    }
}

/**
  * @Description: 遥测异常监测
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void TelemetryAbnormalCheck(void)
{    
    /* 遥测越限 */
    TelemetryOverLimit(&OverLimitUab);
    TelemetryOverLimit(&OverLimitUbc);
    TelemetryOverLimit(&OverLimitUAB);	
    TelemetryOverLimit(&OverLimitUBC);
    TelemetryOverLimit(&OverLimitUac);
    TelemetryOverLimit(&OverLimitU0);
    TelemetryOverLimit(&OverLimitI0);
    TelemetryOverLimit(&OverLimitIa);
    TelemetryOverLimit(&OverLimitIb);
    TelemetryOverLimit(&OverLimitIc);
    TelemetryOverLimit(&OverLimitDC_U);
    TelemetryOverLimit(&OverLimitDC_I);
}

/**
  * @Description: 通讯互锁操作
  * @param:  clock-调用周期
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void ComunicatLock(void)
{
    rt_uint8_t i;
    
    for (i = 0; i < COM_MAX; i++)
    {
        if (*(CommunicatLock.flag[i])&COMMUNICATLOCKCLRCON)//计时清零
        {
            *(CommunicatLock.counter[i]) = 0; 
            *(CommunicatLock.flag[i]) &= ~COMMUNICATLOCKCLRCON;
        } 
        
        if (*(CommunicatLock.flag[i])&COMMUNICATLOCKUSERCON)//计时启动
        {
            if(!(*(CommunicatLock.counter[i])&DB_COUNTER_EN))
            {
                *(CommunicatLock.counter[i]) = DB_COUNTER_EN;   
            }  
            if((*(CommunicatLock.counter[i])&DB_COUNTER)>((rt_uint32_t)*(CommunicatLock.timeout) > 5000 ? (rt_uint32_t)*(CommunicatLock.timeout) : 5000))//超时复位
            {
                    *(CommunicatLock.flag[i]) &= COMMUNICATLOCKFUC;
                    *(CommunicatLock.counter[i]) = 0; 
            }            
        }
        else
        {
            *(CommunicatLock.counter[i]) = 0; 
        }
    }
		
    if ((*(CommunicatLock.flag[COM_FILE])&COMMUNICATLOCKRESET)&&(!(*(CommunicatLock.flag[COM_FILE])&COMMUNICATLOCKUSERSTA)))//复位
    {
        rt_multi_common_data_powerdown_storage(); 
        NVIC_SystemReset(); //软复位
    }
}

/**
  * @biief : other_protect init
  * @param : none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void other_protect_init(void)
{
    rt_uint8_t pdrv = 0;
	
    list_init(&s_ListTimers[pdrv]);
    
    ApplyForCounter(pdrv, &OverLimitUab.counterUp);
    ApplyForCounter(pdrv, &OverLimitUab.counterDown);
    ApplyForCounter(pdrv, &OverLimitUab.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUab.counterDownReverse);	
    OverLimitUab.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitUab.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitUabUp];
    OverLimitUab.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitUabDown];
    OverLimitUab.soeAddrUp = g_TelesignalAddr.overLimitUabUp;
    OverLimitUab.soeAddrDown = g_TelesignalAddr.overLimitUabDown;
    OverLimitUab.uplimit = &g_FixedValueP[UPLIMIT_VOLTAGE_U];
    OverLimitUab.downlimit = &g_FixedValueP[DOWNLIMIT_VOLTAGE_U];
    OverLimitUab.funSwitch = &g_FixedValueP[OVERLIMIT_ALARM_SWITCH_Uab];
    OverLimitUab.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.Uab];
    OverLimitUab.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitUab.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];

	ApplyForCounter(pdrv, &OverLimitUbc.counterUp);
    ApplyForCounter(pdrv, &OverLimitUbc.counterDown);
    ApplyForCounter(pdrv, &OverLimitUbc.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUbc.counterDownReverse);	
    OverLimitUbc.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitUbc.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitUbcUp];
    OverLimitUbc.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitUbcDown];
    OverLimitUbc.soeAddrUp = g_TelesignalAddr.overLimitUbcUp;
    OverLimitUbc.soeAddrDown = g_TelesignalAddr.overLimitUbcDown;
    OverLimitUbc.uplimit = &g_FixedValueP[UPLIMIT_VOLTAGE_U];
    OverLimitUbc.downlimit = &g_FixedValueP[DOWNLIMIT_VOLTAGE_U];
    OverLimitUbc.funSwitch = &g_FixedValueP[OVERLIMIT_ALARM_SWITCH_Ubc];
    OverLimitUbc.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.Ucb];
    OverLimitUbc.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitUbc.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitUAB.counterUp);
    ApplyForCounter(pdrv, &OverLimitUAB.counterDown);
    ApplyForCounter(pdrv, &OverLimitUAB.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUAB.counterDownReverse);	
    OverLimitUAB.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitUAB.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitUABUp];
    OverLimitUAB.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitUABDown];
    OverLimitUAB.soeAddrUp = g_TelesignalAddr.overLimitUABUp;
    OverLimitUAB.soeAddrDown = g_TelesignalAddr.overLimitUABDown;
    OverLimitUAB.uplimit = &g_FixedValueP[UPLIMIT_VOLTAGE_U];
    OverLimitUAB.downlimit = &g_FixedValueP[DOWNLIMIT_VOLTAGE_U];
    OverLimitUAB.funSwitch = &g_FixedValueP[OVERLIMIT_ALARM_SWITCH_UAB];
    OverLimitUAB.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.UAB];
    OverLimitUAB.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitUAB.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];
	
    ApplyForCounter(pdrv, &OverLimitUBC.counterUp);
    ApplyForCounter(pdrv, &OverLimitUBC.counterDown);
    ApplyForCounter(pdrv, &OverLimitUBC.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUBC.counterDownReverse);	
    OverLimitUBC.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitUBC.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitUBCUp];
    OverLimitUBC.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitUBCDown];
    OverLimitUBC.soeAddrUp = g_TelesignalAddr.overLimitUBCUp;
    OverLimitUBC.soeAddrDown = g_TelesignalAddr.overLimitUBCDown;
    OverLimitUBC.uplimit = &g_FixedValueP[UPLIMIT_VOLTAGE_U];
    OverLimitUBC.downlimit = &g_FixedValueP[DOWNLIMIT_VOLTAGE_U];
    OverLimitUBC.funSwitch = &g_FixedValueP[OVERLIMIT_ALARM_SWITCH_UBC];
    OverLimitUBC.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.UCB];
    OverLimitUBC.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitUBC.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitUac.counterUp);
    ApplyForCounter(pdrv, &OverLimitUac.counterDown);
    ApplyForCounter(pdrv, &OverLimitUac.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUac.counterDownReverse);	
    OverLimitUac.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitUac.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitUcaUp];
    OverLimitUac.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitUcaDown];
    OverLimitUac.soeAddrUp = g_TelesignalAddr.overLimitUcaUp;
    OverLimitUac.soeAddrDown = g_TelesignalAddr.overLimitUcaDown;
    OverLimitUac.uplimit = &g_FixedValueP[UPLIMIT_VOLTAGE_U];
    OverLimitUac.downlimit = &g_FixedValueP[DOWNLIMIT_VOLTAGE_U];
    OverLimitUac.funSwitch = &g_FixedValueP[OVERLIMIT_ALARM_SWITCH_Uca];
    OverLimitUac.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.Uac];
    OverLimitUac.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitUac.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitIa.counterUp);
    ApplyForCounter(pdrv, &OverLimitIa.counterDown);
    ApplyForCounter(pdrv, &OverLimitIa.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitIa.counterDownReverse);		
    OverLimitIa.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitIa.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitIaUp];
    OverLimitIa.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitIaDown];
    OverLimitIa.soeAddrUp = g_TelesignalAddr.overLimitIaUp;
    OverLimitIa.soeAddrDown = g_TelesignalAddr.overLimitIaDown;
    OverLimitIa.uplimit = &g_FixedValueP[UPLIMIT_CURRENT_I];
    OverLimitIa.downlimit = &g_FixedValueP[DOWNLIMIT_CURRENT_I];
    OverLimitIa.funSwitch = &g_FixedValueP[OVERLIMIT_ALARM_SWITCH_IA];
    OverLimitIa.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.Ia];
    OverLimitIa.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitIa.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitIb.counterUp);
    ApplyForCounter(pdrv, &OverLimitIb.counterDown);
    ApplyForCounter(pdrv, &OverLimitIb.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitIb.counterDownReverse);	
    OverLimitIb.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitIb.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitIbUp];
    OverLimitIb.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitIbDown];
    OverLimitIb.soeAddrUp = g_TelesignalAddr.overLimitIbUp;
    OverLimitIb.soeAddrDown = g_TelesignalAddr.overLimitIbDown;
    OverLimitIb.uplimit = &g_FixedValueP[UPLIMIT_CURRENT_I];
    OverLimitIb.downlimit = &g_FixedValueP[DOWNLIMIT_CURRENT_I];
    OverLimitIb.funSwitch = &g_FixedValueP[OVERLIMIT_ALARM_SWITCH_IB];
    OverLimitIb.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.Ib];
    OverLimitIb.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitIb.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitIc.counterUp);
    ApplyForCounter(pdrv, &OverLimitIc.counterDown);
    ApplyForCounter(pdrv, &OverLimitIc.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitIc.counterDownReverse);	
    OverLimitIc.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitIc.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitIcUp];
    OverLimitIc.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitIcDown];
    OverLimitIc.soeAddrUp = g_TelesignalAddr.overLimitIcUp;
    OverLimitIc.soeAddrDown = g_TelesignalAddr.overLimitIcDown;
    OverLimitIc.uplimit = &g_FixedValueP[UPLIMIT_CURRENT_I];
    OverLimitIc.downlimit = &g_FixedValueP[DOWNLIMIT_CURRENT_I];
    OverLimitIc.funSwitch = &g_FixedValueP[OVERLIMIT_ALARM_SWITCH_IC];
    OverLimitIc.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.Ic];
    OverLimitIc.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitIc.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitU0.counterUp);
    ApplyForCounter(pdrv, &OverLimitU0.counterDown);
    ApplyForCounter(pdrv, &OverLimitU0.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitU0.counterDownReverse);	
    OverLimitU0.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitU0.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitU0Up];
    OverLimitU0.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitU0Down];
    OverLimitU0.soeAddrUp = g_TelesignalAddr.overLimitU0Up;
    OverLimitU0.soeAddrDown = g_TelesignalAddr.overLimitU0Down;
    OverLimitU0.uplimit = &g_FixedValueP[UPLIMIT_VOLTAGE_U0];
    OverLimitU0.downlimit = &g_FixedValueP[DOWNLIMIT_VOLTAGE_U0];
    OverLimitU0.funSwitch = &g_FixedValueP[OVERLIMIT_ALARM_SWITCH_U0];
    OverLimitU0.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.U0];
    OverLimitU0.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitU0.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitI0.counterUp);
    ApplyForCounter(pdrv, &OverLimitI0.counterDown);
    ApplyForCounter(pdrv, &OverLimitI0.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitI0.counterDownReverse);		
    OverLimitI0.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitI0.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitI0Up];
    OverLimitI0.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitI0Down];
    OverLimitI0.soeAddrUp = g_TelesignalAddr.overLimitI0Up;
    OverLimitI0.soeAddrDown = g_TelesignalAddr.overLimitI0Down;
    OverLimitI0.uplimit = &g_FixedValueP[UPLIMIT_VOLTAGE_I0];
    OverLimitI0.downlimit = &g_FixedValueP[DOWNLIMIT_VOLTAGE_I0];
    OverLimitI0.funSwitch = &g_FixedValueP[OVERLIMIT_ALARM_SWITCH_I0];
    OverLimitI0.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.I0];
    OverLimitI0.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitI0.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitDC_U.counterUp);
    ApplyForCounter(pdrv, &OverLimitDC_U.counterDown);
    ApplyForCounter(pdrv, &OverLimitDC_U.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitDC_U.counterDownReverse);			
    OverLimitDC_U.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitDC_U.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitDC_U_Up];
    OverLimitDC_U.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitDC_U_Down];
    OverLimitDC_U.soeAddrUp = g_TelesignalAddr.overLimitDC_U_Up;
    OverLimitDC_U.soeAddrDown = g_TelesignalAddr.overLimitDC_U_Down;
    OverLimitDC_U.uplimit = &g_FixedValueP[UPLIMIT_DC_VOLTAGE_U];
    OverLimitDC_U.downlimit = &g_FixedValueP[DOWNLIMIT_DC_VOLTAGE_U];
    OverLimitDC_U.funSwitch = &g_FixedValueP[OVERLIMIT_ALARMSWITCH_DC_U];
    OverLimitDC_U.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.DC1];
    OverLimitDC_U.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitDC_U.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitDC_I.counterUp);
    ApplyForCounter(pdrv, &OverLimitDC_I.counterDown);
    ApplyForCounter(pdrv, &OverLimitDC_I.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitDC_I.counterDownReverse);	
    OverLimitDC_I.delay = &g_FixedValueP[OVERLIMIT_TIME];
    OverLimitDC_I.stateUp = &g_TelesignalDB[g_TelesignalAddr.overLimitDC_I_Up];
    OverLimitDC_I.stateDown = &g_TelesignalDB[g_TelesignalAddr.overLimitDC_I_Down];
    OverLimitDC_I.soeAddrUp = g_TelesignalAddr.overLimitDC_I_Up;
    OverLimitDC_I.soeAddrDown = g_TelesignalAddr.overLimitDC_I_Down;
    OverLimitDC_I.uplimit = &g_FixedValueP[UPLIMIT_DC_CURRENT_I];
    OverLimitDC_I.downlimit = &g_FixedValueP[DOWNLIMIT_DC_CURRENT_I];
    OverLimitDC_I.funSwitch = &g_FixedValueP[OVERLIMIT_ALARMSWITCH_DC_I];
    OverLimitDC_I.telemetry = &g_TelemetryBaseDB[g_TelemetryBaseAddr.DC2];
    OverLimitDC_I.uplimitFactor = &g_FixedValueP[UPLIMIT_FACTOR];
    OverLimitDC_I.downlimitFactor = &g_FixedValueP[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &MemoryCounter.counterSave);
    ApplyForCounter(pdrv, &MemoryCounter.counterCharge);    
    CommunicatLock.timeout = &g_Parameter[REVERSE_TIME];
    CommunicatLock.flag[COM_YK] = &g_CommunicatFlag[COM_YK];
    CommunicatLock.flag[COM_PARA] = &g_CommunicatFlag[COM_PARA];
    CommunicatLock.flag[COM_FILE] = &g_CommunicatFlag[COM_FILE];
    ApplyForCounter(pdrv, &CommunicatLock.counter[COM_YK]);
    ApplyForCounter(pdrv, &CommunicatLock.counter[COM_PARA]);
    ApplyForCounter(pdrv, &CommunicatLock.counter[COM_FILE]);
	
    device_fram = rt_device_find(RT_SPI_FRAM_NAME);
    if (device_fram == NULL)
    {
        
    }	
}	

/**
  * @brief: Recoed Memory
  * @param:  none
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void RecoedMemory(void)
{	
    /* 需存储记忆数据 */
    static struct MEMORY_FLAG_Str s_Flag_DB1;   
    static struct MEMORY_FLAG_Str s_Flag_DB2; 
    static rt_uint8_t state=0;
    		
    /* 装置掉电 */
    if (g_TelesignalDB[g_TelesignalAddr.devicePowerDown] == ON)
    {
        *MemoryCounter.counterCharge = DB_COUNTER_EN;
        if(state==1)
        {
            state = 0;
            rt_multi_common_data_powerdown_storage();
            (*CommunicatLock.flag[COM_FILE]) |= COMMUNICATLOCKLOSEPOWER;//掉电保护
        }
    }
    else
    {
        state = 1;
        if(!((*CommunicatLock.flag[COM_FILE])&COMMUNICATLOCKRECORD))
        {
            if ((memcmp(&s_Flag_DB2, &g_FlagDB, sizeof(g_FlagDB))))
            {
                *MemoryCounter.counterSave = DB_COUNTER_EN;
            }
            if ((memcmp(&g_FlagDB, &s_Flag_DB1, sizeof(g_FlagDB))))
            {
                s_Flag_DB2 = g_FlagDB;
                if(!(*MemoryCounter.counterSave&DB_COUNTER_EN))
                {
                    *MemoryCounter.counterSave = DB_COUNTER_EN;
                }
                if (((*MemoryCounter.counterSave) & DB_COUNTER) >= MEMORY_TIME)
                {
                    *MemoryCounter.counterSave = 0;
                    s_Flag_DB1 = g_FlagDB;
                    rt_multi_common_data_powerdown_storage();
                    (*CommunicatLock.flag[COM_FILE]) |= COMMUNICATLOCKRECORD;                    
                }
            } 
        }
        
        if (((*MemoryCounter.counterCharge) & DB_COUNTER) >= CHARGE_TIME)
        {
           *MemoryCounter.counterCharge = CHARGE_TIME;
           (*CommunicatLock.flag[COM_FILE]) &= ~COMMUNICATLOCKLOSEPOWER; //掉电恢复
        }
        else
        { 
            if(!(*MemoryCounter.counterCharge&DB_COUNTER_EN))
            {
                *MemoryCounter.counterCharge = DB_COUNTER_EN;
            }
            (*CommunicatLock.flag[COM_FILE]) |= COMMUNICATLOCKLOSEPOWER;//掉电保护
        }
    }
}

/**
  * @brief: the counter of system
  * @param:  none
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void SystemCounterTask(rt_uint8_t pdrv)
{
    ListElmt *element;
    
    element = s_ListTimers[pdrv].head;
    while(element != NULL)
    {
        if((*((uint32_t *)(element->data)))&DB_COUNTER_EN)
        {
            (*((uint32_t *)(element->data)))++;
        }
        element = element->next;
    }
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : other_protect_clock init
  * @param : none
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void other_protect_clock(void)
{
    /* 系统计数器 */
    SystemCounterTask(DB_DEV0);	

    /* 记录存储处理 */
    RecoedMemory();	
	
    /* 遥测异常监测 */
    TelemetryAbnormalCheck();

    /* 通讯互锁 */
    ComunicatLock();   

    /* 合环 */
    closing_loop_clock();   
    
    /* 系统自检 */
//    SysMonitorTask();	
}

/* END OF FILE ---------------------------------------------------------------*/

