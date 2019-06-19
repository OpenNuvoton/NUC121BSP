/******************************************************************************//**
 * @file     DataFlashProg.h
 * @version  V3.00
 * @brief    Data flash programming driver header
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __DATA_FLASH_PROG_H__
#define __DATA_FLASH_PROG_H__

#if defined(__GNUC__)

    #define MASS_STORAGE_OFFSET       0x00005800  /* To avoid the code to write APROM */
    #define DATA_FLASH_STORAGE_SIZE   (10*1024)   /* Configure the DATA FLASH storage size. To pass USB-IF MSC Test, it needs > 64KB */
    /* Windows 7 will take about 20 KB to do formatting. We cannot format the flash due to the free space is not enough. */
#else //for KEIL/IAR
    #define MASS_STORAGE_OFFSET       0x00002C00  /* To avoid the code to write APROM */
    #define DATA_FLASH_STORAGE_SIZE   (21*1024)   /* Configure the DATA FLASH storage size. To pass USB-IF MSC Test, it needs > 64KB */
    /* Windows 7 will take about 20KB to do formatting. We only get free space, 2 KB, to use after the flash is formatted */
#endif



#define FLASH_PAGE_SIZE           512
#define BUFFER_PAGE_SIZE          512

#endif  /* __DATA_FLASH_PROG_H__ */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
