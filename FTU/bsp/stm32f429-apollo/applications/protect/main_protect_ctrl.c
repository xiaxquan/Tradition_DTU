/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      main_protect_ctrl.c
  * @brief:     The main protect logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.09.05
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */


/* INCLUDES ------------------------------------------------------------------*/
#include "main_protect_ctrl.h"
#include "main_protect_interface.h"


/* PRIVATE VARIABLES --------------------------------------------------------*/
static List s_ListTimers[MAINPRO_DEVMAXNUM];//定时器链表链表//0x80000000使能位

static ComProSts s_ComProSts[MAINPRO_DEVMAXNUM];

static RestSts s_Rest[MAINPRO_DEVMAXNUM];

static IACCSts s_IACC[MAINPRO_DEVMAXNUM];
static OvercurSts s_Overcur[MAINPRO_DEVMAXNUM];
static OvercurI0Sts s_OvercurI0[MAINPRO_DEVMAXNUM];
static SecondaryRecloseLockSts s_SecondaryRecloseLock[MAINPRO_DEVMAXNUM];
static RecloseSts s_Reclose[MAINPRO_DEVMAXNUM];
static RecloseI0Sts s_RecloseI0[MAINPRO_DEVMAXNUM];

static StateJudgeSts s_stateJudge[MAINPRO_DEVMAXNUM];
static LossTripSts s_LossTrip[MAINPRO_DEVMAXNUM];
static GetVolCloseSts s_GetVolClose[MAINPRO_DEVMAXNUM];
static OpenCloseLockingSts s_OpenCloseLocking[MAINPRO_DEVMAXNUM];
static FewVolLockSts s_FewVolLock[MAINPRO_DEVMAXNUM];
static DoubleVolSts s_DoubleVol[MAINPRO_DEVMAXNUM];
static SingleLossCloseSts s_SingleLossClose[MAINPRO_DEVMAXNUM];

static OverTelemetryUpSts s_OverVoltageUp[MAINPRO_DEVMAXNUM];
static OverTelemetryDownSts s_OverVoltageDown[MAINPRO_DEVMAXNUM];
static OverTelemetryUpSts s_OverFrequencyUp[MAINPRO_DEVMAXNUM];
static OverTelemetryDownSts s_OverFrequencyDown[MAINPRO_DEVMAXNUM];
static OverTelemetryUpSts s_OverLoad[MAINPRO_DEVMAXNUM];


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @Description: 申请定时器.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void addtimers(uint8_t pdrv,uint32_t **dev)
{
    uint32_t *s_timer;
        
    s_timer = rt_malloc(sizeof(uint32_t));
    
    *dev = s_timer;
    *s_timer = 0;
    
    list_ins_next(&s_ListTimers[pdrv],NULL,s_timer); 
}

/**
  * @Description: 定时器增加.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void add_timers(uint8_t pdrv)
{
    ListElmt *element;
    
    element = s_ListTimers[pdrv].head;
    while(element != NULL)
    {
        if((*((uint32_t *)(element->data)))&MAINPRO_ENTIMERS)
        {
            (*((uint32_t *)(element->data)))++;
        }
        element = element->next;
    }
}

/**
  * @Description: 生成SOE及变位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void addSOE(ComProSts *comProSts,STelesignalStr *telesignal,uint8_t value)
{
    if(*(telesignal->value) != value)
    {
        comProSts->outputSoe(telesignal->addr,value);
        if(value == ON)
        {
            comProSts->outputFevent(telesignal->addr,NULL,0);
        }
    }
}

/**
  * @Description: 复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void rest_ctrl(uint8_t pdrv,ComProSts *comProSts,RestSts *restSts)
{
    static uint64_t temp[MAINPRO_DEVMAXNUM];
    uint64_t tempk;

    //自动复归
    if(*(restSts->parastr.pSwitch)==SWITCH_ON)
    {
        tempk = temp[pdrv];
        temp[pdrv] = 0;
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.StateJudgeflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.lossTripflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.getVolCloseflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.openCloseLockingflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.fewVolLockflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.doubleVol)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.SingleLossClose)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.iACCflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.inverseflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.overcurflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.overcurI0flag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.recloseflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.recloseI0flag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.secondaryRecloseLockflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|((*(comProSts->yx.shortCircuitFault.value)&ON)||\
                          (*(comProSts->yx.earthingFault.value)&ON)||\
                          (*(comProSts->yx.protectionAct.value)&ON)||\
                          (*(comProSts->yx.lossTrip.value)&ON)||\
                          (*(comProSts->yx.getClossing.value)&ON)||\
                          (*(comProSts->yx.failAfterClosing.value)&ON)||\
                          (*(comProSts->yx.remainClockClossingUab.value)&ON)||\
						  (*(comProSts->yx.remainClockClossingUcb.value)&ON)||\
                          (*(comProSts->yx.doubleProhibitClossing.value)&ON)||\
                          (*(comProSts->yx.singleLossClosing.value)&ON));
        temp[pdrv] = (temp[pdrv]>>1)|((*(comProSts->yx.shortCircuitFault.value)&ON)||\
                          (*(comProSts->yx.earthingFault.value)&ON)||\
                          (*(comProSts->yx.protectionAct.value)&ON)||\
                          (*(comProSts->yx.recloseAct.value)&ON));

        if(tempk != temp[pdrv] )
        {
            *(restSts->valstr.gTime) = MAINPRO_ENTIMERS;//启动定时器
        }

        if((*(restSts->valstr.gTime)&MAINPRO_TITIMERS)>=(uint32_t)(*(restSts->parastr.pTime)*1000))
        {
            restSts->valstr.flag = SETRESETFLAG;
            *(restSts->valstr.gTime) = 0;
            temp[pdrv] = 0;
        }
    }
    else
    {
        *(restSts->valstr.gTime) = 0;
        temp[pdrv] = 0;
    }

    //手动复归
    if(restSts->valstr.flag&MANRESETFLAG)
    {
        restSts->valstr.flag = SETRESETFLAG|SETRESETFLAGMAN;
        *(restSts->valstr.gTime) = 0;
        temp[pdrv] = 0;
    }
}

/**
  * @Description: 状态判断.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void state_judge(ComProSts *comProSts,StateJudgeSts *stateJudegeSts)
{
    const float factor = 0.95;
	const float k = 0.2;
	//涌流抑制
    if ((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))	//合位
    {
        if (*(stateJudegeSts->parastr.pSwitchInrush) == SWITCH_ON)	//涌流抑制投入	
        {
			if (!((stateJudegeSts->valstr.flag)&STATEINRUSHSTA1)||(*(stateJudegeSts->parastr.pTimeInrush) == 0))	//涌流抑制未发生
			{
                if ((!(*(stateJudegeSts->valstr.gTimeInrush)&MAINPRO_ENTIMERS))&&(*(stateJudegeSts->parastr.pTimeInrush) != 0))
                {
                    *(stateJudegeSts->valstr.gTimeInrush) = MAINPRO_ENTIMERS;	//启动时钟
                }
                
                if ((*(stateJudegeSts->valstr.gTimeInrush)&MAINPRO_TITIMERS)>(uint32_t)(*(stateJudegeSts->parastr.pTimeInrush)*1000))
                {
                    stateJudegeSts->valstr.flag |= STATEINRUSHSTA1;
					stateJudegeSts->valstr.flag |= STATEUNINRUSH;    //无涌流抑制
                    *(stateJudegeSts->valstr.gTimeInrush) = 0;
                }
				else
				{
					if(((*(comProSts->yc.Ia2)>*(comProSts->yc.Ia)*k)||\
						(*(comProSts->yc.Ib2)>*(comProSts->yc.Ib)*k)||\
						(*(comProSts->yc.Ic2)>*(comProSts->yc.Ic)*k)))
					{
						stateJudegeSts->valstr.flag &= ~STATEUNINRUSH;	
					}
					else
					{
						stateJudegeSts->valstr.flag |= STATEUNINRUSH;
					}
				}
			}
        }
        else
        {
            stateJudegeSts->valstr.flag |= STATEUNINRUSH;
        }
    }
    else // 分位
    {
        *(stateJudegeSts->valstr.gTimeInrush) = 0;
    }
    //相间过流
    if(*(stateJudegeSts->parastr.pValueOvercur)!= 0 )//过流未投
    {
        stateJudegeSts->valstr.flag &= ~STATEOVERCURNOSW;
    }
    else
    {
        stateJudegeSts->valstr.flag |= STATEOVERCURNOSW;
    }

    if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位
    {
        if(stateJudegeSts->valstr.flag&STATEOVERCURSTA2)//过流后合闸//复位状态量
        {
            stateJudegeSts->valstr.flag &= ~STATEOVERCURSTA2;
            addSOE(comProSts,&comProSts->yx.overcurrentFaultIa,OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentFaultIb,OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentFaultIc,OFF);
            stateJudegeSts->valstr.flag &= ~(STATEOVERCUR);
            addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
        }

        //检测过流
        if(((!(stateJudegeSts->valstr.flag&STATEOVERCURNOSW))&&\
            (!(stateJudegeSts->valstr.flag&(STATEOVERCUR))))&&\
            ((*(comProSts->yc.Ia)>*(stateJudegeSts->parastr.pValueOvercur))||\
            (*(comProSts->yc.Ib)>*(stateJudegeSts->parastr.pValueOvercur))||\
            (*(comProSts->yc.Ic)>*(stateJudegeSts->parastr.pValueOvercur)))&&\
			(stateJudegeSts->valstr.flag&STATEUNINRUSH))//过流、无涌流抑制
        {
            stateJudegeSts->valstr.flag |= (STATEOVERCUR|STATEOVERCURSTA1);
            stateJudegeSts->valstr.flag |= RESETFLAG;
            if(*(comProSts->yc.Ia)>*(stateJudegeSts->parastr.pValueOvercur))
            {
                //SOE
                addSOE(comProSts,&comProSts->yx.overcurrentFaultIa,OFF);
                addSOE(comProSts,&comProSts->yx.overcurrentFaultIa,ON);
            }
            if(*(comProSts->yc.Ib)>*(stateJudegeSts->parastr.pValueOvercur))
            {
                //SOE
                addSOE(comProSts,&comProSts->yx.overcurrentFaultIb,OFF);
                addSOE(comProSts,&comProSts->yx.overcurrentFaultIb,ON);
            }
            if(*(comProSts->yc.Ic)>*(stateJudegeSts->parastr.pValueOvercur))
            {
                //SOE
                addSOE(comProSts,&comProSts->yx.overcurrentFaultIc,OFF);
                addSOE(comProSts,&comProSts->yx.overcurrentFaultIc,ON);
            }
            addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
            addSOE(comProSts,&comProSts->yx.shortCircuitFault,ON);
        }
        else//不过流
        {
            if(((!(stateJudegeSts->valstr.flag&STATEOVERCURNOSW))&&\
            (stateJudegeSts->valstr.flag&(STATEOVERCUR)))&&\
                    ((*(comProSts->yc.Ia)<*(stateJudegeSts->parastr.pValueOvercur)*factor)&&\
                     (*(comProSts->yc.Ib)<*(stateJudegeSts->parastr.pValueOvercur)*factor)&&\
                     (*(comProSts->yc.Ic)<*(stateJudegeSts->parastr.pValueOvercur)*factor)))
            {
                stateJudegeSts->valstr.flag &= ~(STATEOVERCUR);
            }
        }
    }
    else if(stateJudegeSts->valstr.flag&STATEOVERCURSTA1)//分位//过流后分闸
    {
        stateJudegeSts->valstr.flag &= (~STATEOVERCURSTA1);
        stateJudegeSts->valstr.flag |= STATEOVERCURSTA2;
    }

    //零序过流
    if(*(stateJudegeSts->parastr.pValueOvercurI0)!= 0 )//零序过流未投
    {
        stateJudegeSts->valstr.flag &= ~STATEOVERCURI0NOSW;
    }
    else
    {
        stateJudegeSts->valstr.flag |= STATEOVERCURI0NOSW;
    }

    if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位//无涌流抑制
    {
        if(stateJudegeSts->valstr.flag&STATEOVERCURI0STA2)//过流后合闸//复位状态量
        {
            stateJudegeSts->valstr.flag &= ~STATEOVERCURI0STA2;
            addSOE(comProSts,&comProSts->yx.overcurrentFaultI0,OFF);
            addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
        }

        //检测过流
        if(((!(stateJudegeSts->valstr.flag & STATEOVERCURI0NOSW))&&\
            (!(stateJudegeSts->valstr.flag&(STATEOVERCURI0))))&&(*(comProSts->yc.I0)>*(stateJudegeSts->parastr.pValueOvercurI0)))//过流
        {
            stateJudegeSts->valstr.flag |= (STATEOVERCURI0)|STATEOVERCURI0STA1;
            stateJudegeSts->valstr.flag |= RESETFLAG;
            if(*(comProSts->yc.I0)>*(stateJudegeSts->parastr.pValueOvercurI0))
            {
                //SOE
                addSOE(comProSts,&comProSts->yx.overcurrentFaultI0,OFF);
                addSOE(comProSts,&comProSts->yx.overcurrentFaultI0,ON);
            }
            addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
            addSOE(comProSts,&comProSts->yx.earthingFault,ON);
        }
        else
        {
            if(((!(stateJudegeSts->valstr.flag & STATEOVERCURI0NOSW))&&(stateJudegeSts->valstr.flag&STATEOVERCURI0))&&\
                    (*(comProSts->yc.I0)<*(stateJudegeSts->parastr.pValueOvercurI0)*factor))//过流
            {
                stateJudegeSts->valstr.flag &= ~(STATEOVERCURI0);
            }
        }
    }
    else if(stateJudegeSts->valstr.flag&STATEOVERCURI0STA1)//分闸//过流后分闸
    {
        stateJudegeSts->valstr.flag &= (~STATEOVERCURI0STA1);
        stateJudegeSts->valstr.flag |= STATEOVERCURI0STA2;
    }
    
    //相间非遮断
    if(*(stateJudegeSts->parastr.pSwitchBreakcur)==SWITCH_ON)
    {
        if((*(comProSts->yc.Ia)>*(stateJudegeSts->parastr.pValueBreakcur))||\
                (*(comProSts->yc.Ib)>*(stateJudegeSts->parastr.pValueBreakcur))||\
                (*(comProSts->yc.Ic)>*(stateJudegeSts->parastr.pValueBreakcur))||\
                (*(comProSts->yc.I0)>*(stateJudegeSts->parastr.pValueBreakcur)))
        {
            stateJudegeSts->valstr.flag |= STATEUNBLOCKED;
            addSOE(comProSts,&comProSts->yx.breakingCurrent,ON);
        }
        else if((*(comProSts->yc.Ia)<*(stateJudegeSts->parastr.pValueBreakcur)*factor)&&\
                (*(comProSts->yc.Ib)<*(stateJudegeSts->parastr.pValueBreakcur)*factor)&&\
                (*(comProSts->yc.Ic)<*(stateJudegeSts->parastr.pValueBreakcur)*factor)&&\
                (*(comProSts->yc.I0)<*(stateJudegeSts->parastr.pValueBreakcur)*factor))
        {
            stateJudegeSts->valstr.flag &= ~STATEUNBLOCKED;
            addSOE(comProSts,&comProSts->yx.breakingCurrent,OFF);
        }
    }
    else
    {
        stateJudegeSts->valstr.flag &= ~STATEUNBLOCKED;
        addSOE(comProSts,&comProSts->yx.breakingCurrent,OFF);
    }
    
    //无压
    if((*(comProSts->yc.Uab)<=*(stateJudegeSts->parastr.pValueLossvol))&&\
            (*(comProSts->yc.Ucb)<=*(stateJudegeSts->parastr.pValueLossvol)))
    {
        stateJudegeSts->valstr.flag |= STATENOVOLTAGE;
        stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGEGET;
		stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGELOSS;
        stateJudegeSts->valstr.flag &= ~STATEDOUBLEVOLTAGE;
        stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGEUAB;
        stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGEUCB;
    }
    else
    {
        if((*(comProSts->yc.Uab)>*(stateJudegeSts->parastr.pValuehavevol))&&\
                (*(comProSts->yc.Ucb)>*(stateJudegeSts->parastr.pValuehavevol)))//双侧有压
        {
            stateJudegeSts->valstr.flag |= STATEDOUBLEVOLTAGE;
            stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGEGET;
			stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGELOSS;
            stateJudegeSts->valstr.flag &= ~STATENOVOLTAGE;
            stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGEUAB;
            stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGEUCB;
        }
        else if(((*(comProSts->yc.Uab)>*(stateJudegeSts->parastr.pValuehavevol))&&(*(comProSts->yc.Ucb)<=*(stateJudegeSts->parastr.pValueLossvol)))||\
                ((*(comProSts->yc.Ucb)>*(stateJudegeSts->parastr.pValuehavevol))&&(*(comProSts->yc.Uab)<=*(stateJudegeSts->parastr.pValueLossvol))))//单侧有压
        {
			if(stateJudegeSts->valstr.flag & STATEDOUBLEVOLTAGE)
			{
				stateJudegeSts->valstr.flag |= STATESINGLEVOLTAGELOSS;
				stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGEGET;
			}
			else if(stateJudegeSts->valstr.flag & STATENOVOLTAGE)
			{				
				stateJudegeSts->valstr.flag |= STATESINGLEVOLTAGEGET;
				stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGELOSS;
			}
            if(*(comProSts->yc.Uab)>*(stateJudegeSts->parastr.pValuehavevol))
            {stateJudegeSts->valstr.flag |= STATESINGLEVOLTAGEUAB;}
            else
            {stateJudegeSts->valstr.flag |= STATESINGLEVOLTAGEUCB;}
            stateJudegeSts->valstr.flag &= ~STATEDOUBLEVOLTAGE;
            stateJudegeSts->valstr.flag &= ~STATENOVOLTAGE;
        }
    }

    //无流
    if((*(comProSts->yc.Ia)<=*(stateJudegeSts->parastr.pValueLosscur)*factor)&&\
            (*(comProSts->yc.Ib)<=*(stateJudegeSts->parastr.pValueLosscur)*factor)&&\
            (*(comProSts->yc.Ic)<=*(stateJudegeSts->parastr.pValueLosscur)*factor))
    {
        stateJudegeSts->valstr.flag |= STATENOCURRENT;
    }
    else if((*(comProSts->yc.Ia)>=*(stateJudegeSts->parastr.pValueLosscur))||\
            (*(comProSts->yc.Ib)>=*(stateJudegeSts->parastr.pValueLosscur))||\
            (*(comProSts->yc.Ic)>=*(stateJudegeSts->parastr.pValueLosscur)))
    {
        stateJudegeSts->valstr.flag &= ~STATENOCURRENT;
    }
 
    //滑差闭锁
    if((*(stateJudegeSts->parastr.pSwitchSlip)==SWITCH_ON)&&(*(comProSts->yc.f)>45)&&(*(comProSts->yc.f)<55))
    {
        if(*(comProSts->yc.f_s)>*(stateJudegeSts->parastr.pValueSlip))
        {
            stateJudegeSts->valstr.flag |= STATESLIPLOCK;
        }
        else
        {
            stateJudegeSts->valstr.flag &= ~STATESLIPLOCK;
        }
    }
    else
    {
        stateJudegeSts->valstr.flag &= ~STATESLIPLOCK;
    }
}

/**
* @Description: 状态判断复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void state_judge_rest(ComProSts *comProSts,StateJudgeSts *stateJudegeSts)
{
    if(*(stateJudegeSts->valstr.resetflag)&(1<<MAINPRO_STATEJUDGE))//复位
    {
        *(stateJudegeSts->valstr.resetflag) &= ~(1<<MAINPRO_STATEJUDGE);
        stateJudegeSts->valstr.flag = (STATENOVOLTAGE|STATENOCURRENT);
        addSOE(comProSts,&comProSts->yx.overcurrentFaultIa,OFF);
        addSOE(comProSts,&comProSts->yx.overcurrentFaultIb,OFF);
        addSOE(comProSts,&comProSts->yx.overcurrentFaultIc,OFF);
        addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
        addSOE(comProSts,&comProSts->yx.overcurrentFaultI0,OFF);
        addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
        if(*(stateJudegeSts->valstr.resetflag)&SETRESETFLAGMAN)
        {}
    }
}

/**
  * @Descrbiption: 失压跳闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void lossTrip_ctrl(ComProSts *comProSts,LossTripSts *lossTripSts)
{
    if((*(comProSts->yx.switchClose.value)==ON)&&(*(comProSts->yx.switchOpen.value)==OFF))//合位
    {
        if(lossTripSts->valstr.flag&LOSSTRIPSTA2)//动作后合闸//复位状态量
        {
            lossTripSts->valstr.flag &= (~LOSSTRIPSTA2);
			lossTripSts->valstr.flag &= (~LOSSTRIP);
            *(lossTripSts->valstr.gTime) = 0;
            addSOE(comProSts,&comProSts->yx.lossTrip,OFF);
            addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
        }

        if((*(lossTripSts->parastr.pSwitch)==SWITCH_ON)&&(!(lossTripSts->valstr.flag&LOSSTRIPSTA1)))
        {
            //无压无流
            if((*(lossTripSts->valstr.stateJudgeflag)&STATENOVOLTAGE)&&(*(lossTripSts->valstr.stateJudgeflag)&STATENOCURRENT)&&(*(comProSts->yx.openingLocked.value)==OFF)&&\
                (comProSts->WorkMode == TYPE_WORKMODE_VOLCUR))//无压无流
            {
                if(!(*(lossTripSts->valstr.gTime)&MAINPRO_ENTIMERS))
                {
                    *(lossTripSts->valstr.gTime) = MAINPRO_ENTIMERS;//启动定时
                }
                if((*(lossTripSts->valstr.gTime)&MAINPRO_TITIMERS)>=(uint32_t)(*(lossTripSts->parastr.pTime)*1000))
                {
                    comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
                    lossTripSts->valstr.flag |= LOSSTRIP|LOSSTRIPSTA1|RESETFLAG;
                    addSOE(comProSts,&comProSts->yx.lossTrip,ON);
                    if(*(lossTripSts->valstr.openCloseLockingflag)&JUDGRFLAG)
                    {
                        addSOE(comProSts,&comProSts->yx.closingLocked,ON);
                    }
                }
            }
            else
            {
                *(lossTripSts->valstr.gTime) = 0;
            }
        }
        else
        {
            *(lossTripSts->valstr.gTime) = 0;
        }
    }
    else if(lossTripSts->valstr.flag&(LOSSTRIPSTA1))//分位
    {
        lossTripSts->valstr.flag &= (~LOSSTRIPSTA1);
        lossTripSts->valstr.flag |= LOSSTRIPSTA2;
    }
    else
    {
        *(lossTripSts->valstr.gTime) = 0;
    }
}

/**
  * @Descrbiption: 失压跳闸复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void lossTrip_rest(ComProSts *comProSts,LossTripSts *lossTripSts)
{
    if(*(lossTripSts->valstr.resetflag)&(1<<MAINPRO_LOSSTRIP))//复位
    {
        *(lossTripSts->valstr.resetflag) &= ~(1<<MAINPRO_LOSSTRIP);
        lossTripSts->valstr.flag = 0;
        *(lossTripSts->valstr.gTime) = 0;
        addSOE(comProSts,&comProSts->yx.lossTrip,OFF);
        if(*(lossTripSts->valstr.resetflag)&SETRESETFLAGMAN)
        {}
    }
}

/**
  * @Description: 得电合闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void getVolClose_ctrl(ComProSts *comProSts,GetVolCloseSts *getVolCloseSts)
{
    if((*(comProSts->yx.switchOpen.value)==ON)&&(*(comProSts->yx.switchClose.value)==OFF))//分位
    {
        if(getVolCloseSts->valstr.flag&GETVOLCLOSESTA2)//动作后合闸//复位状态量
        {
            getVolCloseSts->valstr.flag = 0;
            *(getVolCloseSts->valstr.gTime) = 0;
            addSOE(comProSts,&comProSts->yx.getClossing,OFF);
        }

        if((((*(getVolCloseSts->parastr.pSwitchUab)==SWITCH_ON)&&(*(getVolCloseSts->valstr.stateJudgeflag)&STATESINGLEVOLTAGEUAB)&&(*(comProSts->yx.remainClockClossingUcb.value)==OFF))||
            ((*(getVolCloseSts->parastr.pSwitchUcb)==SWITCH_ON)&&(*(getVolCloseSts->valstr.stateJudgeflag)&STATESINGLEVOLTAGEUCB)&&(*(comProSts->yx.remainClockClossingUab.value)==OFF)))&&
            (*(comProSts->yx.closingLocked.value)==OFF)&&(*(getVolCloseSts->valstr.stateJudgeflag)&STATESINGLEVOLTAGEGET)&&(!(getVolCloseSts->valstr.flag&GETVOLCLOSESTA1)))//单侧有压
        {
            if(!(*(getVolCloseSts->valstr.gTime)&MAINPRO_ENTIMERS))
            {
                *(getVolCloseSts->valstr.gTime) = MAINPRO_ENTIMERS;//启动定时
            }
            if((*(getVolCloseSts->valstr.gTime)&MAINPRO_TITIMERS)>=(uint32_t)(*(getVolCloseSts->parastr.pXTime))*1000)
            {
                comProSts->closing(ADDR_LOGIC_ACT,DO_LOGIC_CLOSE);
                getVolCloseSts->valstr.flag |= GETVOLCLOSE|GETVOLCLOSESTA1|RESETFLAG;
                addSOE(comProSts,&comProSts->yx.getClossing,ON);
            } 
			 			 
        }
        else
        {
		  #ifdef GETPOWERLOSSVOLTAGELOCKCLOSE
			 if((*(getVolCloseSts->valstr.gTime)&MAINPRO_ENTIMERS) && (*(getVolCloseSts->valstr.stateJudgeflag)&STATENOVOLTAGE))
			 {
				 addSOE(comProSts,&comProSts->yx.closingLocked,ON);
				 getVolCloseSts->valstr.flag |= GETVOLCLOSESTA3;
			 }
		  #endif 			 
            *(getVolCloseSts->valstr.gTime) = 0;
        }  				        
    }
    else if(getVolCloseSts->valstr.flag&GETVOLCLOSESTA1)//分位//过流后
    {
        getVolCloseSts->valstr.flag &= (~GETVOLCLOSESTA1);
        getVolCloseSts->valstr.flag |= GETVOLCLOSESTA2;
    }
	
  #ifdef GETPOWERLOSSVOLTAGELOCKCLOSE
	if((*(comProSts->yx.switchOpen.value)==OFF)&&(*(comProSts->yx.switchClose.value)==ON))//合位
	{
		if(getVolCloseSts->valstr.flag&GETVOLCLOSESTA3)
		{
			addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
			getVolCloseSts->valstr.flag &= (~GETVOLCLOSESTA3);
		}
	}
  #endif
}

/**
  * @Description: 得电合闸复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void getVolClose_rest(ComProSts *comProSts,GetVolCloseSts *getVolCloseSts)
{
    if(*(getVolCloseSts->valstr.resetflag)&(1<<MAINPRO_GETVOLCLOSE))//复位
    {
        *(getVolCloseSts->valstr.resetflag) &= ~(1<<MAINPRO_GETVOLCLOSE);
        getVolCloseSts->valstr.flag = 0;
        *(getVolCloseSts->valstr.gTime) = 0;
        addSOE(comProSts,&comProSts->yx.getClossing,OFF);
	  #ifdef GETPOWERLOSSVOLTAGELOCKCLOSE
		addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
	  #endif
        if(*(getVolCloseSts->valstr.resetflag)&SETRESETFLAGMAN)
        {}
    }
}

/**
  * @Description: 分合闸闭锁.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void openCloseLocking_ctrl(ComProSts *comProSts,OpenCloseLockingSts *openCloseLockingSts)
{
    if((*(comProSts->yx.switchOpen.value)==ON)&&(*(comProSts->yx.switchClose.value)==OFF))//分位
    {
		if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA10)//由合到分
		{
			addSOE(comProSts,&comProSts->yx.openingLocked,OFF);
            *(openCloseLockingSts->valstr.gTime) = 0;
			openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA10;
			openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA2;
            openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA7;
		}
		
        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA8)//手动分闸
        {
            if(!((*(openCloseLockingSts->valstr.lossTripflag)&LOSSTRIP)||\
                (*(comProSts->yx.protectionAct.value)==ON)))//手动分闸或摇杆分闸闭锁合闸
            {
                addSOE(comProSts,&comProSts->yx.closingLocked,ON);
                openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA8;
            }
        }
        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA1)//刚合闸标志
        {
            openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA1;
        }
        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA5)//分闸闭锁计时标志
        {
            openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA5;
            addSOE(comProSts,&comProSts->yx.openingLocked,OFF);
        }
        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA4)//分闸标志
        {
            openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA4;
            openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGCLOSELOCK;
			openCloseLockingSts->valstr.flag &= ~OVERCURRENTCLOSELOCKINGCLOSELOCK;
            openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA6;
        }
    }
    else if((*(comProSts->yx.switchOpen.value)==OFF)&&(*(comProSts->yx.switchClose.value)==ON))//合位
    {
		if(!(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA10))//由分到合
		{
			addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
			openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA10;
		}
		
        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA6)//分闸后复位标志
        {
            openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA6;
            addSOE(comProSts,&comProSts->yx.failAfterClosing,OFF);
            addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
            addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        }
        
        openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA8;

        if(!(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA1))//刚合闸后
        {
            if(!(*(openCloseLockingSts->valstr.gTime)&MAINPRO_ENTIMERS))
            {
                *(openCloseLockingSts->valstr.gTime) = MAINPRO_ENTIMERS;//启动定时
            }
            if((*(openCloseLockingSts->valstr.gTime)&MAINPRO_TITIMERS)<(uint32_t)(*(openCloseLockingSts->parastr.pYTime))*1000)
            {
                //Y时间内 
				if(comProSts->WorkMode == TYPE_WORKMODE_VOLCUR)
				{
                    if(((*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCURNOSW)&&
                        (*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCURI0NOSW))||
                    (*(openCloseLockingSts->parastr.pCloseLockCurSwitch) == SWITCH_OFF))//电压时间型                    
                    {
                        if(*(openCloseLockingSts->valstr.stateJudgeflag)&STATENOVOLTAGE)//无压
                        {
                            openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA1;
                            openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA4;
                            openCloseLockingSts->valstr.flag |= RESETFLAG;
                            addSOE(comProSts,&comProSts->yx.failAfterClosing,ON);
                            if((*(openCloseLockingSts->valstr.getVolCloseflag)&JUDGRFLAG)||\
                              (*(openCloseLockingSts->valstr.singleLossCloseflag)&JUDGRFLAG))//得电合闸或失压合闸闭锁
                            {
                                openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGCLOSELOCK;
                            }
                            *(openCloseLockingSts->valstr.gTime) = 0;
                        }
                    }
                    else if(*(openCloseLockingSts->parastr.pCloseLockCurSwitch) == SWITCH_ON)//电压电流型
                    {
                        if((*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCUR)||\
							(*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCURI0)||\
							(*(openCloseLockingSts->valstr.iACCflag)&JUDGRFLAG))
                        {
                            openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA7;
                        }
                        //过流相序过流
                        if((*(openCloseLockingSts->valstr.stateJudgeflag)&STATENOVOLTAGE)||(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA7))
                        {
//                            if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA7)
                            {
                                addSOE(comProSts,&comProSts->yx.failAfterClosing,ON);
                                if((*(openCloseLockingSts->valstr.getVolCloseflag)&JUDGRFLAG)||\
                                  (*(openCloseLockingSts->valstr.singleLossCloseflag)&JUDGRFLAG))//得电合闸或失压合闸闭锁
                                {
                                    openCloseLockingSts->valstr.flag |= OVERCURRENTCLOSELOCKINGCLOSELOCK;
                                }
                            }
                            openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA1;
                            openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA2;
                            openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA4;
                            openCloseLockingSts->valstr.flag |= RESETFLAG;
                            *(openCloseLockingSts->valstr.gTime) = 0;
                        }
                    }
				}	
            }
            else //Y时间外
            {
				if(!(openCloseLockingSts->valstr.flag & OPENCLOSELOCKINGSTA2))
				{
					openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA3;
				}
                openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA1;
                *(openCloseLockingSts->valstr.gTime) = 0;
            }
        }
		if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA3)//Y时间外
		{
			openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA3;
            if((*(openCloseLockingSts->parastr.pOpenLockSwitch) == SWITCH_ON)&&(*(openCloseLockingSts->parastr.pOpenLockTime) != 0))
            {
                openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA5;
                if(!((*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCUR)||(*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCURI0)
					||(*(openCloseLockingSts->valstr.iACCflag)&JUDGRFLAG)))
                {
                    addSOE(comProSts,&comProSts->yx.openingLocked,ON);
                }
            }
		}

        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA5)//分闸闭锁计时
        {
            if(!(*(openCloseLockingSts->valstr.gTime)&MAINPRO_ENTIMERS))
            {
                *(openCloseLockingSts->valstr.gTime) = MAINPRO_ENTIMERS;//启动定时
            }
            if((*(openCloseLockingSts->valstr.gTime)&MAINPRO_TITIMERS)>=(uint32_t)(*(openCloseLockingSts->parastr.pOpenLockTime))*1000)
            {
                openCloseLockingSts->valstr.flag = OPENCLOSELOCKINGSTA1;//非刚合闸状态
                *(openCloseLockingSts->valstr.gTime) = 0;
                addSOE(comProSts,&comProSts->yx.openingLocked,OFF);
            }
        }
    }
}

/**
  * @Description: 分合闸闭锁复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void openCloseLocking_rest(ComProSts *comProSts,OpenCloseLockingSts *openCloseLockingSts)
{
    if(*(openCloseLockingSts->valstr.resetflag)&(1<<MAINPRO_OPENCLOSELOCK))//复位
    {
        *(openCloseLockingSts->valstr.resetflag) &= ~(1<<MAINPRO_OPENCLOSELOCK);
        openCloseLockingSts->valstr.flag = OPENCLOSELOCKINGSTA1;//非刚合闸状态
		openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA2;
        *(openCloseLockingSts->valstr.gTime) = 0;
        addSOE(comProSts,&comProSts->yx.failAfterClosing,OFF);
        addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        if(*(openCloseLockingSts->valstr.resetflag)&SETRESETFLAGMAN)
        {
            addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
            addSOE(comProSts,&comProSts->yx.openingLocked,OFF);
        }
    }
}

/**
  * @Description: 残压闭锁.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void fewVolLock_ctrl(ComProSts *comProSts,FewVolLockSts *fewVolLockSts)
{
    if((*(comProSts->yx.switchOpen.value)==ON)&&(*(comProSts->yx.switchClose.value)==OFF))//分位
    {
        if(*(fewVolLockSts->parastr.pSwitch) == SWITCH_ON)
        {
            //Uab侧
            if((!(fewVolLockSts->valstr.flag&FEWVOLLOCKUABSTA1))&&\
                    (*(comProSts->yc.Uab)<*(fewVolLockSts->parastr.pValue)))//小于残压
            {
                fewVolLockSts->valstr.flag |= FEWVOLLOCKUABSTA1;
            }

            if((!(fewVolLockSts->valstr.flag&FEWVOLLOCKUABSTA3))&&(fewVolLockSts->valstr.flag&FEWVOLLOCKUABSTA1)&&\
                    ((*(comProSts->yc.Uab)>*(fewVolLockSts->parastr.pValue))))//检测到残压
            {
                fewVolLockSts->valstr.flag |= FEWVOLLOCKUABSTA3;
            }
            if((!(fewVolLockSts->valstr.flag&(FEWVOLLOCKUABSTA2|FEWVOLLOCKUABSTA4)))&&(fewVolLockSts->valstr.flag&FEWVOLLOCKUABSTA3))
            {
                if(!(*(fewVolLockSts->valstr.gUabTime)&MAINPRO_ENTIMERS))
                {
                    *(fewVolLockSts->valstr.gUabTime) = MAINPRO_ENTIMERS;//启动定时
                }
                //分断X时间，联络XL时间
                if(((*(fewVolLockSts->parastr.pTime) != 0)&&((*(fewVolLockSts->valstr.gUabTime)&MAINPRO_TITIMERS)<(uint32_t)(*(fewVolLockSts->parastr.pTime))*1000))||
                    ((*(fewVolLockSts->parastr.pTime) == 0)&&
                    ((((*(comProSts->yx.modBreakHardStrap.value)==ON)&&(*(comProSts->yx.modBreakSoftStrap.value)==ON))&&((*(fewVolLockSts->valstr.gUabTime)&MAINPRO_TITIMERS)<(uint32_t)(*(fewVolLockSts->parastr.pXTime))*1000))||\
                        (((*(comProSts->yx.modContactHardStrap.value)==ON)&&(*(comProSts->yx.modContactSoftStrap.value)==ON))&&((*(fewVolLockSts->valstr.gUabTime)&MAINPRO_TITIMERS)<(uint32_t)(*(fewVolLockSts->parastr.pXLTime))*1000)))))
                {
                    //时间内
                    if(*(comProSts->yc.Uab)<*(fewVolLockSts->parastr.pValue))//小于残压
                    {
                        fewVolLockSts->valstr.flag |= FEWVOLLOCKUABSTA4;
                        fewVolLockSts->valstr.flag |= RESETFLAG;
                        addSOE(comProSts,&comProSts->yx.remainClockClossingUab,ON);
                        //addSOE(comProSts,&comProSts->yx.closingLocked,ON);
                        *(fewVolLockSts->valstr.gUabTime) = 0;
                    }
                }
                else
                {
                    //时间外
                    fewVolLockSts->valstr.flag |= FEWVOLLOCKUABSTA2;
                    *(fewVolLockSts->valstr.gUabTime) = 0;
                }
            }
            if((fewVolLockSts->valstr.flag&FEWVOLLOCKUABSTA2)&&\
                    (*(comProSts->yc.Uab)<*(fewVolLockSts->parastr.pValue)))//无压复位
            {
                fewVolLockSts->valstr.flag &= ~(FEWVOLLOCKUABSTA1|FEWVOLLOCKUABSTA2|FEWVOLLOCKUABSTA3|FEWVOLLOCKUABSTA4);
                *(fewVolLockSts->valstr.gUabTime) = 0;
            }

            //Ucb侧
            if((!(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA1))&&\
                    (*(comProSts->yc.Ucb)<*(fewVolLockSts->parastr.pValue)))//小于残压
            {
                fewVolLockSts->valstr.flag |= FEWVOLLOCKUCBSTA1;
            }

            if((!(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA3))&&(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA1)&&\
                    ((*(comProSts->yc.Ucb)>*(fewVolLockSts->parastr.pValue))))//检测到残压
            {
                fewVolLockSts->valstr.flag |= FEWVOLLOCKUCBSTA3;
            }

            if((!(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA4))&&(!(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA2))&&(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA3))
            {
                if(!(*(fewVolLockSts->valstr.gUcbTime)&MAINPRO_ENTIMERS))
                {
                    *(fewVolLockSts->valstr.gUcbTime) = MAINPRO_ENTIMERS;//启动定时
                }
                //分断X时间，联络XL时间
                if(((*(fewVolLockSts->parastr.pTime) != 0)&&((*(fewVolLockSts->valstr.gUcbTime)&MAINPRO_TITIMERS)<(uint32_t)(*(fewVolLockSts->parastr.pTime))*1000))||
                    ((*(fewVolLockSts->parastr.pTime) == 0)&&
                    ((((*(comProSts->yx.modBreakHardStrap.value)==ON)&&(*(comProSts->yx.modBreakSoftStrap.value)==ON))&&((*(fewVolLockSts->valstr.gUabTime)&MAINPRO_TITIMERS)<(uint32_t)(*(fewVolLockSts->parastr.pXTime))*1000))||\
                        (((*(comProSts->yx.modContactHardStrap.value)==ON)&&(*(comProSts->yx.modContactSoftStrap.value)==ON))&&((*(fewVolLockSts->valstr.gUabTime)&MAINPRO_TITIMERS)<(uint32_t)(*(fewVolLockSts->parastr.pXLTime))*1000)))))
                {
                    //时间内
                    if(*(comProSts->yc.Ucb)<*(fewVolLockSts->parastr.pValue))//小于残压
                    {
                        fewVolLockSts->valstr.flag |= FEWVOLLOCKUCBSTA4;
                        fewVolLockSts->valstr.flag |= RESETFLAG;
                        addSOE(comProSts,&comProSts->yx.remainClockClossingUcb,ON);
                        //addSOE(comProSts,&comProSts->yx.closingLocked,ON);
                        *(fewVolLockSts->valstr.gUcbTime) = 0;
                    }
                }
                else
                {
                    //时间外
                    fewVolLockSts->valstr.flag |= FEWVOLLOCKUCBSTA2;
                    *(fewVolLockSts->valstr.gUcbTime) = 0;
                }
            }
            if((fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA2)&&\
                    (*(comProSts->yc.Ucb)<*(fewVolLockSts->parastr.pValue)))//无压复位
            {
                fewVolLockSts->valstr.flag &= ~(FEWVOLLOCKUCBSTA1|FEWVOLLOCKUCBSTA2|FEWVOLLOCKUCBSTA3|FEWVOLLOCKUCBSTA4);
                *(fewVolLockSts->valstr.gUcbTime) = 0;
            }
        }
    }
    else
    {
        fewVolLockSts->valstr.flag = 0;
        *(fewVolLockSts->valstr.gUabTime) = 0;
        *(fewVolLockSts->valstr.gUcbTime) = 0;
        addSOE(comProSts,&comProSts->yx.remainClockClossingUab,OFF);
		addSOE(comProSts,&comProSts->yx.remainClockClossingUcb,OFF);
    }
}

/**
  * @Description: 残压闭锁复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void fewVolLock_rest(ComProSts *comProSts,FewVolLockSts *fewVolLockSts)
{
    if(*(fewVolLockSts->valstr.resetflag)&(1<<MAINPRO_FEWVOLLOCK))//复位
    {
        *(fewVolLockSts->valstr.resetflag) &= ~(1<<MAINPRO_FEWVOLLOCK);
        fewVolLockSts->valstr.flag = 0;
        *(fewVolLockSts->valstr.gUabTime) = 0;
        *(fewVolLockSts->valstr.gUcbTime) = 0;
        addSOE(comProSts,&comProSts->yx.remainClockClossingUab,OFF);
		addSOE(comProSts,&comProSts->yx.remainClockClossingUcb,OFF);
        if(*(fewVolLockSts->valstr.resetflag)&SETRESETFLAGMAN)
        {
            addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
        }
    }
}

/**
  * @Description: 双侧有压禁止合闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void doubleVol_ctrl(ComProSts *comProSts,DoubleVolSts *doubleVolSts)
{
    if((*(comProSts->yx.switchOpen.value)==ON)&&(*(comProSts->yx.switchClose.value)==OFF))//分位
    {
        if((*(doubleVolSts->valstr.stateJudgeflag)&STATEDOUBLEVOLTAGE)&&(*(doubleVolSts->parastr.pSwitch) == SWITCH_ON))
        {
			if(!(doubleVolSts->valstr.flag&DOUBLEVOL2))
			{
				if(*(comProSts->yx.closingLocked.value)==ON)
				{
					doubleVolSts->valstr.flag |= DOUBLEVOL1;
				}
				else
				{
					doubleVolSts->valstr.flag &= ~DOUBLEVOL1;
				}
				doubleVolSts->valstr.flag |= DOUBLEVOL2;		//标志已存
			}
            addSOE(comProSts,&comProSts->yx.doubleProhibitClossing,ON);
            addSOE(comProSts,&comProSts->yx.closingLocked,ON);
            doubleVolSts->valstr.flag |= DOUBLEVOLSTA1;
        }
        else if(doubleVolSts->valstr.flag&DOUBLEVOLSTA1)
        {
			if(!(doubleVolSts->valstr.flag&DOUBLEVOL1))
			{
				addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
			}
            doubleVolSts->valstr.flag &= (DOUBLEVOL1|DOUBLEVOL2);
			addSOE(comProSts,&comProSts->yx.doubleProhibitClossing,OFF);
        }
    }
    else if((*(comProSts->yx.switchOpen.value)==OFF)&&(*(comProSts->yx.switchClose.value)==ON))//合位
    {
        if(doubleVolSts->valstr.flag&DOUBLEVOLSTA1)
        {
            addSOE(comProSts,&comProSts->yx.doubleProhibitClossing,OFF);
            addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
        }
		doubleVolSts->valstr.flag = 0;
    }
}

/**
  * @Description: 双侧有压禁止合闸复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void doubleVol_rest(ComProSts *comProSts,DoubleVolSts *doubleVolSts)
{
    if(*(doubleVolSts->valstr.resetflag)&(1<<MAINPRO_DOUBLEVOL))//复位
    {
        *(doubleVolSts->valstr.resetflag) &= ~(1<<MAINPRO_DOUBLEVOL);
        doubleVolSts->valstr.flag = 0;
        addSOE(comProSts,&comProSts->yx.doubleProhibitClossing,OFF);        
        if(*(doubleVolSts->valstr.resetflag)&SETRESETFLAGMAN)
        {
            addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
        }
    }
}

/**
  * @Description: 单侧失压合闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void SingleLossClose_ctrl(ComProSts *comProSts,SingleLossCloseSts *singleLossCloseSts)
{

    if(((*(singleLossCloseSts->parastr.pSwitch) == SWITCH_ON)||(*(singleLossCloseSts->parastr.pSwitch) == SWITCH_ALARM))&&\
            ((*(comProSts->yx.switchClose.value)==OFF)&&(*(comProSts->yx.switchOpen.value)==ON)))//分位
    {
        if(singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA4)
        {
            singleLossCloseSts->valstr.flag = 0;
        }
        if(((!(singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA1))||(singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA2))&&\
                (*(singleLossCloseSts->valstr.stateJudgeflag)&STATEDOUBLEVOLTAGE))
        {
            if(singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA2)//复位双侧有压后单侧有压
            {
                singleLossCloseSts->valstr.flag = 0;
                *(singleLossCloseSts->valstr.gTime) = 0;
            }
            if(!(*(singleLossCloseSts->valstr.gTime)&MAINPRO_ENTIMERS))
            {
                *(singleLossCloseSts->valstr.gTime) = MAINPRO_ENTIMERS;//启动定时
            }
            if((*(singleLossCloseSts->valstr.gTime)&MAINPRO_TITIMERS)>SINGELELOSSCLOSETIME)
            {
                singleLossCloseSts->valstr.flag |= SINGELELOSSCLOSESTA1;//双侧有压固有时间以上
                *(singleLossCloseSts->valstr.gTime) = 0;
				addSOE(comProSts,&comProSts->yx.remainClockClossingUab,OFF);
				addSOE(comProSts,&comProSts->yx.remainClockClossingUcb,OFF);
            }
        }
        else if((singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA1)&&(*(comProSts->yx.closingLocked.value)==OFF))
        {
            if(*(singleLossCloseSts->valstr.stateJudgeflag)&STATESINGLEVOLTAGELOSS)
            {
                singleLossCloseSts->valstr.flag |= SINGELELOSSCLOSESTA2;//双侧有压后单侧有压
                if(!(*(singleLossCloseSts->valstr.gTime)&MAINPRO_ENTIMERS))
                {
                    *(singleLossCloseSts->valstr.gTime) = MAINPRO_ENTIMERS;//启动定时
                }
                if((*(singleLossCloseSts->valstr.gTime)&MAINPRO_TITIMERS)>(uint32_t)(*(singleLossCloseSts->parastr.pTime))*1000)
                {
                    singleLossCloseSts->valstr.flag |= SINGELELOSSCLOSE|SINGELELOSSCLOSESTA3|RESETFLAG;
                    if(*(singleLossCloseSts->parastr.pSwitch) == SWITCH_ON)
                    {
                        comProSts->closing(ADDR_LOGIC_ACT,DO_LOGIC_CLOSE);
                    }
                    addSOE(comProSts,&comProSts->yx.singleLossClosing,ON);
                }
            }
            else if(!(*(singleLossCloseSts->valstr.stateJudgeflag)&STATEDOUBLEVOLTAGE))
            {
                singleLossCloseSts->valstr.flag = 0;
                *(singleLossCloseSts->valstr.gTime) = 0;
            }
        }
    }
    else
    {
        if(singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA3)
        {
            singleLossCloseSts->valstr.flag |= SINGELELOSSCLOSESTA4;
        }
        else
        {
            singleLossCloseSts->valstr.flag = 0;
            *(singleLossCloseSts->valstr.gTime) = 0;
            addSOE(comProSts,&comProSts->yx.singleLossClosing,OFF);
        }
    }
}

/**
  * @Description: 单侧失压合闸复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void SingleLossClose_rest(ComProSts *comProSts,SingleLossCloseSts *singleLossCloseSts)
{
    if (*(singleLossCloseSts->valstr.resetflag)&(1<<MAINPRO_SINGELELOSSCLOSE))//复位
    {
        *(singleLossCloseSts->valstr.resetflag) &= ~(1<<MAINPRO_SINGELELOSSCLOSE);
        singleLossCloseSts->valstr.flag = 0;
        *(singleLossCloseSts->valstr.gTime) = 0;
        addSOE(comProSts,&comProSts->yx.singleLossClosing,OFF);
        if(*(singleLossCloseSts->valstr.resetflag)&SETRESETFLAGMAN)
        {}
    }
}

/**
  * @brief : iACC protect.
  * @param : [comProSts]
	* @param : [iACCSts]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void iACC_ctrl(ComProSts *comProSts, IACCSts *iACCSts)
{
    if ((*(comProSts->yx.switchClose.value) == ON) && (*(comProSts->yx.switchOpen.value) == OFF))//合位
    {
        if(iACCSts->valstr.flag&IACCCURSTA2)//过流后合闸//复位状态量
        {
            iACCSts->valstr.flag = IACCCURSTA4;
            addSOE(comProSts,&comProSts->yx.overcurrentAccIa,OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentAccIb,OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentAccIc,OFF);
            addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
//          addSOE(comProSts,&comProSts->yx.recloseClock,OFF);					
            addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
            addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
			addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentAccI0,OFF);
			addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentAccU0,OFF);
        }
		if(iACCSts->valstr.flag&IACCCURSTA4)
		{
			if(!(*(iACCSts->valstr.gTimeEnIACC)&MAINPRO_ENTIMERS))
			{
				*(iACCSts->valstr.gTimeEnIACC) = MAINPRO_ENTIMERS;//启动定时
			}
			if((*(iACCSts->valstr.gTimeEnIACC)&MAINPRO_TITIMERS) <= IACCTIME)
			{
				if(((*(iACCSts->parastr.pSwitchIACCI)==SWITCH_ON)&&\
					((((*(iACCSts->valstr.recloseflag)&JUDGRFLAG)||(*(iACCSts->valstr.recloseI0flag)&JUDGRFLAG))&&(comProSts->WorkMode == TYPE_WORKMODE_COMMON))||\
					((comProSts->WorkMode == TYPE_WORKMODE_VOLCUR)&&(*(iACCSts->valstr.stateJudgeflag)&STATEUNINRUSH)))&&(!(iACCSts->valstr.flag&IACCCURSTA1)))&&\
					(!(*(iACCSts->valstr.overcurflag)&JUDGRFLAG))&&(!(*(iACCSts->valstr.overcurI0flag)&JUDGRFLAG))&&\
					((*(comProSts->yc.Ia)>*(iACCSts->parastr.pValueIACCI))||\
					 (*(comProSts->yc.Ib)>*(iACCSts->parastr.pValueIACCI))||\
					 (*(comProSts->yc.Ic)>*(iACCSts->parastr.pValueIACCI))))//过流
				{
					if(!(*(iACCSts->valstr.gTimeIACCI)&MAINPRO_ENTIMERS))
					{
						*(iACCSts->valstr.gTimeIACCI) = MAINPRO_ENTIMERS;//启动定时
					}
					if((*(iACCSts->valstr.gTimeIACCI)&MAINPRO_TITIMERS)>=(uint32_t)(*(iACCSts->parastr.pTimeIACCI)*1000))
					{
						iACCSts->valstr.flag |= IACCCUR|IACCCURSTA1|IACCCURI;						
						iACCSts->valstr.flag |= RESETFLAG;
						if(*(comProSts->yc.Ia)>*(iACCSts->parastr.pValueIACCI))
						{
							addSOE(comProSts,&comProSts->yx.overcurrentAccIa,ON);
						}
						if(*(comProSts->yc.Ib)>*(iACCSts->parastr.pValueIACCI))
						{
							addSOE(comProSts,&comProSts->yx.overcurrentAccIb,ON);
						}
						if(*(comProSts->yc.Ic)>*(iACCSts->parastr.pValueIACCI))
						{
							addSOE(comProSts,&comProSts->yx.overcurrentAccIc,ON);
						}
						addSOE(comProSts,&comProSts->yx.shortCircuitFault,ON);
					}
				}
				else//不过流
				{
					*(iACCSts->valstr.gTimeIACCI) = 0;
				}
                
				if(((*(iACCSts->parastr.pSwitchIACCI0)==SWITCH_ON)&&\
                    ((((*(iACCSts->valstr.recloseflag)&JUDGRFLAG)||(*(iACCSts->valstr.recloseI0flag)&JUDGRFLAG))&&(comProSts->WorkMode == TYPE_WORKMODE_COMMON))||(comProSts->WorkMode == TYPE_WORKMODE_VOLCUR))&&\
					(!(iACCSts->valstr.flag&IACCCURSTA1)))&&\
					(!(*(iACCSts->valstr.overcurflag)&JUDGRFLAG))&&(!(*(iACCSts->valstr.overcurI0flag)&JUDGRFLAG))&&\
					(*(comProSts->yc.I0)>*(iACCSts->parastr.pValueIACCI0)))//过流
				{
					if(!(*(iACCSts->valstr.gTimeIACCI0)&MAINPRO_ENTIMERS))
					{
						*(iACCSts->valstr.gTimeIACCI0) = MAINPRO_ENTIMERS;//启动定时
					}
					if((*(iACCSts->valstr.gTimeIACCI0)&MAINPRO_TITIMERS)>=(uint32_t)(*(iACCSts->parastr.pTimeIACCI0)*1000))
					{
						iACCSts->valstr.flag |= IACCCUR|IACCCURSTA1|IACCCURI0;						
						iACCSts->valstr.flag |= RESETFLAG;
					    addSOE(comProSts,&comProSts->yx.overcurrentAccI0,ON);
						addSOE(comProSts,&comProSts->yx.earthingFault,ON);
					}
				}
				else//不过流
				{
					*(iACCSts->valstr.gTimeIACCI0) = 0;
				}
 
				if(((*(iACCSts->parastr.pSwitchIACCU0)==SWITCH_ON)&&\
                    ((((*(iACCSts->valstr.recloseflag)&JUDGRFLAG)||(*(iACCSts->valstr.recloseI0flag)&JUDGRFLAG))&&(comProSts->WorkMode == TYPE_WORKMODE_COMMON))||(comProSts->WorkMode == TYPE_WORKMODE_VOLCUR))&&\
					(!(iACCSts->valstr.flag&IACCCURSTA1)))&&\
					(!(*(iACCSts->valstr.overcurflag)&JUDGRFLAG))&&(!(*(iACCSts->valstr.overcurI0flag)&JUDGRFLAG))&&\
					(*(comProSts->yc.U0)>*(iACCSts->parastr.pValueIACCU0)))//过流
				{
					if(!(*(iACCSts->valstr.gTimeIACCU0)&MAINPRO_ENTIMERS))
					{
						*(iACCSts->valstr.gTimeIACCU0) = MAINPRO_ENTIMERS;//启动定时
					}
					if((*(iACCSts->valstr.gTimeIACCU0)&MAINPRO_TITIMERS)>=(uint32_t)(*(iACCSts->parastr.pTimeIACCU0)*1000))
					{
						iACCSts->valstr.flag |= IACCCUR|IACCCURSTA1|IACCCURU0;						
						iACCSts->valstr.flag |= RESETFLAG;
					    addSOE(comProSts,&comProSts->yx.overcurrentAccU0,ON);
						addSOE(comProSts,&comProSts->yx.earthingFault,ON);
					}
				}
				else//不过流
				{
					*(iACCSts->valstr.gTimeIACCU0) = 0;
				}
                
                if((iACCSts->valstr.flag&IACCCURSTA1)&&(!(iACCSts->valstr.flag&IACCCURSTA3)))
                {
                    iACCSts->valstr.flag |= IACCCURSTA3;
					if(g_TelesignalDB[g_TelesignalAddr.workmodeVolcur] == ON)	//电压电流型
					{
						if(*(comProSts->yx.swtichclass.value)==OFF)				//断路器
						{  
							comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
							addSOE(comProSts,&comProSts->yx.protectionAct,ON); 
							addSOE(comProSts,&comProSts->yx.closingLocked,ON);							
						}
						else if((*(comProSts->yx.swtichclass.value)==ON)&&(!(*(iACCSts->valstr.stateJudgeflag)&STATEUNBLOCKED))&&\
                        (*(comProSts->yx.openingLocked.value)==OFF))			//负荷开关
						{
							comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
							addSOE(comProSts,&comProSts->yx.protectionAct,ON); 
							addSOE(comProSts,&comProSts->yx.closingLocked,ON);
						}
					}
					else if(g_TelesignalDB[g_TelesignalAddr.workmodeCommon] == ON)//常规保护
					{
						comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
						addSOE(comProSts,&comProSts->yx.protectionAct,ON); 
						if(iACCSts->valstr.flag&IACCCURU0)
						{
							addSOE(comProSts,&comProSts->yx.recloseClock,ON);
						}
					} 
                }
			}
			else
			{
				iACCSts->valstr.flag &= ~IACCCURSTA4;
                *(iACCSts->valstr.gTimeEnIACC) = 0;
				*(iACCSts->valstr.gTimeIACCI) = 0;
				*(iACCSts->valstr.gTimeIACCI0) = 0;
				*(iACCSts->valstr.gTimeIACCU0) = 0;
			}
		}
        
    }
    else //分位
    {
		if(iACCSts->valstr.flag & IACCCURSTA1)
		{
			iACCSts->valstr.flag &= (~IACCCURSTA1);
			iACCSts->valstr.flag |= IACCCURSTA2;
		}
		iACCSts->valstr.flag |= IACCCURSTA4;
		*(iACCSts->valstr.gTimeEnIACC) = 0;
	    *(iACCSts->valstr.gTimeIACCI) = 0;
		*(iACCSts->valstr.gTimeIACCI0) = 0;
		*(iACCSts->valstr.gTimeIACCU0) = 0;
    }
}

/**
  * @brief:  iACC reset.
  * @param:  none
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void iACC_rest(ComProSts *comProSts,IACCSts *iACCSts)
{
    if(*(iACCSts->valstr.resetflag)&(1<<MAINPRO_IACC))//复位
    {
        *(iACCSts->valstr.resetflag) &= ~(1<<MAINPRO_IACC);
        iACCSts->valstr.flag = 0;
        *(iACCSts->valstr.gTimeEnIACC) = 0;
		*(iACCSts->valstr.gTimeIACCI) = 0;
		*(iACCSts->valstr.gTimeIACCI0) = 0;
		*(iACCSts->valstr.gTimeIACCU0) = 0;
        addSOE(comProSts,&comProSts->yx.overcurrentAccIa,OFF);
        addSOE(comProSts,&comProSts->yx.overcurrentAccIb,OFF);
        addSOE(comProSts,&comProSts->yx.overcurrentAccIc,OFF);
        addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
        addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
        addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
        addSOE(comProSts,&comProSts->yx.overcurrentAccI0,OFF);
        addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
        addSOE(comProSts,&comProSts->yx.overcurrentAccU0,OFF);
        if(*(iACCSts->valstr.resetflag)&SETRESETFLAGMAN)
        {}
    }
}

/**
  * @Description: 相间过流.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void overcur_ctrl(ComProSts *comProSts,OvercurSts *overcurSts)
{
    uint8_t i;
	const float factor = 0.95;
	
    if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位//无涌流抑制
    {
        if(overcurSts->valstr.flag&OVERCURSTA2)//过流后合闸//复位状态量
        {
            overcurSts->valstr.flag = 0;
            for(i=0; i<3; i++)
            {
                addSOE(comProSts,&comProSts->yx.overcurrentIa[i],OFF);
                addSOE(comProSts,&comProSts->yx.overcurrentIb[i],OFF);
                addSOE(comProSts,&comProSts->yx.overcurrentIc[i],OFF);
            }
            addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
            addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        }

        for(i=0; i<3; i++) //检测过流
        {
            if((((*(overcurSts->parastr.pSwitch[i])==SWITCH_ON)&&(!(overcurSts->valstr.flag&(OVERCUR1|OVERCUR2|OVERCUR3))))||\
				((*(overcurSts->parastr.pSwitch[i])==SWITCH_ALARM)&&(!(overcurSts->valstr.flag&(OVERCURALARM1|OVERCURALARM2|OVERCURALARM3)))))&&\
				(((*(overcurSts->valstr.recloseflag)&JUDGRFLAG)||(*(overcurSts->valstr.recloseI0flag)&JUDGRFLAG))||(*(overcurSts->valstr.stateJudgeflag)&STATEUNINRUSH))&&\
                    (!(*(overcurSts->valstr.iACCflag)&JUDGRFLAG))&&(!(*(overcurSts->valstr.overcurI0flag)&JUDGRFLAG))&&\
                    ((*(comProSts->yc.Ia)>*(overcurSts->parastr.pValue[i]))||\
                     (*(comProSts->yc.Ib)>*(overcurSts->parastr.pValue[i]))||\
                     (*(comProSts->yc.Ic)>*(overcurSts->parastr.pValue[i]))))//过流
            {

                if(!(*(overcurSts->valstr.gTime[i])&MAINPRO_ENTIMERS))
                {
                    *(overcurSts->valstr.gTime[i]) = MAINPRO_ENTIMERS;//启动定时
                }
                if((*(overcurSts->valstr.gTime[i])&MAINPRO_TITIMERS)>=(uint32_t)(*(overcurSts->parastr.pTime[i])*1000))
                {
					if(*(overcurSts->parastr.pSwitch[i])==SWITCH_ON)
					{
						overcurSts->valstr.flag |= (OVERCUR1<<i)|OVERCURSTA1;
						comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
						addSOE(comProSts,&comProSts->yx.protectionAct,ON);
						overcurSts->valstr.flag |= RESETFLAG;
					}
					else if(*(overcurSts->parastr.pSwitch[i])==SWITCH_ALARM)
					{
						overcurSts->valstr.flag |= (OVERCURALARM1<<i);
					}
                    
                    if(*(comProSts->yc.Ia)>*(overcurSts->parastr.pValue[i]))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentIa[i],ON);
                    }
                    if(*(comProSts->yc.Ib)>*(overcurSts->parastr.pValue[i]))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentIb[i],ON);
                    }
                    if(*(comProSts->yc.Ic)>*(overcurSts->parastr.pValue[i]))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentIc[i],ON);
                    }
                    addSOE(comProSts,&comProSts->yx.shortCircuitFault,ON);
                    
                }
            }
            else//不过流
            {
                *(overcurSts->valstr.gTime[i]) = 0;
				if((*(overcurSts->parastr.pSwitch[i])==SWITCH_ALARM)&&(!(overcurSts->valstr.flag&(OVERCUR1|OVERCUR2|OVERCUR3))))
                {
                    if(((*(comProSts->yc.Ia)<*(overcurSts->parastr.pValue[i])*factor)&&\
						(*(comProSts->yc.Ib)<*(overcurSts->parastr.pValue[i])*factor)&&\
						(*(comProSts->yc.Ic)<*(overcurSts->parastr.pValue[i])*factor))&&\
						(overcurSts->valstr.flag & (OVERCURALARM1|OVERCURALARM2|OVERCURALARM3)))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentIa[i],OFF);
						addSOE(comProSts,&comProSts->yx.overcurrentIb[i],OFF);
						addSOE(comProSts,&comProSts->yx.overcurrentIc[i],OFF);
						overcurSts->valstr.flag &= ~(OVERCURALARM1<<i);
						if(!(overcurSts->valstr.flag & (OVERCURALARM1|OVERCURALARM2|OVERCURALARM3)))
						{
							addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
						}                      
                    }                                   
                }
            }
        }
    }
    else //分位//过流后分闸
    {
		if(overcurSts->valstr.flag&OVERCURSTA1)
		{
			overcurSts->valstr.flag &= (~OVERCURSTA1);
			overcurSts->valstr.flag |= OVERCURSTA2;
		}
		*(overcurSts->valstr.gTime[0]) = 0;
        *(overcurSts->valstr.gTime[1]) = 0;
        *(overcurSts->valstr.gTime[2]) = 0;
    }
}

/**
  * @Description: 过流复归.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void overcur_rest(ComProSts *comProSts,OvercurSts *overcurSts)
{
    uint8_t i;

    if(*(overcurSts->valstr.resetflag)&(1<<MAINPRO_OVERCUR))//复位
    {
        *(overcurSts->valstr.resetflag) &= ~(1<<MAINPRO_OVERCUR);
        overcurSts->valstr.flag = 0;
        *(overcurSts->valstr.gTime[0]) = 0;
        *(overcurSts->valstr.gTime[1]) = 0;
        *(overcurSts->valstr.gTime[2]) = 0;
        for(i=0; i<3; i++)
        {
            addSOE(comProSts,&comProSts->yx.overcurrentIa[i],OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentIb[i],OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentIc[i],OFF);
        }
        addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
        addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        if(*(overcurSts->valstr.resetflag)&SETRESETFLAGMAN)
        {}
    }
}

/**
  * @Description: 零序过流.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void overcurI0_ctrl(ComProSts *comProSts,OvercurI0Sts *overcurI0Sts)
{
    const float factor = 0.95;
    uint8_t i;

    if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位//无涌流抑制
    {
        if(overcurI0Sts->valstr.flag&OVERCURI0STA2)//过流后合闸//复位状态量
        {
            overcurI0Sts->valstr.flag = 0;
            for(i=0; i<2; i++)
            {
                addSOE(comProSts,&comProSts->yx.overcurrentI0[i],OFF);
            }
            addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
            addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        }

        for(i=0; i<2; i++) //检测过流
        {		//（零序过流保护投入||零序报警）&&（重合闸||零序重合闸||无涌流抑制）&&无后加速&&无零序过流保护&&无零序过流标志&&零序过流
            if(((((*(overcurI0Sts->parastr.pSwitch[i])==SWITCH_ON)&&(!(overcurI0Sts->valstr.flag&(OVERCURI01|OVERCURI02))))||\
                ((*(overcurI0Sts->parastr.pSwitch[i])==SWITCH_ALARM)&&(!(overcurI0Sts->valstr.flag&(OVERCURI0STA3|OVERCURI0STA4)))))&&\
				(((*(overcurI0Sts->valstr.recloseflag)&JUDGRFLAG)||(*(overcurI0Sts->valstr.recloseI0flag)&JUDGRFLAG))||(*(overcurI0Sts->valstr.stateJudgeflag)&STATEUNINRUSH))&&\
                    (!(*(overcurI0Sts->valstr.iACCflag)&JUDGRFLAG))&&(!(*(overcurI0Sts->valstr.overcurflag)&JUDGRFLAG)))&&\
                    (*(comProSts->yc.I0)>*(overcurI0Sts->parastr.pValue[i])))//过流
            {
                if(!(*(overcurI0Sts->valstr.gTime[i])&MAINPRO_ENTIMERS))
                {
                    *(overcurI0Sts->valstr.gTime[i]) = MAINPRO_ENTIMERS;//启动定时
                }
                if((*(overcurI0Sts->valstr.gTime[i])&MAINPRO_TITIMERS)>=(uint32_t)(*(overcurI0Sts->parastr.pTime[i])*1000))
                { 
                    if(*(overcurI0Sts->parastr.pSwitch[i])==SWITCH_ON)
                    {
                        comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
                        addSOE(comProSts,&comProSts->yx.protectionAct,ON);
                        overcurI0Sts->valstr.flag |= (OVERCURI01<<i)|OVERCURI0STA1;
                        overcurI0Sts->valstr.flag |= RESETFLAG;
                    }
                    else if(*(overcurI0Sts->parastr.pSwitch[i])==SWITCH_ALARM)
                    {
                        overcurI0Sts->valstr.flag |= (OVERCURI0STA3<<i);
                    }
                    if(*(comProSts->yc.I0)>*(overcurI0Sts->parastr.pValue[i]))
                    {
                        //SOE                        
                        addSOE(comProSts,&comProSts->yx.overcurrentI0[i],ON);
                    }
                    addSOE(comProSts,&comProSts->yx.earthingFault,ON);
                }
            }
            else
            {
                *(overcurI0Sts->valstr.gTime[i]) = 0;
                if((*(overcurI0Sts->parastr.pSwitch[i])==SWITCH_ALARM)&&(!(overcurI0Sts->valstr.flag&(OVERCURI01|OVERCURI02))))
                {
                    if((*(comProSts->yc.I0)<*(overcurI0Sts->parastr.pValue[i])*factor)&&(overcurI0Sts->valstr.flag & (OVERCURI0STA3|OVERCURI0STA4)))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentI0[i],OFF);
                        overcurI0Sts->valstr.flag &= ~(OVERCURI0STA3<<i);
						if(!(overcurI0Sts->valstr.flag & (OVERCURI0STA3|OVERCURI0STA4)))
						{
							addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
						} 
                    }                                   
                }
            }
        }
    }
    else ////分闸//过流后分闸
    {
		if(overcurI0Sts->valstr.flag&OVERCURI0STA1)
		{
			overcurI0Sts->valstr.flag &= (~OVERCURI0STA1);
			overcurI0Sts->valstr.flag |= OVERCURI0STA2;
		}
		*(overcurI0Sts->valstr.gTime[0]) = 0;
		*(overcurI0Sts->valstr.gTime[1]) = 0;
    }
}

/**
  * @Description: 零序过流复归.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void overcurI0_rest(ComProSts *comProSts,OvercurI0Sts *overcurI0Sts)
{
    uint8_t i;

    if(*(overcurI0Sts->valstr.resetflag)&(1<<MAINPRO_OVERCURI0))//复位
    {
        *(overcurI0Sts->valstr.resetflag) &= ~(1<<MAINPRO_OVERCURI0);
        overcurI0Sts->valstr.flag = 0;
        *(overcurI0Sts->valstr.gTime[0]) = 0;
        *(overcurI0Sts->valstr.gTime[1]) = 0;
        for(i=0; i<2; i++)
        {
            addSOE(comProSts,&comProSts->yx.overcurrentI0[i],OFF);
        }
        addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
        addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        if(*(overcurI0Sts->valstr.resetflag)&SETRESETFLAGMAN)
        {}
    }
}

/**
  * @Description: 二次重合闸闭锁.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void secondaryRecloseLock_ctrl(ComProSts *comProSts,SecondaryRecloseLockSts *secondaryRecloseLockSts)
{
    if(((uint32_t)(*secondaryRecloseLockSts->parastr.pTime))&&\
            ((*(secondaryRecloseLockSts->valstr.recloseflag)&RECLOSE1)||\
             (*(secondaryRecloseLockSts->valstr.recloseI0flag)&RECLOSEI01)))//第一次重合闸状态有效
    {
        if((!(secondaryRecloseLockSts->valstr.flag&SENCONDARYRECLSELOCKSTA1))&&\
                (*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合闸//重合状态
        {
            if(!(*(secondaryRecloseLockSts->valstr.gTime)&MAINPRO_ENTIMERS))
            {
                *(secondaryRecloseLockSts->valstr.gTime) = MAINPRO_ENTIMERS;//启动定时
            }
            secondaryRecloseLockSts->valstr.flag |= SENCONDARYRECLSELOCKSTA1;
        }
        if((!(secondaryRecloseLockSts->valstr.flag&SENCONDARYRECLSELOCKSTA2))&&(secondaryRecloseLockSts->valstr.flag&SENCONDARYRECLSELOCKSTA1))
        {
            if((*(secondaryRecloseLockSts->valstr.gTime)&MAINPRO_TITIMERS)<=(uint32_t)(*(secondaryRecloseLockSts->parastr.pTime)*1000))
            {
                if(((*(comProSts->yx.switchClose.value) == OFF)&&(*(comProSts->yx.switchOpen.value) == ON))&&\
                        ((*(secondaryRecloseLockSts->valstr.overcurflag)&JUDGRFLAG)||(*(secondaryRecloseLockSts->valstr.overcurI0flag)&JUDGRFLAG)||\
                         (*(secondaryRecloseLockSts->valstr.iACCflag)&JUDGRFLAG)))//分闸//任一种过流
                {
                    secondaryRecloseLockSts->valstr.flag |= SENCONDARYRECLSELOCKSTA2;
                    secondaryRecloseLockSts->valstr.flag |= SENCONDARYRECLSELOCK;
                    secondaryRecloseLockSts->valstr.flag |= RESETFLAG;
                    //soe,闭锁
                    addSOE(comProSts,&comProSts->yx.clockSecondaryClose,ON);
                    addSOE(comProSts,&comProSts->yx.recloseClock,ON);
                }
            }
            else
            {
                *(secondaryRecloseLockSts->valstr.gTime) = 0;
                secondaryRecloseLockSts->valstr.flag |= SENCONDARYRECLSELOCKSTA2;
            }
        }
    }
    else
    {
        secondaryRecloseLockSts->valstr.flag = 0;
        *(secondaryRecloseLockSts->valstr.gTime) = 0;
//        addSOE(comProSts,&comProSts->yx.clockSecondaryClose,OFF);
    }
	if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位
	{
		if(secondaryRecloseLockSts->valstr.flag&SENCONDARYRECLSELOCK)
		{
			addSOE(comProSts,&comProSts->yx.recloseClock,OFF);	
			secondaryRecloseLockSts->valstr.flag = 0;
			*(secondaryRecloseLockSts->valstr.gTime) = 0;
			addSOE(comProSts,&comProSts->yx.clockSecondaryClose,OFF);
		}		
	}
}

/**
  * @Description: 二次重合闸闭锁复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void secondaryRecloseLock_rest(ComProSts *comProSts,SecondaryRecloseLockSts *secondaryRecloseLockSts)
{
    if(*(secondaryRecloseLockSts->valstr.resetflag)&(1<<MAINPRO_SENCONDARYRECLSELOCK))//复位
    {
        *(secondaryRecloseLockSts->valstr.resetflag) &= ~(1<<MAINPRO_SENCONDARYRECLSELOCK);
        secondaryRecloseLockSts->valstr.flag = 0;
        *(secondaryRecloseLockSts->valstr.gTime) = 0;
        addSOE(comProSts,&comProSts->yx.clockSecondaryClose,OFF);
        if(*(secondaryRecloseLockSts->valstr.resetflag)&SETRESETFLAGMAN)
        {
            addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
        }
    }
}

/**
  * @Description: 相间重合闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void reclose_ctrl(ComProSts *comProSts,RecloseSts *recloseSts)
{
	static uint8_t Times = 0;
    if((*(comProSts->yx.switchClose.value) == OFF)&&(*(comProSts->yx.switchOpen.value) == ON))//分位
    {
        if((*(comProSts->yx.recloseClock.value) == OFF)&&(!(recloseSts->valstr.flag&RECLOSESTA1)))//无闭锁合闸
        {
            if((*(recloseSts->valstr.iACCflag)&IACCCURI)||(*(recloseSts->valstr.overcurflag)&JUDGRFLAG))//过流导致分闸
            {
				if(*(recloseSts->parastr.pSwitch[0])==SWITCH_ON)		//一次重合投入
				{
					Times = 1;
					if(*(recloseSts->parastr.pSwitch[0])==SWITCH_ON)	//二次重合投入
					{
						Times = 2;
					}
				}
				else
				{
					Times = 0;
				}
                if(recloseSts->valstr.time < Times)//未达到重合次数
                {
                    if(!(*(recloseSts->valstr.gTime[0])&MAINPRO_ENTIMERS))
                    {
                        *(recloseSts->valstr.gTime[0]) = MAINPRO_ENTIMERS;//启动定时
                    }
                    if(((*(recloseSts->valstr.gTime[0])&MAINPRO_TITIMERS))>=(uint32_t)(*(recloseSts->parastr.pTime[recloseSts->valstr.time])*1000))
                    {
                        comProSts->closing(ADDR_LOGIC_ACT,DO_LOGIC_CLOSE);
                        recloseSts->valstr.flag = (1<<recloseSts->valstr.time)|RECLOSESTA1;
                        recloseSts->valstr.flag |= RESETFLAG;
                        *(recloseSts->valstr.gTime[0]) = 0;
                        //soe,重合闸
                        addSOE(comProSts,&comProSts->yx.recloseProtection[recloseSts->valstr.time],ON);
                        addSOE(comProSts,&comProSts->yx.recloseProtection[recloseSts->valstr.time],OFF);
                        addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
                        addSOE(comProSts,&comProSts->yx.recloseAct,ON);
                        recloseSts->valstr.time++;
                    }
                }
                else//达到重合闸次数
                {
                    //soe,闭锁重合闸
                    addSOE(comProSts,&comProSts->yx.recloseClock,ON);
                    recloseSts->valstr.flag |= RECLOSELOCK|RESETFLAG;
                }
            }
            else if(!(*(recloseSts->valstr.overcurI0flag)&JUDGRFLAG))//无零序过流//手动合闸
            {
                recloseSts->valstr.time = 0;//清除重合闸次数
                recloseSts->valstr.flag = 0;//清除重合闸状态
                addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
                addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
            }
        }
        if(*(recloseSts->valstr.secondaryRecloseLockflag)&JUDGRFLAG)//二次重合闸闭锁，清除重合闸标志
        {
            recloseSts->valstr.time = 0;
            recloseSts->valstr.flag &= ~(RECLOSE1|RECLOSE2|RECLOSE3|RECLOSE4);
        }
        if(*(comProSts->yx.recloseClock.value) == ON)
        {
            recloseSts->valstr.flag |= RECLOSELOCK;
        }
        *(recloseSts->valstr.gTime[1]) = 0;
    }
    else if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位
    {
        *(recloseSts->valstr.gTime[0]) = 0;
        if(recloseSts->valstr.flag&RECLOSESTA1)
        {
            recloseSts->valstr.flag &= (~RECLOSESTA1);
        }
		if(recloseSts->valstr.flag&RECLOSELOCK)
		{
			recloseSts->valstr.time = 0;//清除重合闸次数
			recloseSts->valstr.flag = 0;//清除重合闸状态
			addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
			addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
		}
        if((recloseSts->valstr.time != 0)&&(*(recloseSts->parastr.pResetTime) != 0))		//次数不为0						
        {
            if(!(*(recloseSts->valstr.gTime[1])&MAINPRO_ENTIMERS))
            {
                *(recloseSts->valstr.gTime[1]) = MAINPRO_ENTIMERS;//启动定时
            }
            if(((*(recloseSts->valstr.gTime[1])&MAINPRO_TITIMERS))>=(uint32_t)(*(recloseSts->parastr.pResetTime)*1000))
            {
                recloseSts->valstr.time = 0;
                recloseSts->valstr.flag &= ~(RECLOSE1|RECLOSE2|RECLOSE3|RECLOSE4);
                addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
            }
        }
        else
        {
            *(recloseSts->valstr.gTime[1]) = 0;
        }        
    }
}

/**
  * @Description: 重合闸复归.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void reclose_rest(ComProSts *comProSts,RecloseSts *recloseSts)
{
    uint8_t i;
    if(*(recloseSts->valstr.resetflag)&(1<<MAINPRO_RECLOSE))//自动、手动复位
    {
        *(recloseSts->valstr.resetflag) &= ~(1<<MAINPRO_RECLOSE);
		
		{
			recloseSts->valstr.time = 0;
			*(recloseSts->valstr.gTime[0]) = 0;
			recloseSts->valstr.flag = 0;
			for(i=0; i<4; i++)
			{
				addSOE(comProSts,&comProSts->yx.recloseProtection[recloseSts->valstr.time],OFF);
			}
			addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
            if(*(recloseSts->valstr.resetflag)&SETRESETFLAGMAN)
            {
                addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
            }
		}
    }
}

/**
  * @Description: 零序重合闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void recloseI0_ctrl(ComProSts *comProSts,RecloseI0Sts *recloseI0Sts)
{
    if((*(comProSts->yx.switchClose.value) == OFF)&&(*(comProSts->yx.switchOpen.value) == ON))//分位
    {
        if((*(comProSts->yx.recloseClock.value) == OFF)&&(!(recloseI0Sts->valstr.flag&RECLOSEI0STA1)))//无闭锁合闸
        {
            if((*(recloseI0Sts->valstr.iACCflag)&IACCCURI0)|| (*(recloseI0Sts->valstr.overcurI0flag)&JUDGRFLAG))//过流导致分闸
            {
                if(recloseI0Sts->valstr.time < (uint8_t)(*(recloseI0Sts->parastr.pTimes)))//未达到重合次数
                {
                    if(!(*(recloseI0Sts->valstr.gTime[0])&MAINPRO_ENTIMERS))
                    {
                        *(recloseI0Sts->valstr.gTime[0]) = MAINPRO_ENTIMERS;//启动定时
                    }
                    if(((*(recloseI0Sts->valstr.gTime[0])&MAINPRO_TITIMERS))>=((uint32_t)(*(recloseI0Sts->parastr.pTime[recloseI0Sts->valstr.time])*1000)))
                    {
                        comProSts->closing(ADDR_LOGIC_ACT,DO_LOGIC_CLOSE);
                        recloseI0Sts->valstr.flag = (1<<recloseI0Sts->valstr.time)|RECLOSEI0STA1;
                        recloseI0Sts->valstr.flag |= RESETFLAG;
                        *(recloseI0Sts->valstr.gTime[0]) = 0;
                        //soe,重合闸
                        addSOE(comProSts,&comProSts->yx.zeroRecloseProtection[recloseI0Sts->valstr.time],ON);
                        addSOE(comProSts,&comProSts->yx.zeroRecloseProtection[recloseI0Sts->valstr.time],OFF);
                        addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
                        addSOE(comProSts,&comProSts->yx.recloseAct,ON);
                        recloseI0Sts->valstr.time++;
                    }
                }
                else//达到重合闸次数
                {
                    //soe,闭锁重合闸
                    addSOE(comProSts,&comProSts->yx.recloseClock,ON);
                    recloseI0Sts->valstr.flag |= (RECLOSEI0LOCK|RESETFLAG);
                }
            }
            else if((!(*(recloseI0Sts->valstr.overcurflag)&JUDGRFLAG))&&(!(*(recloseI0Sts->valstr.iACCflag)&JUDGRFLAG)))//无相间过流//手动合闸
            {
                recloseI0Sts->valstr.time = 0;//清除重合闸次数
                recloseI0Sts->valstr.flag = 0;//清除重合闸状态
                addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
                addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
            }
        }
        if(*(recloseI0Sts->valstr.secondaryRecloseLockflag)&JUDGRFLAG)//二次重合闸闭锁，清除重合闸标志
        {
            recloseI0Sts->valstr.time = 0;
            recloseI0Sts->valstr.flag &= ~(RECLOSEI01|RECLOSEI02);
        }
        if(*(comProSts->yx.recloseClock.value) == ON)
        {
            recloseI0Sts->valstr.flag |= RECLOSEI0LOCK;
        }
        *(recloseI0Sts->valstr.gTime[1]) = 0;
    }
    else if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位
    {
        *(recloseI0Sts->valstr.gTime[0]) = 0;
        if(recloseI0Sts->valstr.flag&RECLOSEI0STA1)
        {
            recloseI0Sts->valstr.flag &= (~RECLOSEI0STA1);
        }
		if(recloseI0Sts->valstr.flag&RECLOSEI0LOCK)
		{
			recloseI0Sts->valstr.time = 0;//清除重合闸次数
			recloseI0Sts->valstr.flag = 0;//清除重合闸状态
			addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
			addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
		}
        if((recloseI0Sts->valstr.time != 0)&&(*(recloseI0Sts->parastr.pResetTime) != 0))		//次数不为0						
        {
            if(!(*(recloseI0Sts->valstr.gTime[1])&MAINPRO_ENTIMERS))
            {
                *(recloseI0Sts->valstr.gTime[1]) = MAINPRO_ENTIMERS;//启动定时
            }
            if(((*(recloseI0Sts->valstr.gTime[1])&MAINPRO_TITIMERS))>=(uint32_t)(*(recloseI0Sts->parastr.pResetTime)*1000))
            {
                recloseI0Sts->valstr.time = 0;
                recloseI0Sts->valstr.flag &= ~(RECLOSEI01|RECLOSEI02);
                addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
            }
        }
        else
        {
            *(recloseI0Sts->valstr.gTime[1]) = 0;
        }
    }
}

/**
  * @Description: 零序重合闸复归.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void recloseI0_rest(ComProSts *comProSts,RecloseI0Sts *recloseI0Sts)
{
    uint8_t i;

    if(*(recloseI0Sts->valstr.resetflag)&(1<<MAINPRO_RECLOSEI0))//手动复位
    {
        *(recloseI0Sts->valstr.resetflag) &= ~(1<<MAINPRO_RECLOSEI0);		
		recloseI0Sts->valstr.time = 0;
		recloseI0Sts->valstr.flag = 0;
		*(recloseI0Sts->valstr.gTime[0]) = 0;
		for(i=0; i<2; i++)
		{
			addSOE(comProSts,&comProSts->yx.zeroRecloseProtection[recloseI0Sts->valstr.time],OFF);
		}
		addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
        if(*(recloseI0Sts->valstr.resetflag)&SETRESETFLAGMAN)
        {
            addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
        }
    }
}

/**
  * @Description: 超上限跳闸
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void OverTelemetryUp_ctrl(ComProSts *comProSts,OverTelemetryUpSts *overTelemetryUp)
{
    if(*(overTelemetryUp->parastr.pSwitch) != SWITCH_OFF)
    {
        if (((**overTelemetryUp->parastr.telemetry0 > *overTelemetryUp->parastr.pValue)||
            (**overTelemetryUp->parastr.telemetry1 > *overTelemetryUp->parastr.pValue)||
            (**overTelemetryUp->parastr.telemetry2 > *overTelemetryUp->parastr.pValue))&&
            ((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF)))//合位
        {
            if (*overTelemetryUp->parastr.Telesignal.value == OFF)
            {
                if (!(*overTelemetryUp->valstr.gTime & MAINPRO_ENTIMERS))
                {
                    *overTelemetryUp->valstr.gTime = MAINPRO_ENTIMERS;//启动时钟
                }

                if ((*overTelemetryUp->valstr.gTime & MAINPRO_TITIMERS) > (rt_uint32_t)((*overTelemetryUp->parastr.pTime)*1000))
                {
                    *overTelemetryUp->valstr.gTime = 0;
                    addSOE(comProSts,&overTelemetryUp->parastr.Telesignal, ON);
                    overTelemetryUp->valstr.flag = OVERTELEMETRYUP;
                    if (*(overTelemetryUp->parastr.pSwitch) == SWITCH_ON)
                    {
						if(g_TelesignalDB[g_TelesignalAddr.workmodeVolcur] == ON)	//电压电流型
						{
							if(*(comProSts->yx.swtichclass.value)==OFF)				//断路器
							{  
								comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
								addSOE(comProSts,&comProSts->yx.closingLocked,ON);
							}
							else if(*(comProSts->yx.swtichclass.value)==ON && (!(*(overTelemetryUp->valstr.stateJudgeflag)&STATEUNBLOCKED)))//负荷开关
							{
								comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
								addSOE(comProSts,&comProSts->yx.closingLocked,ON);
							}
						}
						else if(g_TelesignalDB[g_TelesignalAddr.workmodeCommon] == ON)//常规保护
						{
							comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
							addSOE(comProSts,&comProSts->yx.recloseClock,ON);
						}
                    }
                }
            }
        }
        else
        {
            *overTelemetryUp->valstr.gTime = 0;
            if (*overTelemetryUp->parastr.Telesignal.value  == ON && 
                ((**overTelemetryUp->parastr.telemetry0 < (*overTelemetryUp->parastr.pValue * (*overTelemetryUp->parastr.pFactor))) &&
                (**overTelemetryUp->parastr.telemetry1 < (*overTelemetryUp->parastr.pValue * (*overTelemetryUp->parastr.pFactor))) &&
                (**overTelemetryUp->parastr.telemetry2 < (*overTelemetryUp->parastr.pValue * (*overTelemetryUp->parastr.pFactor)))))
            {
                overTelemetryUp->valstr.flag &= ~OVERTELEMETRYUP;
                addSOE(comProSts,&overTelemetryUp->parastr.Telesignal, OFF);			
            }
        }
    }
    else
    {
        *overTelemetryUp->valstr.gTime = 0;
        addSOE(comProSts,&overTelemetryUp->parastr.Telesignal, OFF);
        overTelemetryUp->valstr.flag &= ~OVERTELEMETRYUP;
    }
}

/**
  * @Description: 超下限跳闸
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void OverTelemetryDown_ctrl(ComProSts *comProSts,OverTelemetryDownSts *overTelemetryDown)
{
    if(*(overTelemetryDown->parastr.pSwitch) != SWITCH_OFF)
    {
        if (((**overTelemetryDown->parastr.telemetry0 < *overTelemetryDown->parastr.pValue)||
            (**overTelemetryDown->parastr.telemetry1 < *overTelemetryDown->parastr.pValue)||
            (**overTelemetryDown->parastr.telemetry2 < *overTelemetryDown->parastr.pValue))&&
            (overTelemetryDown->valstr.flag&OVERTELEMETRYDOWNSTA1)&&
            (((!(*(overTelemetryDown->valstr.stateJudgeflag)&STATESLIPLOCK))&&(overTelemetryDown == s_OverFrequencyDown))||(overTelemetryDown != s_OverFrequencyDown))&&
            ((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF)))//合位//低频跳闸需要判断滑差闭锁
        {
            if (*overTelemetryDown->parastr.Telesignal.value == OFF)
            {
                if (!(*overTelemetryDown->valstr.gTime[0] & MAINPRO_ENTIMERS))
                {
                    *overTelemetryDown->valstr.gTime[0] = MAINPRO_ENTIMERS;//启动时钟
                }

                if ((*overTelemetryDown->valstr.gTime[0] & MAINPRO_TITIMERS) > (rt_uint32_t)((*overTelemetryDown->parastr.pTime)*1000))
                {
                    *overTelemetryDown->valstr.gTime[0] = 0;
                    overTelemetryDown->valstr.flag = OVERTELEMETRYDOWN;
                    overTelemetryDown->valstr.flag &= ~OVERTELEMETRYDOWNSTA1;
                    if((*(comProSts->yc.Uab) > 15)&&(*(comProSts->yc.Ucb) > 15))
                    {
                        addSOE(comProSts,&overTelemetryDown->parastr.Telesignal, ON);
                        if (*(overTelemetryDown->parastr.pSwitch) == SWITCH_ON)
                        {
							if(g_TelesignalDB[g_TelesignalAddr.workmodeVolcur] == ON)	//电压电流型
							{
								if(*(comProSts->yx.swtichclass.value)==OFF)				//断路器
								{  
									comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
									addSOE(comProSts,&comProSts->yx.closingLocked,ON);
								}
								else if(*(comProSts->yx.swtichclass.value)==ON && (!(*(overTelemetryDown->valstr.stateJudgeflag)&STATEUNBLOCKED)))//负荷开关
								{
									comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
									addSOE(comProSts,&comProSts->yx.closingLocked,ON);
								}
							}
							else if(g_TelesignalDB[g_TelesignalAddr.workmodeCommon] == ON)//常规保护
							{
								comProSts->opening(ADDR_LOGIC_ACT,DO_LOGIC_OPEN);
								addSOE(comProSts,&comProSts->yx.recloseClock,ON);
							}
                        }
                    }
                }
            }
			*overTelemetryDown->valstr.gTime[1] = 0;
        }
        else
        {
            *overTelemetryDown->valstr.gTime[0] = 0;
            if ((!((overTelemetryDown->valstr.flag)&OVERTELEMETRYDOWNSTA1)) && 
                ((**overTelemetryDown->parastr.telemetry0 > (*overTelemetryDown->parastr.pValue * (*overTelemetryDown->parastr.pFactor)))||
                (**overTelemetryDown->parastr.telemetry1 > (*overTelemetryDown->parastr.pValue * (*overTelemetryDown->parastr.pFactor)))||
                (**overTelemetryDown->parastr.telemetry2 > (*overTelemetryDown->parastr.pValue * (*overTelemetryDown->parastr.pFactor)))))
            {
				if (!(*overTelemetryDown->valstr.gTime[1] & MAINPRO_ENTIMERS))
				{
					*overTelemetryDown->valstr.gTime[1] = MAINPRO_ENTIMERS;//启动时钟
				}
                if ((*overTelemetryDown->valstr.gTime[1] & MAINPRO_TITIMERS) > OVERTELEMETRYDOWNTIME)
				{
					*overTelemetryDown->valstr.gTime[1] = 0;
					overTelemetryDown->valstr.flag &= ~OVERTELEMETRYDOWN;
					overTelemetryDown->valstr.flag |= OVERTELEMETRYDOWNSTA1;
					addSOE(comProSts,&overTelemetryDown->parastr.Telesignal, OFF);	
				}                 
            }
			else 
			{
				*overTelemetryDown->valstr.gTime[1] = 0;
			}
        }
    }
    else
    {
        *overTelemetryDown->valstr.gTime[0] = 0;
		*overTelemetryDown->valstr.gTime[1] = 0;
        addSOE(comProSts,&overTelemetryDown->parastr.Telesignal, OFF);
        overTelemetryDown->valstr.flag = ~OVERTELEMETRYDOWN;
    }
}

/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @Description: 初始化.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void MainProtectCtrlInit(void)
{
    uint8_t pdrv;

    for(pdrv=0; pdrv<MAINPRO_DEVMAXNUM; pdrv++)
    {
        switch(pdrv)
        {
        case MAINPRO_DEV0:
            list_init(&s_ListTimers[pdrv]);
            //通用
            s_ComProSts[pdrv].yx.switchOpen.value = &g_TelesignalDB[g_TelesignalAddr.switchOpen];
            s_ComProSts[pdrv].yx.switchClose.value = &g_TelesignalDB[g_TelesignalAddr.switchClose];
            s_ComProSts[pdrv].yx.modBreakHardStrap.value =  &g_TelesignalDB[g_TelesignalAddr.modBreakHardStrap];
            s_ComProSts[pdrv].yx.modContactHardStrap.value =  &g_TelesignalDB[g_TelesignalAddr.modContactHardStrap];
            s_ComProSts[pdrv].yx.functionHardStrap.value = &g_TelesignalDB[g_TelesignalAddr.functionHardStrap];
            s_ComProSts[pdrv].yx.recloseHardStrap.value = &g_TelesignalDB[g_TelesignalAddr.recloseHardStrap];
            s_ComProSts[pdrv].yx.modBreakSoftStrap.value =  &g_TelesignalDB[g_TelesignalAddr.modBreakSoftStrap];
            s_ComProSts[pdrv].yx.modContactSoftStrap.value =  &g_TelesignalDB[g_TelesignalAddr.modContactSoftStrap];
            s_ComProSts[pdrv].yx.functionSoftStrap.value = &g_TelesignalDB[g_TelesignalAddr.functionSoftStrap];
            s_ComProSts[pdrv].yx.recloseSoftStrap.value = &g_TelesignalDB[g_TelesignalAddr.recloseSoftStrap];
            s_ComProSts[pdrv].yx.workmodeCommon.value = &g_TelesignalDB[g_TelesignalAddr.workmodeCommon];
            s_ComProSts[pdrv].yx.workmodeVolcur.value = &g_TelesignalDB[g_TelesignalAddr.workmodeVolcur];
            s_ComProSts[pdrv].yx.swtichclass.value = &g_TelesignalDB[g_TelesignalAddr.swtichClass];
        
            s_ComProSts[pdrv].yx.recloseAct.addr = g_TelesignalAddr.recloseAct;
            s_ComProSts[pdrv].yx.recloseClock.addr = g_TelesignalAddr.recloseLock;
            s_ComProSts[pdrv].yx.overcurrentAccIa.addr = g_TelesignalAddr.overcurrentAccIa;
            s_ComProSts[pdrv].yx.overcurrentAccIb.addr = g_TelesignalAddr.overcurrentAccIb;
            s_ComProSts[pdrv].yx.overcurrentAccIc.addr = g_TelesignalAddr.overcurrentAccIc;
            s_ComProSts[pdrv].yx.overcurrentAccI0.addr = g_TelesignalAddr.overcurrentAccI0;
            s_ComProSts[pdrv].yx.overcurrentAccU0.addr = g_TelesignalAddr.overcurrentAccU0;
            s_ComProSts[pdrv].yx.overcurrentIa[0].addr = g_TelesignalAddr.overcurrentIa1;
            s_ComProSts[pdrv].yx.overcurrentIb[0].addr = g_TelesignalAddr.overcurrentIb1;
            s_ComProSts[pdrv].yx.overcurrentIc[0].addr = g_TelesignalAddr.overcurrentIc1;
            s_ComProSts[pdrv].yx.overcurrentIa[1].addr = g_TelesignalAddr.overcurrentIa2;
            s_ComProSts[pdrv].yx.overcurrentIb[1].addr = g_TelesignalAddr.overcurrentIb2;
            s_ComProSts[pdrv].yx.overcurrentIc[1].addr = g_TelesignalAddr.overcurrentIc2;
            s_ComProSts[pdrv].yx.overcurrentIa[2].addr = g_TelesignalAddr.overcurrentIa3;
            s_ComProSts[pdrv].yx.overcurrentIb[2].addr = g_TelesignalAddr.overcurrentIb3;
            s_ComProSts[pdrv].yx.overcurrentIc[2].addr = g_TelesignalAddr.overcurrentIc3;
            s_ComProSts[pdrv].yx.overcurrentI0[0].addr = g_TelesignalAddr.overcurrentI01;
            s_ComProSts[pdrv].yx.overcurrentI0[1].addr = g_TelesignalAddr.overcurrentI02;
            s_ComProSts[pdrv].yx.recloseProtection[0].addr = g_TelesignalAddr.recloseProtection1;
            s_ComProSts[pdrv].yx.recloseProtection[1].addr = g_TelesignalAddr.recloseProtection2;
            s_ComProSts[pdrv].yx.recloseProtection[2].addr = g_TelesignalAddr.recloseProtection3;
            s_ComProSts[pdrv].yx.recloseProtection[3].addr = g_TelesignalAddr.recloseProtection4;
            s_ComProSts[pdrv].yx.zeroRecloseProtection[0].addr = g_TelesignalAddr.zeroRecloseProtection1;
            s_ComProSts[pdrv].yx.zeroRecloseProtection[1].addr = g_TelesignalAddr.zeroRecloseProtection2;
            s_ComProSts[pdrv].yx.clockSecondaryClose.addr = g_TelesignalAddr.clockSecondaryClose;

            s_ComProSts[pdrv].yx.closingLocked.addr = g_TelesignalAddr.closingLocked;
            s_ComProSts[pdrv].yx.openingLocked.addr = g_TelesignalAddr.openingLocked;
            s_ComProSts[pdrv].yx.shortCircuitFault.addr = g_TelesignalAddr.shortCircuitFault;
            s_ComProSts[pdrv].yx.earthingFault.addr = g_TelesignalAddr.earthingFault;
            s_ComProSts[pdrv].yx.protectionAct.addr = g_TelesignalAddr.protectionAct;
            s_ComProSts[pdrv].yx.overcurrentFaultIa.addr = g_TelesignalAddr.overcurrentFaultIa;
            s_ComProSts[pdrv].yx.overcurrentFaultIb.addr = g_TelesignalAddr.overcurrentFaultIb;
            s_ComProSts[pdrv].yx.overcurrentFaultIc.addr = g_TelesignalAddr.overcurrentFaultIc;
            s_ComProSts[pdrv].yx.overcurrentFaultI0.addr = g_TelesignalAddr.overcurrentFaultI0;
            s_ComProSts[pdrv].yx.lossTrip.addr = g_TelesignalAddr.lossTrip;
            s_ComProSts[pdrv].yx.getClossing.addr = g_TelesignalAddr.getClossing;
            s_ComProSts[pdrv].yx.failAfterClosing.addr = g_TelesignalAddr.closingFaultEvent;
            s_ComProSts[pdrv].yx.remainClockClossingUab.addr = g_TelesignalAddr.remainClockClossingUab;
			s_ComProSts[pdrv].yx.remainClockClossingUcb.addr = g_TelesignalAddr.remainClockClossingUcb;
            s_ComProSts[pdrv].yx.doubleProhibitClossing.addr = g_TelesignalAddr.doubleProhibitClossing;
            s_ComProSts[pdrv].yx.singleLossClosing.addr = g_TelesignalAddr.singleLossClosingEvent;
            s_ComProSts[pdrv].yx.breakingCurrent.addr = g_TelesignalAddr.breakingCurrent;

            s_ComProSts[pdrv].yx.recloseAct.value = &g_TelesignalDB[g_TelesignalAddr.recloseAct];
            s_ComProSts[pdrv].yx.recloseClock.value = &g_TelesignalDB[g_TelesignalAddr.recloseLock];
            s_ComProSts[pdrv].yx.overcurrentAccIa.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentAccIa];
            s_ComProSts[pdrv].yx.overcurrentAccIb.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentAccIb];
            s_ComProSts[pdrv].yx.overcurrentAccIc.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentAccIc];
            s_ComProSts[pdrv].yx.overcurrentAccI0.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentAccI0];
            s_ComProSts[pdrv].yx.overcurrentAccU0.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentAccU0];
            s_ComProSts[pdrv].yx.overcurrentIa[0].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIa1];
            s_ComProSts[pdrv].yx.overcurrentIb[0].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIb1];
            s_ComProSts[pdrv].yx.overcurrentIc[0].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIc1];
            s_ComProSts[pdrv].yx.overcurrentIa[1].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIa2];
            s_ComProSts[pdrv].yx.overcurrentIb[1].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIb2];
            s_ComProSts[pdrv].yx.overcurrentIc[1].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIc2];
            s_ComProSts[pdrv].yx.overcurrentIa[2].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIa3];
            s_ComProSts[pdrv].yx.overcurrentIb[2].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIb3];
            s_ComProSts[pdrv].yx.overcurrentIc[2].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIc3];
            s_ComProSts[pdrv].yx.overcurrentI0[0].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentI01];
            s_ComProSts[pdrv].yx.overcurrentI0[1].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentI02];
            s_ComProSts[pdrv].yx.recloseProtection[0].value = &g_TelesignalDB[g_TelesignalAddr.recloseProtection1];
            s_ComProSts[pdrv].yx.recloseProtection[1].value = &g_TelesignalDB[g_TelesignalAddr.recloseProtection2];
            s_ComProSts[pdrv].yx.recloseProtection[2].value = &g_TelesignalDB[g_TelesignalAddr.recloseProtection3];
            s_ComProSts[pdrv].yx.recloseProtection[3].value = &g_TelesignalDB[g_TelesignalAddr.recloseProtection4];
            s_ComProSts[pdrv].yx.zeroRecloseProtection[0].value = &g_TelesignalDB[g_TelesignalAddr.zeroRecloseProtection1];
            s_ComProSts[pdrv].yx.zeroRecloseProtection[1].value = &g_TelesignalDB[g_TelesignalAddr.zeroRecloseProtection2];
            s_ComProSts[pdrv].yx.clockSecondaryClose.value = &g_TelesignalDB[g_TelesignalAddr.clockSecondaryClose];
            
            s_ComProSts[pdrv].yx.closingLocked.value = &g_TelesignalDB[g_TelesignalAddr.closingLocked];
            s_ComProSts[pdrv].yx.openingLocked.value = &g_TelesignalDB[g_TelesignalAddr.openingLocked];
            s_ComProSts[pdrv].yx.shortCircuitFault.value = &g_TelesignalDB[g_TelesignalAddr.shortCircuitFault];
            s_ComProSts[pdrv].yx.earthingFault.value = &g_TelesignalDB[g_TelesignalAddr.earthingFault];
            s_ComProSts[pdrv].yx.protectionAct.value = &g_TelesignalDB[g_TelesignalAddr.protectionAct];
            s_ComProSts[pdrv].yx.overcurrentFaultIa.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentFaultIa];
            s_ComProSts[pdrv].yx.overcurrentFaultIb.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentFaultIb];
            s_ComProSts[pdrv].yx.overcurrentFaultIc.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentFaultIc];
            s_ComProSts[pdrv].yx.overcurrentFaultI0.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentFaultI0];
            s_ComProSts[pdrv].yx.lossTrip.value = &g_TelesignalDB[g_TelesignalAddr.lossTrip];
            s_ComProSts[pdrv].yx.getClossing.value = &g_TelesignalDB[g_TelesignalAddr.getClossing];
            s_ComProSts[pdrv].yx.failAfterClosing.value = &g_TelesignalDB[g_TelesignalAddr.closingFaultEvent];
            s_ComProSts[pdrv].yx.remainClockClossingUab.value = &g_TelesignalDB[g_TelesignalAddr.remainClockClossingUab];
			s_ComProSts[pdrv].yx.remainClockClossingUcb.value = &g_TelesignalDB[g_TelesignalAddr.remainClockClossingUcb];
            s_ComProSts[pdrv].yx.doubleProhibitClossing.value = &g_TelesignalDB[g_TelesignalAddr.doubleProhibitClossing];
            s_ComProSts[pdrv].yx.singleLossClosing.value = &g_TelesignalDB[g_TelesignalAddr.singleLossClosingEvent];
            s_ComProSts[pdrv].yx.breakingCurrent.value = &g_TelesignalDB[g_TelesignalAddr.breakingCurrent];

            s_ComProSts[pdrv].yc.f = &g_TelemetryBaseDB[g_TelemetryBaseAddr.F];
            s_ComProSts[pdrv].yc.Ia = &g_TelemetryBaseDB[g_TelemetryBaseAddr.Ia];
            s_ComProSts[pdrv].yc.Ib = &g_TelemetryBaseDB[g_TelemetryBaseAddr.Ib];
            s_ComProSts[pdrv].yc.Ic = &g_TelemetryBaseDB[g_TelemetryBaseAddr.Ic];
            s_ComProSts[pdrv].yc.Ia2 = &g_secondHarmonicIa;
            s_ComProSts[pdrv].yc.Ib2 = &g_secondHarmonicIb;
            s_ComProSts[pdrv].yc.Ic2 = &g_secondHarmonicIc;
            s_ComProSts[pdrv].yc.I0 = &g_TelemetryBaseDB[g_TelemetryBaseAddr.I0];
            s_ComProSts[pdrv].yc.Uab = &g_TelemetryBaseDB[g_TelemetryBaseAddr.Uab];            
            s_ComProSts[pdrv].yc.U0 = &g_TelemetryBaseDB[g_TelemetryBaseAddr.U0];
            s_ComProSts[pdrv].yc.f_s = &g_TelemetryBaseDB[g_TelemetryBaseAddr.F_S];

            s_ComProSts[pdrv].opening = &rt_hw_do_operate;
            s_ComProSts[pdrv].closing = &rt_hw_do_operate;
            s_ComProSts[pdrv].outputSoe = &DBWriteSOE;
            s_ComProSts[pdrv].outputFevent = &DBWriteFEVENT;
            
            //复位
            s_Rest[pdrv].valstr.flag = 0;
            addtimers(pdrv,&s_Rest[pdrv].valstr.gTime);
            s_Rest[pdrv].valstr.StateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.lossTripflag = &s_LossTrip[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.getVolCloseflag = &s_GetVolClose[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.openCloseLockingflag = &s_OpenCloseLocking[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.fewVolLockflag = &s_FewVolLock[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.doubleVol = &s_DoubleVol[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.SingleLossClose = &s_SingleLossClose[pdrv].valstr.flag;
            s_Rest[pdrv].parastr.pSwitch = &g_FixedValueP[AUTOMATIC_RESET_SWITCH];
            s_Rest[pdrv].parastr.pTime = &g_FixedValueP[AUTOMATIC_RESET_TIME];
            //状态标志
            s_stateJudge[pdrv].valstr.flag = 0;
            s_stateJudge[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            s_stateJudge[pdrv].parastr.pValueOvercur = &g_FixedValueP[OVERCURRENTFAULT_VALUE];
            s_stateJudge[pdrv].parastr.pValueOvercurI0 = &g_FixedValueP[OVERCURRENTFAULTI0_VALUE];
            s_stateJudge[pdrv].parastr.pValuehavevol = &g_FixedValueP[VOLTAGE_VALUE];
            s_stateJudge[pdrv].parastr.pValueLossvol = &g_FixedValueP[NONVOLTAGE_VALUE];
            s_stateJudge[pdrv].parastr.pValueLosscur = &g_FixedValueP[CURRENT_VALUE];
            s_stateJudge[pdrv].parastr.pSwitchBreakcur = &g_FixedValueP[BREAKING_CURRENT_SWITCH];
            s_stateJudge[pdrv].parastr.pValueBreakcur = &g_FixedValueP[BREAKING_CURRENT_VALUE];            
            addtimers(pdrv,&s_stateJudge[pdrv].valstr.gTimeInrush);
            s_stateJudge[pdrv].parastr.pSwitchInrush = &g_FixedValueP[INRUSH_SWITCH];
            s_stateJudge[pdrv].parastr.pTimeInrush = &g_FixedValueP[INRUSH_TIME];      
            s_stateJudge[pdrv].parastr.pSwitchSlip = &g_FixedValueP[SLIPLOCK_SWITCH];
            s_stateJudge[pdrv].parastr.pValueSlip = &g_FixedValueP[SLIPLOCK_VALUE];            
			s_stateJudge[pdrv].valstr.flag = (STATENOVOLTAGE|STATENOCURRENT);
            //失电分闸
            s_LossTrip[pdrv].valstr.flag = 0;
            s_LossTrip[pdrv].valstr.openCloseLockingflag = &s_OpenCloseLocking[pdrv].valstr.flag;
            s_LossTrip[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_LossTrip[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_LossTrip[pdrv].valstr.gTime);
            s_LossTrip[pdrv].parastr.pSwitch = &g_FixedValueP[LOSS_ELECTRICITY_SWITCH];
            s_LossTrip[pdrv].parastr.pTime = &g_FixedValueP[LOSS_OPENING_TIME];
            //得电合闸
            s_GetVolClose[pdrv].valstr.flag = 0;
            s_GetVolClose[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_GetVolClose[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_GetVolClose[pdrv].valstr.gTime);
            s_GetVolClose[pdrv].parastr.pSwitchUab = &g_FixedValueP[UAB_GET_VOLTAGE_CLOSSING_SWITCH];
            s_GetVolClose[pdrv].parastr.pSwitchUcb = &g_FixedValueP[UCB_GET_VOLTAGE_CLOSSING_SWITCH];
            s_GetVolClose[pdrv].parastr.pXTime = &g_FixedValueP[GET_VOLTAGE_CLOSSING_X_TIME];
            //合分闸闭锁
            s_OpenCloseLocking[pdrv].valstr.flag = 0;
            s_OpenCloseLocking[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_OpenCloseLocking[pdrv].valstr.getVolCloseflag = &s_GetVolClose[pdrv].valstr.flag;
            s_OpenCloseLocking[pdrv].valstr.singleLossCloseflag = &s_SingleLossClose[pdrv].valstr.flag;
            s_OpenCloseLocking[pdrv].valstr.lossTripflag = &s_LossTrip[pdrv].valstr.flag;
            s_OpenCloseLocking[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_OpenCloseLocking[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_OpenCloseLocking[pdrv].valstr.gTime);
            s_OpenCloseLocking[pdrv].parastr.pYTime = &g_FixedValueP[FAULT_CHECK_Y_TIME];
            s_OpenCloseLocking[pdrv].parastr.pCloseLockCurSwitch = &g_FixedValueP[LOCKED_CLOSING_CUR_SWITCH];
            s_OpenCloseLocking[pdrv].parastr.pOpenLockSwitch = &g_FixedValueP[LOCKED_OPENING_SWITCH];
            s_OpenCloseLocking[pdrv].parastr.pOpenLockTime = &g_FixedValueP[SHORT_LOCKED_OPENING_TIME];
            //残压闭锁
            s_FewVolLock[pdrv].valstr.flag = 0;
            s_FewVolLock[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_FewVolLock[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_FewVolLock[pdrv].valstr.gUabTime);
            addtimers(pdrv,&s_FewVolLock[pdrv].valstr.gUcbTime);
            s_FewVolLock[pdrv].parastr.pSwitch = &g_FixedValueP[REMAIN_VOLTAGE_SWITCH];
            s_FewVolLock[pdrv].parastr.pValue = &g_FixedValueP[REMAIN_VOLTAGE_VALUE];
            s_FewVolLock[pdrv].parastr.pTime = &g_FixedValueP[REMAIN_VOLTAGE_TIME];
            s_FewVolLock[pdrv].parastr.pXTime = &g_FixedValueP[GET_VOLTAGE_CLOSSING_X_TIME];
            s_FewVolLock[pdrv].parastr.pXLTime = &g_FixedValueP[SINGLE_LOSS_VOLTAGE_XLTIME];
            //双侧有压
            s_DoubleVol[pdrv].valstr.flag = 0;
            s_DoubleVol[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_DoubleVol[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            s_DoubleVol[pdrv].parastr.pSwitch = &g_FixedValueP[DOUBLE_VOLTAGE_SWITCH];
            //单侧失电合闸
            s_SingleLossClose[pdrv].valstr.flag = 0;
            s_SingleLossClose[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_SingleLossClose[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_SingleLossClose[pdrv].valstr.gTime);
            s_SingleLossClose[pdrv].parastr.pSwitch = &g_FixedValueP[SINGLE_LOSS_VOLTAGE_SWITCH];
            s_SingleLossClose[pdrv].parastr.pTime = &g_FixedValueP[SINGLE_LOSS_VOLTAGE_XLTIME];
            //后加速
			s_IACC[pdrv].valstr.flag = 0;
            s_IACC[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_IACC[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_IACC[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_IACC[pdrv].valstr.recloseflag = &s_Reclose[pdrv].valstr.flag;
            s_IACC[pdrv].valstr.recloseI0flag = &s_RecloseI0[pdrv].valstr.flag;
            s_IACC[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_IACC[pdrv].valstr.gTimeEnIACC);
			addtimers(pdrv,&s_IACC[pdrv].valstr.gTimeIACCI);
			addtimers(pdrv,&s_IACC[pdrv].valstr.gTimeIACCI0);
			addtimers(pdrv,&s_IACC[pdrv].valstr.gTimeIACCU0);
            s_IACC[pdrv].parastr.pSwitchIACCI = &g_FixedValueP[IACCI_SWITCH];
            s_IACC[pdrv].parastr.pValueIACCI = &g_FixedValueP[IACCI_CURRENT_VALUE];
            s_IACC[pdrv].parastr.pTimeIACCI = &g_FixedValueP[IACCI_TIME];
            s_IACC[pdrv].parastr.pSwitchIACCI0 = &g_FixedValueP[IACCI0_SWITCH];
            s_IACC[pdrv].parastr.pValueIACCI0 = &g_FixedValueP[IACCI0_CURRENT_VALUE];
            s_IACC[pdrv].parastr.pTimeIACCI0 = &g_FixedValueP[IACCI0_TIME];
            s_IACC[pdrv].parastr.pSwitchIACCU0 = &g_FixedValueP[IACCU0_SWITCH];
            s_IACC[pdrv].parastr.pValueIACCU0 = &g_FixedValueP[IACCU0_CURRENT_VALUE];
            s_IACC[pdrv].parastr.pTimeIACCU0 = &g_FixedValueP[IACCU0_TIME];
            //过流
			s_Overcur[pdrv].valstr.flag = 0;
            s_Overcur[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_Overcur[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_Overcur[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
			s_Overcur[pdrv].valstr.recloseflag = &s_Reclose[pdrv].valstr.flag;
            s_Overcur[pdrv].valstr.recloseI0flag = &s_RecloseI0[pdrv].valstr.flag;
            s_Overcur[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_Overcur[pdrv].valstr.gTime[0]);
            addtimers(pdrv,&s_Overcur[pdrv].valstr.gTime[1]);
            addtimers(pdrv,&s_Overcur[pdrv].valstr.gTime[2]);
            s_Overcur[pdrv].parastr.pSwitch[0] = &g_FixedValueP[OVER_CURRENT_SWITCH1];
            s_Overcur[pdrv].parastr.pSwitch[1] = &g_FixedValueP[OVER_CURRENT_SWITCH2];
            s_Overcur[pdrv].parastr.pSwitch[2] = &g_FixedValueP[OVER_CURRENT_SWITCH3];
            s_Overcur[pdrv].parastr.pValue[0] = &g_FixedValueP[OVER_CURRENT_VALUE1];
            s_Overcur[pdrv].parastr.pValue[1] = &g_FixedValueP[OVER_CURRENT_VALUE2];
            s_Overcur[pdrv].parastr.pValue[2] = &g_FixedValueP[OVER_CURRENT_VALUE3];
            s_Overcur[pdrv].parastr.pTime[0] = &g_FixedValueP[OVER_CURRENT_TIME1];
            s_Overcur[pdrv].parastr.pTime[1] = &g_FixedValueP[OVER_CURRENT_TIME2];
            s_Overcur[pdrv].parastr.pTime[2] = &g_FixedValueP[OVER_CURRENT_TIME3];
            //零序过流
			s_OvercurI0[pdrv].valstr.flag = 0;
            s_OvercurI0[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_OvercurI0[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_OvercurI0[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
			s_OvercurI0[pdrv].valstr.recloseflag = &s_Reclose[pdrv].valstr.flag;
            s_OvercurI0[pdrv].valstr.recloseI0flag = &s_RecloseI0[pdrv].valstr.flag;
            s_OvercurI0[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_OvercurI0[pdrv].valstr.gTime[0]);
            addtimers(pdrv,&s_OvercurI0[pdrv].valstr.gTime[1]);
            s_OvercurI0[pdrv].parastr.pSwitch[0] = &g_FixedValueP[OVER_CURRENTI0_SWITCH1];
            s_OvercurI0[pdrv].parastr.pSwitch[1] = &g_FixedValueP[OVER_CURRENTI0_SWITCH2];
            s_OvercurI0[pdrv].parastr.pValue[0] = &g_FixedValueP[OVER_CURRENTI0_VALUE1];
            s_OvercurI0[pdrv].parastr.pValue[1] = &g_FixedValueP[OVER_CURRENTI0_VALUE2];
            s_OvercurI0[pdrv].parastr.pTime[0] = &g_FixedValueP[OVER_CURRENTI0_TIME1];
            s_OvercurI0[pdrv].parastr.pTime[1] = &g_FixedValueP[OVER_CURRENTI0_TIME2];
            // 二次重合闸闭锁
			s_SecondaryRecloseLock[pdrv].valstr.flag = 0;
			s_SecondaryRecloseLock[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_SecondaryRecloseLock[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_SecondaryRecloseLock[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_SecondaryRecloseLock[pdrv].valstr.recloseflag = &s_Reclose[pdrv].valstr.flag;
            s_SecondaryRecloseLock[pdrv].valstr.recloseI0flag = &s_RecloseI0[pdrv].valstr.flag;
            s_SecondaryRecloseLock[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_SecondaryRecloseLock[pdrv].valstr.gTime);
            s_SecondaryRecloseLock[pdrv].parastr.pTime = &g_FixedValueP[SECONDARY_RECLOSE_LOCK_TIME];
            // 过流重合闸
			s_Reclose[pdrv].valstr.flag = 0;
            s_Reclose[pdrv].valstr.secondaryRecloseLockflag = &s_SecondaryRecloseLock[pdrv].valstr.flag;
			s_Reclose[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_Reclose[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_Reclose[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_Reclose[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_Reclose[pdrv].valstr.gTime[0]);
			addtimers(pdrv,&s_Reclose[pdrv].valstr.gTime[1]);
//            s_Reclose[pdrv].parastr.pTimes = &g_FixedValueP[RECLOSE_TIMES_I];
			s_Reclose[pdrv].parastr.pSwitch[0] = &g_FixedValueP[RECLOSE_ONE_SWITCH];
			s_Reclose[pdrv].parastr.pSwitch[1] = &g_FixedValueP[RECLOSE_TOW_SWITCH];
            s_Reclose[pdrv].parastr.pTime[0] = &g_FixedValueP[RECLOSE_TIMER1];
            s_Reclose[pdrv].parastr.pTime[1] = &g_FixedValueP[RECLOSE_TIMER2];
//            s_Reclose[pdrv].parastr.pTime[2] = &g_FixedValueP[RECLOSE_TIMER3];
//            s_Reclose[pdrv].parastr.pTime[3] = &g_FixedValueP[RECLOSE_TIMER4];
			s_Reclose[pdrv].parastr.pResetTime = &g_FixedValueP[RE_RESET_TIME];
            //零序过流重合闸
			s_RecloseI0[pdrv].valstr.flag = 0;
            s_RecloseI0[pdrv].valstr.secondaryRecloseLockflag = &s_SecondaryRecloseLock[pdrv].valstr.flag;
			s_RecloseI0[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_RecloseI0[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_RecloseI0[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_RecloseI0[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_RecloseI0[pdrv].valstr.gTime[0]);
			addtimers(pdrv,&s_RecloseI0[pdrv].valstr.gTime[1]);
            s_RecloseI0[pdrv].parastr.pTimes = &g_FixedValueP[RECLOSE_TIMES_I0];
            s_RecloseI0[pdrv].parastr.pTime[0] = &g_FixedValueP[RECLOSE_TIMER1];
            s_RecloseI0[pdrv].parastr.pTime[1] = &g_FixedValueP[RECLOSE_TIMER2];
			s_RecloseI0[pdrv].parastr.pResetTime = &g_FixedValueP[RE_RESET_TIME];
            //过压跳闸
            s_OverVoltageUp[pdrv].valstr.flag = 0;
			s_OverVoltageUp[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            addtimers(pdrv,&s_OverVoltageUp[pdrv].valstr.gTime);
            s_OverVoltageUp[pdrv].parastr.Telesignal.addr = g_TelesignalAddr.overVoltageProtection;
            s_OverVoltageUp[pdrv].parastr.Telesignal.value = &g_TelesignalDB[g_TelesignalAddr.overVoltageProtection];
            s_OverVoltageUp[pdrv].parastr.telemetry0 = &s_ComProSts[pdrv].yc.Uab;
            s_OverVoltageUp[pdrv].parastr.telemetry1 = &s_ComProSts[pdrv].yc.Ucb;
            s_OverVoltageUp[pdrv].parastr.telemetry2 = &s_ComProSts[pdrv].yc.Uab;
            s_OverVoltageUp[pdrv].parastr.pSwitch = &g_FixedValueP[OVERVOLTAGE_SWITCH];
            s_OverVoltageUp[pdrv].parastr.pValue = &g_FixedValueP[OVERVOLTAGE_VALUE];
            s_OverVoltageUp[pdrv].parastr.pTime = &g_FixedValueP[OVERVOLTAGE_TIME];
            s_OverVoltageUp[pdrv].parastr.pFactor = &g_FixedValueP[OVERVOLTAGE_FACTOR];
            //低压跳闸
            s_OverVoltageDown[pdrv].valstr.flag = 0;
            s_OverVoltageDown[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            addtimers(pdrv,&s_OverVoltageDown[pdrv].valstr.gTime[0]);
			addtimers(pdrv,&s_OverVoltageDown[pdrv].valstr.gTime[1]);
            s_OverVoltageDown[pdrv].parastr.Telesignal.addr = g_TelesignalAddr.downVoltageProtection;
            s_OverVoltageDown[pdrv].parastr.Telesignal.value = &g_TelesignalDB[g_TelesignalAddr.downVoltageProtection];
            s_OverVoltageDown[pdrv].parastr.telemetry0 = &s_ComProSts[pdrv].yc.Uab;
            s_OverVoltageDown[pdrv].parastr.telemetry1 = &s_ComProSts[pdrv].yc.Ucb;
            s_OverVoltageDown[pdrv].parastr.telemetry2 = &s_ComProSts[pdrv].yc.Uab;
            s_OverVoltageDown[pdrv].parastr.pSwitch = &g_FixedValueP[DOWNVOLTAGE_SWITCH];
            s_OverVoltageDown[pdrv].parastr.pValue = &g_FixedValueP[DOWNVOLTAGE_VALUE];
            s_OverVoltageDown[pdrv].parastr.pTime = &g_FixedValueP[DOWNVOLTAGE_TIME];
            s_OverVoltageDown[pdrv].parastr.pFactor = &g_FixedValueP[DOWNVOLTAGE_FACTOR];
            //过频跳闸
            s_OverFrequencyUp[pdrv].valstr.flag = 0;
			s_OverFrequencyUp[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            addtimers(pdrv,&s_OverFrequencyUp[pdrv].valstr.gTime);
            s_OverFrequencyUp[pdrv].parastr.Telesignal.addr = g_TelesignalAddr.overFrequencyProtection;
            s_OverFrequencyUp[pdrv].parastr.Telesignal.value = &g_TelesignalDB[g_TelesignalAddr.overFrequencyProtection];
            s_OverFrequencyUp[pdrv].parastr.telemetry0 = &s_ComProSts[pdrv].yc.f;
            s_OverFrequencyUp[pdrv].parastr.telemetry1 = &s_ComProSts[pdrv].yc.f;
            s_OverFrequencyUp[pdrv].parastr.telemetry2 = &s_ComProSts[pdrv].yc.f;
            s_OverFrequencyUp[pdrv].parastr.pSwitch = &g_FixedValueP[OVERFREQUENCY_SWITCH];
            s_OverFrequencyUp[pdrv].parastr.pValue = &g_FixedValueP[OVERFREQUENCY_VALUE];
            s_OverFrequencyUp[pdrv].parastr.pTime = &g_FixedValueP[OVERFREQUENCY_TIME];
            s_OverFrequencyUp[pdrv].parastr.pFactor = &g_FixedValueP[OVERFREQUENCY_FACTOR];
            //低频跳闸
            s_OverFrequencyDown[pdrv].valstr.flag = 0;
            s_OverFrequencyDown[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            addtimers(pdrv,&s_OverFrequencyDown[pdrv].valstr.gTime[0]);
			addtimers(pdrv,&s_OverFrequencyDown[pdrv].valstr.gTime[1]);
            s_OverFrequencyDown[pdrv].parastr.Telesignal.addr = g_TelesignalAddr.downFrequencyProtection;
            s_OverFrequencyDown[pdrv].parastr.Telesignal.value = &g_TelesignalDB[g_TelesignalAddr.downFrequencyProtection];
            s_OverFrequencyDown[pdrv].parastr.telemetry0 = &s_ComProSts[pdrv].yc.f;
            s_OverFrequencyDown[pdrv].parastr.telemetry1 = &s_ComProSts[pdrv].yc.f;
            s_OverFrequencyDown[pdrv].parastr.telemetry2 = &s_ComProSts[pdrv].yc.f;
            s_OverFrequencyDown[pdrv].parastr.pSwitch = &g_FixedValueP[DOWNFREQUENCY_SWITCH];
            s_OverFrequencyDown[pdrv].parastr.pValue = &g_FixedValueP[DOWNFREQUENCY_VALUE];
            s_OverFrequencyDown[pdrv].parastr.pTime = &g_FixedValueP[DOWNFREQUENCY_TIME];
            s_OverFrequencyDown[pdrv].parastr.pFactor = &g_FixedValueP[DOWNFREQUENCY_FACTOR];
            //过负荷
            s_OverLoad[pdrv].valstr.flag = 0;
            addtimers(pdrv,&s_OverLoad[pdrv].valstr.gTime);
            s_OverLoad[pdrv].parastr.Telesignal.addr = g_TelesignalAddr.overloadEvent;
            s_OverLoad[pdrv].parastr.Telesignal.value = &g_TelesignalDB[g_TelesignalAddr.overloadEvent];
            s_OverLoad[pdrv].parastr.telemetry0 = &s_ComProSts[pdrv].yc.Ia;
            s_OverLoad[pdrv].parastr.telemetry1 = &s_ComProSts[pdrv].yc.Ib;
            s_OverLoad[pdrv].parastr.telemetry2 = &s_ComProSts[pdrv].yc.Ic;
            s_OverLoad[pdrv].parastr.pSwitch = &g_FixedValueP[OVERLOAD_SWITCH];
            s_OverLoad[pdrv].parastr.pValue = &g_FixedValueP[OVERLOAD_VALUE];
            s_OverLoad[pdrv].parastr.pTime = &g_FixedValueP[OVERLOAD_TIME];
            s_OverLoad[pdrv].parastr.pFactor = &g_FixedValueP[OVERLOAD_FACTOR];
            //初始化清除SOE
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.shortCircuitFault,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.earthingFault,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.protectionAct,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentAccIa,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentAccIb,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentAccIc,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIa[0],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIb[0],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIc[0],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIa[1],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIb[1],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIc[1],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIa[2],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIb[2],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIc[2],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentI0[0],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentI0[1],OFF); 
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.clockSecondaryClose,OFF);
			//初始闭锁清除
			addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.openingLocked,OFF);
            break;
        }
    }
}

/**
  * @Description: 主保护程序.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void MainProtectCtrlClock(void)
{
    uint8_t pdrv;

    for(pdrv=0; pdrv<MAINPRO_DEVMAXNUM; pdrv++)
    {
        add_timers(pdrv);//定时增加
        		
        switch(pdrv)
        {
            case MAINPRO_DEV0:
                if(*(s_ComProSts[pdrv].yx.workmodeCommon.value)==ON)
                {s_ComProSts[pdrv].WorkMode = TYPE_WORKMODE_COMMON;}
                else if(*(s_ComProSts[pdrv].yx.workmodeVolcur.value)==ON)
                {s_ComProSts[pdrv].WorkMode = TYPE_WORKMODE_VOLCUR;}
                else
                {s_ComProSts[pdrv].WorkMode = TYPE_WORKMODE_NONE;}
                
                if(g_Parameter[CFG_PRO_VOL_N] == 0)
                {s_ComProSts[pdrv].yc.Ucb = &g_TelemetryBaseDB[g_TelemetryBaseAddr.UAB];}
                else
                {s_ComProSts[pdrv].yc.Ucb = &g_TelemetryBaseDB[g_TelemetryBaseAddr.UCB];}
                state_judge(&s_ComProSts[pdrv],&s_stateJudge[pdrv]);//状态判断
                if((*(s_ComProSts[pdrv].yx.functionHardStrap.value)==ON)&&(*(s_ComProSts[pdrv].yx.functionSoftStrap.value)==ON))//保护压板
                { 
                    if(s_ComProSts[pdrv].WorkMode == TYPE_WORKMODE_COMMON)
                    {
						s_IACC[pdrv].parastr.pValueIACCI = &g_FixedValueP[IACCI_CURRENT_VALUE];
						s_IACC[pdrv].parastr.pSwitchIACCI = &g_FixedValueP[IACCI_SWITCH];
                        overcur_ctrl(&s_ComProSts[pdrv],&s_Overcur[pdrv]);//过流
                        overcurI0_ctrl(&s_ComProSts[pdrv],&s_OvercurI0[pdrv]);//零序过流
                        secondaryRecloseLock_ctrl(&s_ComProSts[pdrv],&s_SecondaryRecloseLock[pdrv]);//二次重合闸闭锁
                        if((*(s_ComProSts[pdrv].yx.recloseHardStrap.value) == ON)&&(*(s_ComProSts[pdrv].yx.recloseSoftStrap.value) == ON))
                        {
                            reclose_ctrl(&s_ComProSts[pdrv],&s_Reclose[pdrv]);//重合闸
                            recloseI0_ctrl(&s_ComProSts[pdrv],&s_RecloseI0[pdrv]);//零序重合闸
                        }
						OverTelemetryUp_ctrl(&s_ComProSts[pdrv],&s_OverVoltageUp[pdrv]);//过压跳闸
						OverTelemetryDown_ctrl(&s_ComProSts[pdrv],&s_OverVoltageDown[pdrv]);//低压跳闸
						OverTelemetryUp_ctrl(&s_ComProSts[pdrv],&s_OverFrequencyUp[pdrv]);//过频跳闸
						OverTelemetryDown_ctrl(&s_ComProSts[pdrv],&s_OverFrequencyDown[pdrv]);//低频跳闸
						OverTelemetryUp_ctrl(&s_ComProSts[pdrv],&s_OverLoad[pdrv]);//过负荷
                    }           
                    if(s_ComProSts[pdrv].WorkMode == TYPE_WORKMODE_VOLCUR)
                    {
						s_IACC[pdrv].parastr.pValueIACCI = &g_FixedValueP[IACCFI_CURRENT_VALUE];
						s_IACC[pdrv].parastr.pSwitchIACCI = &g_FixedValueP[IACCFI_SWITCH];
						if(((*(s_ComProSts[pdrv].yx.modContactHardStrap.value)==ON)&&(*(s_ComProSts[pdrv].yx.modContactSoftStrap.value)==ON))&&
							((*(s_ComProSts[pdrv].yx.modBreakHardStrap.value)==ON)&&(*(s_ComProSts[pdrv].yx.modBreakSoftStrap.value)==ON)))//同时投入无效
						{}	
                        else if((*(s_ComProSts[pdrv].yx.modContactHardStrap.value)==ON)&&(*(s_ComProSts[pdrv].yx.modContactSoftStrap.value)==ON))//联络
                        {
                            SingleLossClose_ctrl(&s_ComProSts[pdrv],&s_SingleLossClose[pdrv]);//单侧失压合闸
                            openCloseLocking_ctrl(&s_ComProSts[pdrv],&s_OpenCloseLocking[pdrv]);//分合闸闭锁
                            lossTrip_ctrl(&s_ComProSts[pdrv],&s_LossTrip[pdrv]);//失压跳闸
                            fewVolLock_ctrl(&s_ComProSts[pdrv],&s_FewVolLock[pdrv]);//残压闭锁
                            doubleVol_ctrl(&s_ComProSts[pdrv],&s_DoubleVol[pdrv]);//双侧有压禁止合闸
                        }
                        else if((*(s_ComProSts[pdrv].yx.modBreakHardStrap.value)==ON)&&(*(s_ComProSts[pdrv].yx.modBreakSoftStrap.value)==ON))//分段
                        {
                            getVolClose_ctrl(&s_ComProSts[pdrv],&s_GetVolClose[pdrv]);//得电合闸
                            openCloseLocking_ctrl(&s_ComProSts[pdrv],&s_OpenCloseLocking[pdrv]);//分合闸闭锁
                            lossTrip_ctrl(&s_ComProSts[pdrv],&s_LossTrip[pdrv]);//失压跳闸
                            fewVolLock_ctrl(&s_ComProSts[pdrv],&s_FewVolLock[pdrv]);//残压闭锁
							doubleVol_ctrl(&s_ComProSts[pdrv],&s_DoubleVol[pdrv]);//双侧有压禁止合闸
                        }
                    }
					iACC_ctrl(&s_ComProSts[pdrv],&s_IACC[pdrv]);//后加速
                }
				else
				{
					addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.openingLocked,OFF);
				}
                rest_ctrl(pdrv,&s_ComProSts[pdrv],&s_Rest[pdrv]);//复位
                iACC_rest(&s_ComProSts[pdrv],&s_IACC[pdrv]);//后加速
                overcur_rest(&s_ComProSts[pdrv],&s_Overcur[pdrv]);//过流
                overcurI0_rest(&s_ComProSts[pdrv],&s_OvercurI0[pdrv]);//零序过流
                secondaryRecloseLock_rest(&s_ComProSts[pdrv],&s_SecondaryRecloseLock[pdrv]);//二次重合闸闭锁
                reclose_rest(&s_ComProSts[pdrv],&s_Reclose[pdrv]);//重合闸
                recloseI0_rest(&s_ComProSts[pdrv],&s_RecloseI0[pdrv]);//零序重合闸
                state_judge_rest(&s_ComProSts[pdrv],&s_stateJudge[pdrv]);//状态复位
                lossTrip_rest(&s_ComProSts[pdrv],&s_LossTrip[pdrv]);//失压跳闸
                getVolClose_rest(&s_ComProSts[pdrv],&s_GetVolClose[pdrv]);//得电合闸
                openCloseLocking_rest(&s_ComProSts[pdrv],&s_OpenCloseLocking[pdrv]);//分合闸闭锁
                fewVolLock_rest(&s_ComProSts[pdrv],&s_FewVolLock[pdrv]);//残压闭锁
                doubleVol_rest(&s_ComProSts[pdrv],&s_DoubleVol[pdrv]);//双侧有压禁止合闸
                fewVolLock_rest(&s_ComProSts[pdrv],&s_FewVolLock[pdrv]);//残压闭锁
                SingleLossClose_rest(&s_ComProSts[pdrv],&s_SingleLossClose[pdrv]);//单侧失压合闸
                break;
        }
    }
}

/**
  * @Description: 手动复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void MainProtectCtrlReset(uint8_t pdrv)
{
    s_Rest[pdrv].valstr.flag |= MANRESETFLAG;//复位标志
}


/* END OF FILE ---------------------------------------------------------------*/

