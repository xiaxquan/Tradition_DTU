/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      point_table_config.h
  * @brief:     The point_table_config of the project.
  * @version:   ver 1.1
  * @author:    Mr.Sxr
  * @date:      2018-05-09
  * @update:    2018-05-09 [][]
  */
	
#ifndef __POINT_TABLE_CONFIG_H__
#define __POINT_TABLE_CONFIG_H__

extern char *BeiJingSuangJie;
extern char *YingKaiRu;

enum TELEMETRY_MENU
{
    SECONDRY,    // 二次遥测
    ONCE,            // 一次遥测
    HARMONIC,       // 谐波分量
	TELEMETRY_MENU_ALLNUM
};

enum PARAMETER_MENU
{
    ME_BASIC_SET,   // 基本设置
    ME_ZERODRIFT,       // 零漂
    ME_DEADEZONE,       // 死区
    ME_UART_COM,        // UART通讯参数
    ME_NET_COM,         // NET通讯参数
	PARAMETER_MENU_ALLNUM
};

enum FIXED_VALUE_MENU
{
	COM_PROTEC,      // 常规保护
	LOGICAL_FUN,     // 逻辑功能
	LIMITATION,      // 越限告警
	HEAVY_LOAD,      // 重过载
    IACC_PROTEC,     // 后加速
	OVERLOAD,        // 过负荷
	LIMIT_V_F,       // 越压越频
	BATTERY_SET,     // 电池设置
	AUTO_RESET,      // 自动复归
	LOOP_CLOSE,      // 合环
	FAULT_SWITCH,    // 故障投退
	FIXED_VALUE_MENU_ALLNUM
};  

/**
 * @note  注意：所有名称为字符串数组格式，且长度为30字节
 */
/**
 * @brief 点表配置转Json结构体定义
 */
typedef struct tagConfigurationSetDatabaseToJson
{
    char name[20];      //数据名称
    int dataLen;        //标明该数据的长度
    float value[200];   //存储数据的数组
}ConfigurationSetDatabaseToJson;

/**
 * @brief 面板开入结构体
 */
struct taghmiDigitalInputCfg
{
	unsigned char enable;   // 使能
    unsigned char hmiNum;   // 面板开入号
    unsigned short *pAddr;  // 点号
    unsigned char negate;   // 取反	
};

/**
 * @brief 主控模入结构体
 */
struct tagzkDigitalInputCfg
{
	unsigned char enable;   // 使能
    unsigned char pin;      // 引脚
    unsigned short *pAddr;  // 点号
    unsigned char lastVal;  // 上次值
    unsigned char negate;   // 取反	
    unsigned long count;    // 计数器
};

/**
 * @brief 主控开入结构体
 */
struct tagzkAnalogInputCfg
{
	unsigned char enable;   // 使能
    unsigned char pin;      // 引脚
    unsigned short *pAddr;  // 点号
};

/**
 * @brief 遥信配置结构体
 */
struct tagTelesignalCfg
{
	unsigned char enable;   // 使能
    char *pName;            // 名称
    unsigned short *pAddr;  // 点号
    unsigned char *pVal;    // 值	
    unsigned char defaultval;  // 默认
	unsigned char enableFault;  // 故障记录使能
	char *pContentYx[2];    // 数据显示内容	
	char *pContentSoe[2];   // 数据显示内容		
};

/* 遥测配置结构 */
struct tagTelemetryCfg
{
	unsigned char enable;  // 使能
	unsigned char menuNum; // 菜单编号	
    char *pName;           // 面板名称
    char *pNameUp;         // 上位机名称
    unsigned short *pAddr;  // 点号
    float *pVal;           // 值	
    char *pUnit;           // 单位
    float RatedValue;    // 额定值		
	float *pZerodrift;     // 零漂    
	float *pDeadzone;      // 死区	
};
	
/* 遥控配置结构 */
struct tagTelecontrolCfg
{
	unsigned char enable;   // 使能
    char *pName;            // 名称
	unsigned char dataType; // 数据类型
	char *pContentSoe[13];   // 数据显示内容	
};

/* 日志配置结构 */
struct tagUlogCfg
{
	unsigned char enable;   // 使能
    char *pName;            // 名称
	unsigned char dataType; // 数据类型
	char *pContentSoe[4];   // 数据显示内容	
};

/* 校准系数配置结构 */
struct tagCalibrateFactor
{
	unsigned char enable;  // 使能
    char *pName;           // 名称
    float calibrateVal;    // 校准值
    unsigned short *pAddr;  // 点号
    float *factorVal;      // 系数值
    unsigned char dataType; // 数据类型
    float factorMax;	   // 系数上限值	
    float factorMin;       // 系数下限值
    float factorDefault;   // 系数缺省值	
};

/* 定值配置结构 */
struct tagValueParaCfg
{
	int *pEnable;  // 面板使能
    int enable;  // 上位机使能
	unsigned char menuNum; // 菜单编号
    char *pName;           // 名称
    float *pVal;           // 值	
    char *pUnit;           // 单位
    float valMax;	       // 上限值	
    float valMin;          // 下限值
    float defaultVal;      // 缺省值	
	unsigned char dataType;// 数据类型
    unsigned char decimals;// 小数点位数
	char *pContent[5];        // 数据显示内容
	char *pNote;           // 备注
};

/* 固有参数配置结构 */
struct tagInherentParaCfg
{
	unsigned char enable;  // 使能
    char *pName;           // 名称
    char *pVal;            // 值	
};
    
extern struct taghmiDigitalInputCfg hmiDigitalInputCfg[];
extern struct tagzkAnalogInputCfg zkAnalogInputCfg[];
extern struct tagzkDigitalInputCfg zkDigitalInputCfg[];
extern struct tagTelesignalCfg TelesignalCfg[];
extern struct tagTelemetryCfg TelemetryCfgBase[];
extern struct tagCalibrateFactor CalibrateFactorCfg[];
extern struct tagValueParaCfg ParameterCfg[];
extern struct tagValueParaCfg FixedValueCfg1[];
extern struct tagValueParaCfg FixedValueCfg2[];
extern struct tagTelecontrolCfg TelecontrolCfg[];
extern struct tagUlogCfg UlogCfg[];
extern struct tagInherentParaCfg InherentParaCfg[];
extern ConfigurationSetDatabaseToJson SetDatabaseCfg[];
extern struct tagTelemetryCfg TelemetryCfg[];

extern unsigned int g_FixedValueCfg1_Len;
extern unsigned int g_FixedValueCfg2_Len;
extern unsigned int g_ParameterCfg_Len;
extern unsigned int g_CalibrateFactorCfg_Len;
extern unsigned int g_TelemetryCfgBase_Len;
extern unsigned int g_taghmiDigitalInputCfg_Len;
extern unsigned int g_tagzkAnalogInputCfg_Len;
extern unsigned int g_tagzkDigitalInputCfg_Len;
extern unsigned int g_TelesignalCfg_Len;
extern unsigned int g_TelecontrolCfg_Len;
extern unsigned int g_UlogCfg_Len;
extern unsigned int g_InherentParaCfg_Len;
extern unsigned int g_TelemetryCfg_Len;

#endif /* __POINT_TABLE_CONFIG_H__ */

/* END OF FILE ---------------------------------------------------------------*/
