/**
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_sdram.c
  * @brief:     The driver of SDRAM,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    Lexun
  * @date:      2009-01-05
  * @update:    [2017-11-22][Lexun][make the code clean up]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include "drv_sdram.h"
#include "stm32f4xx_ll_fmc.h"
#include <rtdevice.h>
#include "board.h"
#include "drv_mpu.h"
#include "malloc.h"


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static SDRAM_HandleTypeDef hsdram1;
static FMC_SDRAM_CommandTypeDef command;


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : Perform the SDRAM exernal memory inialization sequence
  * @param : [hsdram] SDRAM handle
  * @param : [Command] Pointer to SDRAM command structure  
	* @return: None. 
  */  
static void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
    __IO uint32_t tmpmrd =0;
    /* Step 3:  Configure a clock configuration enable command */
    Command->CommandMode 		 = FMC_SDRAM_CMD_CLK_ENABLE;
    Command->CommandTarget 		 = FMC_SDRAM_CMD_TARGET_BANK1;
    Command->AutoRefreshNumber 	 = 1;
    Command->ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

    /* Step 4: Insert 100 ms delay */
    /* interrupt is not enable, just to delay some time. */
    for (tmpmrd = 0; tmpmrd < 0xfffff; tmpmrd ++) ;

    /* Step 5: Configure a PALL (precharge all) command */
    Command->CommandMode 		 = FMC_SDRAM_CMD_PALL;
    Command->CommandTarget 	     = FMC_SDRAM_CMD_TARGET_BANK1;
    Command->AutoRefreshNumber 	 = 1;
    Command->ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

    /* Step 6 : Configure a Auto-Refresh command */
    Command->CommandMode 			 = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    Command->CommandTarget 		 = FMC_SDRAM_CMD_TARGET_BANK1;
    Command->AutoRefreshNumber 	 = 8;
    Command->ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

    /* Step 7: Program the external memory mode register */
    tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
                       SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                       SDRAM_MODEREG_CAS_LATENCY_3           |
                       SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                       SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    Command->CommandTarget 		 = FMC_SDRAM_CMD_TARGET_BANK1;
    Command->AutoRefreshNumber 	 = 1;
    Command->ModeRegisterDefinition = tmpmrd;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

    /* Step 8: Set the refresh rate counter */
    /* (15.62 us x Freq) - 20 */
    /* Set the device refresh counter */
    HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT);
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : SDRAM MSP Initialization
  *          This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param : [hsdram] SDRAM handle pointer
  * @return: None. 
  */  
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef  GPIO_Init_Structure;

    /* #1- Enable peripherals and GPIO Clocks */
    /* Enable GPIO clocks */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
	
    /* Enable FMC clock */
    __HAL_RCC_FMC_CLK_ENABLE();

    /* #2- Configure peripheral GPIO */
    /* GPIOs Configuration */
/*
 +-------------------+--------------------+--------------------+--------------------+
 +                       SDRAM pins assignment                                      +
 +-------------------+--------------------+--------------------+--------------------+
 | PD0  <-> FMC_D2   | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0    | PG0  <-> FMC_A10   |
 | PD1  <-> FMC_D3   | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1    | PG1  <-> FMC_A11   |
 | PD8  <-> FMC_D13  | PE7  <-> FMC_D4    | PF2  <-> FMC_A2    | PG2  <-> FMC_A12   |
 | PD9  <-> FMC_D14  | PE8  <-> FMC_D5    | PF3  <-> FMC_A3    | PG4  <-> FMC_BA0   |
 | PD10 <-> FMC_D15  | PE9  <-> FMC_D6    | PF4  <-> FMC_A4    | PG5  <-> FMC_BA1   |
 | PD14 <-> FMC_D0   | PE10 <-> FMC_D7    | PF5  <-> FMC_A5    | PG8  <-> FMC_SDCLK |
 | PD15 <-> FMC_D1   | PE11 <-> FMC_D8    | PF11 <-> FMC_NRAS  | PG15 <-> FMC_NCAS  |
 +-------------------| PE12 <-> FMC_D9    | PF12 <-> FMC_A6    |--------------------+
                     | PE13 <-> FMC_D10   | PF13 <-> FMC_A7    |
                     | PE14 <-> FMC_D11   | PF14 <-> FMC_A8    |
                     | PE15 <-> FMC_D12   | PF15 <-> FMC_A9    |
 +-------------------+--------------------+--------------------+
 | PC3 <-> FMC_SDCKE0|
 | PC2 <-> FMC_SDNE0 |
 | PC0 <-> FMC_SDNWE |
 +-------------------+
*/
    /* Common GPIO configuration */
    GPIO_Init_Structure.Mode  = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init_Structure.Pull  = GPIO_PULLUP;
    GPIO_Init_Structure.Alternate = GPIO_AF12_FMC;

    /* GPIOC configuration */
    GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3;
    HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

    /* GPIOD configuration */
    GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1  | GPIO_PIN_8 |
                              GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 |
                              GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

    /* GPIOE configuration */
    GPIO_Init_Structure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_7 |
                              GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 |
                              GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
                              GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

    /* GPIOF configuration */
    GPIO_Init_Structure.Pin = GPIO_PIN_0  | GPIO_PIN_1 | GPIO_PIN_2 |
                              GPIO_PIN_3  | GPIO_PIN_4 | GPIO_PIN_5 |
                              GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
                              GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

    /* GPIOG configuration */
    GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 |
                              GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);
}

/**
  * @brief : SDRAM MSP De-Initialization
  *          This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to their default state
  * @param : [hsdram] SDRAM handle pointer
  * @return: None. 
  */  
void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef *hsdram)
{
    /* Disable peripherals and GPIO Clocks */

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_1  | GPIO_PIN_8 |\
                           GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 |\
                           GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_7 |\
                           GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 |\
                           GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |\
                           GPIO_PIN_14 | GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0  | GPIO_PIN_1 | GPIO_PIN_2 |\
                           GPIO_PIN_3  | GPIO_PIN_4 | GPIO_PIN_5 |\
                           GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |\
                           GPIO_PIN_14 | GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 |\
                           GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15);
}

/**
  * @brief : Configures the FMC and GPIOs to interface with the SDRAM memory.
  *          This function must be called before any read/write operation
  *          on the SDRAM.
  * @param : None.
  * @return: None. 
  */  
int rt_hw_sdram_init(void)
{
    FMC_SDRAM_TimingTypeDef SDRAM_Timing;

    /* SDRAM device configuration */
    hsdram1.Instance = FMC_SDRAM_DEVICE;

    /* Timing configuration for 90 MHz of SD clock frequency (180MHz/2) */
    SDRAM_Timing.LoadToActiveDelay    = 2; /* TMRD: 2 Clock cycles */
    SDRAM_Timing.ExitSelfRefreshDelay = 8; /* TXSR: min=70ns (6x11.90ns) */
    SDRAM_Timing.SelfRefreshTime      = 6; /* TRAS: min=42ns (4x11.90ns) max=120k (ns) */
    SDRAM_Timing.RowCycleDelay        = 6; /* TRC:  min=63 (6x11.90ns) */
    SDRAM_Timing.WriteRecoveryTime    = 2; /* TWR:  2 Clock cycles */
    SDRAM_Timing.RPDelay              = 2; /* TRP:  15ns => 2x11.90ns */
    SDRAM_Timing.RCDDelay             = 2; /* TRCD: 15ns => 2x11.90ns */

    hsdram1.Init.SDBank             = FMC_SDRAM_BANK1;
    hsdram1.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;
    hsdram1.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_13;
    hsdram1.Init.MemoryDataWidth    = SDRAM_MEMORY_WIDTH;
    hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    hsdram1.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;
    hsdram1.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    hsdram1.Init.SDClockPeriod      = SDCLOCK_PERIOD;
    hsdram1.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
    hsdram1.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_1;

    /* Initialize the SDRAM controller */
    if (HAL_SDRAM_Init(&hsdram1, &SDRAM_Timing) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* Program the SDRAM external device */
    SDRAM_Initialization_Sequence(&hsdram1, &command);
    	
    return(0);
}

INIT_BOARD_EXPORT(rt_hw_sdram_init);


/* END OF FILE ---------------------------------------------------------------*/

