#ifndef _MAIN_PROTECT_CTRL_H_
#define _MAIN_PROTECT_CTRL_H_

#include "drv_do.h"
#include "common_data.h"

#include "stm32f4xx_hal.h"

#define MAINPRO_ENTIMERS 0x80000000
#define MAINPRO_TITIMERS 0x7fffffff

#define SWITCH_ON  (1.0f)
#define SWITCH_OFF (0.0f)

#define RESETFLAG  0x80000000 //复位标志
#define MIDDLEFLAG 0x7fff0000 //自身中间标志
#define JUDGRFLAG  0x0000ffff //对外判断标志

enum
{
    MAINPRO_FALSE,                // 
    MAINPRO_TRUE
};

enum
{
    MAINPRO_RESET,                // 复位
    MAINPRO_STATEJUDGE,        //状态判断
    MAINPRO_LOSSTRIP,            // 失电分闸
    MAINPRO_GETVOLCLOSE,            // 得电合闸
    MAINPRO_OPENCLOSELOCK,     //合分闸闭锁
    MAINPRO_OVERVOLU0,     //零序电压保护
    MAINPRO_FEWVOLLOCK,     //残压闭锁
    MAINPRO_DOUBLEVOL,      //双侧有压
    MAINPRO_SINGELELOSSCLOSE, //单侧失电延时合闸
    MAINPRO_INRUSH,                // 涌流抑制
    MAINPRO_IACC,                // 后加速
    MAINPRO_OVERCUR,                // 过流
    MAINPRO_OVERCURI0,            // 零序过流
    MAINPRO_SENCONDARYRECLSELOCK,            // 二次重合闸闭锁
    MAINPRO_RECLOSE,                // 重合闸
    MAINPRO_RECLOSEI0,                // 零序重合闸
    MAINPRO_PROMAXNUM
};

typedef struct 
{
    uint16_t addr;             // 地址
    uint8_t *value;            // 值  
}STelesignalStr;

typedef struct 
{
    STelesignalStr switchOpen;             // 开关分位
    STelesignalStr switchClose;            // 开关合位    
    STelesignalStr modBreakHardStrap;      // 分段类型
    STelesignalStr modContactHardStrap;    // 联络类型
    STelesignalStr functionHardStrap;      // 功能投退硬压板 
	STelesignalStr recloseHardStrap;       // 重合闸硬压板
    STelesignalStr modBreakSoftStrap;      // 分段类型
    STelesignalStr modContactSoftStrap;    // 联络类型
    STelesignalStr functionSoftStrap;      // 功能投退硬压板 
	STelesignalStr recloseSoftStrap;       // 重合闸硬压板
    STelesignalStr workmodeCommon;         // 常规保护型
    STelesignalStr workmodeVolcur;         // 电压电流型
    STelesignalStr swtichclass;            // 开关类型

    STelesignalStr recloseAct;               // 重合动作 
    STelesignalStr recloseClock;             // 重合闭锁
    STelesignalStr overcurrentAccIa;         // A相后加速过流
    STelesignalStr overcurrentAccIb;         // B相后加速过流
    STelesignalStr overcurrentAccIc;         // C相后加速过流      
    STelesignalStr overcurrentAccI0;         // 零序后加速过流
    STelesignalStr overcurrentAccU0;         // 零序后加速过压  
    STelesignalStr overcurrentIa[3];           // A相过流保护 
    STelesignalStr overcurrentIb[3];           // B相过流保护 
    STelesignalStr overcurrentIc[3];           // C相过流保护             
    STelesignalStr overcurrentI0[2];           // 零序过流保护 
    STelesignalStr recloseProtection[4];            // 重合闸保护     
    STelesignalStr zeroRecloseProtection[2];   // 零序重合闸保护  
    STelesignalStr clockSecondaryClose;      // 闭锁二次重合闸 
    
    STelesignalStr closingLocked;               // 合闸闭锁
    STelesignalStr openingLocked;               // 分闸闭锁
    STelesignalStr shortCircuitFault;        // 短路故障   
    STelesignalStr earthingFault;            // 接地故障
    STelesignalStr protectionAct;            // 保护动作  
    STelesignalStr overcurrentFaultIa;           // A相过流保护 
    STelesignalStr overcurrentFaultIb;           // B相过流保护 
    STelesignalStr overcurrentFaultIc;           // C相过流保护             
    STelesignalStr overcurrentFaultI0;           // 零序过流保护
    STelesignalStr lossTrip;                   //失电分闸
    STelesignalStr getClossing;                //得电合闸
    STelesignalStr failAfterClosing;           //合闸到故障
    STelesignalStr remainClockClossingUab;      //Uab残压闭锁
	STelesignalStr remainClockClossingUcb;      //Ucb残压闭锁
    STelesignalStr doubleProhibitClossing;     //双侧有压禁止合闸
    STelesignalStr singleLossClosing;          //单侧失压延时合闸
    STelesignalStr breakingCurrent;        //遮断电流
}TelesignalStr;

typedef struct 
{
    float *f;                        // 频率
    float *Ia;                       // A相电流 
    float *Ib;                       // B相电流 
    float *Ic;                       // C相电流 
	float *Ia2;                      // A相二次谐波电流 
    float *Ib2;                      // B相二次谐波电流 
    float *Ic2;                      // C相二次谐波电流 
    float *I0;                       // 零序电流
    float *Uab;                      // 线电压Uab
    float *Ucb;                      // 线电压Ucb
    float *Uac;                      // 线电压Uac
    float *U0;                       // 零序电压   
    float *f_s;                      // 滑差    
}TelemetryStr;

#define MANRESETFLAG     0x80000000
#define SETRESETFLAGMAN  0x40000000
#define SETRESETFLAG     0x3fffffff
typedef struct
{
    struct
    {
        uint32_t *gTime;       //定时器  
        uint32_t flag;
        uint32_t *StateJudgeflag;
        uint32_t *lossTripflag;
        uint32_t *getVolCloseflag;
        uint32_t *openCloseLockingflag;
        uint32_t *fewVolLockflag;
        uint32_t *doubleVol;
        uint32_t *SingleLossClose;
        uint32_t *iACCflag;
		uint32_t *inverseflag;
        uint32_t *overcurflag;
        uint32_t *overcurI0flag;
        uint32_t *recloseflag;
        uint32_t *recloseI0flag;
        uint32_t *secondaryRecloseLockflag;
    }valstr;
    struct
    {
        float *pSwitch;            // 自动复归使能  软压板定值，可以选择“投入”、“退出”
        float *pTime;              // 自动复位时间 时间定值，整定范围：0000~9999ms  
    }parastr;
}RestSts;

//状态标志
#define STATENOVOLTAGE      	0x00000001//无压
#define STATENOCURRENT      	0x00000002//无流
#define STATEUNBLOCKED      	0x00000004//非遮断
#define STATEOVERCURNOSW    	0x00000008//过流未投
#define STATEOVERCURI0NOSW  	0x00000010//零序过流未投
#define STATEOVERCUR        	0x00000020//相间过流
#define STATEOVERCURI0      	0x00000040//零序过流
#define STATESINGLEVOLTAGEGET  	0x00000080//单侧有压(得电)
#define STATESINGLEVOLTAGELOSS  0x00000100//单侧有压(失电)
#define STATESINGLEVOLTAGEUAB  	0x00000200//电源侧有压
#define STATESINGLEVOLTAGEUCB  	0x00000400//负荷侧有压
#define STATEDOUBLEVOLTAGE  	0x00000800//双侧有压
#define STATEUNINRUSH          	0x00001000//无涌流抑制
#define STATESLIPLOCK          	0x00002000//滑差闭锁
#define STATEOVERCURSTA1    	0x00010000//合闸位置发生过流
#define STATEOVERCURSTA2    	0x00020000//分闸位置允许复位
#define STATEOVERCURI0STA1  	0x00040000//合闸位置发生过流
#define STATEOVERCURI0STA2  	0x00080000//分闸位置允许复位
#define STATEINRUSHSTA1      	0x00100000//涌流抑制时间到
typedef struct
{
    struct
    {
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTimeInrush;       //定时器  
    }valstr;
    struct
    {
        float *pValueOvercur;             // 过流电流 流定值，整定范围：00.00A~99.99A
        float *pValueOvercurI0;           // 零序过流 流定值，整定范围：00.00A~99.99A
        float *pValuehavevol;             // 有压 流定值，整定范围：00.00A~99.99v
        float *pValueLossvol;             // 无压 流定值，整定范围：00.00A~99.99v
        float *pValueLosscur;             // 无流 流定值，整定范围：00.00A~99.99A
        float *pSwitchBreakcur;            // 非遮断投退 “1投入”、“0退出”
        float *pValueBreakcur;             // 非遮断电流 流定值，整定范围：00.00A~99.99A
        float *pSwitchInrush;            // 涌流抑制投退 软压板定值，可以选择“投入”、“退出”
        float *pTimeInrush;              // 涌流抑制时间 时间定值，整定范围：0000~9999ms  
        float *pSwitchSlip;            // 滑差闭锁投退 软压板定值，可以选择“投入”、“退出”
        float *pValueSlip;             // 滑差闭锁定值，整定范围：0.3~10
    }parastr;
}StateJudgeSts;

//失电分闸
#define LOSSTRIP 0x00000001
#define LOSSTRIPSTA1  0x00010000    //发生失电分闸
#define LOSSTRIPSTA2  0x00020000    //失电分闸后第一次合闸
typedef struct
{
    struct
    {
		uint32_t *openCloseLockingflag;
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime;       //定时器  
    }valstr;
    struct
    {
        float *pSwitch;             // 失电分闸投退 “1投入”、“0退出”
        float *pTime;               // 失压分闸时间
    }parastr;
}LossTripSts;

//得电合闸
#define GETVOLCLOSE      0x00000001       //得电合闸
#define GETVOLCLOSESTA1  0x00010000    //得电合闸
#define GETVOLCLOSESTA2  0x00020000    //得电合闸后第一次分闸
#define GETVOLCLOSESTA3  0x00040000    //得电合闸过程中失压闭锁
typedef struct
{
    struct
    {
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime;       //定时器  
    }valstr;
    struct
    {
        float *pSwitchUab;             // 得电合闸投退 “1投入”、“0退出”
        float *pSwitchUcb;             // 得电合闸投退 “1投入”、“0退出”
        float *pXTime;              // 得电合闸X时间
    }parastr;
}GetVolCloseSts;

//合分闸闭锁
#define OPENCLOSELOCKINGCLOSELOCK  			0x00000001    //合闸闭锁
#define OVERCURRENTCLOSELOCKINGCLOSELOCK  	0x00000002    //过流合闸闭锁
#define OPENCLOSELOCKINGSTA1  				0x00010000    //刚合闸后
#define OPENCLOSELOCKINGSTA2  				0x00020000    //Y时间内故障
#define OPENCLOSELOCKINGSTA3  				0x00040000    //Y时间外
#define OPENCLOSELOCKINGSTA4  				0x00080000    //分闸动作
#define OPENCLOSELOCKINGSTA5  				0x00100000    //分闸闭锁计时
#define OPENCLOSELOCKINGSTA6  				0x00200000    //分闸后复位
#define OPENCLOSELOCKINGSTA7  			    0x00400000    //Y时间内过流
#define OPENCLOSELOCKINGSTA8  				0x00800000    //手动分闸
//#define OPENCLOSELOCKINGSTA9  			0x01000000    //过流跳闸
#define OPENCLOSELOCKINGSTA10 				0x02000000    //分合闸闭锁复归
//#define OPENCLOSELOCKINGSTA11 			0x04000000    //Y时间内故障（瞬间）
typedef struct
{
    struct
    {
        uint32_t *iACCflag;
		uint32_t *getVolCloseflag;
		uint32_t *singleLossCloseflag;        
        uint32_t *lossTripflag;       
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime;       //定时器  
    }valstr;
    struct
    {
        float *pYTime;              // 故障Y时间
        float *pCloseLockCurSwitch;             // 投退 “1投入”、“0退出”
        float *pOpenLockSwitch;             // 投退 “1投入”、“0退出”
        float *pOpenLockTime;              // 分闸闭锁时间
    }parastr;
}OpenCloseLockingSts;

//残压闭锁
#define FEWVOLLOCK      0x00000001        //
#define FEWVOLLOCKUABSTA1  0x00010000    //初始无压状态
#define FEWVOLLOCKUABSTA2  0x00020000    //有压状态
#define FEWVOLLOCKUABSTA3  0x00040000    //检测到最低残压判断
#define FEWVOLLOCKUABSTA4  0x00080000    //合闸闭锁
#define FEWVOLLOCKUCBSTA1  0x00100000    //初始无压状态
#define FEWVOLLOCKUCBSTA2  0x00200000    //有压状态
#define FEWVOLLOCKUCBSTA3  0x00400000    //检测到最低残压判断
#define FEWVOLLOCKUCBSTA4  0x00800000    //合闸闭锁
typedef struct
{
    struct
    {
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gUabTime;       //定时器  
		uint32_t *gUcbTime;       //定时器  
    }valstr;
    struct
    {
        float *pSwitch;             // 投退 “1投入”、“0退出”
        float *pValue;            // 电压值
        float *pTime;              // 故障时间
        float *pXTime;              // 故障X时间
		float *pXLTime;              // 失压延时时间
    }parastr;
}FewVolLockSts;

//双侧有压
#define DOUBLEVOL      0x00000001    //
#define DOUBLEVOL1     0x00000002	//双侧有压闭锁合闸前存在闭锁合闸
#define DOUBLEVOL2     0x00000004	//标志已存
#define DOUBLEVOLSTA1  0x00010000   //双侧有压状态
typedef struct
{
    struct
    {
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag; 
    }valstr;
        struct
    {
        float *pSwitch;             // 投退 “1投入”、“0退出”
    }parastr;
}DoubleVolSts;

//单侧失电合闸
#define SINGELELOSSCLOSE      0x00000001
#define SINGELELOSSCLOSETIME  0x00003A98    //双侧有压固有时间15s
#define SINGELELOSSCLOSESTA1  0x00010000    //双侧有压大于固有时间
#define SINGELELOSSCLOSESTA2  0x00020000    //单侧有压
#define SINGELELOSSCLOSESTA3  0x00040000    //单侧有压合闸
#define SINGELELOSSCLOSESTA4  0x00080000    //单侧有压合闸复位
typedef struct
{
    struct
    {
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime;       //定时器  
    }valstr;
    struct
    {
        float *pSwitch;             // 失电分闸投退 “1投入”、“0退出”
        float *pTime;                // 失压分闸时间
    }parastr;
}SingleLossCloseSts;

//后加速标志保持到重新合闸
#define IACCTIME     			3000				//后加速保护时间
#define IACCCUR 				0x00000001
#define IACCCURI 				0x00000002
#define IACCCURI0 				0x00000004
#define IACCCURU0 				0x00000008

#define IACCCURSTA1  			0x00010000    	//发生过流
#define IACCCURSTA2  			0x00020000    	//过流导致分闸
#define IACCCURSTA3  			0x00040000    	//动作
#define IACCCURSTA4  			0x00080000    	//后加速有效时间
typedef struct
{
    struct
    {
		uint32_t *overcurflag;
        uint32_t *overcurI0flag;
        uint32_t *stateJudgeflag;
        uint32_t *recloseflag;
		uint32_t *recloseI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTimeEnIACC;       //定时器  
        uint32_t *gTimeIACCI;       //定时器 
        uint32_t *gTimeIACCI0;       //定时器 
        uint32_t *gTimeIACCU0;       //定时器 
    }valstr;
    struct
    {
        float *pSwitchIACCI;            // 过流投退 “1投入”、“0退出”
        float *pValueIACCI;             // 过流电流 流定值
        float *pTimeIACCI;              // 过流时间 时间定值  
        float *pSwitchIACCI0;            // 过流投退 “1投入”、“0退出”
        float *pValueIACCI0;             // 过流电流 流定值
        float *pTimeIACCI0;              // 过流时间 时间定值，
        float *pSwitchIACCU0;            // 过流投退 “1投入”、“0退出”
        float *pValueIACCU0;             // 过流电压 流定值
        float *pTimeIACCU0;              // 过流时间 时间定值
    }parastr;
}IACCSts;

//过流标志保持到重新合闸
#define OVERCUR1 		0x00000001
#define OVERCUR2 		0x00000002
#define OVERCUR3 		0x00000004
#define OVERCURALARM1 	0x00000008
#define OVERCURALARM2 	0x00000010
#define OVERCURALARM3 	0x00000020
#define OVERCURSTA1  	0x00010000    //发生过流
#define OVERCURSTA2  	0x00020000    //过流导致分闸
typedef struct
{
    struct
    {
        uint32_t *iACCflag;
        uint32_t *overcurI0flag;
        uint32_t *stateJudgeflag;
		uint32_t *recloseflag;
		uint32_t *recloseI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime[3];       //定时器  
    }valstr;
    struct
    {
        float *pSwitch[3];            // 过流1段投退 “1投入”、“0退出”
        float *pValue[3];             // 过流1段电流 流定值，整定范围：00.00A~99.99A
        float *pTime[3];              // 过流1段时间 时间定值，整定范围：00.00s~99.99s  
    }parastr;
}OvercurSts;

//零序过流标志保持到重新合闸
#define OVERCURI01 0x00000001
#define OVERCURI02 0x00000002
#define OVERCURI0STA1  0x00010000    //合闸位置发生过流
#define OVERCURI0STA2  0x00020000    //分闸位置允许复位
#define OVERCURI0STA3  0x01000000    //ALAMIO1
#define OVERCURI0STA4  0x02000000    //ALAMIO2
typedef struct
{
    struct
    {
		uint32_t *iACCflag;
        uint32_t *overcurflag;
        uint32_t *stateJudgeflag;
		uint32_t *recloseflag;
		uint32_t *recloseI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime[2];       //定时器  
    }valstr;
    struct
    {
        float *pSwitch[2];            // 过流1段投退 “1投入”、“0退出”
        float *pValue[2];             // 过流1段电流 流定值，整定范围：00.00A~99.99A
        float *pTime[2];              // 过流1段时间 时间定值，整定范围：00.00s~99.99s  
    }parastr;
}OvercurI0Sts;

//第一次重合闸范围内有效
#define SENCONDARYRECLSELOCK 0x00000001
#define SENCONDARYRECLSELOCKSTA1  0x00010000    //合闸计时
#define SENCONDARYRECLSELOCKSTA2  0x00020000    //计时结束
typedef struct
{
    struct
    {
		uint32_t *iACCflag;
        uint32_t *overcurflag;
        uint32_t *overcurI0flag;
        uint32_t *recloseflag;
        uint32_t *recloseI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime;       //定时器  
    }valstr;
    struct
    {
        float *pTime;              // 过流1段时间 时间定值，整定范围：00.00s~99.99s  
    }parastr;
}SecondaryRecloseLockSts;

//重合闸标志保持到下次重合闸之前
#define RECLOSE1 0x00000001
#define RECLOSE2 0x00000002
#define RECLOSE3 0x00000004
#define RECLOSE4 0x00000008
#define RECLOSELOCK 0x00000100
#define RECLOSESTA1  0x00010000
typedef struct
{
    struct
    {
        uint32_t *secondaryRecloseLockflag;
		uint32_t *iACCflag;
        uint32_t *overcurflag;
        uint32_t *overcurI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime[2];       //定时器  
        uint8_t time;
    }valstr;
    struct
    {
		float *pSwitch[2];            // 一、二次重合投退
        float *pTime[2];          // 重合闸时间 时间定值，整定范围：00.00s~99.99s  
		float *pResetTime;		  // 重合闸复位时间
    }parastr;
}RecloseSts;

//零序重合闸标志保持到下次重合闸之前
#define RECLOSEI01 0x00000001
#define RECLOSEI02 0x00000002
#define RECLOSEI0LOCK 0x00000100
#define RECLOSEI0STA1  0x00010000
typedef struct
{
    struct
    {
        uint32_t *secondaryRecloseLockflag;
		uint32_t *iACCflag;
        uint32_t *overcurflag;
        uint32_t *overcurI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime[2];       //定时器  
        uint8_t time;
    }valstr;
    struct
    {
        float *pTimes;            // 重合闸次数 可以选择：0，1，2，3，4 
        float *pTime[2];          // 重合闸时间 时间定值，整定范围：00.00s~99.99s 
		float *pResetTime;	  // 重合闸复位时间
    }parastr;
}RecloseI0Sts;

//超上限跳闸
#define OVERTELEMETRYUP      0x00000001
typedef struct
{
    struct
    {
        uint32_t flag;
		uint32_t *stateJudgeflag;
        uint32_t *gTime;       //定时器   
    }valstr;
    struct
    {
        STelesignalStr Telesignal; // 遥信值
        float **telemetry0;          // 遥测值
        float **telemetry1;          // 遥测值
        float **telemetry2;          // 遥测值
        float *pSwitch;            // 投退 “1投入”、“0退出”
        float *pValue;             // 值 流定值，整定范围：00.00A~99.99A
        float *pTime;              // 时间 时间定值，整定范围：00.00s~99.99s  
        float *pFactor;            // 因数 时间定值，整定范围：00.00s~99.99s
    }parastr;
}OverTelemetryUpSts;

//超下限跳闸
#define OVERTELEMETRYDOWN      	0x00000001
#define OVERTELEMETRYDOWNSTA1  	0x00010000    //超过电压上限
#define OVERTELEMETRYDOWNTIME				1000		//正常值持续时间
typedef struct
{
    struct
    {
        uint32_t flag;
        uint32_t *stateJudgeflag;
        uint32_t *gTime[2];       //定时器   
    }valstr;
    struct
    {
        STelesignalStr Telesignal; // 遥信值
        float **telemetry0;          // 遥测值
        float **telemetry1;          // 遥测值
        float **telemetry2;          // 遥测值
        float *pSwitch;            // 投退 “1投入”、“0退出”
        float *pValue;             // 值 流定值，整定范围：00.00A~99.99A
        float *pTime;              // 时间 时间定值，整定范围：00.00s~99.99s  
        float *pFactor;            // 因数 时间定值，整定范围：00.00s~99.99s
    }parastr;
}OverTelemetryDownSts;

typedef struct
{
    TelesignalStr yx;
    TelemetryStr yc;
	int WorkMode;                          // 工作模式
    uint8_t (*opening)(uint16_t addr, uint8_t operateType);
    uint8_t (*closing)(uint16_t addr, uint8_t operateType);
    uint8_t (*outputSoe)(uint16_t addr, uint8_t state);
    uint8_t (*outputFevent)(uint16_t yx_addr, uint16_t *yc_addr, uint16_t yc_num);
}ComProSts;

#endif
