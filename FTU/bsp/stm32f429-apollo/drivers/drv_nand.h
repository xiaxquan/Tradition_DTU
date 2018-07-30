/**
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_nand.h
  * @brief:     The driver of nand flash,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    lizhen9880
  * @date:      2017-04-10
  * @update:    [2018-02-05][Lexun][make the code clean up]
  */
#ifndef __DRV_NAND_H__
#define __DRV_NAND_H__


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

	 
/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include <board.h>
#include "stm32f4xx.h"
#include <rtdevice.h>
     
 
/* DEFINES -------------------------------------------------------------------*/
//#define DEBUG // debug option

#ifdef DEBUG
#define NAND_PRINTF(...)        rt_kprintf(__VA_ARGS)
#else
#define NAND_PRINTF(...)
#endif

#define NAND_MAX_PAGE_SIZE			4096		//����NAND FLASH������PAGE��С��������SPARE������Ĭ��4096�ֽ�
#define NAND_ECC_SECTOR_SIZE		512			//ִ��ECC����ĵ�Ԫ��С��Ĭ��512�ֽ�

/* nandflash confg */
#define PAGES_PER_BLOCK         64
#define PAGE_DATA_SIZE          2048
#define PAGE_OOB_SIZE           64
#define ECC_SIZE                4

#define SET_NAND_CMD(_c)        do{*(volatile rt_uint8_t*)(NAND_ADDRESS|NAND_CMD) = _c;}while(0)
#define SET_NAND_ADD(_a)        do{*(volatile rt_uint8_t*)(NAND_ADDRESS|NAND_ADDR) = _a;}while(0)    
#define SET_NAND_DAT(_d)        do{*(volatile rt_uint8_t*)NAND_ADDRESS = _d;}while(0) 
#define GET_NAND_DAT(_d)        do{_d = *(volatile rt_uint8_t*)NAND_ADDRESS;}while(0) 

// λ������,ʵ��51���Ƶ�GPIO���ƹ���
// ����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).M4ͬM3����,ֻ�ǼĴ�����ַ����.
// IO�ڲ����궨��
#define GPIOD_IDR_Addr          (GPIOD_BASE+16) //0x40020C10 
#define BITBAND(addr, bitnum)   ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)          *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)  MEM_ADDR(BITBAND(addr, bitnum)) 
#define PDin(n)                 BIT_ADDR(GPIOD_IDR_Addr,n) // ���� 

#define NAND_RB                 PDin(6) // NAND Flash����/æ���� 

#define NAND_ADDRESS            ((rt_uint32_t)0x80000000)	// nand flash�ķ��ʵ�ַ,��NCE3,��ַΪ:0X8000 0000
#define NAND_CMD                (uint32_t)(0x010000) // ��������
#define NAND_ADDR               (uint32_t)(0x020000) // ���͵�ַ

// NAND FLASH����
#define NAND_READID             0X90 // ��IDָ��
#define NAND_FEATURE            0XEF // ��������ָ��
#define NAND_RESET              0XFF // ��λNAND
#define NAND_READSTA            0X70 // ��״̬
#define NAND_AREA_A             0X00   
#define NAND_AREA_TRUE1         0X30  
#define NAND_WRITE0             0X80
#define NAND_WRITE_TURE1        0X10
#define NAND_ERASE0             0X60
#define NAND_ERASE1             0XD0
#define NAND_MOVEDATA_CMD0      0X00
#define NAND_MOVEDATA_CMD1      0X35
#define NAND_MOVEDATA_CMD2      0X85
#define NAND_MOVEDATA_CMD3      0X10

// NAND FLASH״̬
#define NSTA_READY              0X40 // nand�Ѿ�׼����
#define NSTA_ERROR              0X01 // nand����
#define NSTA_TIMEOUT            0X02 // ��ʱ
#define NSTA_ECC1BITERR       	0X03 // ECC 1bit����
#define NSTA_ECC2BITERR       	0X04 // ECC 2bit���ϴ���

// NAND FLASH�ͺźͶ�Ӧ��ID��
#define MT29F4G08ABADA          0XDC909556 // MT29F4G08ABADA
#define MT29F16G08ABABA         0X48002689 // MT29F16G08ABABA


/* STRUCTS -------------------------------------------------------------------*/
// NAND���Խṹ��
typedef struct
{
    uint16_t page_totalsize;     	//ÿҳ�ܴ�С��main����spare���ܺ�
    uint16_t page_mainsize;      	//ÿҳ��main����С
    uint16_t page_sparesize;     	//ÿҳ��spare����С
    uint8_t  block_pagenum;      	//ÿ���������ҳ����
    uint16_t plane_blocknum;     	//ÿ��plane�����Ŀ�����
    uint16_t block_totalnum;     	//�ܵĿ�����
    uint16_t good_blocknum;      	//�ÿ�����    
    uint16_t valid_blocknum;     	//��Ч������(���ļ�ϵͳʹ�õĺÿ�����)
    uint32_t id;             		//NAND FLASH ID
    uint16_t *lut;      			   	//LUT�������߼���-�����ת��
    uint32_t ecc_hard;				//Ӳ�����������ECCֵ
    uint32_t ecc_hdbuf[NAND_MAX_PAGE_SIZE/NAND_ECC_SECTOR_SIZE];//ECCӲ������ֵ������  	
    uint32_t ecc_rdbuf[NAND_MAX_PAGE_SIZE/NAND_ECC_SECTOR_SIZE];//ECC��ȡ��ֵ������
}nand_attriute;      

struct stm32f4_nand
{
    rt_uint8_t id[5];
    struct rt_mutex lock;
    struct rt_completion comp;
};


/* PUBLIC VARIABLES ----------------------------------------------------------*/


/* PUBLIC FUNCTION -----------------------------------------------------------*/
extern int rt_hw_nand_init(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __DRV_NAND_H__ */


/* END OF FILE ---------------------------------------------------------------*/

