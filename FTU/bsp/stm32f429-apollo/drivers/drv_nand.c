/**
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_nand.c
  * @brief:     The driver of nand flash,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    lizhen9880
  * @date:      2017-04-10
  * @update:    [2018-02-05][Lexun][make the code clean up]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtdevice.h>
#include <string.h>
#include "drv_nand.h"

#ifdef RT_USING_NFTL
#include <nftl.h>
#endif /* RT_USING_NFTL */


/* PRVIATE FUNCTION ----------------------------------------------------------*/
static rt_err_t _nand_readid(struct rt_mtd_nand_device *device);
static rt_err_t _nand_readpage(struct rt_mtd_nand_device *device, rt_off_t page, rt_uint8_t *data, rt_uint32_t data_len, rt_uint8_t *spare, rt_uint32_t spare_len);
static rt_err_t _nand_writepage(struct rt_mtd_nand_device *device, rt_off_t page, const rt_uint8_t *data, rt_uint32_t data_len, const rt_uint8_t *spare, rt_uint32_t spare_len);
static rt_err_t _nand_pagecopy(struct rt_mtd_nand_device *device, rt_off_t src_page, rt_off_t dst_page);
static rt_err_t _nand_eraseblock(struct rt_mtd_nand_device *device, rt_uint32_t block);
static rt_err_t _nand_checkblock(struct rt_mtd_nand_device* device, rt_uint32_t block);
static rt_err_t _nand_markbad(struct rt_mtd_nand_device* device, rt_uint32_t block);


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static struct stm32f4_nand _device;
static NAND_HandleTypeDef NAND_Handler; // NAND FLASH handler

static const struct rt_mtd_nand_driver_ops ops =
{
    _nand_readid,
    _nand_readpage,
    _nand_writepage,
    _nand_pagecopy,
    _nand_eraseblock,
    _nand_checkblock,
    _nand_markbad,
};
static struct rt_mtd_nand_device _partition[1];


/* PRVIATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : the NAND delay function.
  * @param : None.
	* @return: None. 
  */  
static void NAND_Delay(volatile rt_uint32_t i)
{
    while (i > 0)
    {
        i--;
    }
}
		
/**
  * @brief : wait for RB signal.
  * @param : [rb] 0-wait for rb=0;1-wait for rb=1.
	* @return: [0] success; [1] timeout. 
  */  
static rt_uint8_t NAND_WaitRB(volatile rt_uint8_t rb)
{
    volatile rt_uint16_t time = 0;  
    while (time < 10000)
    {
        time++;
        if (NAND_RB == rb)
        {
            NAND_PRINTF("time:%d/%d  R/B:%d\n",time,10000,rb);
            return(0);
        }
    }
    NAND_PRINTF("timeOUT\n");
    return(1);
}

/**
  * @brief : read the nand status.
  * @param : None.
  * @return: return the nand falsh status.[bit0=0:success;bit0=1:error(program/erase/read)];[bit6=0:busy;bit6=1:ready] 
  */  
rt_uint8_t NAND_ReadStatus(void)
{
    volatile rt_uint8_t data = 0; 
    SET_NAND_CMD(NAND_READSTA); // send read status command
    data++;data++;data++;data++;data++;	// delay,prevent (-o2)optimization,cause errors
    data = *(volatile rt_uint8_t*)NAND_ADDRESS; // read the value.
    return(data);
}

/**
  * @brief : wait for the nand is or not ready.
  * @param : None.
  * @return: [NSTA_TIMEOUT] timeout;[NSTA_READY] ready 
  */  
static rt_uint8_t wait_for_ready(void)
{
    rt_uint8_t status = 0;
    volatile rt_uint32_t time = 0; 
	
    while (1)
    {
        status = NAND_ReadStatus();	// get the read status
        if (status & NSTA_READY)
        {
            break;
        }
        time++;
				
        if (time >= 0X1FFFF)
        {
            return(NSTA_TIMEOUT); // timeout
        }
    }  
    return(NSTA_READY);
}  

/**
  * @brief : reset the nand.
  * @param : None.
  * @return: [0] success;[others] failure 
  */  
static rt_uint8_t nand_reset(void)
{ 
    SET_NAND_CMD(NAND_RESET); // reset nand flash
	
    if (wait_for_ready() == NSTA_READY)
    {
        return(0); // success
    }
    else
    {
        return(1); // failure
    }
} 

/**
  * @brief : read the nand id.
  * @param : [mode] the command.
  * @return: [0] success;[others] failure 
  */  
static rt_uint8_t NAND_ModeSet(rt_uint8_t mode)
{   
    SET_NAND_CMD(NAND_FEATURE);
    SET_NAND_ADD(0X01);
    SET_NAND_DAT(mode);
    SET_NAND_DAT(0);
    SET_NAND_DAT(0);
    SET_NAND_DAT(0);
	
    if (wait_for_ready() == NSTA_READY)
    {
        return(0); // success
    }
    else
    {
        return(1); // failure
    }
}

/**
  * @brief : read nand flash status.
  * @param : [hnand] the nand handler.
	* @return: return the nand status. [bit0] 0:success;1:error.[bit6] 0:busy;1:ready. 
  */  
static rt_bool_t read_status(rt_uint8_t cmd)
{
    volatile rt_uint8_t value = 0; 
    SET_NAND_CMD(NAND_READSTA); // ���Ͷ�״̬����
    value++;value++;value++;value++;value++; // ����ʱ,��ֹ-O2�Ż�,���µĴ���.
    value=*(volatile rt_uint8_t*)NAND_ADDRESS; // ��ȡ״ֵ̬
    switch (cmd)
    {
        case NAND_WRITE0:
        case NAND_ERASE1:
            if (value & 0x01) // Erase/Program failure(1) or pass(0)
            {
                return (RT_FALSE);
            }
            else
            {
                return (RT_TRUE);
            }

        case NAND_AREA_TRUE1: // bit 5 and 6, Read busy(0) or ready(1)
            return (RT_TRUE);

        default:
            break;
    }

    return (RT_FALSE);
}

/**
  * @brief : read nand flash id.
  * @param : [device] the device pointer.
	* @return: [RT_EOK] success. 
  */  
static rt_err_t _nand_readid(struct rt_mtd_nand_device *device)
{
    rt_uint32_t id;  
    SET_NAND_CMD(NAND_READID); //���Ͷ�ȡID����
    SET_NAND_ADD(0X00);
    GET_NAND_DAT(_device.id[0]);//IDһ����5���ֽ�
    GET_NAND_DAT(_device.id[1]);
    GET_NAND_DAT(_device.id[2]);
    GET_NAND_DAT(_device.id[3]);
    GET_NAND_DAT(_device.id[4]);

    //þ���NAND FLASH��IDһ��5���ֽڣ�����Ϊ�˷�������ֻȡ4���ֽ����һ��32λ��IDֵ
    //����NAND FLASH�������ֲᣬֻҪ��þ���NAND FLASH����ôһ���ֽ�ID�ĵ�һ���ֽڶ���0X2C
    //�������ǾͿ����������0X2C��ֻȡ�������ֽڵ�IDֵ��
    id = ((rt_uint32_t)_device.id[1])<<24|((rt_uint32_t)_device.id[2])<<16|((rt_uint32_t)_device.id[3])<<8|_device.id[4];
    
    rt_kprintf("\nNAND ID: 0x%08X\n", id);
    return(RT_EOK);
}

/**
  * @brief : correct nand flash data.
  * @param : [generatedEcc]
  * @param : [readEcc]
  * @param : [data] the data.
	* @return: [RT_MTD_EOK] ok. 
  */  
static rt_err_t nand_datacorrect(uint32_t generatedEcc, uint32_t readEcc, uint8_t *data)
{
#define ECC_MASK28    0x0FFFFFFF          /* 28 valid ECC parity bits. */
#define ECC_MASK      0x05555555          /* 14 ECC parity bits.       */

    rt_uint32_t count, bitNum, byteAddr;
    rt_uint32_t mask;
    rt_uint32_t syndrome;
    rt_uint32_t eccP;                            /* 14 even ECC parity bits. */
    rt_uint32_t eccPn;                           /* 14 odd ECC parity bits.  */

    syndrome = (generatedEcc ^ readEcc) & ECC_MASK28;

    if (syndrome == 0)
    {
        return (RT_MTD_EOK);                  /* No errors in data. */
    }

    eccPn = syndrome & ECC_MASK;              /* Get 14 odd parity bits.  */
    eccP  = (syndrome >> 1) & ECC_MASK;       /* Get 14 even parity bits. */

    if ((eccPn ^ eccP) == ECC_MASK)           /* 1-bit correctable error ? */
    {
        bitNum = (eccP & 0x01) | ((eccP >> 1) & 0x02) | ((eccP >> 2) & 0x04);
        NAND_PRINTF("ECC bit %d\n",bitNum);
        byteAddr = ((eccP >> 6) & 0x001) |
                   ((eccP >> 7) & 0x002) |
                   ((eccP >> 8) & 0x004) |
                   ((eccP >> 9) & 0x008) |
                   ((eccP >> 10) & 0x010) |
                   ((eccP >> 11) & 0x020) |
                   ((eccP >> 12) & 0x040) |
                   ((eccP >> 13) & 0x080) |
                   ((eccP >> 14) & 0x100) |
                   ((eccP >> 15) & 0x200) |
                   ((eccP >> 16) & 0x400) ;

        data[ byteAddr ] ^= 1 << bitNum;

        return(RT_MTD_EOK);
    }

    /* Count number of one's in the syndrome. */
    count = 0;
    mask  = 0x00800000;
    while (mask)
    {
        if (syndrome & mask)
        {
            count++;
        }
        mask >>= 1;
    }

    if (count == 1) // Error in the ECC itself.
    {
        return(RT_MTD_EECC);
    }

    return(-RT_MTD_EECC); /* Unable to correct data. */

#undef ECC_MASK
#undef ECC_MASK24
}

/**
  * @brief : read nand flash page data.
  * @param : [device] the device pointer.
  * @param : [page] the flash page.
  * @param : [data] the data.
  * @param : [data_len] the length of data.
  * @param : [spare] the spare data.
  * @param : [spare_len] the length of spare.
	* @return: [RT_MTD_EOK] ok. 
  */  
static rt_err_t _nand_readpage(struct rt_mtd_nand_device *device,
                                     rt_off_t                   page,
                                     rt_uint8_t                *data,
                                     rt_uint32_t                data_len,
                                     rt_uint8_t                *spare,
                                     rt_uint32_t                spare_len)
{
    rt_uint32_t index;
    rt_uint32_t gecc, recc;
    rt_uint8_t tmp[4];
    rt_err_t result;
    rt_uint32_t i;
    page = page + device->block_start * device->pages_per_block;
    if (page/device->pages_per_block > device->block_end)
    {
        return -RT_MTD_EIO;
    }

    result = RT_MTD_EOK;
    rt_mutex_take(&_device.lock, RT_WAITING_FOREVER);

    if (data && data_len)
    {
        SET_NAND_CMD(NAND_AREA_A); //���͵�ַ
        SET_NAND_ADD((rt_uint8_t)(0&0xFF));
        SET_NAND_ADD((rt_uint8_t)(0>>8));
        SET_NAND_ADD((rt_uint8_t)(page & 0xFF));
        SET_NAND_ADD((rt_uint8_t)(page >> 8));
        SET_NAND_ADD((rt_uint8_t)(page >> 16));
        SET_NAND_CMD(NAND_AREA_TRUE1);
        //�������д����ǵȴ�R/B���ű�Ϊ�͵�ƽ����ʵ��Ҫ����ʱ���õģ��ȴ�NAND����R/B���š���Ϊ������ͨ��
        //��STM32��NWAIT����(NAND��R/B����)����Ϊ��ͨIO��������ͨ����ȡNWAIT���ŵĵ�ƽ���ж�NAND�Ƿ�׼��
        //�����ġ����Ҳ����ģ��ķ������������ٶȺܿ��ʱ���п���NAND��û���ü�����R/B��������ʾNAND��æ
        //��״̬���Ͷ�ȡ��R/B����,���ʱ��϶������ģ���ʵ��ȷʵ�ǻ����!
        NAND_WaitRB(0);			//�ȴ�RB=0 
        //����2�д����������ж�NAND�Ƿ�׼���õ�
        NAND_WaitRB(1);			//�ȴ�RB=1 
        FMC_NAND_ECC_Enable(NAND_Handler.Instance,FMC_NAND_BANK3);
        
        for (i = 0; i < data_len; i ++)
        {
            GET_NAND_DAT(data[i]);  
        }
        gecc = FMC_NAND_GetECC(NAND_Handler.Instance,(uint32_t*)&gecc,FMC_NAND_BANK3,10);

        if (data_len == PAGE_DATA_SIZE)
        {
            for (index = 0; index < ECC_SIZE; index ++)
            {
                GET_NAND_DAT(tmp[index]);
            }
                
            if (spare && spare_len)
            {
                for (i = ECC_SIZE; i < spare_len; i ++)
                {
                    GET_NAND_DAT(spare[i]);      
                }
                rt_memcpy(spare, tmp , ECC_SIZE);
            }

            recc   = (tmp[3] << 24) | (tmp[2] << 16) | (tmp[1] << 8) | tmp[0];

            if (recc != 0xFFFFFFFF && gecc != 0)
						{
                result = nand_datacorrect(gecc, recc, data);
						}

            if (result != RT_MTD_EOK)
						{
                NAND_PRINTF("page: %d, gecc %X, recc %X>",page, gecc, recc);
						}

            goto _exit;
        }
    }

    if (spare && spare_len)
    {
        SET_NAND_CMD(NAND_AREA_A); //���͵�ַ
        SET_NAND_ADD((rt_uint8_t)(PAGE_DATA_SIZE&0xFF));
        SET_NAND_ADD((rt_uint8_t)(PAGE_DATA_SIZE>>8));
        SET_NAND_ADD((rt_uint8_t)(page & 0xFF));
        SET_NAND_ADD((rt_uint8_t)(page >> 8));
        SET_NAND_ADD((rt_uint8_t)(page >> 16));
        SET_NAND_CMD(NAND_AREA_TRUE1);
        
        //�������д����ǵȴ�R/B���ű�Ϊ�͵�ƽ����ʵ��Ҫ����ʱ���õģ��ȴ�NAND����R/B���š���Ϊ������ͨ��
        //��STM32��NWAIT����(NAND��R/B����)����Ϊ��ͨIO��������ͨ����ȡNWAIT���ŵĵ�ƽ���ж�NAND�Ƿ�׼��
        //�����ġ����Ҳ����ģ��ķ������������ٶȺܿ��ʱ���п���NAND��û���ü�����R/B��������ʾNAND��æ
        //��״̬���Ͷ�ȡ��R/B����,���ʱ��϶������ģ���ʵ��ȷʵ�ǻ����!
        NAND_WaitRB(0);			//�ȴ�RB=0 
        //����2�д����������ж�NAND�Ƿ�׼���õ�
        NAND_WaitRB(1);			//�ȴ�RB=1 

        for (i = 0; i < spare_len; i ++)
        {
            GET_NAND_DAT(spare[i]);
        }

    }
_exit:
    rt_mutex_release(&_device.lock);

    return (result);
}

/**
  * @brief : write nand flash page data.
  * @param : [device] the device pointer.
  * @param : [page] the flash page.
  * @param : [data] the data.
  * @param : [data_len] the length of data.
  * @param : [spare] the spare data.
  * @param : [spare_len] the length of spare.
	* @return: [RT_MTD_EOK] ok. 
  */  
static rt_err_t _nand_writepage(struct rt_mtd_nand_device *device,
                                      rt_off_t                   page,
                                      const rt_uint8_t          *data,
                                      rt_uint32_t                data_len,
                                      const rt_uint8_t          *spare,
                                      rt_uint32_t                spare_len)
{
    rt_err_t result;
    rt_uint32_t gecc;
    rt_uint32_t i;
    
    page = page + device->block_start * device->pages_per_block;
    if (page/device->pages_per_block > device->block_end)
    {
        return -RT_MTD_EIO;
    }

    result = RT_MTD_EOK;
    rt_mutex_take(&_device.lock, RT_WAITING_FOREVER);

    if (data && data_len)
    {
        SET_NAND_CMD(NAND_WRITE0); //���͵�ַ
        SET_NAND_ADD((rt_uint8_t)(0&0xFF));
        SET_NAND_ADD((rt_uint8_t)(0>>8));
        SET_NAND_ADD((rt_uint8_t)(page & 0xFF));
        SET_NAND_ADD((rt_uint8_t)(page >> 8));
        SET_NAND_ADD((rt_uint8_t)(page >> 16));
        
        FMC_NAND_ECC_Enable(NAND_Handler.Instance,FMC_NAND_BANK3);
        
        for (i = 0; i < data_len; i ++)
        {
            SET_NAND_DAT(data[i]);     
        }
        gecc = FMC_NAND_GetECC(NAND_Handler.Instance,(uint32_t*)&gecc,FMC_NAND_BANK3,10);
        
        FMC_NAND_ECC_Disable(NAND_Handler.Instance,FMC_NAND_BANK3);

        if (data_len == PAGE_DATA_SIZE)
        {
            SET_NAND_DAT((uint8_t)gecc);
            SET_NAND_DAT((uint8_t)(gecc >> 8));
            SET_NAND_DAT((uint8_t)(gecc >> 16));
            SET_NAND_DAT((uint8_t)(gecc >> 24));

            if (spare && spare_len)
            {
                for (i = ECC_SIZE; i < spare_len; i ++)
                {
                    SET_NAND_DAT(spare[i]);     
                }
            }
            
        }
        SET_NAND_CMD(NAND_WRITE_TURE1);
        if(wait_for_ready()!=NSTA_READY)
        {
            nand_reset();
            result = -RT_MTD_EIO;//ʧ��
        }
        goto _exit;
    }

    if (spare && spare_len)
    {
        SET_NAND_CMD(NAND_WRITE0); //���͵�ַ
        SET_NAND_ADD((rt_uint8_t)(PAGE_DATA_SIZE&0xFF));
        SET_NAND_ADD((rt_uint8_t)(PAGE_DATA_SIZE>>8));
        SET_NAND_ADD((rt_uint8_t)(page & 0xFF));
        SET_NAND_ADD((rt_uint8_t)(page >> 8));
        SET_NAND_ADD((rt_uint8_t)(page >> 16));
        
        if (spare && spare_len)
        for (i = ECC_SIZE; i < spare_len; i ++)
        {
            SET_NAND_DAT(spare[i]);
        }
        SET_NAND_CMD(NAND_WRITE_TURE1);
        if(wait_for_ready()!=NSTA_READY)
        {
            nand_reset();
            result = -RT_MTD_EIO;//ʧ��
        }
    }

_exit:
    rt_mutex_release(&_device.lock);

    return (result);
}

/**
  * @brief : erase nand flash block data.
  * @param : [device] the device pointer.
  * @param : [block] the flash block.
	* @return: [RT_MTD_EOK] ok. 
  */  
static rt_err_t _nand_eraseblock(struct rt_mtd_nand_device *device, rt_uint32_t block)
{
    unsigned int blockPage;
    rt_err_t result;
    /* add the start blocks */
    block = block + device->block_start;
    blockPage = (block << 6);
    result = RT_MTD_EOK;
    
    rt_mutex_take(&_device.lock, RT_WAITING_FOREVER);

    SET_NAND_CMD(NAND_ERASE0); //���͵�ַ
    SET_NAND_ADD((rt_uint8_t)blockPage);
    SET_NAND_ADD((rt_uint8_t)(blockPage>>8));
    SET_NAND_ADD((rt_uint8_t)(blockPage>>16));
    SET_NAND_CMD(NAND_ERASE1);
	
    if (wait_for_ready() != NSTA_READY)
    {
        nand_reset();
        result = -RT_MTD_EIO;//ʧ��
    }
    rt_mutex_release(&_device.lock);
    return result;
}

/**
  * @brief : copy nand flash data.
  * @param : [device] the device pointer.
  * @param : [src_page] the source flash page.
  * @param : [dst_page] the destination flash page.
	* @return: [RT_MTD_EOK] ok. 
  */  
static rt_err_t _nand_pagecopy(struct rt_mtd_nand_device *device, rt_off_t src_page, rt_off_t dst_page)
{
    rt_err_t result = RT_MTD_EOK;
    rt_uint32_t source_block=0,dest_block=0; 
    src_page = src_page + device->block_start * device->pages_per_block;
    dst_page = dst_page + device->block_start * device->pages_per_block;
    //�ж�Դҳ��Ŀ��ҳ�Ƿ���ͬһ��plane��
    source_block=src_page/device->pages_per_block;
    dest_block=dst_page/device->pages_per_block;
    if((source_block%2)!=(dest_block%2))return RT_MTD_ESRC;	//����ͬһ��plane�� 
    
    SET_NAND_CMD(NAND_MOVEDATA_CMD0);//��������0X00
    SET_NAND_ADD((rt_uint8_t)(0&0xFF));    //����Դҳ��ַ
    SET_NAND_ADD((rt_uint8_t)(0>>8));
    SET_NAND_ADD((rt_uint8_t)(src_page & 0xFF));
    SET_NAND_ADD((rt_uint8_t)(src_page >> 8));
    SET_NAND_ADD((rt_uint8_t)(src_page >> 16));
    SET_NAND_CMD(NAND_MOVEDATA_CMD1);//��������0X35 

    //�������д����ǵȴ�R/B���ű�Ϊ�͵�ƽ����ʵ��Ҫ����ʱ���õģ��ȴ�NAND����R/B���š���Ϊ������ͨ��
    //��STM32��NWAIT����(NAND��R/B����)����Ϊ��ͨIO��������ͨ����ȡNWAIT���ŵĵ�ƽ���ж�NAND�Ƿ�׼��
    //�����ġ����Ҳ����ģ��ķ������������ٶȺܿ��ʱ���п���NAND��û���ü�����R/B��������ʾNAND��æ
    //��״̬��������ǾͶ�ȡ��R/B����,���ʱ��϶������ģ���ʵ��ȷʵ�ǻ����!���Ҳ���Խ���������
    //���뻻����ʱ����,ֻ������������Ϊ��Ч������û������ʱ�����

    result = NAND_WaitRB(0); // �ȴ�RB=0 
    if (result)
    {
        return(-RT_MTD_EIO);	//��ʱ�˳�
    }
		
    // ����2�д����������ж�NAND�Ƿ�׼���õ�
    result = NAND_WaitRB(1); // �ȴ�RB=1 
		
    if (result)
    {
        return(-RT_MTD_EIO);	//��ʱ�˳� 
    }
    
    SET_NAND_CMD(NAND_MOVEDATA_CMD2);//��������0X85
    SET_NAND_ADD((rt_uint8_t)(0 & 0xFF)); //����Ŀ��ҳ��ַ
    SET_NAND_ADD((rt_uint8_t)(0 >> 8));
    SET_NAND_ADD((rt_uint8_t)(dst_page & 0xFF));
    SET_NAND_ADD((rt_uint8_t)(dst_page >> 8));
    SET_NAND_ADD((rt_uint8_t)(dst_page >> 16));
    SET_NAND_CMD(NAND_MOVEDATA_CMD3); // ��������0X10
 
    if (wait_for_ready() != NSTA_READY)
    {
        nand_reset();
        return(-RT_MTD_EIO);//ʧ��
    }

    return(RT_MTD_EOK);
}

/**
  * @brief : check nand flash block data.
  * @param : [device] the device pointer.
  * @param : [block] the flash block data.
	* @return: [RT_MTD_EOK] ok. 
  */  
static rt_err_t _nand_checkblock(struct rt_mtd_nand_device* device, rt_uint32_t block)
{
    return (RT_MTD_EOK);
}

/**
  * @brief : mark the nand flash bad block.
  * @param : [device] the device pointer.
  * @param : [block] the flash block data.
	* @return: [RT_MTD_EOK] ok. 
  */  
static rt_err_t _nand_markbad(struct rt_mtd_nand_device* device, rt_uint32_t block)
{
    return (RT_MTD_EOK);
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : nand flash initialization.
  * @param : None.
  * @return: None. 
  */  
void NAND_Init(void)
{
    FMC_NAND_PCC_TimingTypeDef ComSpaceTiming,AttSpaceTiming;
                                              
    NAND_Handler.Instance=FMC_NAND_DEVICE;
    NAND_Handler.Init.NandBank=FMC_NAND_BANK3;                          //NAND����BANK3��
    NAND_Handler.Init.Waitfeature=FMC_NAND_PCC_WAIT_FEATURE_DISABLE;    //�رյȴ�����
    NAND_Handler.Init.MemoryDataWidth=FMC_NAND_PCC_MEM_BUS_WIDTH_8;     //8λ���ݿ��
    NAND_Handler.Init.EccComputation=FMC_NAND_ECC_DISABLE;              //��ʹ��ECC
    NAND_Handler.Init.ECCPageSize=FMC_NAND_ECC_PAGE_SIZE_2048BYTE;      //ECCҳ��СΪ2k
    NAND_Handler.Init.TCLRSetupTime=0;                                  //����TCLR(tCLR=CLE��RE����ʱ)=(TCLR+TSET+2)*THCLK,THCLK=1/180M=5.5ns
    NAND_Handler.Init.TARSetupTime=1;                                   //����TAR(tAR=ALE��RE����ʱ)=(TAR+TSET+2)*THCLK,THCLK=1/180M=5.5n��   
   
    ComSpaceTiming.SetupTime=2;         //����ʱ��
    ComSpaceTiming.WaitSetupTime=3;     //�ȴ�ʱ��
    ComSpaceTiming.HoldSetupTime=2;     //����ʱ��
    ComSpaceTiming.HiZSetupTime=1;      //����̬ʱ��
    
    AttSpaceTiming.SetupTime=2;         //����ʱ��
    AttSpaceTiming.WaitSetupTime=3;     //�ȴ�ʱ��
    AttSpaceTiming.HoldSetupTime=2;     //����ʱ��
    AttSpaceTiming.HiZSetupTime=1;      //����̬ʱ��
    
    HAL_NAND_Init(&NAND_Handler,&ComSpaceTiming,&AttSpaceTiming); 
    nand_reset(); // ��λNAND
//    delay_ms(100);
    wait_for_ready();
    NAND_ModeSet(4); // ����ΪMODE4,����ģʽ 
}

/**
  * @brief : nand flash msp initialization.
  * @param : [hnand] the nand handler.
	* @return: [RT_EOK] success; [RT_ERROR] error. 
  */  
void HAL_NAND_MspInit(NAND_HandleTypeDef *hnand)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    
    GPIO_Initure.Pin = GPIO_PIN_6;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    GPIO_Initure.Pull = GPIO_PULLUP;    
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);
	   
    GPIO_Initure.Pin = GPIO_PIN_9;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_NOPULL;         
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    GPIO_Initure.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOG, &GPIO_Initure);  
	
    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | \
                       GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_Initure.Pull = GPIO_NOPULL;              
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);

    // ��ʼ��PE7,8,9,10
    GPIO_Initure.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);
}

/**
  * @brief : nand flash initialization.
  * @param : None.
  * @return: None. 
  */  
int rt_hw_nand_init(void)
{
    NAND_Init();
    rt_mutex_init(&_device.lock, "nand", RT_IPC_FLAG_FIFO);
    /* register nand0 */
    _partition[0].page_size       = PAGE_DATA_SIZE;
    _partition[0].pages_per_block = PAGES_PER_BLOCK;
    _partition[0].plane_num       = 2;
    _partition[0].oob_size        = PAGE_OOB_SIZE;
    _partition[0].oob_free        = PAGE_OOB_SIZE - ((PAGE_DATA_SIZE) * 3 / 256);
    _partition[0].block_start     = 0;
    _partition[0].block_end       = 4095;

    _partition[0].block_total     = _partition[0].block_end - _partition[0].block_start;
    _partition[0].ops             = &ops;

//    rt_mtd_nand_register_device("nand0", &_partition[0]);
    rt_mtd_nand_register_device(RT_MTD_NAND_NAME, &_partition[0]);
    _nand_readid(&_partition[0]);

    return(RT_EOK);
}
//INIT_BOARD_EXPORT(rt_hw_nand_init);


/* END OF FILE ---------------------------------------------------------------*/

