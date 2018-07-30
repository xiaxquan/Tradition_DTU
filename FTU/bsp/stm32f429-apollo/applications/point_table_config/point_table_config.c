/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      point_table_config.c
  * @brief:     The point_table_config  of the project.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2018-05-09
  * @update:    [2018-05-09][name][change]
  */
	
	
/* INCLUDES FILES ------------------------------------------------------------*/
#include "common_data.h"
#include "point_table_config.h"
#include "drv_gpio.h"

/* 面板硬遥信 */
struct taghmiDigitalInputCfg hmiDigitalInputCfg[] = 
{
//   使能   面板开入号	        点号                                  取反 
    //{ 1,       15,       &g_TelesignalAddr.recloseHardStrap,           0},
    //{ 1,       16,       &g_TelesignalAddr.recloseHardStrap,         0},//同期压板
    //{ 1,       17,       &g_TelesignalAddr.recloseHardStrap,         0},//智能FA
    { 1,       18,       &g_TelesignalAddr.earth,                      0},
    { 1,       19,       &g_TelesignalAddr.remote,                     0},
    //{ 1,       20,       &g_TelesignalAddr.modContactHardStrap,        0},
    //{ 1,       21,       &g_TelesignalAddr.modBreakHardStrap,          0},
    { 1,       22,       &g_TelesignalAddr.functionHardStrap,          1},
    { 1,       23,       &g_TelesignalAddr.workmodeCommon,             0},
    { 1,       24,       &g_TelesignalAddr.workmodeVolcur,             0},
};

/* 主板硬遥信 */
struct tagzkDigitalInputCfg zkDigitalInputCfg[] = 
{
//   使能          面板开入号	                点号                                      上次PIN   取反   计时器
    { 1,       INDEX_DI1,          &g_TelesignalAddr.switchOpen,                   0,      0,      0},
    { 1,       INDEX_DI2,          &g_TelesignalAddr.switchClose,                  0,      0,      0},
    { 1,       INDEX_DI3,          &g_TelesignalAddr.operatingMechanism,           0,      1,      0},
    { 1,       INDEX_DI4,          &g_TelesignalAddr.lowPressure,                  0,      0,      0},
//    { 1,       INDEX_DI5,          &g_TelesignalAddr.powerFaultAlarm,              0,      0,      0},
//    { 1,       INDEX_DI6,          &g_TelesignalAddr.batteryUnderVoltageAlarm,     0,      0,      0},
//    { 1,       INDEX_DI7,          &g_TelesignalAddr.batteryActivationStatus,      0,      0,      0},
//    { 1,       INDEX_DI8,          &g_TelesignalAddr.batteryLossAlarm,             0,      1,      0},
};

/* 遥信 */
struct tagTelesignalCfg TelesignalCfg[] = 
{
//   使能    名称	        点号                                         遥信      属性  故障记录使能    描述
    { 1,  "开关分位",     &g_TelesignalAddr.switchOpen,                  NULL,     0xff,    	1,		{"分", "合"},      		{"合->分","分->合"}},
    { 1,  "开关合位",     &g_TelesignalAddr.switchClose,                 NULL,     0xff,    	1,		{"分", "合"},      		{"合->分","分->合"}},
    { 1,  "储能",         &g_TelesignalAddr.operatingMechanism,          NULL,     0xff,    	1,		{"无", "有"},      		{"有->无","无->有"}},
    { 1,  "低气压",       &g_TelesignalAddr.lowPressure,                 NULL,     0xff,    	1,		{"无", "告警"},    		{"告警->复位","无->告警"}},
    { 0,  "备用1",        &g_TelesignalAddr.spareDi1,                    NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},
    { 0,  "备用2",        &g_TelesignalAddr.spareDi2,                    NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},
    { 0,  "备用3",        &g_TelesignalAddr.spareDi3,                    NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},
    { 0,  "备用4",        &g_TelesignalAddr.spareDi4,                    NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},
    { 0,  "备用5",        &g_TelesignalAddr.spareDi5,                    NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},
    { 0,  "备用6",        &g_TelesignalAddr.spareDi6,                    NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},
    { 0,  "备用7",        &g_TelesignalAddr.spareDi7,                    NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},
    { 0,  "备用8",        &g_TelesignalAddr.spareDi8,                    NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},
    { 0,  "备用9",        &g_TelesignalAddr.spareDi9,                    NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},
    { 0,  "备用10",       &g_TelesignalAddr.spareDi10,                   NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},
    { 0,  "备用11",       &g_TelesignalAddr.spareDi11,                   NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},
    { 0,  "备用12",       &g_TelesignalAddr.spareDi12,                   NULL,     0xff,        1,      {"分", "合"},           {"合->分","分->合"}},	
    { 1,  "开关类型",     &g_TelesignalAddr.swtichClass,                 NULL,     0xff,    	1,		{"断路器","负荷开关"},	{"断路器","负荷开关"}},
    { 1,  "常规保护型",   &g_TelesignalAddr.workmodeCommon,              NULL,     0xff,    	1,		{"退出", "投入"},  		{"投->退","退->投"}},
    { 1,  "电压电流型",   &g_TelesignalAddr.workmodeVolcur,              NULL,     0xff,    	1,		{"退出", "投入"},  		{"投->退","退->投"}},
    { 1,  "分段硬压板",   &g_TelesignalAddr.modBreakHardStrap,           NULL,     ON,      	1,		{"退出", "投入"},  		{"投->退","退->投"}},
    { 1,  "联络硬压板",   &g_TelesignalAddr.modContactHardStrap,         NULL,     ON,      	1,		{"退出", "投入"},  		{"投->退","退->投"}},
    { 1,  "重合闸硬压板", &g_TelesignalAddr.recloseHardStrap,            NULL,     ON,      	1,		{"退出", "投入"},  		{"投->退","退->投"}},
    { 1,  "保护硬压板",   &g_TelesignalAddr.functionHardStrap,           NULL,     0xff,    	1,		{"退出", "投入"},  		{"投->退","退->投"}},
    { 1,  "分段软压板",   &g_TelesignalAddr.modBreakSoftStrap,           NULL,     0xff,    	1,		{"退出", "投入"},  		{"投->退","退->投"}},
    { 1,  "联络软压板",   &g_TelesignalAddr.modContactSoftStrap,         NULL,     0xff,    	1,		{"退出", "投入"},  		{"投->退","退->投"}},
    { 1,  "重合闸软压板", &g_TelesignalAddr.recloseSoftStrap,            NULL,     0xff,    	1,		{"退出", "投入"},  		{"投->退","退->投"}},
    { 1,  "保护软压板",   &g_TelesignalAddr.functionSoftStrap,           NULL,     0xff,    	1,		{"退出", "投入"},  		{"投->退","退->投"}},
    { 1,  "远方",         &g_TelesignalAddr.remote,                      NULL,     0xff,    	1,		{"无", "远方"},  		{"远方->无","无->远方"}},
    { 1,  "就地",         &g_TelesignalAddr.earth,                       NULL,     0xff,    	1,		{"无", "就地"},  		{"就地->无","无->就地"}},
    { 0,  "短路故障",     &g_TelesignalAddr.shortCircuitFault,           NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "零序故障",     &g_TelesignalAddr.earthingFault,               NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "保护动作",     &g_TelesignalAddr.protectionAct,               NULL,     0xff,    	0,		{"复位","动作"},   		{"复位","动作"}},
    { 0,  "合闸闭锁",     &g_TelesignalAddr.closingLocked,               NULL,     0xff,    	0,		{"复位","闭锁"},   		{"复位","闭锁"}},	
    { 0,  "分闸闭锁",     &g_TelesignalAddr.openingLocked,               NULL,     0xff,    	0,		{"复位","闭锁"},   		{"复位","闭锁"}},
    { 0,  "重合动作",     &g_TelesignalAddr.recloseAct,                  NULL,     0xff,    	0,		{"复位","动作"},   		{"复位","动作"}},
    { 0,  "重合闭锁",     &g_TelesignalAddr.recloseLock,                 NULL,     0xff,    	0,		{"复位","闭锁"},   		{"复位","闭锁"}},
    { 0,  "装置故障",     &g_TelesignalAddr.deviceFault,                 NULL,     0xff,    	0,		{"无","故障"},     		{"无","故障"}},
    { 0,  "自检异常",     &g_TelesignalAddr.selfCheckAbnomal,            NULL,     0xff,    	0,		{"无","异常"},     		{"无","异常"}},
    { 0,  "通讯",         &g_TelesignalAddr.communication,               NULL,     0xff,    	0,		{"断","通"},       		{"断","通"}},	
    { 0,  "电池故障告警", &g_TelesignalAddr.batteryFaultAlarm,           NULL,     0xff,    	0,		{"无","告警"},     		{"无","告警"}},
    { 0,  "双点开关分合", &g_TelesignalAddr.doubleSwitch,                NULL,     0xff,    	0,		{"合->分","分->合"}, 	{"合->分","分->合"}}, 
    { 0,  "控制回路断线", &g_TelesignalAddr.controlLoopDisconnection,    NULL,     0xff,    	0,		{"复位","断线"},   		{"复位","断线"}},
    { 0,  "A相过流一段",  &g_TelesignalAddr.overcurrentIa1,              NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "B相过流一段",  &g_TelesignalAddr.overcurrentIb1,              NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},	
    { 0,  "C相过流一段",  &g_TelesignalAddr.overcurrentIc1,              NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "A相过流二段",  &g_TelesignalAddr.overcurrentIa2,              NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "B相过流二段",  &g_TelesignalAddr.overcurrentIb2,              NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},	
    { 0,  "C相过流二段",  &g_TelesignalAddr.overcurrentIc2,              NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "A相过流三段",  &g_TelesignalAddr.overcurrentIa3,              NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "B相过流三段",  &g_TelesignalAddr.overcurrentIb3,              NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},	
    { 0,  "C相过流三段",  &g_TelesignalAddr.overcurrentIc3,              NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "零序过流一段", &g_TelesignalAddr.overcurrentI01,              NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "零序过流二段", &g_TelesignalAddr.overcurrentI02,              NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},		
    { 0,  "A相加速过流",  &g_TelesignalAddr.overcurrentAccIa,            NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "B相加速过流",  &g_TelesignalAddr.overcurrentAccIb,            NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "C相加速过流",  &g_TelesignalAddr.overcurrentAccIc,            NULL,     0xff,   		1,		{"复位","保护"},   		{"复位","保护"}},	
    { 0,  "零序加速过流", &g_TelesignalAddr.overcurrentAccI0,            NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "零序加速过压", &g_TelesignalAddr.overcurrentAccU0,            NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},    
    { 0,  "一次重合闸",   &g_TelesignalAddr.recloseProtection1,          NULL,     0xff,    	0,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "二次重合闸",   &g_TelesignalAddr.recloseProtection2,          NULL,     0xff,    	0,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "三次重合闸",   &g_TelesignalAddr.recloseProtection3,          NULL,     0xff,    	0,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "四次重合闸",   &g_TelesignalAddr.recloseProtection4,          NULL,     0xff,    	0,		{"复位","保护"},   		{"复位","保护"}},	
    { 0,  "零序一次重合", &g_TelesignalAddr.zeroRecloseProtection1,      NULL,     0xff,    	0,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "零序二次重合", &g_TelesignalAddr.zeroRecloseProtection2,      NULL,     0xff,    	0,		{"复位","保护"},   		{"复位","保护"}},			
    { 0,  "二次重合闭锁", &g_TelesignalAddr.clockSecondaryClose,         NULL,     0xff,    	0,		{"复位","闭锁"},   		{"复位","闭锁"}},
    { 0,  "A相过流故障",  &g_TelesignalAddr.overcurrentFaultIa,          NULL,     0xff,    	1,		{"复位","告警"},   		{"复位","告警"}},  
    { 0,  "B相过流故障",  &g_TelesignalAddr.overcurrentFaultIb,          NULL,     0xff,    	1,		{"复位","告警"},   		{"复位","告警"}},
    { 0,  "C相过流故障",  &g_TelesignalAddr.overcurrentFaultIc,          NULL,     0xff,    	1,		{"复位","告警"},   		{"复位","告警"}},
    { 0,  "零序过流故障", &g_TelesignalAddr.overcurrentFaultI0,          NULL,     0xff,    	1,		{"复位","告警"},   		{"复位","告警"}},
    { 0,  "失电分闸",     &g_TelesignalAddr.lossTrip,                    NULL,     0xff,    	1,		{"复位","跳闸"},   		{"复位","跳闸"}},
    { 0,  "得电合闸",     &g_TelesignalAddr.getClossing,                 NULL,     0xff,    	1,		{"复位","合闸"},   		{"复位","合闸"}},
    { 0,  "合闸于故障",   &g_TelesignalAddr.closingFaultEvent,           NULL,     0xff,    	1,		{"无","故障"},     		{"无","故障"}},	
    { 0,  "双压禁止合闸", &g_TelesignalAddr.doubleProhibitClossing,      NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "单侧失压合闸", &g_TelesignalAddr.singleLossClosingEvent,      NULL,     0xff,    	1,		{"复位","动作"},   		{"复位","动作"}},
    { 0,  "电源侧残压闭锁合闸", &g_TelesignalAddr.remainClockClossingUab, NULL,    0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},	
	{ 0,  "负荷侧残压闭锁合闸", &g_TelesignalAddr.remainClockClossingUcb, NULL,    0xff,   		1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "小信号接地",   &g_TelesignalAddr.smallSignalEarthingTrip,     NULL,     0xff,    	1,		{"复位","跳闸"},   		{"复位","跳闸"}},
    { 0,  "遮断电流",     &g_TelesignalAddr.breakingCurrent,             NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},	
    { 0,  "过负荷",       &g_TelesignalAddr.overloadEvent,               NULL,     0xff,    	1,		{"复位","告警"},   		{"复位","告警"}},
    { 0,  "过压保护", 	  &g_TelesignalAddr.overVoltageProtection,       NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},		
    { 0,  "高频保护", 	  &g_TelesignalAddr.overFrequencyProtection,     NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "低压保护", 	  &g_TelesignalAddr.downVoltageProtection,       NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "低频保护",     &g_TelesignalAddr.downFrequencyProtection,     NULL,     0xff,    	1,		{"复位","保护"},   		{"复位","保护"}},
    { 0,  "Uab越上限",    &g_TelesignalAddr.overLimitUabUp,              NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},
    { 0,  "Uab越下限",    &g_TelesignalAddr.overLimitUabDown,            NULL,     0xff,    	1,		{"复位","越下限"}, 		{"复位","越下限"}},	
    { 0,  "Ubc越上限",    &g_TelesignalAddr.overLimitUbcUp,              NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},
    { 0,  "Ubc越下限",    &g_TelesignalAddr.overLimitUbcDown,            NULL,     0xff,    	1,		{"复位","越下限"}, 		{"复位","越下限"}},
    { 0,  "Uca越上限",    &g_TelesignalAddr.overLimitUcaUp,              NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},	
    { 0,  "Uca越下限",    &g_TelesignalAddr.overLimitUcaDown,            NULL,     0xff,    	1,		{"复位","越下限"}, 		{"复位","越下限"}},	
    { 0,  "UAB越上限",    &g_TelesignalAddr.overLimitUABUp,              NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},
    { 0,  "UAB越下限",    &g_TelesignalAddr.overLimitUABDown,            NULL,     0xff,    	1,		{"复位","越下限"}, 		{"复位","越下限"}},	
    { 0,  "UBC越上限",    &g_TelesignalAddr.overLimitUBCUp,              NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},
    { 0,  "UBC越下限",    &g_TelesignalAddr.overLimitUBCDown,            NULL,     0xff,    	1,		{"复位","越下限"}, 		{"复位","越下限"}},		
    { 0,  "U0越上限",     &g_TelesignalAddr.overLimitU0Up,               NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},	
    { 0,  "U0越下限",     &g_TelesignalAddr.overLimitU0Down,             NULL,     0xff,    	1,		{"复位","越下限"}, 		{"复位","越下限"}},	
    { 0,  "Ia越上限",     &g_TelesignalAddr.overLimitIaUp,               NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},
    { 0,  "Ia越下限",     &g_TelesignalAddr.overLimitIaDown,             NULL,     0xff,    	1,		{"复位","越下限"}, 		{"复位","越下限"}},	
    { 0,  "Ib越上限",     &g_TelesignalAddr.overLimitIbUp,               NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},
    { 0,  "Ib越下限",     &g_TelesignalAddr.overLimitIbDown,             NULL,     0xff,   		1,		{"复位","越下限"}, 		{"复位","越下限"}},	
    { 0,  "Ic越上限",     &g_TelesignalAddr.overLimitIcUp,               NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},	
    { 0,  "Ic越下限",     &g_TelesignalAddr.overLimitIcDown,             NULL,     0xff,    	1,		{"复位","越下限"}, 		{"复位","越下限"}},	
    { 0,  "I0越上限",     &g_TelesignalAddr.overLimitI0Up,               NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},	
    { 0,  "I0越下限",     &g_TelesignalAddr.overLimitI0Down,             NULL,     0xff,    	1,		{"复位","越下限"}, 		{"复位","越下限"}},		
    { 0,  "DC_U越上限",   &g_TelesignalAddr.overLimitDC_U_Up,            NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},	
    { 0,  "DC_U越下限",   &g_TelesignalAddr.overLimitDC_U_Down,          NULL,     0xff,    	1,		{"复位","越下限"}, 		{"复位","越下限"}},	
    { 0,  "DC_I越上限",   &g_TelesignalAddr.overLimitDC_I_Up,            NULL,     0xff,    	1,		{"复位","越上限"}, 		{"复位","越上限"}},	
    { 0,  "DC_I越下限",   &g_TelesignalAddr.overLimitDC_I_Down,          NULL,     0xff,    	1,		{"复位","越下限"}, 		{"复位","越下限"}},		
    { 0,  "装置掉电",     &g_TelesignalAddr.devicePowerDown,             NULL,     0xff,    	0,		{"复位","掉电"},   		{"复位","掉电"}},
};
	
/* 基本遥测 */
struct tagTelemetryCfg TelemetryCfgBase[] = 
{
//   使能 菜单编号   名称           上位机名称          点号                                	值         单位      额定值             	零漂                             死区	
    { 1,  SECONDRY,  "f",           "频率(f)",          &g_TelemetryBaseAddr.F,                 NULL,      "Hz",    50.00f,      		&g_Parameter[ZERODRIFT_F],      &g_Parameter[DEADZONE_F]},
    { 1,  SECONDRY,  "Ia",          "相电流(Ia)",       &g_TelemetryBaseAddr.Ia,                NULL,      "A",     5.00f,      		&g_Parameter[ZERODRIFT_Ia],     &g_Parameter[DEADZONE_Ia]},
    { 1,  SECONDRY,  "Ib",          "相电流(Ib)",       &g_TelemetryBaseAddr.Ib,                NULL,      "A",     5.00f,      		&g_Parameter[ZERODRIFT_Ib],     &g_Parameter[DEADZONE_Ib]},
    { 1,  SECONDRY,  "Ic",          "相电流(Ic)",       &g_TelemetryBaseAddr.Ic,                NULL,      "A",     5.00f,      		&g_Parameter[ZERODRIFT_Ic],     &g_Parameter[DEADZONE_Ic]},	
    { 1,  SECONDRY,  "I0",          "相电流(I0)",       &g_TelemetryBaseAddr.I0,                NULL,      "A",    	1.00f,     			&g_Parameter[ZERODRIFT_I0],     &g_Parameter[DEADZONE_I0]},
    { 1,  SECONDRY,  "Uab",         "线电压(Uab)",      &g_TelemetryBaseAddr.Uab,               NULL,      "V",     220.00f,      		&g_Parameter[ZERODRIFT_Uab],    &g_Parameter[DEADZONE_Uab]},
    { 1,  SECONDRY,  "Ubc",         "线电压(Ubc)",      &g_TelemetryBaseAddr.Ucb,               NULL,      "V",     220.00f,      		&g_Parameter[ZERODRIFT_Ucb],    &g_Parameter[DEADZONE_Ucb]},
    { 1,  SECONDRY,  "Uac",         "线电压(Uac)",      &g_TelemetryBaseAddr.Uac,               NULL,      "V",     220.00f,      		&g_Parameter[ZERODRIFT_Uac],    &g_Parameter[DEADZONE_Uac]},
    { 1,  SECONDRY,  "U0",          "电压(U0)",         &g_TelemetryBaseAddr.U0,                NULL,      "V",    	100.00f,     		&g_Parameter[ZERODRIFT_U0],     &g_Parameter[DEADZONE_U0]},
    { 1,  SECONDRY,  "UAB",         "线电压(UAB)",      &g_TelemetryBaseAddr.UAB,               NULL,      "V",     220.00f,      		&g_Parameter[ZERODRIFT_UAB],    &g_Parameter[DEADZONE_UAB]},
    { 1,  SECONDRY,  "UBC",         "线电压(UBC)",      &g_TelemetryBaseAddr.UCB,               NULL,      "V",     220.00f,      		&g_Parameter[ZERODRIFT_UCB],    &g_Parameter[DEADZONE_UCB]},
    { 1,  SECONDRY,  "P",           "有功功率(P)",      &g_TelemetryBaseAddr.P,                 NULL,      "kW",    1.905f,      		&g_Parameter[ZERODRIFT_P],      &g_Parameter[DEADZONE_P]},
    { 1,  SECONDRY,  "Q",           "无功功率(Q)",      &g_TelemetryBaseAddr.Q,                 NULL,      "kVar",  1.905f,      		&g_Parameter[ZERODRIFT_Q],      &g_Parameter[DEADZONE_Q]},
    { 1,  SECONDRY,  "S",           "视在功率(S)",      &g_TelemetryBaseAddr.S,                 NULL,      "kVA",   1.905f,      		&g_Parameter[ZERODRIFT_S],      &g_Parameter[DEADZONE_S]},
    { 1,  SECONDRY,  "pf",          "功率因数(pf)",     &g_TelemetryBaseAddr.PF,                NULL,      "-",    	1.00f,     			&g_Parameter[ZERODRIFT_PF],     &g_Parameter[DEADZONE_PF]},
    { 1,  SECONDRY,  "DC1",         "直流(DC1)",        &g_TelemetryBaseAddr.DC1,               NULL,      "V",     24.00f,      		&g_Parameter[ZERODRIFT_DC1],    &g_Parameter[DEADZONE_DC1]},
    { 0,  SECONDRY,  "DC2",         "直流(DC2)",        &g_TelemetryBaseAddr.DC2,               NULL,      "V",     24.00f,      		&g_Parameter[ZERODRIFT_DC2],    &g_Parameter[DEADZONE_DC2]},
    { 1,  SECONDRY,  "T",           "温度(T)",          &g_TelemetryBaseAddr.T,                 NULL,      "°C",   40.00f,      		&g_Parameter[ZERODRIFT_T],      &g_Parameter[DEADZONE_T]},

	{ 1,  SECONDRY,  "∠Uab_Ia",    "∠Uab_Ia",         &g_TelemetryBaseAddr.alphy_Uab_Ia,      NULL,      "°",    360.00f,      		NULL,                            NULL},
    { 1,  SECONDRY,  "∠Ucb_Ic",    "∠Ucb_Ic",         &g_TelemetryBaseAddr.alphy_Ucb_Ic,      NULL,      "°",    360.00f,      		NULL,                            NULL},	
    { 1,  SECONDRY,  "∠U0_I0",     "∠U0_I0",          &g_TelemetryBaseAddr.alphy_U0_I0,       NULL,      "°",    360.00f,      		NULL,                            NULL},
    { 1,  SECONDRY,  "∠Ux_Ux",     "∠Ux_Ux",          &g_TelemetryBaseAddr.alphy_Ux_Ux,       NULL,      "°",    360.00f,      		NULL,                            NULL},	
    { 1,  SECONDRY,  "f2",          "频率(f2)",         &g_TelemetryBaseAddr.F2,                NULL,      "Hz",    50.00f,      		&g_Parameter[ZERODRIFT_F],      &g_Parameter[DEADZONE_F]},
    { 1,  SECONDRY,  "F/S",         "滑差(F/S)",        &g_TelemetryBaseAddr.F_S,               NULL,      "H/s",   50.00f,      		NULL,                            NULL},	
	
    { 1,  ONCE,      "Ia",          "一次Ia",           &g_TelemetryBaseAddr.IaOnce,            NULL,      "A",     600.0f,      		NULL,                           &g_Parameter[DEADZONE_Ia]},
    { 1,  ONCE,      "Ib",          "一次Ib",           &g_TelemetryBaseAddr.IbOnce,            NULL,      "A",     600.0f,      		NULL,                           &g_Parameter[DEADZONE_Ib]},
    { 1,  ONCE,      "Ic",          "一次Ic",           &g_TelemetryBaseAddr.IcOnce,            NULL,      "A",     600.0f,      		NULL,                           &g_Parameter[DEADZONE_Ic]},
    { 1,  ONCE,      "I0",          "一次I0",           &g_TelemetryBaseAddr.I0Once,            NULL,      "A",     20.0f,      		NULL,                           &g_Parameter[DEADZONE_I0]},
    { 1,  ONCE,      "Uab",         "一次Uab",          &g_TelemetryBaseAddr.UabOnce,           NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_Uab]},
    { 1,  ONCE,      "Ubc",         "一次Ubc",          &g_TelemetryBaseAddr.UbcOnce,           NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_Ucb]},
    { 1,  ONCE,      "Uca",         "一次Uca",          &g_TelemetryBaseAddr.UcaOnce,           NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_Uac]},
    { 1,  ONCE,      "U0",          "一次U0",           &g_TelemetryBaseAddr.U0Once,            NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_U0]},
    { 1,  ONCE,      "UAB",         "一次UAB",          &g_TelemetryBaseAddr.UABOnce,           NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_UAB]},
    { 1,  ONCE,      "UBC",         "一次UBC",          &g_TelemetryBaseAddr.UBCOnce,           NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_UCB]},
    { 1,  ONCE,      "P",           "一次P",            &g_TelemetryBaseAddr.POnce,             NULL,      "kW",    8181.81f,      		NULL,                           &g_Parameter[DEADZONE_P]},
    { 1,  ONCE,      "Q",           "一次Q",            &g_TelemetryBaseAddr.QOnce,             NULL,      "kVar",  8181.81,      		NULL,                           &g_Parameter[DEADZONE_Q]},
    { 1,  ONCE,      "S",           "一次S",            &g_TelemetryBaseAddr.SOnce,             NULL,      "kVA",   8181.81,      		NULL,                           &g_Parameter[DEADZONE_S]},		

    { 1,  HARMONIC,  "三次Uab",     "三次谐波Uab",      &g_TelemetryBaseAddr.thirdharmonicUab,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Uab],    &g_Parameter[DEADZONE_Uab]},	
    { 1,  HARMONIC,  "三次Ubc",     "三次谐波Ubc",      &g_TelemetryBaseAddr.thirdharmonicUbc,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Ucb],    &g_Parameter[DEADZONE_Ucb]},
    { 1,  HARMONIC,  "三次Uca",     "三次谐波Uca",      &g_TelemetryBaseAddr.thirdharmonicUca,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Uac],    &g_Parameter[DEADZONE_Uac]},
    { 1,  HARMONIC,  "三次U0",      "三次谐波U0",       &g_TelemetryBaseAddr.thirdharmonicU0,   NULL,      "V",    	100.0f,     	&g_Parameter[ZERODRIFT_U0],     &g_Parameter[DEADZONE_U0]},
    { 1,  HARMONIC,  "三次Ia",      "三次谐波Ia",       &g_TelemetryBaseAddr.thirdharmonicIa,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ia],     &g_Parameter[DEADZONE_Ia]},
    { 1,  HARMONIC,  "三次Ib",      "三次谐波Ib",       &g_TelemetryBaseAddr.thirdharmonicIb,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ib],     &g_Parameter[DEADZONE_Ib]},
    { 1,  HARMONIC,  "三次Ic",      "三次谐波Ic",       &g_TelemetryBaseAddr.thirdharmonicIc,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ic],     &g_Parameter[DEADZONE_Ic]},
    { 1,  HARMONIC,  "三次I0",      "三次谐波I0",       &g_TelemetryBaseAddr.thirdharmonicI0,   NULL,      "A",    	100.0f,     	&g_Parameter[ZERODRIFT_I0],     &g_Parameter[DEADZONE_I0]},	
    { 1,  HARMONIC,  "五次Uab",     "五次谐波Uab",      &g_TelemetryBaseAddr.fifthharmonicUab,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Uab],    &g_Parameter[DEADZONE_Uab]},	
    { 1,  HARMONIC,  "五次Ubc",     "五次谐波Ubc",      &g_TelemetryBaseAddr.fifthharmonicUbc,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Ucb],    &g_Parameter[DEADZONE_Ucb]},
    { 1,  HARMONIC,  "五次Uca",     "五次谐波Uca",      &g_TelemetryBaseAddr.fifthharmonicUca,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Uac],    &g_Parameter[DEADZONE_Uac]},
    { 1,  HARMONIC,  "五次U0",      "五次谐波U0",       &g_TelemetryBaseAddr.fifthharmonicU0,   NULL,      "V",    	100.0f,     	&g_Parameter[ZERODRIFT_U0],     &g_Parameter[DEADZONE_U0]},
    { 1,  HARMONIC,  "五次Ia",      "五次谐波Ia",       &g_TelemetryBaseAddr.fifthharmonicIa,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ia],     &g_Parameter[DEADZONE_Ia]},
    { 1,  HARMONIC,  "五次Ib",      "五次谐波Ib",       &g_TelemetryBaseAddr.fifthharmonicIb,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ib],     &g_Parameter[DEADZONE_Ib]},
    { 1,  HARMONIC,  "五次Ic",      "五次谐波Ic",       &g_TelemetryBaseAddr.fifthharmonicIc,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ic],     &g_Parameter[DEADZONE_Ic]},
    { 1,  HARMONIC,  "五次I0",      "五次谐波I0",       &g_TelemetryBaseAddr.fifthharmonicI0,   NULL,      "A",    	1.00f,     		&g_Parameter[ZERODRIFT_I0],     &g_Parameter[DEADZONE_I0]},		

    { 0,  0xff,      "软件版本",    "软件版本",         &g_TelemetryBaseAddr.softwareVersion,   NULL,      "--",    	0,     NULL,     NULL},	
    { 0,  0xff,      "工作模式",    "工作模式",         &g_TelemetryBaseAddr.workMode,          NULL,      "--",    	0,     NULL,     NULL},	
};	

/* 遥控 */
struct tagTelecontrolCfg TelecontrolCfg[] =
{
    {1, "远方遥控",    13,  {"分闸选择失败", "分闸选择成功", "分闸执行失败", "分闸执行成功", "合闸选择失败", "合闸选择成功", "合闸执行失败", "合闸执行成功", "遥控取消","合环选择失败", "合环选择成功", "合环执行失败", "合环执行成功"}},
    {1, "远方活化",     2,  {"失败", "成功"}},	
	{1, "远方复位",     2,  {"失败", "成功"}},
    {1, "远方清除记录", 2,  {"失败", "成功"}},
    {1, "远方分段模式",13,  {"退出选择失败", "退出选择成功", "退出执行失败", "退出执行成功", "投入选择失败", "投入选择成功", "投入执行失败", "投入执行成功", "遥控取消","", "", "", ""}},
    {1, "远方联络模式",13,  {"退出选择失败", "退出选择成功", "退出执行失败", "退出执行成功", "投入选择失败", "投入选择成功", "投入执行失败", "投入执行成功", "遥控取消","", "", "", ""}},
    {1, "远方重合闸",  13,  {"退出选择失败", "退出选择成功", "退出执行失败", "退出执行成功", "投入选择失败", "投入选择成功", "投入执行失败", "投入执行成功", "遥控取消","", "", "", ""}},
    {1, "远方保护",    13,  {"退出选择失败", "退出选择成功", "退出执行失败", "退出执行成功", "投入选择失败", "投入选择成功", "投入执行失败", "投入执行成功", "遥控取消","", "", "", ""}},	
    {0, "本地操作",    13,  {"分闸选择失败", "分闸选择成功", "分闸执行失败", "分闸执行成功", "合闸选择失败", "合闸选择成功", "合闸执行失败", "合闸执行成功", "遥控取消","合环选择失败", "合环选择成功", "合环执行失败", "合环执行成功"}},
	{0, "本地复位",     2,  {"失败", "成功"}},
    {0, "本地清除记录", 2,  {"失败", "成功"}},
    {0, "手持操作",    13,  {"分闸选择失败", "分闸选择成功", "分闸执行失败", "分闸执行成功", "合闸选择失败", "合闸选择成功", "合闸执行失败", "合闸执行成功", "遥控取消","合环选择失败", "合环选择成功", "合环执行失败", "合环执行成功"}},	
};

/* 日志 */
struct tagUlogCfg UlogCfg[] =
{
    {1, "终端启动", 2,  {"上电", "上电"}},
    {1, "定值修改", 2,  {"修改", "修改"}},
    {1, "开关状态", 2,  {"异常", "恢复"}},
    {1, "设备软件", 2,  {"升级", "升级"}},
    {1, "任务调度", 2,  {"异常", "恢复"}},
};

/* 固有参数 */
struct tagInherentParaCfg InherentParaCfg[] = 
{
    {1, "终端类型",             g_InherentPara.terminalType},  
    {1, "终端操作系统",         g_InherentPara.terminalOpreatSystem}, 
    {1, "终端制造商",           g_InherentPara.terminalManufacturer}, 	
    {1, "终端硬件版本",         g_InherentPara.terminalHardwareVersion},  
    {1, "终端软件版本",         g_InherentPara.terminalSoftwareVersion}, 
    {1, "软件版本校验码",       g_InherentPara.terminalSoftwareVersionCheckCode}, 	
    {1, "通讯规约类型",         g_InherentPara.terminalCommunicationProtocolType},  
    {1, "终端出厂型号",         g_InherentPara.terminalFactoryModel}, 
    {1, "终端ID号",             g_InherentPara.terminalID}, 		
    {1, "终端网卡MAC地址",      g_InherentPara.terminalMAC}, 
    {1, "终端产品序列号",       g_InherentPara.terminalProductSerialNumber}, 	
};
	
/* 校准系数 */
struct tagCalibrateFactor CalibrateFactorCfg[] = 
{
//   使能          名称                  校准值	          点号                             系数值                         		数据类型   系数上限值       系数下限值      系数缺省值
    { 1,    "频率(f)校准系数",           50.00f,   &g_TelemetryBaseAddr.F,                &g_CalibrateFactor[CALIFACTOR_F],      		0,    1.2f,            0.8f,           1.0f},
    { 1,    "A相电流(Ia)校准系数",       5.00f,    &g_TelemetryBaseAddr.Ia,               &g_CalibrateFactor[CALIFACTOR_Ia],     		0,    0.005f,         0.0035f,         0.00433378899f},
    { 1,    "B相电流(Ib)校准系数",       5.00f,    &g_TelemetryBaseAddr.Ib,               &g_CalibrateFactor[CALIFACTOR_Ib],     		0,    0.005f,         0.0035f,         0.00432677846f},
    { 1,    "C相电流(Ic)校准系数",       5.00f,    &g_TelemetryBaseAddr.Ic,               &g_CalibrateFactor[CALIFACTOR_Ic],     		0,    0.005f,         0.0035f,         0.00432838406f},
  #ifdef SYNTHESIS_I0_ENABLE
    { 1,    "零序电流(I0)校准系数",      1.00f,    &g_TelemetryBaseAddr.I0,               &g_CalibrateFactor[CALIFACTOR_I0],     		0,    0.005f,         0.0035f,         0.00432f},
  #else		
    { 1,    "零序电流(I0)校准系数",      1.00f,    &g_TelemetryBaseAddr.I0,               &g_CalibrateFactor[CALIFACTOR_I0],     		0,    0.001f,         0.0004f,         0.000866f}, 
  #endif		
    { 1,    "线电压(Uab)校准系数",       100.00f,  &g_TelemetryBaseAddr.Uab,              &g_CalibrateFactor[CALIFACTOR_Uab],    		0,    0.02f,          0.0065f,         0.009516f},
    { 1,    "线电压(Ucb)校准系数",       100.00f,  &g_TelemetryBaseAddr.Ucb,              &g_CalibrateFactor[CALIFACTOR_Ucb],    		0,    0.02f,          0.0065f,         0.009516f},
    { 1,    "线电压(Uac)校准系数",       100.00f,  &g_TelemetryBaseAddr.Uac,              &g_CalibrateFactor[CALIFACTOR_Uac],    		0,    0.02f,          0.0065f,         0.009516f},
    { 1,    "线电压(U0)校准系数",        50.00f,   &g_TelemetryBaseAddr.U0,               &g_CalibrateFactor[CALIFACTOR_U0],     		0,    0.02f,          0.0065f,         0.009516f},
    { 1,    "线电压(UAB)校准系数",       100.00f,  &g_TelemetryBaseAddr.UAB,              &g_CalibrateFactor[CALIFACTOR_UAB],    		0,    0.02f,          0.0065f,         0.009516f},
    { 1,    "线电压(UCB)校准系数",       100.00f,  &g_TelemetryBaseAddr.UCB,              &g_CalibrateFactor[CALIFACTOR_UCB],    		0,    0.02f,          0.0065f,         0.009516f},

    { 1,    "直流电压1校准系数",         24,       &g_TelemetryBaseAddr.DC1,              &g_CalibrateFactor[CALIFACTOR_DC1],    		0,    0.05f,          0.0001f,         0.0013745f},
    { 1,    "直流电压2校准系数",         24,       &g_TelemetryBaseAddr.DC2,              &g_CalibrateFactor[CALIFACTOR_DC2],    		0,    0.05f,          0.0001f,         0.0013745f},
    { 1,    "温度校准系数",              0,        &g_TelemetryBaseAddr.T,                &g_CalibrateFactor[CALIFACTOR_T],      		0,    2.0f,           0.0001f,         1.0f},
	
    { 1,    "Uab_Ia夹角校准系数",        0,        &g_TelemetryBaseAddr.alphy_Uab_Ia,     &g_CalibrateFactor[CALIFACTOR_ALPHA_UabIa],   1,    0.6f,           0.2f,           0.359226197f},
    { 1,    "Ucb_Ic夹角校准系数",        0,        &g_TelemetryBaseAddr.alphy_Ucb_Ic,     &g_CalibrateFactor[CALIFACTOR_ALPHA_UcbIc],   1,    0.6f,           0.2f,           0.416621923f},
    { 1,    "U0_I0夹角校准系数",         0,        &g_TelemetryBaseAddr.alphy_U0_I0,      &g_CalibrateFactor[CALIFACTOR_ALPHA_U0I0],    1,    0.6f,           0.2f,           0.32171011f},
    { 1,    "Ux_Ux夹角校准系数",         0,        &g_TelemetryBaseAddr.alphy_Ux_Ux,      &g_CalibrateFactor[CALIFACTOR_ALPHA_UxUx],    1,    0.2f,           -0.2f,          0.0502243042f},	
    { 1,    "频率(f2)校准系数",          50.00f,   &g_TelemetryBaseAddr.F2,               &g_CalibrateFactor[CALIFACTOR_F2],     		0,    1.2f,            0.8f,           1.0f},	
};


/* 定值0区 */
struct tagValueParaCfg ParameterCfg[] = 
{
//   使能            使能 菜单编号          名称                             值                         单位      上限        下限   缺省值    数据类型  小数点位数       显示内容           备注  
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "开关类型",        &g_Parameter[SWTICH_CLASS],                 "-",       1,           0,     0,         2,         0,         {"断路器","负荷开关"},   " "   },        	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "分段软压板",      &g_Parameter[MODBREAK_STRAP],               "-",       1,           0,     1,         2,         0,         {"退", "投"        },   " "   },   
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "联络软压板",      &g_Parameter[MODCONTACT_STRAP],             "-",       1,           0,     1,         2,         0,         {"退", "投"        },   " "   }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "重合闸软压板",    &g_Parameter[RECLOSE_STRAP],                "-",       1,           0,     1,         2,         0,         {"退", "投"        },   " "   }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "保护软压板",      &g_Parameter[FUNCTION_STRAP],               "-",       1,           0,     1,         2,         0,         {"退", "投"        },   " "   }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "负荷保护电压",    &g_Parameter[CFG_PRO_VOL_N],                "-",       1,           0,     1,         2,         0,         {"UAB", "UCB"},   " "   },     
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "功率电压cb",      &g_Parameter[CFG_POW_VOL_CB],               "-",       1,           0,     1,         2,         0,         {"Ucb", "UCB"},   " "   }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "一次变比_U",      &g_Parameter[RATIO_U_ONE_TURN],             "V",    100000.00f,   0.00f,   10000.00f, 0,         3,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "二次变比_U",      &g_Parameter[RATIO_U_SECONDARY],            "V",     1000.00f,   0.00f,    220.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "一次变比_U0",     &g_Parameter[RATIO_U0_ONE_TURN],            "V",     10000.00f,   0.00f,   1,         0,         3,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "二次变比_U0",     &g_Parameter[RATIO_U0_SECONDARY],           "V",     1000.00f,   0.00f,    1,         0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "一次变比_I",      &g_Parameter[RATIO_I_ONE_TURN],             "A",    100000.00f,   0.00f,   600,       0,         3,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "二次变比_I",      &g_Parameter[RATIO_I_SECONDARY],            "A",     1000.00f,   0.00f,    5,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "一次变比_I0",     &g_Parameter[RATIO_I0_ONE_TURN],            "A",     10000.00f,   0.00f,   20,        0,         3,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "二次变比_I0",     &g_Parameter[RATIO_I0_SECONDARY],           "A",     10000.00f,   0.00f,   1,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "开入防抖时间",    &g_Parameter[DI_SHAKING_TIME],              "ms",     99999,        0,     20.0f,     0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "合闸脉宽时间",    &g_Parameter[CLOSING_PULSE_TIME],           "ms",     99999,        0,     60,        0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "分闸脉宽时间",    &g_Parameter[OPENING_PULSE_TIME],           "ms",     99999,        0,     60,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "反校时间",        &g_Parameter[REVERSE_TIME],                 "ms",     99999,        5,     60000.0,   0,         0,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "电源告警取反",    &g_Parameter[POWERLOSS_NEGATE],             "-",       1,           0,     0,         2,         0,         {"无", "取反"},   " "   },	  
 
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "频率零漂",        &g_Parameter[ZERODRIFT_F],                 	"Hz",     99.99f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "A相电流零漂",     &g_Parameter[ZERODRIFT_Ia],                	"A",      99.99f,     0.00001f,   0.005f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "B相电流零漂",     &g_Parameter[ZERODRIFT_Ib],                	"A",      99.99f,     0.00001f,   0.005f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "C相电流零漂",     &g_Parameter[ZERODRIFT_Ic],                	"A",      99.99f,     0.00001f,   0.005f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "零序电流零漂",    &g_Parameter[ZERODRIFT_I0],                	"A",      99.99f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "Uab零漂",         &g_Parameter[ZERODRIFT_Uab],               	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "Ucb零漂",         &g_Parameter[ZERODRIFT_Ucb],               	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },		
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "Uac零漂",         &g_Parameter[ZERODRIFT_Uac],               	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "U0零漂",          &g_Parameter[ZERODRIFT_U0],                	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "UAB零漂",         &g_Parameter[ZERODRIFT_UAB],               	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "UCB零漂",         &g_Parameter[ZERODRIFT_UCB],               	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "有功功率零漂",    &g_Parameter[ZERODRIFT_P],                 	"kW",     999.99f,    0.00001f,   0.0001f,     0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "无功功率零漂",    &g_Parameter[ZERODRIFT_Q],                 	"kVar",   999.99f,    0.00001f,   0.0001f,     0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "视在功率零漂",    &g_Parameter[ZERODRIFT_S],                 	"kVA",    999.99f,    0.00001f,   0.0001f,     0,         3,         {"-",  "-"         },   " "    },		
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "功率因数零漂",    &g_Parameter[ZERODRIFT_PF],                	"-",      1.0f,       0.00001f,   0.001f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "直流1零漂",       &g_Parameter[ZERODRIFT_DC1],               	"V",      60.0f,      0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "直流2零漂",       &g_Parameter[ZERODRIFT_DC2],               	"V",      60.0f,      0.00001f,   9.9f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "温度零漂",        &g_Parameter[ZERODRIFT_T],                 	"°C",    100.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "频率死区",        &g_Parameter[DEADZONE_F],                   "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "A相电流死区",     &g_Parameter[DEADZONE_Ia],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "B相电流死区",     &g_Parameter[DEADZONE_Ib],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "C相电流死区",     &g_Parameter[DEADZONE_Ic],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "零序电流死区",    &g_Parameter[DEADZONE_I0],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "Uab死区",         &g_Parameter[DEADZONE_Uab],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "Ucb死区",         &g_Parameter[DEADZONE_Ucb],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },		
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "Uac死区",         &g_Parameter[DEADZONE_Uac],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "U0死区",          &g_Parameter[DEADZONE_U0],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "UAB死区",         &g_Parameter[DEADZONE_UAB],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "UCB死区",         &g_Parameter[DEADZONE_UCB],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "有功功率死区",    &g_Parameter[DEADZONE_P],                   "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "无功功率死区",    &g_Parameter[DEADZONE_Q],                   "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "视在功率死区",    &g_Parameter[DEADZONE_S],                   "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },		
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "功率因数死区",    &g_Parameter[DEADZONE_PF],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "直流1死区",       &g_Parameter[DEADZONE_DC1],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "直流2死区",       &g_Parameter[DEADZONE_DC2],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "温度死区",        &g_Parameter[DEADZONE_T],                   "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "U额定值",         &g_Parameter[DEADZONE_U_VALUE],             "V",      300.0f,     	0.01f,   220.0f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "I额定值",         &g_Parameter[DEADZONE_I_VALUE],             "A",      999.0f,     	0.01f,   5.0f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "U0额定值",        &g_Parameter[DEADZONE_U0_VALUE],            "V",      300.0f,     	0.01f,   100.0f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "I0额定值",        &g_Parameter[DEADZONE_I0_VALUE],            "A",      999.0f,     	0.01f,   1.0f,        0,         3,         {"-",  "-"         },   " "    },	
	
    { &g_MenuEnable, 1,  ME_UART_COM,    "串口号",          &g_Parameter[UART_PORT],                    "-",       1,           0,     0,          2,         0,         {"串口中", "串口右"},   " "   }, 
    { &g_MenuEnable, 1,  ME_UART_COM,    "波特率",          &g_Parameter[UART_BAUDRATE],                "-",       4,           0,     2,          5,         0,         {"2400", "4800", "9600", "38400", "115200"},   " "   }, 
    { &g_MenuEnable, 1,  ME_UART_COM,    "数据位",          &g_Parameter[UART_WORDLENGTH],              "-",       9.00f,     5.00f,    8.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_UART_COM,    "停止位",          &g_Parameter[UART_STOPBITS],                "-",       3.00f,     0.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                    
    { &g_MenuEnable, 1,  ME_UART_COM,    "校验位",          &g_Parameter[UART_PARITY],                  "-",       2,           0,     0,          3,         0,         {"无校验", "奇校验", "偶校验"},   " "   },                      
    { &g_MenuEnable, 1,  ME_UART_COM,    "101模式",         &g_Parameter[UART_BALANMODE],               "-",       1,           0,     1,          2,         0,         {"非平衡", "平衡"},   " "   },                 
    { &g_MenuEnable, 1,  ME_UART_COM,    "从站地址",        &g_Parameter[UART_SOURCEADDR],              "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                 
    { &g_MenuEnable, 1,  ME_UART_COM,    "从站地址长度",    &g_Parameter[UART_LINKADDRSIZE],            "-",       2.00f,     1.00f,    2.00f,     0,         0,         {"-",  "-"         },   " "    },                    
    { &g_MenuEnable, 1,  ME_UART_COM,    "传送原因长度",    &g_Parameter[UART_ASDUCOTSIZE],             "-",       2.00f,     1.00f,    2.00f,     0,         0,         {"-",  "-"         },   " "    },              
    { &g_MenuEnable, 1,  ME_UART_COM,    "ASDU地址",        &g_Parameter[UART_ASDUADDR],                "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                
    { &g_MenuEnable, 1,  ME_UART_COM,    "ASDU地址长度",    &g_Parameter[UART_ASDUADDRSIZE],            "-",       2.00f,     1.00f,    2.00f,     0,         0,         {"-",  "-"         },   " "    },                         
    
    { &g_MenuEnable, 1,  ME_NET_COM,     "上IP_1",         &g_Parameter[NET_IP1_0],                     "-",     255.00f,     0.00f,  192.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_NET_COM,     "上IP_2",         &g_Parameter[NET_IP1_1],                     "-",     255.00f,     0.00f,  168.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "上IP_3",         &g_Parameter[NET_IP1_2],                     "-",     255.00f,     0.00f,   60.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "上IP_4",         &g_Parameter[NET_IP1_3],                     "-",     255.00f,     0.00f,  100.00f,     0,         0,         {"-",  "-"         },   " "    },    
    { &g_MenuEnable, 1,  ME_NET_COM,     "下IP_1",         &g_Parameter[NET_IP2_0],                     "-",     255.00f,     0.00f,  192.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_NET_COM,     "下IP_2",         &g_Parameter[NET_IP2_1],                     "-",     255.00f,     0.00f,  168.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "下IP_3",         &g_Parameter[NET_IP2_2],                     "-",     255.00f,     0.00f,   60.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "下IP_4",         &g_Parameter[NET_IP2_3],                     "-",     255.00f,     0.00f,  120.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_NET_COM,     "网关_1",         &g_Parameter[NET_GATEWAY_0],                 "-",     255.00f,     0.00f,  192.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_NET_COM,     "网关_2",         &g_Parameter[NET_GATEWAY_1],                 "-",     255.00f,     0.00f,  168.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "网关_3",         &g_Parameter[NET_GATEWAY_2],                 "-",     255.00f,     0.00f,   60.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "网关_4",         &g_Parameter[NET_GATEWAY_3],                 "-",     255.00f,     0.00f,  254.00f,     0,         0,         {"-",  "-"         },   " "    },                         
    { &g_MenuEnable, 1,  ME_NET_COM,     "从站地址",       &g_Parameter[NET_SOURCEADDR],                "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                
    { &g_MenuEnable, 1,  ME_NET_COM,     "ASDU地址",       &g_Parameter[NET_ASDUADDR],                  "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },   
}; 

/* 定值一区 */
struct tagValueParaCfg FixedValueCfg1[] = 
{
//   使能                  使能   菜单编号       名称                            值                           	   单位      上限        下限   	缺省值     数据类型   小数点位数       显示内容           备注
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "过流一段投入",    	&g_FixedValue1[OVER_CURRENT_SWITCH1],          "-",     2,        	0,     		0,          3,         0,         {"退", "投", "告警"},   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "过流一段定值",    	&g_FixedValue1[OVER_CURRENT_VALUE1],           "A",     100.00f,    0.00f,  	10,         0,         3,         {"-",  "-"         },   " "   }, 
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "过流一段时间",    	&g_FixedValue1[OVER_CURRENT_TIME1],            "S",     20.00f,     0.00f,  	0,          0,         3,         {"-",  "-"         },   " "   },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "过流二段投入",    	&g_FixedValue1[OVER_CURRENT_SWITCH2],          "-",     2,          0,     		0,          3,         0,         {"退", "投", "告警"},   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "过流二段定值",    	&g_FixedValue1[OVER_CURRENT_VALUE2],           "A",     100.00f,    0.00f,  	8,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "过流二段时间",    	&g_FixedValue1[OVER_CURRENT_TIME2],            "S",     20.00f,     0.00f,  	3,          0,         3,         {"-",  "-"         },   " "   },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "过流三段投退",    	&g_FixedValue1[OVER_CURRENT_SWITCH3],          "-",     2,          0,     		0,          3,         0,         {"退", "投", "告警"},   " "   },    
    { &g_MenuUnable,       0,  COM_PROTEC,   "过流三段电流",    	&g_FixedValue1[OVER_CURRENT_VALUE3],           "A",     100.00f,    0.00f,  	5,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_MenuUnable,       0,  COM_PROTEC,   "过流三段时间",    	&g_FixedValue1[OVER_CURRENT_TIME3],            "S",     20.00f,     0.00f,   	5,          0,         3,         {"-",  "-"         },   " "   },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,  "过流加速段投入",    	&g_FixedValue1[IACCI_SWITCH],                  "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,  "过流加速段定值",    	&g_FixedValue1[IACCI_CURRENT_VALUE],           "A",     99.99f,     0.00f,   	6,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_MenuUnable,       0,  COM_PROTEC,  "相间加速延时",    		&g_FixedValue1[IACCI_TIME],                    "S",     99.99f,     0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },
	{ &g_FixedMenu0Enable, 1,  COM_PROTEC,   "零序过流一段投入",	&g_FixedValue1[OVER_CURRENTI0_SWITCH1],        "-",     2,          0,     		0,          3,         0,         {"退", "投", "告警"},   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "零序过流一段定值",	&g_FixedValue1[OVER_CURRENTI0_VALUE1],         "A",     100.00f,    0.00f,   	5,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "零序过流一段时间",	&g_FixedValue1[OVER_CURRENTI0_TIME1],          "S",     20.00f,     0.00f,   	3,          0,         3,         {"-",  "-"         },   " "   },	    
    { &g_MenuUnable,       0,  COM_PROTEC,   "零序二段投退",    	&g_FixedValue1[OVER_CURRENTI0_SWITCH2],        " ",     2,          0,     		0,          3,         0,         {"退", "投", "告警"},   " "    },    
    { &g_MenuUnable,       0,  COM_PROTEC,   "零序二段电流",    	&g_FixedValue1[OVER_CURRENTI0_VALUE2],         "A",     100.00f,    0.00f,   	5,          0,         3,         {"-",  "-"         },   " "    }, 
    { &g_MenuUnable,       0,  COM_PROTEC,   "零序二段时间",    	&g_FixedValue1[OVER_CURRENTI0_TIME2],          "S",     20.00f,     0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },		
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,  "零序过流加速段投入",  	&g_FixedValue1[IACCI0_SWITCH],                 "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,  "零序过流加速段定值",  	&g_FixedValue1[IACCI0_CURRENT_VALUE],          "A",     99.99f,     0.00f,   	6,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_MenuUnable,       0,  COM_PROTEC,  "零流加速延时",    		&g_FixedValue1[IACCI0_TIME],                   "S",     99.99f,     0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "二次重合闸闭锁时间",	&g_FixedValue1[SECONDARY_RECLOSE_LOCK_TIME],   "S",     99.99f,     0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "一次重合闸投入",    	&g_FixedValue1[RECLOSE_ONE_SWITCH],            "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "二次重合闸投入",    	&g_FixedValue1[RECLOSE_TOW_SWITCH],            "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "   },    
	{ &g_MenuUnable, 	   0,  COM_PROTEC,   "相间重合次数",    	&g_FixedValue1[RECLOSE_TIMES_I],               "次",    4,          0,     		2,          0,         3,         {"-",  "-"         },   " "   },
    { &g_MenuUnable,       0,  COM_PROTEC,   "零序重合次数",    	&g_FixedValue1[RECLOSE_TIMES_I0],              "次",    2,          0,     		0,          0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "一次重合闸时间",    	&g_FixedValue1[RECLOSE_TIMER1],                "S",     99.99f,     0.00f,   	2,          0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "二次重合闸时间",    	&g_FixedValue1[RECLOSE_TIMER2],                "S",     99.99f,     0.00f,   	10,         0,         3,         {"-",  "-"         },   " "   },
    { &g_MenuUnable,       0,  COM_PROTEC,   "三次重合时间",    	&g_FixedValue1[RECLOSE_TIMER3],                "S",     99.99f,     0.00f,   	15,         0,         3,         {"-",  "-"         },   " "   },
    { &g_MenuUnable,       0,  COM_PROTEC,   "四次重合时间",    	&g_FixedValue1[RECLOSE_TIMER4],                "S",     99.99f,     0.00f,   	20,         0,         3,         {"-",  "-"         },   " "   },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "重合闸充电时间",    	&g_FixedValue1[RE_RESET_TIME],                 "S",     99.99f,     0.00f,   	30,          0,         3,         {"-",  "-"         },   " "   },
	{ &g_FixedMenu0Enable, 1,  COM_PROTEC,   "过电压投入",    		&g_FixedValue1[OVERVOLTAGE_SWITCH],            "-",     2,          0,     		0,          3,         0,         {"退", "投", "告警"},   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "过电压定值",    		&g_FixedValue1[OVERVOLTAGE_VALUE],             "V",     300,       	0.00f,   	110,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "过电压时间",    		&g_FixedValue1[OVERVOLTAGE_TIME],              "S",     999,     	0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "高压解列系数",    	&g_FixedValue1[OVERVOLTAGE_FACTOR],            "-",     1.00f,      0.01f,   	1,          0,         3,         {"-",  "-"         },   " "    },			
    { &g_MenuUnable,       0,  COM_PROTEC,   "低压解列投退",    	&g_FixedValue1[DOWNVOLTAGE_SWITCH],            "-",     2,          0,     		0,          3,         0,         {"退", "投", "告警"},   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "低压解列定值",    	&g_FixedValue1[DOWNVOLTAGE_VALUE],             "V",     300,   		0.00f,   	30,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "低压解列延时",    	&g_FixedValue1[DOWNVOLTAGE_TIME],              "S",     999,     	0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "低压解列系数",    	&g_FixedValue1[DOWNVOLTAGE_FACTOR],            "-",     2.00f,      1.00f,   	1,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "高频投入",    		&g_FixedValue1[OVERFREQUENCY_SWITCH],          "-",     2,          0,     		0,          3,         0,         {"退", "投", "告警"},   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "高频定值",    		&g_FixedValue1[OVERFREQUENCY_VALUE],           "Hz",    55.00f,     50.01f,  	60,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "高频时间",    		&g_FixedValue1[OVERFREQUENCY_TIME],            "S",     999,     	0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "高周解列系数",    	&g_FixedValue1[OVERFREQUENCY_FACTOR],          "-",     1.00f,      0.01f,   	1,          0,         3,         {"-",  "-"         },   " "    },
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "低频投入",    		&g_FixedValue1[DOWNFREQUENCY_SWITCH],          "-",     2,          0,     		0,          3,         0,         {"退", "投", "告警"},   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "低频定值",    		&g_FixedValue1[DOWNFREQUENCY_VALUE],           "Hz",    49.99f,     45.00f,  	49.5,       0,         3,         {"-",  "-"         },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "低频时间",    		&g_FixedValue1[DOWNFREQUENCY_TIME],            "S",     999,     	0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "低周解列系数",    	&g_FixedValue1[DOWNFREQUENCY_FACTOR],          "-",     2.00f,      1.00f,   	1,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "滑差闭锁投入",    	&g_FixedValue1[SLIPLOCK_SWITCH],               "-",     2,          0,     		0,          2,         0,         {"退", "投",       },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "滑差闭锁定值",    	&g_FixedValue1[SLIPLOCK_VALUE],                "Hz/s",  10.00f,      0.30f,   	10,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "过负荷投退",      	&g_FixedValue1[OVERLOAD_SWITCH],               "-",     2,          0,     		0,          3,         0,         {"退", "投", "告警"},   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "过负荷定值",      	&g_FixedValue1[OVERLOAD_VALUE],                "A",     999.99f,    0.00f,   	8,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "过负荷系数",      	&g_FixedValue1[OVERLOAD_FACTOR],               "-",     1.00f,      0.01f,   	1,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "过负荷延时",      	&g_FixedValue1[OVERLOAD_TIME],                 "S",     999.99f,    0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
	
	{ &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "相间故障检测电流值",  &g_FixedValue1[OVERCURRENTFAULT_VALUE],        "A",     100,        0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "接地故障检测电流值",  &g_FixedValue1[OVERCURRENTFAULTI0_VALUE],      "A",     100,        0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },
    { &g_MenuUnable, 	   0,  LOGICAL_FUN,  "无流定值",        	&g_FixedValue1[CURRENT_VALUE],                 "A",     20,         0.00f,   	0.2f,       0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "有压定值",        	&g_FixedValue1[VOLTAGE_VALUE],                 "V",     300,        0.00f,   	176,        0,         3,         {"-",  "-"         },   " "   },	
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "失压定值",        	&g_FixedValue1[NONVOLTAGE_VALUE],              "V",     300,        0.00f,   	80,         0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "故障检测时限(Y时限)", &g_FixedValue1[FAULT_CHECK_Y_TIME],            "S",     999,        0.00f,   	5,          0,         3,         {"-",  "-"         }, "Y时限 "},
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "合到故障电流判据",    &g_FixedValue1[LOCKED_CLOSING_CUR_SWITCH],     "-",     1,          0,     		1,          2,         0,         {"退", "投"        },   " "   }, 
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "合闸成功闭锁失压分",  &g_FixedValue1[LOCKED_OPENING_SWITCH],         "-",     1,          0,     		1,          2,         0,         {"退", "投"        },   " "   },      
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "闭锁分闸复归时间",    &g_FixedValue1[SHORT_LOCKED_OPENING_TIME],     "S",     999,      	0.00f,   	10,         0,         3,         {"-",  "-"         }, "Z时限 "},
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "失电延时分闸投入",    &g_FixedValue1[LOSS_ELECTRICITY_SWITCH],       "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "失电分闸延时",    	&g_FixedValue1[LOSS_OPENING_TIME],             "S",     999,       	0.10f,   	3,          0,         3,         {"-",  "-"         },   " "   },		
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "电源侧得电合闸投入",  &g_FixedValue1[UAB_GET_VOLTAGE_CLOSSING_SWITCH], "-",   1,          0,     		0,          2,         0,         {"退", "投"        },   " "   }, 
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "负荷侧得电合闸投入",  &g_FixedValue1[UCB_GET_VOLTAGE_CLOSSING_SWITCH], "-",   1,          0,     		0,          2,         0,         {"退", "投"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "得电合闸延时(X时限)", &g_FixedValue1[GET_VOLTAGE_CLOSSING_X_TIME],   "S",     999,       	0.00f,   	7,          0,         3,         {"-",  "-"         }, "X时限 "},	
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "单侧失压合闸投入",    &g_FixedValue1[SINGLE_LOSS_VOLTAGE_SWITCH],    "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "失压合闸延时(XL时限)",&g_FixedValue1[SINGLE_LOSS_VOLTAGE_XLTIME],    "S",     999,       	0.00f,   	5,          0,         3,         {"-",  "-"         },"XL时限 "},	
    { &g_MenuUnable, 	   0,  LOGICAL_FUN,  "双侧有压投退",    	&g_FixedValue1[DOUBLE_VOLTAGE_SWITCH],         "-",     1,          0,     		1,          2,         0,         {"退", "投"        },   " "   },  
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "残压闭锁投入",    	&g_FixedValue1[REMAIN_VOLTAGE_SWITCH],         "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "残压定值",        	&g_FixedValue1[REMAIN_VOLTAGE_VALUE],          "V",     300,      	0.00f,   	30,         0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "残压判别时间",    	&g_FixedValue1[REMAIN_VOLTAGE_TIME],           "S",     999,       	0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },		
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "非遮断电流保护投入",  &g_FixedValue1[BREAKING_CURRENT_SWITCH],       "-",     1,          0,     		0,          2,         0,         {"退", "投",       },   " "   },	
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "非遮断电流值",      	&g_FixedValue1[BREAKING_CURRENT_VALUE],        "A",     100,       	0.00f,   	5,          0,         3,         {"-",  "-"         },   " "   },	
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "合于故障后加速投入",  &g_FixedValue1[IACCFI_SWITCH],                 "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "合于故障后加速定值",  &g_FixedValue1[IACCFI_CURRENT_VALUE],          "A",     99.99f,     0.00f,   	6,          0,         3,         {"-",  "-"         },   " "   }, 
	{ &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "涌流识别投入",    	&g_FixedValue1[INRUSH_SWITCH],                 "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "涌流识别定值",    	&g_FixedValue1[INRUSH_TIME],                   "S",     99.99f,     0.00f,   	5,          0,         3,         {"-",  "-"         },   " "   },	
	
    { &g_MenuUnable,       0,  IACC_PROTEC,  "零压加速投退",    	&g_FixedValue1[IACCU0_SWITCH],                 "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "   },    
    { &g_MenuUnable,       0,  IACC_PROTEC,  "零压加速定值",    	&g_FixedValue1[IACCU0_CURRENT_VALUE],          "v",     99.99f,     0.00f,   	6,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_MenuUnable,       0,  IACC_PROTEC,  "零压加速延时",    	&g_FixedValue1[IACCU0_TIME],                   "S",     99.99f,     0.00f,   	1,          0,         3,         {"-",  "-"         },   " "   },    		
   
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "检同期投退",      	&g_FixedValue1[CLOSING_LOOP_SWITCH],           "-",     1,          0,     		0,          2,         0,         {"退", "投",       },   " "    },	
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "同期有压定值",    	&g_FixedValue1[CLOSING_LOOP_VOLTAGE],          "V",     300,    	0.00f,   	176,        0,         3,         {"-",  "-"         },   " "   },	
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "同期无压定值",    	&g_FixedValue1[CLOSING_LOOP_NONVOLTAGE],       "V",     300,    	0.00f,   	80,         0,         3,         {"-",  "-"         },   " "   },
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "两侧压差",        	&g_FixedValue1[VOLTAGE_DIFFERENCE],            "V",     30.00f,     0.00f,   	5,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "相角差",          	&g_FixedValue1[PHASEANGLE_DIFFERENCE],         "°",    10.00f,     0.00f,   	5,          0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "频率差",          	&g_FixedValue1[FREQUENCY_DIFFERENCE],          "Hz",    99.99f,     0.00f,   	5,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "检同期时限",      	&g_FixedValue1[CLOSING_LOOP_DELAY],            "S",     999,     	0.00f,   	5,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  FAULT_SWITCH, "控制回路异常",    	&g_FixedValue1[CONTROL_LOOP_ABNOMAL_ENABLE],   "-",     1,          0,     		0,          2,         0,         {"退", "投",       },   " "    },		
	{ &g_MenuUnable,       0,  BATTERY_SET,  "电池活化投退",    	&g_FixedValue1[BATTERY_ACTIVE_SWITCH],         "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },	
    { &g_MenuUnable,       0,  BATTERY_SET,  "电池活化周期",    	&g_FixedValue1[BATTERY_ACTIVE_CYCLE],          "天",    99999.99f,  0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  BATTERY_SET,  "活化持续时间",    	&g_FixedValue1[BATTERY_ACTIVE_TIME],           "h",     23,         0,     		0,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  BATTERY_SET,  "活化故障电压",    	&g_FixedValue1[BATTERY_ACTIVE_FAULT_VOLTAGE],  "V",     999.99f,    0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuUnable,       0,  BATTERY_SET,  "活化故障延时",    	&g_FixedValue1[BATTERY_ACTIVE_FAULT_TIME],     "S",     999.99f,    0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  AUTO_RESET,   "自动复归投退",    	&g_FixedValue1[AUTOMATIC_RESET_SWITCH],        "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },	
    { &g_MenuUnable,       0,  AUTO_RESET,   "自动复归延时",    	&g_FixedValue1[AUTOMATIC_RESET_TIME],          "S",     99999.99f,  0.00f,   	99,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "越限告警投退",    	&g_FixedValue1[OVERLIMIT_SWITCH],              "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "越限上限系数",    	&g_FixedValue1[UPLIMIT_FACTOR],                "-",     1.00f,      0.00f,   	0.8f,       0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "越限下限系数",    	&g_FixedValue1[DOWNLIMIT_FACTOR],              "-",     2.00f,      1.00f,   	1.2,        0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "越限延时",        	&g_FixedValue1[OVERLIMIT_TIME],                "S",     999.99f,    0,     		3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "Uab越限投退",     	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_Uab],    "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "Ubc越限投退",     	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_Ubc],    "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "Uac越限投退",     	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_Uca],    "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "UAB越限投退",     	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_UAB],    "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "UBC越限投退",     	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_UBC],    "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "电压上限定值",    	&g_FixedValue1[UPLIMIT_VOLTAGE_U],             "V",     999.99f,    0.00f,   	110,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "电压下限定值",    	&g_FixedValue1[DOWNLIMIT_VOLTAGE_U],           "V",     999.99f,    0.00f,   	20,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "Ia越限投退",      	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_IA],     "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "Ib越限投退",      	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_IB],     "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "Ic越限投退",      	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_IC],     "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "电流上限定值",    	&g_FixedValue1[UPLIMIT_CURRENT_I],             "A",     999.99f,    0.00f,   	6,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "电流下限定值",    	&g_FixedValue1[DOWNLIMIT_CURRENT_I],           "A",     999.99f,    0.00f,   	0.5,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "U0越限投退",      	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_U0],     "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "U0上限定值",      	&g_FixedValue1[UPLIMIT_VOLTAGE_U0],            "V",     999.99f,    0.00f,   	100,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "U0下限定值",      	&g_FixedValue1[DOWNLIMIT_VOLTAGE_U0],          "V",     999.99f,    0.00f,   	20,         0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "I0越限投退",      	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_I0],     "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "I0上限定值",      	&g_FixedValue1[UPLIMIT_VOLTAGE_I0],            "A",     999.99f,    0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "I0下限定值",      	&g_FixedValue1[DOWNLIMIT_VOLTAGE_I0],          "A",     999.99f,    0.00f,   	0.2f,       0,         3,         {"-",  "-"         },   " "    },		
    { &g_MenuEnable,       1,  LIMITATION,   "U_DC越限投退",    	&g_FixedValue1[OVERLIMIT_ALARMSWITCH_DC_U],    "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "U_DC上限定值",    	&g_FixedValue1[UPLIMIT_DC_VOLTAGE_U],          "V",     999.99f,    0.00f,  	30,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "U_DC下限定值",    	&g_FixedValue1[DOWNLIMIT_DC_VOLTAGE_U],        "V",     999.99f,    0.00f,   	18,         0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "I_DC越限投退",    	&g_FixedValue1[OVERLIMIT_ALARMSWITCH_DC_I],    "-",     1,          0,     		0,          2,         0,         {"退", "投"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "I_DC上限定值",    	&g_FixedValue1[UPLIMIT_DC_CURRENT_I],          "A",     999.99f,    0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "I_DC下限定值",    	&g_FixedValue1[DOWNLIMIT_DC_CURRENT_I],        "A",     999.99f,    0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },		
}; 

/* 定值二区 */
struct tagValueParaCfg FixedValueCfg2[sizeof(FixedValueCfg1) / sizeof(FixedValueCfg1[0])];

/* 扩展遥测 */
struct tagTelemetryCfg TelemetryCfg[sizeof(TelemetryCfgBase) / sizeof(TelemetryCfgBase[0]) + sizeof(FixedValueCfg1) / sizeof(FixedValueCfg1[0])];

unsigned int g_FixedValueCfg1_Len = sizeof(FixedValueCfg1) / sizeof(FixedValueCfg1[0]);
unsigned int g_FixedValueCfg2_Len = sizeof(FixedValueCfg2) / sizeof(FixedValueCfg2[0]);
unsigned int g_ParameterCfg_Len = sizeof(ParameterCfg) / sizeof(ParameterCfg[0]);
unsigned int g_CalibrateFactorCfg_Len = sizeof(CalibrateFactorCfg) / sizeof(CalibrateFactorCfg[0]);
unsigned int g_TelemetryCfgBase_Len = sizeof(TelemetryCfgBase) / sizeof(TelemetryCfgBase[0]);
unsigned int g_taghmiDigitalInputCfg_Len = sizeof(hmiDigitalInputCfg) / sizeof(hmiDigitalInputCfg[0]);
unsigned int g_tagzkDigitalInputCfg_Len = sizeof(zkDigitalInputCfg) / sizeof(zkDigitalInputCfg[0]);
unsigned int g_TelesignalCfg_Len = sizeof(TelesignalCfg) / sizeof(TelesignalCfg[0]);
unsigned int g_TelecontrolCfg_Len = sizeof(TelecontrolCfg) / sizeof(TelecontrolCfg[0]);
unsigned int g_UlogCfg_Len = sizeof(UlogCfg) / sizeof(UlogCfg[0]);
unsigned int g_InherentParaCfg_Len = sizeof(InherentParaCfg) / sizeof(InherentParaCfg[0]);
unsigned int g_TelemetryCfg_Len = sizeof(TelemetryCfg) / sizeof(TelemetryCfg[0]);

/* END OF FILE ---------------------------------------------------------------*/


