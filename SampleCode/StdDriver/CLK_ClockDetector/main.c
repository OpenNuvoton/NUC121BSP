/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief    Show the usage of clock fail detector and clock frequency monitor function.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "stdio.h"
#include "NuMicro.h"

#define PLL_CLOCK       50000000

extern char GetChar(void);

/*---------------------------------------------------------------------------------------------------------*/
/*  Clock Fail Detector IRQ Handler                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void CLKDIRC_IRQHandler(void)
{
    uint32_t u32Reg;

    /* Unlock protected registers */
    SYS_UnlockReg();

    u32Reg = CLK->CLKDSTS;

    if (u32Reg & CLK_CLKDSTS_HXTFIF_Msk)
    {
        /* HCLK is switched to HIRC automatically if HXT clock fail interrupt is happened */
        printf("HXT Clock is stopped!      HCLK is switched to HIRC.\n");

        /* Disable HXT clock fail interrupt */
        CLK->CLKDCTL &= ~(CLK_CLKDCTL_HXTFDEN_Msk | CLK_CLKDCTL_HXTFIEN_Msk);

        /* Write 1 to clear HXT Clock fail interrupt flag */
        CLK->CLKDSTS = CLK_CLKDSTS_HXTFIF_Msk;
    }

    if (u32Reg & CLK_CLKDSTS_LXTFIF_Msk)
    {
        /* LXT clock fail interrupt is happened */
        printf("LXT Clock is stopped!\n");

        /* Disable HXT clock fail interrupt */
        CLK->CLKDCTL &= ~(CLK_CLKDCTL_LXTFIEN_Msk | CLK_CLKDCTL_LXTFDEN_Msk);

        /* Write 1 to clear LXT Clock fail interrupt flag */
        CLK->CLKDSTS = CLK_CLKDSTS_LXTFIF_Msk;
    }

    if (u32Reg & CLK_CLKDSTS_HXTFQIF_Msk)
    {
        /* HCLK should be switched to HIRC if HXT clock frequency monitor interrupt is happened */
        CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
        printf("HXT Frequency is abnormal! HCLK is switched to HIRC.\n");

        /* Disable HXT clock frequency monitor interrupt */
        CLK->CLKDCTL &= ~(CLK_CLKDCTL_HXTFQDEN_Msk | CLK_CLKDCTL_HXTFQIEN_Msk);

        /* Write 1 to clear HXT Clock frequency monitor interrupt */
        CLK->CLKDSTS = CLK_CLKDSTS_HXTFQIF_Msk;
    }

    /* Lock protected registers */
    SYS_LockReg();

}


void SYS_Init(void)
{

    /* Set XT1_OUT(PF.0) and XT1_IN(PF.1) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE0_Msk | GPIO_MODE_MODE1_Msk);

    /* Disable Digital Input Path of PF.0 and PF.1 */
    GPIO_DISABLE_DIGITAL_PATH(PF, BIT0 | BIT1);

    /* Set PF multi-function pins for X32_OUT(PF.0) and X32_IN(PF.1) */
    SYS->GPF_MFPL = (SYS->GPF_MFPL & (~SYS_GPF_MFPL_PF0MFP_Msk)) | SYS_GPF_MFPL_PF0MFP_XT_OUT;
    SYS->GPF_MFPL = (SYS->GPF_MFPL & (~SYS_GPF_MFPL_PF1MFP_Msk)) | SYS_GPF_MFPL_PF1MFP_XT_IN;

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

#if 0//NUC121: HXT and LXT are using the same pin so that only one of them can exist at the same  time.

    /* Enable HIRC and LXT clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk | CLK_PWRCTL_HXTEN);

    /* Wait for HIRC and HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk | CLK_STATUS_HXTSTB_Msk);

#else /*NUC121 tiny board is default to use LXT.*/
    /* Enable HIRC and LXT clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk | CLK_PWRCTL_LXTEN);

    /* Wait for HIRC and LXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk | CLK_STATUS_LXTSTB_Msk);
#endif
    /* Select HCLK clock source as HIRC and HCLK clock divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART module clock source as HIRC and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UARTSEL_HIRC_DIV2, CLK_CLKDIV0_UART(1));

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set PB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB0MFP_Msk | SYS_GPB_MFPL_PB1MFP_Msk);
    SYS->GPB_MFPL = SYS_GPB_MFPL_PB0MFP_UART0_RXD | SYS_GPB_MFPL_PB1MFP_UART0_TXD;


    /* Set PB multi-function pins for CLKO(PB.12) */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~SYS_GPB_MFPH_PB12MFP_Msk) | SYS_GPB_MFPH_PB12MFP_CLKO;
}

void UART0_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 */
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART0 for printf */
    UART0_Init();

    printf("\n\nCPU @ %u Hz\n", SystemCoreClock);
    printf("+-------------------------------------------------------------+\n");
    printf("|             NUC121 Clock Detector Sample Code               |\n");
    printf("+-------------------------------------------------------------+\n");
    printf("| 1. HXT clock fail interrupt will happen if HXT is stopped.  |\n");
    printf("|    HCLK clock source will be switched from HXT to HIRC.     |\n");
    printf("| 2. LXT clock fail interrupt will happen if LXT is stopped.  |\n");
    printf("+-------------------------------------------------------------+\n");
    printf("Notice!!!: User should make sure that HXT or LXT connect to XTAL pin if user want to test clock detection for HXT or LXT\n");
    printf("\nStop HXT or LXT to test.\n\n");

    /* Enable clock output, select CLKO clock source as HCLK and set clock output frequency is HCLK/4.
       HCLK clock source will be switched to HIRC if HXT stop and HCLK clock source is from HXT.
       You can check if HCLK clock source is switched to HIRC by clock output pin output frequency.
    */

    /* Output selected clock to CKO, CKO Clock = HCLK / 2^(1 + 1) */
    CLK_EnableCKO(CLK_CLKSEL2_CLKOSEL_HCLK, 1, 0);

    /* Set the HXT clock frequency monitor upper and lower boundary value.*/
    CLK->CDUPB = 260; //260 is the recommended value for upper bound.
    CLK->CDLOWB = 250;//250 is the recommended value for lower bound.

    /* Set clock fail detector function enabled and interrupt enabled */
    CLK->CLKDCTL = CLK_CLKDCTL_HXTFDEN_Msk |
                   CLK_CLKDCTL_HXTFIEN_Msk |
                   CLK_CLKDCTL_LXTFDEN_Msk |
                   CLK_CLKDCTL_LXTFIEN_Msk |
                   CLK_CLKDCTL_HXTFQDEN_Msk |
                   CLK_CLKDCTL_HXTFQIEN_Msk;

    /* Enable clock fail detector interrupt */
    NVIC_EnableIRQ(CLKDIRC_IRQn);

    /* Wait for clock fail detector interrupt happened */
    while (1);

}
