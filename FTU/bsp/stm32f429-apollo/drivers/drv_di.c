/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_di.c
  * @brief:     The driver of di.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2017-09-05
  * @updata  : [2018-01-16][Sunxr][modify the driver]
  */
	
	
/* INCLUDES FILSE ------------------------------------------------------------*/
#include "drv_di.h"
#include "drv_gpio.h"
#include "string.h"
#include "common_data.h"


/* PUBLIC VARIABLES ----------------------------------------------------------*/
DiStr g_DiCollect;


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static rt_device_t rt_di_dev;


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : di GPIO initialize
  * @param : [none].
  * @return: [0] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */  
int rt_hw_di_init(void)
{
    rt_di_dev = rt_device_find(RT_PIN_NAME);	
    
    if (rt_di_dev == RT_NULL)
    {
        DI_PRINTF("di pin device is not found !!!"); 
    }
    else
    {
        DI_PRINTF("di pin device is found ~~~"); 
    }
		
    return (0);
}
INIT_DEVICE_EXPORT(rt_hw_di_init);

/**
  * @brief : double-point detection
  * @param : [shakingTime]-eliminate shaking time.
  * @return: [none] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */ 
static void rt_hw_double_point_check(float shaking_time)
{
    g_DiCollect.doubleState = g_DiCollect.state[0] & 0x03;

    if ((g_DiCollect.doubleState == 0x00 || g_DiCollect.doubleState == 0x03))
    {		
        if (g_pFixedValue[CONTROL_LOOP_ABNOMAL_ENABLE] == SWITCH_OFF)
        {
            return;
        }
            
        if (g_TelesignalDB[g_TelesignalAddr.deviceFault] == OFF && g_DiCollect.doubleState == 0x00)
        {
            /* 进入装置故障判断 */
            g_DiCollect.deviceFalutCounter++;
            if (g_DiCollect.deviceFalutCounter > DEVICE_FAULT_TIME)
            {
                g_DiCollect.deviceFalutCounter = 0;

                DBWriteSOE(g_TelesignalAddr.deviceFault, ON);
                g_DiCollect.doubleStateLast = g_DiCollect.doubleState;
            }
        }

        if (g_DiCollect.doubleState == 0x03 && g_TelesignalDB[g_TelesignalAddr.controlLoopDisconnection] == OFF)
        {
            /* 控制回路断线 */
            g_DiCollect.disconnectCounter++;

            if (g_DiCollect.disconnectCounter > LOOP_DISCONNECT_TIME)
            {
                g_DiCollect.disconnectCounter = 0;

                DBWriteSOE(g_TelesignalAddr.controlLoopDisconnection, ON);
                g_DiCollect.doubleStateLast = g_DiCollect.doubleState;
            }
        }
    }
    else if (g_DiCollect.doubleState == 0x01 || g_DiCollect.doubleState == 0x02)
    {
        if (g_TelesignalDB[g_TelesignalAddr.controlLoopDisconnection] == ON)
        {
            DBWriteSOE(g_TelesignalAddr.controlLoopDisconnection, OFF);
        }

        g_TelesignalDB[g_TelesignalAddr.controlLoopDisconnection] = OFF;

        if (g_DiCollect.doubleState != g_DiCollect.doubleStateLast)
        {
            g_DiCollect.doubleCounter++;
            if (g_DiCollect.doubleCounter > shaking_time)
            {
                g_DiCollect.doubleCounter = 0;

                DBWriteSOE(g_TelesignalAddr.doubleSwitch, ~g_DiCollect.doubleState & 0x03);
                g_DiCollect.doubleStateLast = g_DiCollect.doubleState;
            }
        }
    }
}

/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : di detection
  * @param : [clock]-call cycle
  * @return: [none] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */ 
void rt_hw_di_check_task(rt_uint8_t clock)
{
    rt_uint8_t i;
    static float s_shaking_time = 0;
		
	pin_status[INDEX_KI_CS1].status = GPIO_PIN_RESET;			
	rt_device_write(rt_di_dev, 0, &pin_status[INDEX_KI_CS1], sizeof(struct rt_device_pin_status));	

    for (i = 0; i < DI_NUM; i++)
    {
        rt_device_read(rt_di_dev, 0, &pin_status[INDEX_DI1 + i], sizeof(struct rt_device_pin_status));	
		if(zkDigitalInputCfg[i].negate)
		{
			pin_status[INDEX_DI1 + i].status = (~pin_status[INDEX_DI1 + i].status)&0x00000001;
		}
    }

	pin_status[INDEX_KI_CS1].status = GPIO_PIN_SET;			
	rt_device_write(rt_di_dev, 0, &pin_status[INDEX_KI_CS1], sizeof(struct rt_device_pin_status));		
	
    s_shaking_time = (uint32_t)g_Parameter[DI_SHAKING_TIME] / clock;

    for (i = 0; i < DI_NUM; i++)
    {
        if (pin_status[INDEX_DI1 + i].status == zkDigitalInputCfg[i].lastVal)
        {
            zkDigitalInputCfg[i].count = 0;
        }
        else if(zkDigitalInputCfg[i].enable)
        {
            zkDigitalInputCfg[i].count++;

            if (zkDigitalInputCfg[i].count >= s_shaking_time)
            {
                zkDigitalInputCfg[i].count = 0;			
				
                DBWriteSOE(*(zkDigitalInputCfg[i].pAddr),(pin_status[INDEX_DI1 + i].status ? OFF : ON));
                
                zkDigitalInputCfg[i].lastVal = pin_status[INDEX_DI1 + i].status;
            }
        }
    }	

    rt_hw_double_point_check(s_shaking_time);	
/* 第二片 ------------------------------------------------------------------------------------------------------------------------*/
	pin_status[INDEX_KI_CS2].status = GPIO_PIN_RESET;			
	rt_device_write(rt_di_dev, 0, &pin_status[INDEX_KI_CS2], sizeof(struct rt_device_pin_status));		

    for (i = 0; i < DI_NUM; i++)
    {
        rt_device_read(rt_di_dev, 0, &pin_status[INDEX_DI1 + i], sizeof(struct rt_device_pin_status));	
		if(zkDigitalInputCfg[i + DI_NUM].negate)
		{
			pin_status[INDEX_DI1 + i].status = (~pin_status[INDEX_DI1 + i].status)&0x00000001;
		}
    }

	pin_status[INDEX_KI_CS2].status = GPIO_PIN_SET;			
	rt_device_write(rt_di_dev, 0, &pin_status[INDEX_KI_CS2], sizeof(struct rt_device_pin_status));	
    
    s_shaking_time = (uint32_t)g_Parameter[DI_SHAKING_TIME] / clock;

    for (i = 0; i < DI_NUM; i++)
    {
        if (pin_status[INDEX_DI1 + i].status == zkDigitalInputCfg[i + DI_NUM].lastVal)
        {
            zkDigitalInputCfg[i + DI_NUM].count = 0;
        }
        else if(zkDigitalInputCfg[i + DI_NUM].enable)
        {
            zkDigitalInputCfg[i + DI_NUM].count++;

            if (zkDigitalInputCfg[i + DI_NUM].count >= s_shaking_time)
            {
                zkDigitalInputCfg[i + DI_NUM].count = 0;			
				
                DBWriteSOE(*(zkDigitalInputCfg[i + DI_NUM].pAddr),(pin_status[INDEX_DI1 + i].status ? OFF : ON));
                
                zkDigitalInputCfg[i + DI_NUM].lastVal = pin_status[INDEX_DI1 + i].status;
            }
        }
    }	
    
    rt_device_read(rt_di_dev, 0, &pin_status[INDEX_MCU_POWER_ALARM_DI], sizeof(struct rt_device_pin_status));
    DBWriteSOE(g_TelesignalAddr.devicePowerDown, (pin_status[INDEX_MCU_POWER_ALARM_DI].status) ? ON : OFF);
}

/* END OF FILE ---------------------------------------------------------------*/
