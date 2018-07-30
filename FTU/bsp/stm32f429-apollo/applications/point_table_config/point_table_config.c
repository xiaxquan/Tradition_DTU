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

/* ���Ӳң�� */
struct taghmiDigitalInputCfg hmiDigitalInputCfg[] = 
{
//   ʹ��   ��忪���	        ���                                  ȡ�� 
    //{ 1,       15,       &g_TelesignalAddr.recloseHardStrap,           0},
    //{ 1,       16,       &g_TelesignalAddr.recloseHardStrap,         0},//ͬ��ѹ��
    //{ 1,       17,       &g_TelesignalAddr.recloseHardStrap,         0},//����FA
    { 1,       18,       &g_TelesignalAddr.earth,                      0},
    { 1,       19,       &g_TelesignalAddr.remote,                     0},
    //{ 1,       20,       &g_TelesignalAddr.modContactHardStrap,        0},
    //{ 1,       21,       &g_TelesignalAddr.modBreakHardStrap,          0},
    { 1,       22,       &g_TelesignalAddr.functionHardStrap,          1},
    { 1,       23,       &g_TelesignalAddr.workmodeCommon,             0},
    { 1,       24,       &g_TelesignalAddr.workmodeVolcur,             0},
};

/* ����Ӳң�� */
struct tagzkDigitalInputCfg zkDigitalInputCfg[] = 
{
//   ʹ��          ��忪���	                ���                                      �ϴ�PIN   ȡ��   ��ʱ��
    { 1,       INDEX_DI1,          &g_TelesignalAddr.switchOpen,                   0,      0,      0},
    { 1,       INDEX_DI2,          &g_TelesignalAddr.switchClose,                  0,      0,      0},
    { 1,       INDEX_DI3,          &g_TelesignalAddr.operatingMechanism,           0,      1,      0},
    { 1,       INDEX_DI4,          &g_TelesignalAddr.lowPressure,                  0,      0,      0},
//    { 1,       INDEX_DI5,          &g_TelesignalAddr.powerFaultAlarm,              0,      0,      0},
//    { 1,       INDEX_DI6,          &g_TelesignalAddr.batteryUnderVoltageAlarm,     0,      0,      0},
//    { 1,       INDEX_DI7,          &g_TelesignalAddr.batteryActivationStatus,      0,      0,      0},
//    { 1,       INDEX_DI8,          &g_TelesignalAddr.batteryLossAlarm,             0,      1,      0},
};

/* ң�� */
struct tagTelesignalCfg TelesignalCfg[] = 
{
//   ʹ��    ����	        ���                                         ң��      ����  ���ϼ�¼ʹ��    ����
    { 1,  "���ط�λ",     &g_TelesignalAddr.switchOpen,                  NULL,     0xff,    	1,		{"��", "��"},      		{"��->��","��->��"}},
    { 1,  "���غ�λ",     &g_TelesignalAddr.switchClose,                 NULL,     0xff,    	1,		{"��", "��"},      		{"��->��","��->��"}},
    { 1,  "����",         &g_TelesignalAddr.operatingMechanism,          NULL,     0xff,    	1,		{"��", "��"},      		{"��->��","��->��"}},
    { 1,  "����ѹ",       &g_TelesignalAddr.lowPressure,                 NULL,     0xff,    	1,		{"��", "�澯"},    		{"�澯->��λ","��->�澯"}},
    { 0,  "����1",        &g_TelesignalAddr.spareDi1,                    NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},
    { 0,  "����2",        &g_TelesignalAddr.spareDi2,                    NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},
    { 0,  "����3",        &g_TelesignalAddr.spareDi3,                    NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},
    { 0,  "����4",        &g_TelesignalAddr.spareDi4,                    NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},
    { 0,  "����5",        &g_TelesignalAddr.spareDi5,                    NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},
    { 0,  "����6",        &g_TelesignalAddr.spareDi6,                    NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},
    { 0,  "����7",        &g_TelesignalAddr.spareDi7,                    NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},
    { 0,  "����8",        &g_TelesignalAddr.spareDi8,                    NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},
    { 0,  "����9",        &g_TelesignalAddr.spareDi9,                    NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},
    { 0,  "����10",       &g_TelesignalAddr.spareDi10,                   NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},
    { 0,  "����11",       &g_TelesignalAddr.spareDi11,                   NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},
    { 0,  "����12",       &g_TelesignalAddr.spareDi12,                   NULL,     0xff,        1,      {"��", "��"},           {"��->��","��->��"}},	
    { 1,  "��������",     &g_TelesignalAddr.swtichClass,                 NULL,     0xff,    	1,		{"��·��","���ɿ���"},	{"��·��","���ɿ���"}},
    { 1,  "���汣����",   &g_TelesignalAddr.workmodeCommon,              NULL,     0xff,    	1,		{"�˳�", "Ͷ��"},  		{"Ͷ->��","��->Ͷ"}},
    { 1,  "��ѹ������",   &g_TelesignalAddr.workmodeVolcur,              NULL,     0xff,    	1,		{"�˳�", "Ͷ��"},  		{"Ͷ->��","��->Ͷ"}},
    { 1,  "�ֶ�Ӳѹ��",   &g_TelesignalAddr.modBreakHardStrap,           NULL,     ON,      	1,		{"�˳�", "Ͷ��"},  		{"Ͷ->��","��->Ͷ"}},
    { 1,  "����Ӳѹ��",   &g_TelesignalAddr.modContactHardStrap,         NULL,     ON,      	1,		{"�˳�", "Ͷ��"},  		{"Ͷ->��","��->Ͷ"}},
    { 1,  "�غ�բӲѹ��", &g_TelesignalAddr.recloseHardStrap,            NULL,     ON,      	1,		{"�˳�", "Ͷ��"},  		{"Ͷ->��","��->Ͷ"}},
    { 1,  "����Ӳѹ��",   &g_TelesignalAddr.functionHardStrap,           NULL,     0xff,    	1,		{"�˳�", "Ͷ��"},  		{"Ͷ->��","��->Ͷ"}},
    { 1,  "�ֶ���ѹ��",   &g_TelesignalAddr.modBreakSoftStrap,           NULL,     0xff,    	1,		{"�˳�", "Ͷ��"},  		{"Ͷ->��","��->Ͷ"}},
    { 1,  "������ѹ��",   &g_TelesignalAddr.modContactSoftStrap,         NULL,     0xff,    	1,		{"�˳�", "Ͷ��"},  		{"Ͷ->��","��->Ͷ"}},
    { 1,  "�غ�բ��ѹ��", &g_TelesignalAddr.recloseSoftStrap,            NULL,     0xff,    	1,		{"�˳�", "Ͷ��"},  		{"Ͷ->��","��->Ͷ"}},
    { 1,  "������ѹ��",   &g_TelesignalAddr.functionSoftStrap,           NULL,     0xff,    	1,		{"�˳�", "Ͷ��"},  		{"Ͷ->��","��->Ͷ"}},
    { 1,  "Զ��",         &g_TelesignalAddr.remote,                      NULL,     0xff,    	1,		{"��", "Զ��"},  		{"Զ��->��","��->Զ��"}},
    { 1,  "�͵�",         &g_TelesignalAddr.earth,                       NULL,     0xff,    	1,		{"��", "�͵�"},  		{"�͵�->��","��->�͵�"}},
    { 0,  "��·����",     &g_TelesignalAddr.shortCircuitFault,           NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "�������",     &g_TelesignalAddr.earthingFault,               NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "��������",     &g_TelesignalAddr.protectionAct,               NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "��բ����",     &g_TelesignalAddr.closingLocked,               NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},	
    { 0,  "��բ����",     &g_TelesignalAddr.openingLocked,               NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "�غ϶���",     &g_TelesignalAddr.recloseAct,                  NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "�غϱ���",     &g_TelesignalAddr.recloseLock,                 NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "װ�ù���",     &g_TelesignalAddr.deviceFault,                 NULL,     0xff,    	0,		{"��","����"},     		{"��","����"}},
    { 0,  "�Լ��쳣",     &g_TelesignalAddr.selfCheckAbnomal,            NULL,     0xff,    	0,		{"��","�쳣"},     		{"��","�쳣"}},
    { 0,  "ͨѶ",         &g_TelesignalAddr.communication,               NULL,     0xff,    	0,		{"��","ͨ"},       		{"��","ͨ"}},	
    { 0,  "��ع��ϸ澯", &g_TelesignalAddr.batteryFaultAlarm,           NULL,     0xff,    	0,		{"��","�澯"},     		{"��","�澯"}},
    { 0,  "˫�㿪�طֺ�", &g_TelesignalAddr.doubleSwitch,                NULL,     0xff,    	0,		{"��->��","��->��"}, 	{"��->��","��->��"}}, 
    { 0,  "���ƻ�·����", &g_TelesignalAddr.controlLoopDisconnection,    NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "A�����һ��",  &g_TelesignalAddr.overcurrentIa1,              NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "B�����һ��",  &g_TelesignalAddr.overcurrentIb1,              NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},	
    { 0,  "C�����һ��",  &g_TelesignalAddr.overcurrentIc1,              NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "A���������",  &g_TelesignalAddr.overcurrentIa2,              NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "B���������",  &g_TelesignalAddr.overcurrentIb2,              NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},	
    { 0,  "C���������",  &g_TelesignalAddr.overcurrentIc2,              NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "A���������",  &g_TelesignalAddr.overcurrentIa3,              NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "B���������",  &g_TelesignalAddr.overcurrentIb3,              NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},	
    { 0,  "C���������",  &g_TelesignalAddr.overcurrentIc3,              NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "�������һ��", &g_TelesignalAddr.overcurrentI01,              NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "�����������", &g_TelesignalAddr.overcurrentI02,              NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},		
    { 0,  "A����ٹ���",  &g_TelesignalAddr.overcurrentAccIa,            NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "B����ٹ���",  &g_TelesignalAddr.overcurrentAccIb,            NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "C����ٹ���",  &g_TelesignalAddr.overcurrentAccIc,            NULL,     0xff,   		1,		{"��λ","����"},   		{"��λ","����"}},	
    { 0,  "������ٹ���", &g_TelesignalAddr.overcurrentAccI0,            NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "������ٹ�ѹ", &g_TelesignalAddr.overcurrentAccU0,            NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},    
    { 0,  "һ���غ�բ",   &g_TelesignalAddr.recloseProtection1,          NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "�����غ�բ",   &g_TelesignalAddr.recloseProtection2,          NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "�����غ�բ",   &g_TelesignalAddr.recloseProtection3,          NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "�Ĵ��غ�բ",   &g_TelesignalAddr.recloseProtection4,          NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},	
    { 0,  "����һ���غ�", &g_TelesignalAddr.zeroRecloseProtection1,      NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "��������غ�", &g_TelesignalAddr.zeroRecloseProtection2,      NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},			
    { 0,  "�����غϱ���", &g_TelesignalAddr.clockSecondaryClose,         NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "A���������",  &g_TelesignalAddr.overcurrentFaultIa,          NULL,     0xff,    	1,		{"��λ","�澯"},   		{"��λ","�澯"}},  
    { 0,  "B���������",  &g_TelesignalAddr.overcurrentFaultIb,          NULL,     0xff,    	1,		{"��λ","�澯"},   		{"��λ","�澯"}},
    { 0,  "C���������",  &g_TelesignalAddr.overcurrentFaultIc,          NULL,     0xff,    	1,		{"��λ","�澯"},   		{"��λ","�澯"}},
    { 0,  "�����������", &g_TelesignalAddr.overcurrentFaultI0,          NULL,     0xff,    	1,		{"��λ","�澯"},   		{"��λ","�澯"}},
    { 0,  "ʧ���բ",     &g_TelesignalAddr.lossTrip,                    NULL,     0xff,    	1,		{"��λ","��բ"},   		{"��λ","��բ"}},
    { 0,  "�õ��բ",     &g_TelesignalAddr.getClossing,                 NULL,     0xff,    	1,		{"��λ","��բ"},   		{"��λ","��բ"}},
    { 0,  "��բ�ڹ���",   &g_TelesignalAddr.closingFaultEvent,           NULL,     0xff,    	1,		{"��","����"},     		{"��","����"}},	
    { 0,  "˫ѹ��ֹ��բ", &g_TelesignalAddr.doubleProhibitClossing,      NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "����ʧѹ��բ", &g_TelesignalAddr.singleLossClosingEvent,      NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "��Դ���ѹ������բ", &g_TelesignalAddr.remainClockClossingUab, NULL,    0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},	
	{ 0,  "���ɲ��ѹ������բ", &g_TelesignalAddr.remainClockClossingUcb, NULL,    0xff,   		1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "С�źŽӵ�",   &g_TelesignalAddr.smallSignalEarthingTrip,     NULL,     0xff,    	1,		{"��λ","��բ"},   		{"��λ","��բ"}},
    { 0,  "�ڶϵ���",     &g_TelesignalAddr.breakingCurrent,             NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},	
    { 0,  "������",       &g_TelesignalAddr.overloadEvent,               NULL,     0xff,    	1,		{"��λ","�澯"},   		{"��λ","�澯"}},
    { 0,  "��ѹ����", 	  &g_TelesignalAddr.overVoltageProtection,       NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},		
    { 0,  "��Ƶ����", 	  &g_TelesignalAddr.overFrequencyProtection,     NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "��ѹ����", 	  &g_TelesignalAddr.downVoltageProtection,       NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "��Ƶ����",     &g_TelesignalAddr.downFrequencyProtection,     NULL,     0xff,    	1,		{"��λ","����"},   		{"��λ","����"}},
    { 0,  "UabԽ����",    &g_TelesignalAddr.overLimitUabUp,              NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},
    { 0,  "UabԽ����",    &g_TelesignalAddr.overLimitUabDown,            NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "UbcԽ����",    &g_TelesignalAddr.overLimitUbcUp,              NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},
    { 0,  "UbcԽ����",    &g_TelesignalAddr.overLimitUbcDown,            NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},
    { 0,  "UcaԽ����",    &g_TelesignalAddr.overLimitUcaUp,              NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "UcaԽ����",    &g_TelesignalAddr.overLimitUcaDown,            NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "UABԽ����",    &g_TelesignalAddr.overLimitUABUp,              NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},
    { 0,  "UABԽ����",    &g_TelesignalAddr.overLimitUABDown,            NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "UBCԽ����",    &g_TelesignalAddr.overLimitUBCUp,              NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},
    { 0,  "UBCԽ����",    &g_TelesignalAddr.overLimitUBCDown,            NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},		
    { 0,  "U0Խ����",     &g_TelesignalAddr.overLimitU0Up,               NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "U0Խ����",     &g_TelesignalAddr.overLimitU0Down,             NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "IaԽ����",     &g_TelesignalAddr.overLimitIaUp,               NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},
    { 0,  "IaԽ����",     &g_TelesignalAddr.overLimitIaDown,             NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "IbԽ����",     &g_TelesignalAddr.overLimitIbUp,               NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},
    { 0,  "IbԽ����",     &g_TelesignalAddr.overLimitIbDown,             NULL,     0xff,   		1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "IcԽ����",     &g_TelesignalAddr.overLimitIcUp,               NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "IcԽ����",     &g_TelesignalAddr.overLimitIcDown,             NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "I0Խ����",     &g_TelesignalAddr.overLimitI0Up,               NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "I0Խ����",     &g_TelesignalAddr.overLimitI0Down,             NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},		
    { 0,  "DC_UԽ����",   &g_TelesignalAddr.overLimitDC_U_Up,            NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "DC_UԽ����",   &g_TelesignalAddr.overLimitDC_U_Down,          NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "DC_IԽ����",   &g_TelesignalAddr.overLimitDC_I_Up,            NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},	
    { 0,  "DC_IԽ����",   &g_TelesignalAddr.overLimitDC_I_Down,          NULL,     0xff,    	1,		{"��λ","Խ����"}, 		{"��λ","Խ����"}},		
    { 0,  "װ�õ���",     &g_TelesignalAddr.devicePowerDown,             NULL,     0xff,    	0,		{"��λ","����"},   		{"��λ","����"}},
};
	
/* ����ң�� */
struct tagTelemetryCfg TelemetryCfgBase[] = 
{
//   ʹ�� �˵����   ����           ��λ������          ���                                	ֵ         ��λ      �ֵ             	��Ư                             ����	
    { 1,  SECONDRY,  "f",           "Ƶ��(f)",          &g_TelemetryBaseAddr.F,                 NULL,      "Hz",    50.00f,      		&g_Parameter[ZERODRIFT_F],      &g_Parameter[DEADZONE_F]},
    { 1,  SECONDRY,  "Ia",          "�����(Ia)",       &g_TelemetryBaseAddr.Ia,                NULL,      "A",     5.00f,      		&g_Parameter[ZERODRIFT_Ia],     &g_Parameter[DEADZONE_Ia]},
    { 1,  SECONDRY,  "Ib",          "�����(Ib)",       &g_TelemetryBaseAddr.Ib,                NULL,      "A",     5.00f,      		&g_Parameter[ZERODRIFT_Ib],     &g_Parameter[DEADZONE_Ib]},
    { 1,  SECONDRY,  "Ic",          "�����(Ic)",       &g_TelemetryBaseAddr.Ic,                NULL,      "A",     5.00f,      		&g_Parameter[ZERODRIFT_Ic],     &g_Parameter[DEADZONE_Ic]},	
    { 1,  SECONDRY,  "I0",          "�����(I0)",       &g_TelemetryBaseAddr.I0,                NULL,      "A",    	1.00f,     			&g_Parameter[ZERODRIFT_I0],     &g_Parameter[DEADZONE_I0]},
    { 1,  SECONDRY,  "Uab",         "�ߵ�ѹ(Uab)",      &g_TelemetryBaseAddr.Uab,               NULL,      "V",     220.00f,      		&g_Parameter[ZERODRIFT_Uab],    &g_Parameter[DEADZONE_Uab]},
    { 1,  SECONDRY,  "Ubc",         "�ߵ�ѹ(Ubc)",      &g_TelemetryBaseAddr.Ucb,               NULL,      "V",     220.00f,      		&g_Parameter[ZERODRIFT_Ucb],    &g_Parameter[DEADZONE_Ucb]},
    { 1,  SECONDRY,  "Uac",         "�ߵ�ѹ(Uac)",      &g_TelemetryBaseAddr.Uac,               NULL,      "V",     220.00f,      		&g_Parameter[ZERODRIFT_Uac],    &g_Parameter[DEADZONE_Uac]},
    { 1,  SECONDRY,  "U0",          "��ѹ(U0)",         &g_TelemetryBaseAddr.U0,                NULL,      "V",    	100.00f,     		&g_Parameter[ZERODRIFT_U0],     &g_Parameter[DEADZONE_U0]},
    { 1,  SECONDRY,  "UAB",         "�ߵ�ѹ(UAB)",      &g_TelemetryBaseAddr.UAB,               NULL,      "V",     220.00f,      		&g_Parameter[ZERODRIFT_UAB],    &g_Parameter[DEADZONE_UAB]},
    { 1,  SECONDRY,  "UBC",         "�ߵ�ѹ(UBC)",      &g_TelemetryBaseAddr.UCB,               NULL,      "V",     220.00f,      		&g_Parameter[ZERODRIFT_UCB],    &g_Parameter[DEADZONE_UCB]},
    { 1,  SECONDRY,  "P",           "�й�����(P)",      &g_TelemetryBaseAddr.P,                 NULL,      "kW",    1.905f,      		&g_Parameter[ZERODRIFT_P],      &g_Parameter[DEADZONE_P]},
    { 1,  SECONDRY,  "Q",           "�޹�����(Q)",      &g_TelemetryBaseAddr.Q,                 NULL,      "kVar",  1.905f,      		&g_Parameter[ZERODRIFT_Q],      &g_Parameter[DEADZONE_Q]},
    { 1,  SECONDRY,  "S",           "���ڹ���(S)",      &g_TelemetryBaseAddr.S,                 NULL,      "kVA",   1.905f,      		&g_Parameter[ZERODRIFT_S],      &g_Parameter[DEADZONE_S]},
    { 1,  SECONDRY,  "pf",          "��������(pf)",     &g_TelemetryBaseAddr.PF,                NULL,      "-",    	1.00f,     			&g_Parameter[ZERODRIFT_PF],     &g_Parameter[DEADZONE_PF]},
    { 1,  SECONDRY,  "DC1",         "ֱ��(DC1)",        &g_TelemetryBaseAddr.DC1,               NULL,      "V",     24.00f,      		&g_Parameter[ZERODRIFT_DC1],    &g_Parameter[DEADZONE_DC1]},
    { 0,  SECONDRY,  "DC2",         "ֱ��(DC2)",        &g_TelemetryBaseAddr.DC2,               NULL,      "V",     24.00f,      		&g_Parameter[ZERODRIFT_DC2],    &g_Parameter[DEADZONE_DC2]},
    { 1,  SECONDRY,  "T",           "�¶�(T)",          &g_TelemetryBaseAddr.T,                 NULL,      "��C",   40.00f,      		&g_Parameter[ZERODRIFT_T],      &g_Parameter[DEADZONE_T]},

	{ 1,  SECONDRY,  "��Uab_Ia",    "��Uab_Ia",         &g_TelemetryBaseAddr.alphy_Uab_Ia,      NULL,      "��",    360.00f,      		NULL,                            NULL},
    { 1,  SECONDRY,  "��Ucb_Ic",    "��Ucb_Ic",         &g_TelemetryBaseAddr.alphy_Ucb_Ic,      NULL,      "��",    360.00f,      		NULL,                            NULL},	
    { 1,  SECONDRY,  "��U0_I0",     "��U0_I0",          &g_TelemetryBaseAddr.alphy_U0_I0,       NULL,      "��",    360.00f,      		NULL,                            NULL},
    { 1,  SECONDRY,  "��Ux_Ux",     "��Ux_Ux",          &g_TelemetryBaseAddr.alphy_Ux_Ux,       NULL,      "��",    360.00f,      		NULL,                            NULL},	
    { 1,  SECONDRY,  "f2",          "Ƶ��(f2)",         &g_TelemetryBaseAddr.F2,                NULL,      "Hz",    50.00f,      		&g_Parameter[ZERODRIFT_F],      &g_Parameter[DEADZONE_F]},
    { 1,  SECONDRY,  "F/S",         "����(F/S)",        &g_TelemetryBaseAddr.F_S,               NULL,      "H/s",   50.00f,      		NULL,                            NULL},	
	
    { 1,  ONCE,      "Ia",          "һ��Ia",           &g_TelemetryBaseAddr.IaOnce,            NULL,      "A",     600.0f,      		NULL,                           &g_Parameter[DEADZONE_Ia]},
    { 1,  ONCE,      "Ib",          "һ��Ib",           &g_TelemetryBaseAddr.IbOnce,            NULL,      "A",     600.0f,      		NULL,                           &g_Parameter[DEADZONE_Ib]},
    { 1,  ONCE,      "Ic",          "һ��Ic",           &g_TelemetryBaseAddr.IcOnce,            NULL,      "A",     600.0f,      		NULL,                           &g_Parameter[DEADZONE_Ic]},
    { 1,  ONCE,      "I0",          "һ��I0",           &g_TelemetryBaseAddr.I0Once,            NULL,      "A",     20.0f,      		NULL,                           &g_Parameter[DEADZONE_I0]},
    { 1,  ONCE,      "Uab",         "һ��Uab",          &g_TelemetryBaseAddr.UabOnce,           NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_Uab]},
    { 1,  ONCE,      "Ubc",         "һ��Ubc",          &g_TelemetryBaseAddr.UbcOnce,           NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_Ucb]},
    { 1,  ONCE,      "Uca",         "һ��Uca",          &g_TelemetryBaseAddr.UcaOnce,           NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_Uac]},
    { 1,  ONCE,      "U0",          "һ��U0",           &g_TelemetryBaseAddr.U0Once,            NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_U0]},
    { 1,  ONCE,      "UAB",         "һ��UAB",          &g_TelemetryBaseAddr.UABOnce,           NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_UAB]},
    { 1,  ONCE,      "UBC",         "һ��UBC",          &g_TelemetryBaseAddr.UBCOnce,           NULL,      "kV",    4.54545f,      		NULL,                           &g_Parameter[DEADZONE_UCB]},
    { 1,  ONCE,      "P",           "һ��P",            &g_TelemetryBaseAddr.POnce,             NULL,      "kW",    8181.81f,      		NULL,                           &g_Parameter[DEADZONE_P]},
    { 1,  ONCE,      "Q",           "һ��Q",            &g_TelemetryBaseAddr.QOnce,             NULL,      "kVar",  8181.81,      		NULL,                           &g_Parameter[DEADZONE_Q]},
    { 1,  ONCE,      "S",           "һ��S",            &g_TelemetryBaseAddr.SOnce,             NULL,      "kVA",   8181.81,      		NULL,                           &g_Parameter[DEADZONE_S]},		

    { 1,  HARMONIC,  "����Uab",     "����г��Uab",      &g_TelemetryBaseAddr.thirdharmonicUab,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Uab],    &g_Parameter[DEADZONE_Uab]},	
    { 1,  HARMONIC,  "����Ubc",     "����г��Ubc",      &g_TelemetryBaseAddr.thirdharmonicUbc,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Ucb],    &g_Parameter[DEADZONE_Ucb]},
    { 1,  HARMONIC,  "����Uca",     "����г��Uca",      &g_TelemetryBaseAddr.thirdharmonicUca,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Uac],    &g_Parameter[DEADZONE_Uac]},
    { 1,  HARMONIC,  "����U0",      "����г��U0",       &g_TelemetryBaseAddr.thirdharmonicU0,   NULL,      "V",    	100.0f,     	&g_Parameter[ZERODRIFT_U0],     &g_Parameter[DEADZONE_U0]},
    { 1,  HARMONIC,  "����Ia",      "����г��Ia",       &g_TelemetryBaseAddr.thirdharmonicIa,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ia],     &g_Parameter[DEADZONE_Ia]},
    { 1,  HARMONIC,  "����Ib",      "����г��Ib",       &g_TelemetryBaseAddr.thirdharmonicIb,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ib],     &g_Parameter[DEADZONE_Ib]},
    { 1,  HARMONIC,  "����Ic",      "����г��Ic",       &g_TelemetryBaseAddr.thirdharmonicIc,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ic],     &g_Parameter[DEADZONE_Ic]},
    { 1,  HARMONIC,  "����I0",      "����г��I0",       &g_TelemetryBaseAddr.thirdharmonicI0,   NULL,      "A",    	100.0f,     	&g_Parameter[ZERODRIFT_I0],     &g_Parameter[DEADZONE_I0]},	
    { 1,  HARMONIC,  "���Uab",     "���г��Uab",      &g_TelemetryBaseAddr.fifthharmonicUab,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Uab],    &g_Parameter[DEADZONE_Uab]},	
    { 1,  HARMONIC,  "���Ubc",     "���г��Ubc",      &g_TelemetryBaseAddr.fifthharmonicUbc,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Ucb],    &g_Parameter[DEADZONE_Ucb]},
    { 1,  HARMONIC,  "���Uca",     "���г��Uca",      &g_TelemetryBaseAddr.fifthharmonicUca,  NULL,      "V",     220.0f,     	&g_Parameter[ZERODRIFT_Uac],    &g_Parameter[DEADZONE_Uac]},
    { 1,  HARMONIC,  "���U0",      "���г��U0",       &g_TelemetryBaseAddr.fifthharmonicU0,   NULL,      "V",    	100.0f,     	&g_Parameter[ZERODRIFT_U0],     &g_Parameter[DEADZONE_U0]},
    { 1,  HARMONIC,  "���Ia",      "���г��Ia",       &g_TelemetryBaseAddr.fifthharmonicIa,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ia],     &g_Parameter[DEADZONE_Ia]},
    { 1,  HARMONIC,  "���Ib",      "���г��Ib",       &g_TelemetryBaseAddr.fifthharmonicIb,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ib],     &g_Parameter[DEADZONE_Ib]},
    { 1,  HARMONIC,  "���Ic",      "���г��Ic",       &g_TelemetryBaseAddr.fifthharmonicIc,   NULL,      "A",     5.00f,     		&g_Parameter[ZERODRIFT_Ic],     &g_Parameter[DEADZONE_Ic]},
    { 1,  HARMONIC,  "���I0",      "���г��I0",       &g_TelemetryBaseAddr.fifthharmonicI0,   NULL,      "A",    	1.00f,     		&g_Parameter[ZERODRIFT_I0],     &g_Parameter[DEADZONE_I0]},		

    { 0,  0xff,      "����汾",    "����汾",         &g_TelemetryBaseAddr.softwareVersion,   NULL,      "--",    	0,     NULL,     NULL},	
    { 0,  0xff,      "����ģʽ",    "����ģʽ",         &g_TelemetryBaseAddr.workMode,          NULL,      "--",    	0,     NULL,     NULL},	
};	

/* ң�� */
struct tagTelecontrolCfg TelecontrolCfg[] =
{
    {1, "Զ��ң��",    13,  {"��բѡ��ʧ��", "��բѡ��ɹ�", "��բִ��ʧ��", "��բִ�гɹ�", "��բѡ��ʧ��", "��բѡ��ɹ�", "��բִ��ʧ��", "��բִ�гɹ�", "ң��ȡ��","�ϻ�ѡ��ʧ��", "�ϻ�ѡ��ɹ�", "�ϻ�ִ��ʧ��", "�ϻ�ִ�гɹ�"}},
    {1, "Զ���",     2,  {"ʧ��", "�ɹ�"}},	
	{1, "Զ����λ",     2,  {"ʧ��", "�ɹ�"}},
    {1, "Զ�������¼", 2,  {"ʧ��", "�ɹ�"}},
    {1, "Զ���ֶ�ģʽ",13,  {"�˳�ѡ��ʧ��", "�˳�ѡ��ɹ�", "�˳�ִ��ʧ��", "�˳�ִ�гɹ�", "Ͷ��ѡ��ʧ��", "Ͷ��ѡ��ɹ�", "Ͷ��ִ��ʧ��", "Ͷ��ִ�гɹ�", "ң��ȡ��","", "", "", ""}},
    {1, "Զ������ģʽ",13,  {"�˳�ѡ��ʧ��", "�˳�ѡ��ɹ�", "�˳�ִ��ʧ��", "�˳�ִ�гɹ�", "Ͷ��ѡ��ʧ��", "Ͷ��ѡ��ɹ�", "Ͷ��ִ��ʧ��", "Ͷ��ִ�гɹ�", "ң��ȡ��","", "", "", ""}},
    {1, "Զ���غ�բ",  13,  {"�˳�ѡ��ʧ��", "�˳�ѡ��ɹ�", "�˳�ִ��ʧ��", "�˳�ִ�гɹ�", "Ͷ��ѡ��ʧ��", "Ͷ��ѡ��ɹ�", "Ͷ��ִ��ʧ��", "Ͷ��ִ�гɹ�", "ң��ȡ��","", "", "", ""}},
    {1, "Զ������",    13,  {"�˳�ѡ��ʧ��", "�˳�ѡ��ɹ�", "�˳�ִ��ʧ��", "�˳�ִ�гɹ�", "Ͷ��ѡ��ʧ��", "Ͷ��ѡ��ɹ�", "Ͷ��ִ��ʧ��", "Ͷ��ִ�гɹ�", "ң��ȡ��","", "", "", ""}},	
    {0, "���ز���",    13,  {"��բѡ��ʧ��", "��բѡ��ɹ�", "��բִ��ʧ��", "��բִ�гɹ�", "��բѡ��ʧ��", "��բѡ��ɹ�", "��բִ��ʧ��", "��բִ�гɹ�", "ң��ȡ��","�ϻ�ѡ��ʧ��", "�ϻ�ѡ��ɹ�", "�ϻ�ִ��ʧ��", "�ϻ�ִ�гɹ�"}},
	{0, "���ظ�λ",     2,  {"ʧ��", "�ɹ�"}},
    {0, "���������¼", 2,  {"ʧ��", "�ɹ�"}},
    {0, "�ֲֳ���",    13,  {"��բѡ��ʧ��", "��բѡ��ɹ�", "��բִ��ʧ��", "��բִ�гɹ�", "��բѡ��ʧ��", "��բѡ��ɹ�", "��բִ��ʧ��", "��բִ�гɹ�", "ң��ȡ��","�ϻ�ѡ��ʧ��", "�ϻ�ѡ��ɹ�", "�ϻ�ִ��ʧ��", "�ϻ�ִ�гɹ�"}},	
};

/* ��־ */
struct tagUlogCfg UlogCfg[] =
{
    {1, "�ն�����", 2,  {"�ϵ�", "�ϵ�"}},
    {1, "��ֵ�޸�", 2,  {"�޸�", "�޸�"}},
    {1, "����״̬", 2,  {"�쳣", "�ָ�"}},
    {1, "�豸���", 2,  {"����", "����"}},
    {1, "�������", 2,  {"�쳣", "�ָ�"}},
};

/* ���в��� */
struct tagInherentParaCfg InherentParaCfg[] = 
{
    {1, "�ն�����",             g_InherentPara.terminalType},  
    {1, "�ն˲���ϵͳ",         g_InherentPara.terminalOpreatSystem}, 
    {1, "�ն�������",           g_InherentPara.terminalManufacturer}, 	
    {1, "�ն�Ӳ���汾",         g_InherentPara.terminalHardwareVersion},  
    {1, "�ն�����汾",         g_InherentPara.terminalSoftwareVersion}, 
    {1, "����汾У����",       g_InherentPara.terminalSoftwareVersionCheckCode}, 	
    {1, "ͨѶ��Լ����",         g_InherentPara.terminalCommunicationProtocolType},  
    {1, "�ն˳����ͺ�",         g_InherentPara.terminalFactoryModel}, 
    {1, "�ն�ID��",             g_InherentPara.terminalID}, 		
    {1, "�ն�����MAC��ַ",      g_InherentPara.terminalMAC}, 
    {1, "�ն˲�Ʒ���к�",       g_InherentPara.terminalProductSerialNumber}, 	
};
	
/* У׼ϵ�� */
struct tagCalibrateFactor CalibrateFactorCfg[] = 
{
//   ʹ��          ����                  У׼ֵ	          ���                             ϵ��ֵ                         		��������   ϵ������ֵ       ϵ������ֵ      ϵ��ȱʡֵ
    { 1,    "Ƶ��(f)У׼ϵ��",           50.00f,   &g_TelemetryBaseAddr.F,                &g_CalibrateFactor[CALIFACTOR_F],      		0,    1.2f,            0.8f,           1.0f},
    { 1,    "A�����(Ia)У׼ϵ��",       5.00f,    &g_TelemetryBaseAddr.Ia,               &g_CalibrateFactor[CALIFACTOR_Ia],     		0,    0.005f,         0.0035f,         0.00433378899f},
    { 1,    "B�����(Ib)У׼ϵ��",       5.00f,    &g_TelemetryBaseAddr.Ib,               &g_CalibrateFactor[CALIFACTOR_Ib],     		0,    0.005f,         0.0035f,         0.00432677846f},
    { 1,    "C�����(Ic)У׼ϵ��",       5.00f,    &g_TelemetryBaseAddr.Ic,               &g_CalibrateFactor[CALIFACTOR_Ic],     		0,    0.005f,         0.0035f,         0.00432838406f},
  #ifdef SYNTHESIS_I0_ENABLE
    { 1,    "�������(I0)У׼ϵ��",      1.00f,    &g_TelemetryBaseAddr.I0,               &g_CalibrateFactor[CALIFACTOR_I0],     		0,    0.005f,         0.0035f,         0.00432f},
  #else		
    { 1,    "�������(I0)У׼ϵ��",      1.00f,    &g_TelemetryBaseAddr.I0,               &g_CalibrateFactor[CALIFACTOR_I0],     		0,    0.001f,         0.0004f,         0.000866f}, 
  #endif		
    { 1,    "�ߵ�ѹ(Uab)У׼ϵ��",       100.00f,  &g_TelemetryBaseAddr.Uab,              &g_CalibrateFactor[CALIFACTOR_Uab],    		0,    0.02f,          0.0065f,         0.009516f},
    { 1,    "�ߵ�ѹ(Ucb)У׼ϵ��",       100.00f,  &g_TelemetryBaseAddr.Ucb,              &g_CalibrateFactor[CALIFACTOR_Ucb],    		0,    0.02f,          0.0065f,         0.009516f},
    { 1,    "�ߵ�ѹ(Uac)У׼ϵ��",       100.00f,  &g_TelemetryBaseAddr.Uac,              &g_CalibrateFactor[CALIFACTOR_Uac],    		0,    0.02f,          0.0065f,         0.009516f},
    { 1,    "�ߵ�ѹ(U0)У׼ϵ��",        50.00f,   &g_TelemetryBaseAddr.U0,               &g_CalibrateFactor[CALIFACTOR_U0],     		0,    0.02f,          0.0065f,         0.009516f},
    { 1,    "�ߵ�ѹ(UAB)У׼ϵ��",       100.00f,  &g_TelemetryBaseAddr.UAB,              &g_CalibrateFactor[CALIFACTOR_UAB],    		0,    0.02f,          0.0065f,         0.009516f},
    { 1,    "�ߵ�ѹ(UCB)У׼ϵ��",       100.00f,  &g_TelemetryBaseAddr.UCB,              &g_CalibrateFactor[CALIFACTOR_UCB],    		0,    0.02f,          0.0065f,         0.009516f},

    { 1,    "ֱ����ѹ1У׼ϵ��",         24,       &g_TelemetryBaseAddr.DC1,              &g_CalibrateFactor[CALIFACTOR_DC1],    		0,    0.05f,          0.0001f,         0.0013745f},
    { 1,    "ֱ����ѹ2У׼ϵ��",         24,       &g_TelemetryBaseAddr.DC2,              &g_CalibrateFactor[CALIFACTOR_DC2],    		0,    0.05f,          0.0001f,         0.0013745f},
    { 1,    "�¶�У׼ϵ��",              0,        &g_TelemetryBaseAddr.T,                &g_CalibrateFactor[CALIFACTOR_T],      		0,    2.0f,           0.0001f,         1.0f},
	
    { 1,    "Uab_Ia�н�У׼ϵ��",        0,        &g_TelemetryBaseAddr.alphy_Uab_Ia,     &g_CalibrateFactor[CALIFACTOR_ALPHA_UabIa],   1,    0.6f,           0.2f,           0.359226197f},
    { 1,    "Ucb_Ic�н�У׼ϵ��",        0,        &g_TelemetryBaseAddr.alphy_Ucb_Ic,     &g_CalibrateFactor[CALIFACTOR_ALPHA_UcbIc],   1,    0.6f,           0.2f,           0.416621923f},
    { 1,    "U0_I0�н�У׼ϵ��",         0,        &g_TelemetryBaseAddr.alphy_U0_I0,      &g_CalibrateFactor[CALIFACTOR_ALPHA_U0I0],    1,    0.6f,           0.2f,           0.32171011f},
    { 1,    "Ux_Ux�н�У׼ϵ��",         0,        &g_TelemetryBaseAddr.alphy_Ux_Ux,      &g_CalibrateFactor[CALIFACTOR_ALPHA_UxUx],    1,    0.2f,           -0.2f,          0.0502243042f},	
    { 1,    "Ƶ��(f2)У׼ϵ��",          50.00f,   &g_TelemetryBaseAddr.F2,               &g_CalibrateFactor[CALIFACTOR_F2],     		0,    1.2f,            0.8f,           1.0f},	
};


/* ��ֵ0�� */
struct tagValueParaCfg ParameterCfg[] = 
{
//   ʹ��            ʹ�� �˵����          ����                             ֵ                         ��λ      ����        ����   ȱʡֵ    ��������  С����λ��       ��ʾ����           ��ע  
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "��������",        &g_Parameter[SWTICH_CLASS],                 "-",       1,           0,     0,         2,         0,         {"��·��","���ɿ���"},   " "   },        	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "�ֶ���ѹ��",      &g_Parameter[MODBREAK_STRAP],               "-",       1,           0,     1,         2,         0,         {"��", "Ͷ"        },   " "   },   
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "������ѹ��",      &g_Parameter[MODCONTACT_STRAP],             "-",       1,           0,     1,         2,         0,         {"��", "Ͷ"        },   " "   }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "�غ�բ��ѹ��",    &g_Parameter[RECLOSE_STRAP],                "-",       1,           0,     1,         2,         0,         {"��", "Ͷ"        },   " "   }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "������ѹ��",      &g_Parameter[FUNCTION_STRAP],               "-",       1,           0,     1,         2,         0,         {"��", "Ͷ"        },   " "   }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "���ɱ�����ѹ",    &g_Parameter[CFG_PRO_VOL_N],                "-",       1,           0,     1,         2,         0,         {"UAB", "UCB"},   " "   },     
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "���ʵ�ѹcb",      &g_Parameter[CFG_POW_VOL_CB],               "-",       1,           0,     1,         2,         0,         {"Ucb", "UCB"},   " "   }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "һ�α��_U",      &g_Parameter[RATIO_U_ONE_TURN],             "V",    100000.00f,   0.00f,   10000.00f, 0,         3,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "���α��_U",      &g_Parameter[RATIO_U_SECONDARY],            "V",     1000.00f,   0.00f,    220.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "һ�α��_U0",     &g_Parameter[RATIO_U0_ONE_TURN],            "V",     10000.00f,   0.00f,   1,         0,         3,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "���α��_U0",     &g_Parameter[RATIO_U0_SECONDARY],           "V",     1000.00f,   0.00f,    1,         0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "һ�α��_I",      &g_Parameter[RATIO_I_ONE_TURN],             "A",    100000.00f,   0.00f,   600,       0,         3,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "���α��_I",      &g_Parameter[RATIO_I_SECONDARY],            "A",     1000.00f,   0.00f,    5,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "һ�α��_I0",     &g_Parameter[RATIO_I0_ONE_TURN],            "A",     10000.00f,   0.00f,   20,        0,         3,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "���α��_I0",     &g_Parameter[RATIO_I0_SECONDARY],           "A",     10000.00f,   0.00f,   1,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "�������ʱ��",    &g_Parameter[DI_SHAKING_TIME],              "ms",     99999,        0,     20.0f,     0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "��բ����ʱ��",    &g_Parameter[CLOSING_PULSE_TIME],           "ms",     99999,        0,     60,        0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "��բ����ʱ��",    &g_Parameter[OPENING_PULSE_TIME],           "ms",     99999,        0,     60,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "��Уʱ��",        &g_Parameter[REVERSE_TIME],                 "ms",     99999,        5,     60000.0,   0,         0,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_BASIC_SET,   "��Դ�澯ȡ��",    &g_Parameter[POWERLOSS_NEGATE],             "-",       1,           0,     0,         2,         0,         {"��", "ȡ��"},   " "   },	  
 
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "Ƶ����Ư",        &g_Parameter[ZERODRIFT_F],                 	"Hz",     99.99f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "A�������Ư",     &g_Parameter[ZERODRIFT_Ia],                	"A",      99.99f,     0.00001f,   0.005f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "B�������Ư",     &g_Parameter[ZERODRIFT_Ib],                	"A",      99.99f,     0.00001f,   0.005f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "C�������Ư",     &g_Parameter[ZERODRIFT_Ic],                	"A",      99.99f,     0.00001f,   0.005f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "���������Ư",    &g_Parameter[ZERODRIFT_I0],                	"A",      99.99f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "Uab��Ư",         &g_Parameter[ZERODRIFT_Uab],               	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "Ucb��Ư",         &g_Parameter[ZERODRIFT_Ucb],               	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },		
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "Uac��Ư",         &g_Parameter[ZERODRIFT_Uac],               	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "U0��Ư",          &g_Parameter[ZERODRIFT_U0],                	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "UAB��Ư",         &g_Parameter[ZERODRIFT_UAB],               	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "UCB��Ư",         &g_Parameter[ZERODRIFT_UCB],               	"V",      250.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "�й�������Ư",    &g_Parameter[ZERODRIFT_P],                 	"kW",     999.99f,    0.00001f,   0.0001f,     0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "�޹�������Ư",    &g_Parameter[ZERODRIFT_Q],                 	"kVar",   999.99f,    0.00001f,   0.0001f,     0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "���ڹ�����Ư",    &g_Parameter[ZERODRIFT_S],                 	"kVA",    999.99f,    0.00001f,   0.0001f,     0,         3,         {"-",  "-"         },   " "    },		
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "����������Ư",    &g_Parameter[ZERODRIFT_PF],                	"-",      1.0f,       0.00001f,   0.001f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "ֱ��1��Ư",       &g_Parameter[ZERODRIFT_DC1],               	"V",      60.0f,      0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "ֱ��2��Ư",       &g_Parameter[ZERODRIFT_DC2],               	"V",      60.0f,      0.00001f,   9.9f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_ZERODRIFT,   "�¶���Ư",        &g_Parameter[ZERODRIFT_T],                 	"��C",    100.0f,     0.00001f,   0.1f,        0,         3,         {"-",  "-"         },   " "    },	
	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "Ƶ������",        &g_Parameter[DEADZONE_F],                   "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "A���������",     &g_Parameter[DEADZONE_Ia],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "B���������",     &g_Parameter[DEADZONE_Ib],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "C���������",     &g_Parameter[DEADZONE_Ic],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "�����������",    &g_Parameter[DEADZONE_I0],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "Uab����",         &g_Parameter[DEADZONE_Uab],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "Ucb����",         &g_Parameter[DEADZONE_Ucb],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },		
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "Uac����",         &g_Parameter[DEADZONE_Uac],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "U0����",          &g_Parameter[DEADZONE_U0],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "UAB����",         &g_Parameter[DEADZONE_UAB],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "UCB����",         &g_Parameter[DEADZONE_UCB],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "�й���������",    &g_Parameter[DEADZONE_P],                   "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "�޹���������",    &g_Parameter[DEADZONE_Q],                   "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "���ڹ�������",    &g_Parameter[DEADZONE_S],                   "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },		
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "������������",    &g_Parameter[DEADZONE_PF],                  "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "ֱ��1����",       &g_Parameter[DEADZONE_DC1],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "ֱ��2����",       &g_Parameter[DEADZONE_DC2],                 "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "�¶�����",        &g_Parameter[DEADZONE_T],                   "%",      99999.0f,     0.01f,   99999.0f,    0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "U�ֵ",         &g_Parameter[DEADZONE_U_VALUE],             "V",      300.0f,     	0.01f,   220.0f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "I�ֵ",         &g_Parameter[DEADZONE_I_VALUE],             "A",      999.0f,     	0.01f,   5.0f,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "U0�ֵ",        &g_Parameter[DEADZONE_U0_VALUE],            "V",      300.0f,     	0.01f,   100.0f,      0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable, 1,  ME_DEADEZONE,   "I0�ֵ",        &g_Parameter[DEADZONE_I0_VALUE],            "A",      999.0f,     	0.01f,   1.0f,        0,         3,         {"-",  "-"         },   " "    },	
	
    { &g_MenuEnable, 1,  ME_UART_COM,    "���ں�",          &g_Parameter[UART_PORT],                    "-",       1,           0,     0,          2,         0,         {"������", "������"},   " "   }, 
    { &g_MenuEnable, 1,  ME_UART_COM,    "������",          &g_Parameter[UART_BAUDRATE],                "-",       4,           0,     2,          5,         0,         {"2400", "4800", "9600", "38400", "115200"},   " "   }, 
    { &g_MenuEnable, 1,  ME_UART_COM,    "����λ",          &g_Parameter[UART_WORDLENGTH],              "-",       9.00f,     5.00f,    8.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_UART_COM,    "ֹͣλ",          &g_Parameter[UART_STOPBITS],                "-",       3.00f,     0.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                    
    { &g_MenuEnable, 1,  ME_UART_COM,    "У��λ",          &g_Parameter[UART_PARITY],                  "-",       2,           0,     0,          3,         0,         {"��У��", "��У��", "żУ��"},   " "   },                      
    { &g_MenuEnable, 1,  ME_UART_COM,    "101ģʽ",         &g_Parameter[UART_BALANMODE],               "-",       1,           0,     1,          2,         0,         {"��ƽ��", "ƽ��"},   " "   },                 
    { &g_MenuEnable, 1,  ME_UART_COM,    "��վ��ַ",        &g_Parameter[UART_SOURCEADDR],              "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                 
    { &g_MenuEnable, 1,  ME_UART_COM,    "��վ��ַ����",    &g_Parameter[UART_LINKADDRSIZE],            "-",       2.00f,     1.00f,    2.00f,     0,         0,         {"-",  "-"         },   " "    },                    
    { &g_MenuEnable, 1,  ME_UART_COM,    "����ԭ�򳤶�",    &g_Parameter[UART_ASDUCOTSIZE],             "-",       2.00f,     1.00f,    2.00f,     0,         0,         {"-",  "-"         },   " "    },              
    { &g_MenuEnable, 1,  ME_UART_COM,    "ASDU��ַ",        &g_Parameter[UART_ASDUADDR],                "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                
    { &g_MenuEnable, 1,  ME_UART_COM,    "ASDU��ַ����",    &g_Parameter[UART_ASDUADDRSIZE],            "-",       2.00f,     1.00f,    2.00f,     0,         0,         {"-",  "-"         },   " "    },                         
    
    { &g_MenuEnable, 1,  ME_NET_COM,     "��IP_1",         &g_Parameter[NET_IP1_0],                     "-",     255.00f,     0.00f,  192.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_NET_COM,     "��IP_2",         &g_Parameter[NET_IP1_1],                     "-",     255.00f,     0.00f,  168.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "��IP_3",         &g_Parameter[NET_IP1_2],                     "-",     255.00f,     0.00f,   60.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "��IP_4",         &g_Parameter[NET_IP1_3],                     "-",     255.00f,     0.00f,  100.00f,     0,         0,         {"-",  "-"         },   " "    },    
    { &g_MenuEnable, 1,  ME_NET_COM,     "��IP_1",         &g_Parameter[NET_IP2_0],                     "-",     255.00f,     0.00f,  192.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_NET_COM,     "��IP_2",         &g_Parameter[NET_IP2_1],                     "-",     255.00f,     0.00f,  168.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "��IP_3",         &g_Parameter[NET_IP2_2],                     "-",     255.00f,     0.00f,   60.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "��IP_4",         &g_Parameter[NET_IP2_3],                     "-",     255.00f,     0.00f,  120.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_NET_COM,     "����_1",         &g_Parameter[NET_GATEWAY_0],                 "-",     255.00f,     0.00f,  192.00f,     0,         0,         {"-",  "-"         },   " "    }, 
    { &g_MenuEnable, 1,  ME_NET_COM,     "����_2",         &g_Parameter[NET_GATEWAY_1],                 "-",     255.00f,     0.00f,  168.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "����_3",         &g_Parameter[NET_GATEWAY_2],                 "-",     255.00f,     0.00f,   60.00f,     0,         0,         {"-",  "-"         },   " "    },
    { &g_MenuEnable, 1,  ME_NET_COM,     "����_4",         &g_Parameter[NET_GATEWAY_3],                 "-",     255.00f,     0.00f,  254.00f,     0,         0,         {"-",  "-"         },   " "    },                         
    { &g_MenuEnable, 1,  ME_NET_COM,     "��վ��ַ",       &g_Parameter[NET_SOURCEADDR],                "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },                
    { &g_MenuEnable, 1,  ME_NET_COM,     "ASDU��ַ",       &g_Parameter[NET_ASDUADDR],                  "-",   65535.00f,     1.00f,    1.00f,     0,         0,         {"-",  "-"         },   " "    },   
}; 

/* ��ֵһ�� */
struct tagValueParaCfg FixedValueCfg1[] = 
{
//   ʹ��                  ʹ��   �˵����       ����                            ֵ                           	   ��λ      ����        ����   	ȱʡֵ     ��������   С����λ��       ��ʾ����           ��ע
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "����һ��Ͷ��",    	&g_FixedValue1[OVER_CURRENT_SWITCH1],          "-",     2,        	0,     		0,          3,         0,         {"��", "Ͷ", "�澯"},   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "����һ�ζ�ֵ",    	&g_FixedValue1[OVER_CURRENT_VALUE1],           "A",     100.00f,    0.00f,  	10,         0,         3,         {"-",  "-"         },   " "   }, 
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "����һ��ʱ��",    	&g_FixedValue1[OVER_CURRENT_TIME1],            "S",     20.00f,     0.00f,  	0,          0,         3,         {"-",  "-"         },   " "   },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "��������Ͷ��",    	&g_FixedValue1[OVER_CURRENT_SWITCH2],          "-",     2,          0,     		0,          3,         0,         {"��", "Ͷ", "�澯"},   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "�������ζ�ֵ",    	&g_FixedValue1[OVER_CURRENT_VALUE2],           "A",     100.00f,    0.00f,  	8,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "��������ʱ��",    	&g_FixedValue1[OVER_CURRENT_TIME2],            "S",     20.00f,     0.00f,  	3,          0,         3,         {"-",  "-"         },   " "   },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "��������Ͷ��",    	&g_FixedValue1[OVER_CURRENT_SWITCH3],          "-",     2,          0,     		0,          3,         0,         {"��", "Ͷ", "�澯"},   " "   },    
    { &g_MenuUnable,       0,  COM_PROTEC,   "�������ε���",    	&g_FixedValue1[OVER_CURRENT_VALUE3],           "A",     100.00f,    0.00f,  	5,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_MenuUnable,       0,  COM_PROTEC,   "��������ʱ��",    	&g_FixedValue1[OVER_CURRENT_TIME3],            "S",     20.00f,     0.00f,   	5,          0,         3,         {"-",  "-"         },   " "   },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,  "�������ٶ�Ͷ��",    	&g_FixedValue1[IACCI_SWITCH],                  "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,  "�������ٶζ�ֵ",    	&g_FixedValue1[IACCI_CURRENT_VALUE],           "A",     99.99f,     0.00f,   	6,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_MenuUnable,       0,  COM_PROTEC,  "��������ʱ",    		&g_FixedValue1[IACCI_TIME],                    "S",     99.99f,     0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },
	{ &g_FixedMenu0Enable, 1,  COM_PROTEC,   "�������һ��Ͷ��",	&g_FixedValue1[OVER_CURRENTI0_SWITCH1],        "-",     2,          0,     		0,          3,         0,         {"��", "Ͷ", "�澯"},   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "�������һ�ζ�ֵ",	&g_FixedValue1[OVER_CURRENTI0_VALUE1],         "A",     100.00f,    0.00f,   	5,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "�������һ��ʱ��",	&g_FixedValue1[OVER_CURRENTI0_TIME1],          "S",     20.00f,     0.00f,   	3,          0,         3,         {"-",  "-"         },   " "   },	    
    { &g_MenuUnable,       0,  COM_PROTEC,   "�������Ͷ��",    	&g_FixedValue1[OVER_CURRENTI0_SWITCH2],        " ",     2,          0,     		0,          3,         0,         {"��", "Ͷ", "�澯"},   " "    },    
    { &g_MenuUnable,       0,  COM_PROTEC,   "������ε���",    	&g_FixedValue1[OVER_CURRENTI0_VALUE2],         "A",     100.00f,    0.00f,   	5,          0,         3,         {"-",  "-"         },   " "    }, 
    { &g_MenuUnable,       0,  COM_PROTEC,   "�������ʱ��",    	&g_FixedValue1[OVER_CURRENTI0_TIME2],          "S",     20.00f,     0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },		
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,  "����������ٶ�Ͷ��",  	&g_FixedValue1[IACCI0_SWITCH],                 "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,  "����������ٶζ�ֵ",  	&g_FixedValue1[IACCI0_CURRENT_VALUE],          "A",     99.99f,     0.00f,   	6,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_MenuUnable,       0,  COM_PROTEC,  "����������ʱ",    		&g_FixedValue1[IACCI0_TIME],                   "S",     99.99f,     0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "�����غ�բ����ʱ��",	&g_FixedValue1[SECONDARY_RECLOSE_LOCK_TIME],   "S",     99.99f,     0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "һ���غ�բͶ��",    	&g_FixedValue1[RECLOSE_ONE_SWITCH],            "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   },    
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "�����غ�բͶ��",    	&g_FixedValue1[RECLOSE_TOW_SWITCH],            "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   },    
	{ &g_MenuUnable, 	   0,  COM_PROTEC,   "����غϴ���",    	&g_FixedValue1[RECLOSE_TIMES_I],               "��",    4,          0,     		2,          0,         3,         {"-",  "-"         },   " "   },
    { &g_MenuUnable,       0,  COM_PROTEC,   "�����غϴ���",    	&g_FixedValue1[RECLOSE_TIMES_I0],              "��",    2,          0,     		0,          0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "һ���غ�բʱ��",    	&g_FixedValue1[RECLOSE_TIMER1],                "S",     99.99f,     0.00f,   	2,          0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "�����غ�բʱ��",    	&g_FixedValue1[RECLOSE_TIMER2],                "S",     99.99f,     0.00f,   	10,         0,         3,         {"-",  "-"         },   " "   },
    { &g_MenuUnable,       0,  COM_PROTEC,   "�����غ�ʱ��",    	&g_FixedValue1[RECLOSE_TIMER3],                "S",     99.99f,     0.00f,   	15,         0,         3,         {"-",  "-"         },   " "   },
    { &g_MenuUnable,       0,  COM_PROTEC,   "�Ĵ��غ�ʱ��",    	&g_FixedValue1[RECLOSE_TIMER4],                "S",     99.99f,     0.00f,   	20,         0,         3,         {"-",  "-"         },   " "   },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "�غ�բ���ʱ��",    	&g_FixedValue1[RE_RESET_TIME],                 "S",     99.99f,     0.00f,   	30,          0,         3,         {"-",  "-"         },   " "   },
	{ &g_FixedMenu0Enable, 1,  COM_PROTEC,   "����ѹͶ��",    		&g_FixedValue1[OVERVOLTAGE_SWITCH],            "-",     2,          0,     		0,          3,         0,         {"��", "Ͷ", "�澯"},   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "����ѹ��ֵ",    		&g_FixedValue1[OVERVOLTAGE_VALUE],             "V",     300,       	0.00f,   	110,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "����ѹʱ��",    		&g_FixedValue1[OVERVOLTAGE_TIME],              "S",     999,     	0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "��ѹ����ϵ��",    	&g_FixedValue1[OVERVOLTAGE_FACTOR],            "-",     1.00f,      0.01f,   	1,          0,         3,         {"-",  "-"         },   " "    },			
    { &g_MenuUnable,       0,  COM_PROTEC,   "��ѹ����Ͷ��",    	&g_FixedValue1[DOWNVOLTAGE_SWITCH],            "-",     2,          0,     		0,          3,         0,         {"��", "Ͷ", "�澯"},   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "��ѹ���ж�ֵ",    	&g_FixedValue1[DOWNVOLTAGE_VALUE],             "V",     300,   		0.00f,   	30,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "��ѹ������ʱ",    	&g_FixedValue1[DOWNVOLTAGE_TIME],              "S",     999,     	0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "��ѹ����ϵ��",    	&g_FixedValue1[DOWNVOLTAGE_FACTOR],            "-",     2.00f,      1.00f,   	1,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "��ƵͶ��",    		&g_FixedValue1[OVERFREQUENCY_SWITCH],          "-",     2,          0,     		0,          3,         0,         {"��", "Ͷ", "�澯"},   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "��Ƶ��ֵ",    		&g_FixedValue1[OVERFREQUENCY_VALUE],           "Hz",    55.00f,     50.01f,  	60,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "��Ƶʱ��",    		&g_FixedValue1[OVERFREQUENCY_TIME],            "S",     999,     	0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "���ܽ���ϵ��",    	&g_FixedValue1[OVERFREQUENCY_FACTOR],          "-",     1.00f,      0.01f,   	1,          0,         3,         {"-",  "-"         },   " "    },
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "��ƵͶ��",    		&g_FixedValue1[DOWNFREQUENCY_SWITCH],          "-",     2,          0,     		0,          3,         0,         {"��", "Ͷ", "�澯"},   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "��Ƶ��ֵ",    		&g_FixedValue1[DOWNFREQUENCY_VALUE],           "Hz",    49.99f,     45.00f,  	49.5,       0,         3,         {"-",  "-"         },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "��Ƶʱ��",    		&g_FixedValue1[DOWNFREQUENCY_TIME],            "S",     999,     	0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "���ܽ���ϵ��",    	&g_FixedValue1[DOWNFREQUENCY_FACTOR],          "-",     2.00f,      1.00f,   	1,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "�������Ͷ��",    	&g_FixedValue1[SLIPLOCK_SWITCH],               "-",     2,          0,     		0,          2,         0,         {"��", "Ͷ",       },   " "    },	
    { &g_FixedMenu0Enable, 1,  COM_PROTEC,   "���������ֵ",    	&g_FixedValue1[SLIPLOCK_VALUE],                "Hz/s",  10.00f,      0.30f,   	10,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "������Ͷ��",      	&g_FixedValue1[OVERLOAD_SWITCH],               "-",     2,          0,     		0,          3,         0,         {"��", "Ͷ", "�澯"},   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "�����ɶ�ֵ",      	&g_FixedValue1[OVERLOAD_VALUE],                "A",     999.99f,    0.00f,   	8,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "������ϵ��",      	&g_FixedValue1[OVERLOAD_FACTOR],               "-",     1.00f,      0.01f,   	1,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  COM_PROTEC,   "��������ʱ",      	&g_FixedValue1[OVERLOAD_TIME],                 "S",     999.99f,    0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
	
	{ &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "�����ϼ�����ֵ",  &g_FixedValue1[OVERCURRENTFAULT_VALUE],        "A",     100,        0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "�ӵع��ϼ�����ֵ",  &g_FixedValue1[OVERCURRENTFAULTI0_VALUE],      "A",     100,        0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },
    { &g_MenuUnable, 	   0,  LOGICAL_FUN,  "������ֵ",        	&g_FixedValue1[CURRENT_VALUE],                 "A",     20,         0.00f,   	0.2f,       0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "��ѹ��ֵ",        	&g_FixedValue1[VOLTAGE_VALUE],                 "V",     300,        0.00f,   	176,        0,         3,         {"-",  "-"         },   " "   },	
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "ʧѹ��ֵ",        	&g_FixedValue1[NONVOLTAGE_VALUE],              "V",     300,        0.00f,   	80,         0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "���ϼ��ʱ��(Yʱ��)", &g_FixedValue1[FAULT_CHECK_Y_TIME],            "S",     999,        0.00f,   	5,          0,         3,         {"-",  "-"         }, "Yʱ�� "},
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "�ϵ����ϵ����о�",    &g_FixedValue1[LOCKED_CLOSING_CUR_SWITCH],     "-",     1,          0,     		1,          2,         0,         {"��", "Ͷ"        },   " "   }, 
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "��բ�ɹ�����ʧѹ��",  &g_FixedValue1[LOCKED_OPENING_SWITCH],         "-",     1,          0,     		1,          2,         0,         {"��", "Ͷ"        },   " "   },      
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "������բ����ʱ��",    &g_FixedValue1[SHORT_LOCKED_OPENING_TIME],     "S",     999,      	0.00f,   	10,         0,         3,         {"-",  "-"         }, "Zʱ�� "},
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "ʧ����ʱ��բͶ��",    &g_FixedValue1[LOSS_ELECTRICITY_SWITCH],       "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "ʧ���բ��ʱ",    	&g_FixedValue1[LOSS_OPENING_TIME],             "S",     999,       	0.10f,   	3,          0,         3,         {"-",  "-"         },   " "   },		
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "��Դ��õ��բͶ��",  &g_FixedValue1[UAB_GET_VOLTAGE_CLOSSING_SWITCH], "-",   1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   }, 
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "���ɲ�õ��բͶ��",  &g_FixedValue1[UCB_GET_VOLTAGE_CLOSSING_SWITCH], "-",   1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "�õ��բ��ʱ(Xʱ��)", &g_FixedValue1[GET_VOLTAGE_CLOSSING_X_TIME],   "S",     999,       	0.00f,   	7,          0,         3,         {"-",  "-"         }, "Xʱ�� "},	
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "����ʧѹ��բͶ��",    &g_FixedValue1[SINGLE_LOSS_VOLTAGE_SWITCH],    "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "ʧѹ��բ��ʱ(XLʱ��)",&g_FixedValue1[SINGLE_LOSS_VOLTAGE_XLTIME],    "S",     999,       	0.00f,   	5,          0,         3,         {"-",  "-"         },"XLʱ�� "},	
    { &g_MenuUnable, 	   0,  LOGICAL_FUN,  "˫����ѹͶ��",    	&g_FixedValue1[DOUBLE_VOLTAGE_SWITCH],         "-",     1,          0,     		1,          2,         0,         {"��", "Ͷ"        },   " "   },  
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "��ѹ����Ͷ��",    	&g_FixedValue1[REMAIN_VOLTAGE_SWITCH],         "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "��ѹ��ֵ",        	&g_FixedValue1[REMAIN_VOLTAGE_VALUE],          "V",     300,      	0.00f,   	30,         0,         3,         {"-",  "-"         },   " "   },
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "��ѹ�б�ʱ��",    	&g_FixedValue1[REMAIN_VOLTAGE_TIME],           "S",     999,       	0.00f,   	0,          0,         3,         {"-",  "-"         },   " "   },		
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "���ڶϵ�������Ͷ��",  &g_FixedValue1[BREAKING_CURRENT_SWITCH],       "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ",       },   " "   },	
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "���ڶϵ���ֵ",      	&g_FixedValue1[BREAKING_CURRENT_VALUE],        "A",     100,       	0.00f,   	5,          0,         3,         {"-",  "-"         },   " "   },	
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "���ڹ��Ϻ����Ͷ��",  &g_FixedValue1[IACCFI_SWITCH],                 "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "���ڹ��Ϻ���ٶ�ֵ",  &g_FixedValue1[IACCFI_CURRENT_VALUE],          "A",     99.99f,     0.00f,   	6,          0,         3,         {"-",  "-"         },   " "   }, 
	{ &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "ӿ��ʶ��Ͷ��",    	&g_FixedValue1[INRUSH_SWITCH],                 "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   },    
    { &g_FixedMenu1Enable, 1,  LOGICAL_FUN,  "ӿ��ʶ��ֵ",    	&g_FixedValue1[INRUSH_TIME],                   "S",     99.99f,     0.00f,   	5,          0,         3,         {"-",  "-"         },   " "   },	
	
    { &g_MenuUnable,       0,  IACC_PROTEC,  "��ѹ����Ͷ��",    	&g_FixedValue1[IACCU0_SWITCH],                 "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "   },    
    { &g_MenuUnable,       0,  IACC_PROTEC,  "��ѹ���ٶ�ֵ",    	&g_FixedValue1[IACCU0_CURRENT_VALUE],          "v",     99.99f,     0.00f,   	6,          0,         3,         {"-",  "-"         },   " "   }, 
    { &g_MenuUnable,       0,  IACC_PROTEC,  "��ѹ������ʱ",    	&g_FixedValue1[IACCU0_TIME],                   "S",     99.99f,     0.00f,   	1,          0,         3,         {"-",  "-"         },   " "   },    		
   
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "��ͬ��Ͷ��",      	&g_FixedValue1[CLOSING_LOOP_SWITCH],           "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ",       },   " "    },	
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "ͬ����ѹ��ֵ",    	&g_FixedValue1[CLOSING_LOOP_VOLTAGE],          "V",     300,    	0.00f,   	176,        0,         3,         {"-",  "-"         },   " "   },	
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "ͬ����ѹ��ֵ",    	&g_FixedValue1[CLOSING_LOOP_NONVOLTAGE],       "V",     300,    	0.00f,   	80,         0,         3,         {"-",  "-"         },   " "   },
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "����ѹ��",        	&g_FixedValue1[VOLTAGE_DIFFERENCE],            "V",     30.00f,     0.00f,   	5,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "��ǲ�",          	&g_FixedValue1[PHASEANGLE_DIFFERENCE],         "��",    10.00f,     0.00f,   	5,          0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "Ƶ�ʲ�",          	&g_FixedValue1[FREQUENCY_DIFFERENCE],          "Hz",    99.99f,     0.00f,   	5,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LOOP_CLOSE,   "��ͬ��ʱ��",      	&g_FixedValue1[CLOSING_LOOP_DELAY],            "S",     999,     	0.00f,   	5,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  FAULT_SWITCH, "���ƻ�·�쳣",    	&g_FixedValue1[CONTROL_LOOP_ABNOMAL_ENABLE],   "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ",       },   " "    },		
	{ &g_MenuUnable,       0,  BATTERY_SET,  "��ػͶ��",    	&g_FixedValue1[BATTERY_ACTIVE_SWITCH],         "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },	
    { &g_MenuUnable,       0,  BATTERY_SET,  "��ػ����",    	&g_FixedValue1[BATTERY_ACTIVE_CYCLE],          "��",    99999.99f,  0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  BATTERY_SET,  "�����ʱ��",    	&g_FixedValue1[BATTERY_ACTIVE_TIME],           "h",     23,         0,     		0,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  BATTERY_SET,  "����ϵ�ѹ",    	&g_FixedValue1[BATTERY_ACTIVE_FAULT_VOLTAGE],  "V",     999.99f,    0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuUnable,       0,  BATTERY_SET,  "�������ʱ",    	&g_FixedValue1[BATTERY_ACTIVE_FAULT_TIME],     "S",     999.99f,    0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuUnable,       0,  AUTO_RESET,   "�Զ�����Ͷ��",    	&g_FixedValue1[AUTOMATIC_RESET_SWITCH],        "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },	
    { &g_MenuUnable,       0,  AUTO_RESET,   "�Զ�������ʱ",    	&g_FixedValue1[AUTOMATIC_RESET_TIME],          "S",     99999.99f,  0.00f,   	99,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "Խ�޸澯Ͷ��",    	&g_FixedValue1[OVERLIMIT_SWITCH],              "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "Խ������ϵ��",    	&g_FixedValue1[UPLIMIT_FACTOR],                "-",     1.00f,      0.00f,   	0.8f,       0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "Խ������ϵ��",    	&g_FixedValue1[DOWNLIMIT_FACTOR],              "-",     2.00f,      1.00f,   	1.2,        0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "Խ����ʱ",        	&g_FixedValue1[OVERLIMIT_TIME],                "S",     999.99f,    0,     		3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "UabԽ��Ͷ��",     	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_Uab],    "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "UbcԽ��Ͷ��",     	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_Ubc],    "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "UacԽ��Ͷ��",     	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_Uca],    "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "UABԽ��Ͷ��",     	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_UAB],    "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "UBCԽ��Ͷ��",     	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_UBC],    "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "��ѹ���޶�ֵ",    	&g_FixedValue1[UPLIMIT_VOLTAGE_U],             "V",     999.99f,    0.00f,   	110,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "��ѹ���޶�ֵ",    	&g_FixedValue1[DOWNLIMIT_VOLTAGE_U],           "V",     999.99f,    0.00f,   	20,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "IaԽ��Ͷ��",      	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_IA],     "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "IbԽ��Ͷ��",      	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_IB],     "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "IcԽ��Ͷ��",      	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_IC],     "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "�������޶�ֵ",    	&g_FixedValue1[UPLIMIT_CURRENT_I],             "A",     999.99f,    0.00f,   	6,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "�������޶�ֵ",    	&g_FixedValue1[DOWNLIMIT_CURRENT_I],           "A",     999.99f,    0.00f,   	0.5,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "U0Խ��Ͷ��",      	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_U0],     "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "U0���޶�ֵ",      	&g_FixedValue1[UPLIMIT_VOLTAGE_U0],            "V",     999.99f,    0.00f,   	100,        0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "U0���޶�ֵ",      	&g_FixedValue1[DOWNLIMIT_VOLTAGE_U0],          "V",     999.99f,    0.00f,   	20,         0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "I0Խ��Ͷ��",      	&g_FixedValue1[OVERLIMIT_ALARM_SWITCH_I0],     "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "I0���޶�ֵ",      	&g_FixedValue1[UPLIMIT_VOLTAGE_I0],            "A",     999.99f,    0.00f,   	3,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "I0���޶�ֵ",      	&g_FixedValue1[DOWNLIMIT_VOLTAGE_I0],          "A",     999.99f,    0.00f,   	0.2f,       0,         3,         {"-",  "-"         },   " "    },		
    { &g_MenuEnable,       1,  LIMITATION,   "U_DCԽ��Ͷ��",    	&g_FixedValue1[OVERLIMIT_ALARMSWITCH_DC_U],    "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "U_DC���޶�ֵ",    	&g_FixedValue1[UPLIMIT_DC_VOLTAGE_U],          "V",     999.99f,    0.00f,  	30,         0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "U_DC���޶�ֵ",    	&g_FixedValue1[DOWNLIMIT_DC_VOLTAGE_U],        "V",     999.99f,    0.00f,   	18,         0,         3,         {"-",  "-"         },   " "    },
    { &g_MenuEnable,       1,  LIMITATION,   "I_DCԽ��Ͷ��",    	&g_FixedValue1[OVERLIMIT_ALARMSWITCH_DC_I],    "-",     1,          0,     		0,          2,         0,         {"��", "Ͷ"        },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "I_DC���޶�ֵ",    	&g_FixedValue1[UPLIMIT_DC_CURRENT_I],          "A",     999.99f,    0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },	
    { &g_MenuEnable,       1,  LIMITATION,   "I_DC���޶�ֵ",    	&g_FixedValue1[DOWNLIMIT_DC_CURRENT_I],        "A",     999.99f,    0.00f,   	0,          0,         3,         {"-",  "-"         },   " "    },		
}; 

/* ��ֵ���� */
struct tagValueParaCfg FixedValueCfg2[sizeof(FixedValueCfg1) / sizeof(FixedValueCfg1[0])];

/* ��չң�� */
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


