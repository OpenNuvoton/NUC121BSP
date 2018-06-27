/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief    Capture the BPWM0 Channel 0 waveform by BPWM1 Channel 2.
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define PLLCTL_SETTING  CLK_PLLCTL_100MHz_HIRC_DIV2

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/* Capture function to calculate the input waveform information                         */
/* u32Count[4] : Keep the internal counter value when input signal rising / falling     */
/*               happens                                                                */
/*                                                                                      */
/* time    A    B     C     D                                                           */
/*           ___   ___   ___   ___   ___   ___   ___   ___                              */
/*      ____|   |_|   |_|   |_|   |_|   |_|   |_|   |_|   |_____                        */
/* index              0 1   2 3                                                         */
/*                                                                                      */
/* The capture internal counter down count from 0x10000, and reload to 0x10000 after    */
/* input signal falling happens (Time B/C/D)                                            */
/*--------------------------------------------------------------------------------------*/
void CalPeriodTime()
{
    uint16_t u32Count[4];
    uint32_t u32i;
    uint16_t u16RisingTime, u16FallingTime, u16HighPeriod, u16LowPeriod, u16TotalPeriod;

    /* Clear Capture Rising Indicator (Time A) */
    BPWM1->CAPIF = BPWM_CAPIF_CRLIF2_Msk;

    /* Wait for Capture Falling Indicator  */
    while ((BPWM1->CAPIF & BPWM_CAPIF_CFLIF2_Msk) == 0);

    /* Clear Capture Falling Indicator (Time B)*/
    BPWM1->CAPIF = BPWM_CAPIF_CFLIF2_Msk;

    u32i = 0;

    while (u32i < 4)
    {
        /* Wait for Capture Falling Indicator */
        while ((BPWM1->CAPIF & BPWM_CAPIF_CFLIF2_Msk) == 0);

        /* Clear Capture Falling and Rising Indicator */
        BPWM1->CAPIF = BPWM_CAPIF_CFLIF2_Msk | BPWM_CAPIF_CRLIF2_Msk;

        /* Get Capture Falling Latch Counter Data */
        u32Count[u32i++] = BPWM_GET_CAPTURE_FALLING_DATA(BPWM1, 2);

        /* Wait for Capture Rising Indicator */
        while ((BPWM1->CAPIF & BPWM_CAPIF_CRLIF2_Msk) == 0);

        /* Clear Capture Rising Indicator */
        BPWM1->CAPIF = BPWM_CAPIF_CRLIF2_Msk;

        /* Get Capture Rising Latch Counter Data */
        u32Count[u32i++] = BPWM_GET_CAPTURE_RISING_DATA(BPWM1, 2);
    }

    u16RisingTime = u32Count[1];

    u16FallingTime = u32Count[0];

    u16HighPeriod = u32Count[1] - u32Count[2];

    u16LowPeriod = 0x10000 - u32Count[1];

    u16TotalPeriod = 0x10000 - u32Count[2];

    printf("\nBPWM generate: \nHigh Period=14999 ~ 15001, Low Period=34999 ~ 35001, Total Period=49999 ~ 50001\n");
    printf("\nCapture Result: Rising Time = %d, Falling Time = %d \nHigh Period = %d, Low Period = %d, Total Period = %d.\n\n",
           u16RisingTime, u16FallingTime, u16HighPeriod, u16LowPeriod, u16TotalPeriod);

    if ((u16HighPeriod < 14999) || (u16HighPeriod > 15001) || (u16LowPeriod < 34999) || (u16LowPeriod > 35001) || (u16TotalPeriod < 49999) || (u16TotalPeriod > 50001))
        printf("Capture Test Fail!!\n");
    else
        printf("Capture Test Pass!!\n");
}


void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable HIRC clock */
    CLK->PWRCTL |= CLK_PWRCTL_HIRCEN_Msk;

    /* Waiting for HIRC clock ready */
    while (!(CLK->STATUS & CLK_STATUS_HIRCSTB_Msk));

    /* Select HCLK clock source as HIRC and and HCLK clock divider as 1 */
    CLK->CLKSEL0 = (CLK->CLKSEL0 & ~CLK_CLKSEL0_HCLKSEL_Msk) | CLK_CLKSEL0_HCLKSEL_HIRC;
    CLK->CLKDIV0 = (CLK->CLKDIV0 & ~CLK_CLKDIV0_HCLKDIV_Msk) | CLK_CLKDIV0_HCLK(1);

    /* Enable PLL */
    CLK->PLLCTL = PLLCTL_SETTING;

    /* Waiting for PLL clock ready */
    while (!(CLK->STATUS & CLK_STATUS_PLLSTB_Msk));

    /*Select HCLK clock source as PLL */
    CLK->CLKSEL0 = (CLK->CLKSEL0 & ~CLK_CLKSEL0_HCLKSEL_Msk) | CLK_CLKSEL0_HCLKSEL_PLL;

    /* Enable BPWM module clock */
    CLK->APBCLK0 |= CLK_APBCLK0_BPWM0CKEN_Msk;
    CLK->APBCLK0 |= CLK_APBCLK0_BPWM1CKEN_Msk;

    /*---------------------------------------------------------------------------------------------------------*/
    /* BPWM clock frequency configuration                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Select HCLK clock divider as 2 */
    CLK->CLKDIV0 = (CLK->CLKDIV0 & ~CLK_CLKDIV0_HCLKDIV_Msk) | CLK_CLKDIV0_HCLK(2);

    /* BPWM clock frequency can be set equal or double to HCLK by choosing case 1 or case 2 */
    /* case 1.BPWM clock frequency is set equal to HCLK: select BPWM module clock source as PCLK */
    CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_BPWM0SEL_Msk) | CLK_CLKSEL1_BPWM0SEL_PCLK0;
    CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_BPWM1SEL_Msk) | CLK_CLKSEL1_BPWM1SEL_PCLK1;

    /* case 2.BPWM clock frequency is set double to HCLK: select BPWM module clock source as PLL */
    //CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_BPWM0SEL_Msk) | CLK_CLKSEL1_BPWM0SEL_PLL;
    //CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_BPWM1SEL_Msk) | CLK_CLKSEL1_BPWM1SEL_PLL;
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable UART module clock */
    CLK->APBCLK0 |= CLK_APBCLK0_UART0CKEN_Msk;

    /* Select UART module clock source as HIRC_DIV2 and UART module clock divider as 1 */
    CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UARTSEL_Msk) | CLK_CLKSEL1_UARTSEL_HIRC_DIV2;

    /* Reset BPWM module */
    SYS->IPRST1 |= SYS_IPRST1_BPWM0RST_Msk;
    SYS->IPRST1 &= ~SYS_IPRST1_BPWM0RST_Msk;
    SYS->IPRST1 |= SYS_IPRST1_BPWM1RST_Msk;
    SYS->IPRST1 &= ~SYS_IPRST1_BPWM1RST_Msk;

    /* Update System Core Clock */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB0MFP_Msk | SYS_GPB_MFPL_PB1MFP_Msk);
    SYS->GPB_MFPL |= (SYS_GPB_MFPL_PB0MFP_UART0_RXD | SYS_GPB_MFPL_PB1MFP_UART0_TXD);

    /* Set PB multi-function pins for BPWM0 Channel 0 */
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB7MFP_Msk));
    SYS->GPB_MFPL |= SYS_GPB_MFPL_PB7MFP_BPWM0_CH0;
    /* Set PF multi-function pins for BPWM1 Channel 2 */
    SYS->GPF_MFPL = (SYS->GPF_MFPL & (~SYS_GPF_MFPL_PF1MFP_Msk));
    SYS->GPF_MFPL |= SYS_GPF_MFPL_PF1MFP_BPWM1_CH2;
}

void UART0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART IP */
    SYS->IPRST1 |=  SYS_IPRST1_UART0RST_Msk;
    SYS->IPRST1 &= ~SYS_IPRST1_UART0RST_Msk;

    /* Configure UART0 and set UART0 baud rate */
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_MODE2_DIVIDER(__HIRC_DIV2, 115200);
    UART0->LINE = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1;
}


/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
    /* Init System, IP clock and multi-function I/O
       In the end of SYS_Init() will issue SYS_LockReg()
       to lock protected register. If user want to write
       protected register, please issue SYS_UnlockReg()
       to unlock protected register if necessary */

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, IP clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART to 115200-8n1 for print message */
    UART0_Init();

    printf("\n\nCPU @ %dHz(PLL@ %dHz)\n", SystemCoreClock, PllClock);
    printf("BPWM0 clock is from %s\n", (CLK->CLKSEL1 & CLK_CLKSEL1_BPWM0SEL_Msk) ? "CPU" : "PLL");
    printf("BPWM1 clock is from %s\n", (CLK->CLKSEL1 & CLK_CLKSEL1_BPWM1SEL_Msk) ? "CPU" : "PLL");
    printf("+------------------------------------------------------------------------+\n");
    printf("|                          BPWM Driver Sample Code                       |\n");
    printf("+------------------------------------------------------------------------+\n");
    printf("  This sample code will use BPWM1 channel 2 to capture\n  the signal from BPWM0 channel 0.\n");
    printf("  I/O configuration:\n");
    printf("    BPWM1 channel 2(PF.1) <--> BPWM0 channel 0(PB.7)\n\n");
    printf("Use BPWM1 Channel 2(PF.1) to capture the BPWM0 Channel 0(PB.7) Waveform\n");

    while (1)
    {
        printf("\n\nPress any key to start BPWM Capture Test\n");
        getchar();

        /*--------------------------------------------------------------------------------------*/
        /* Set the BPWM0 Channel 0 as BPWM output function.                                     */
        /*--------------------------------------------------------------------------------------*/

        /* Assume BPWM output frequency is 250Hz and duty ratio is 30%, user can calculate BPWM settings by follows.
           up counter type:
           duty ratio = (CMR)/(CNR+1)
           cycle time = CNR+1
           High level = CMR
           BPWM clock source frequency = PLL = 50000000
           (CNR+1) = BPWM clock source frequency/prescaler/BPWM output frequency
                   = 50000000/4/250 = 50000
           (Note: CNR is 16 bits, so if calculated value is larger than 65535, user should increase prescale value.)
           CNR = 49999
           duty ratio = 30% ==> (CMR)/(CNR+1) = 30%
           CMR = 15000
           Prescale value is 3 : prescaler= 4
        */

        /*Set counter as up count*/
        BPWM0->CTL1 = (BPWM0->CTL1 & ~BPWM_CTL1_CNTTYPE0_Msk);

        /*Set BPWM Timer clock prescaler*/
        BPWM_SET_PRESCALER(BPWM0, 0, 3); // Divided by 4

        /*Set BPWM Timer duty*/
        BPWM_SET_CMR(BPWM0, 0, 15000);

        /*Set BPWM Timer period*/
        BPWM_SET_CNR(BPWM0, 0, 49999);

        /* Set waveform generation */
        //BPWM0->WGCTL0 = 0x00000002;
        //BPWM0->WGCTL1 = 0x00000001;
        BPWM_SET_OUTPUT_LEVEL(BPWM0, BPWM_CH_0_MASK, BPWM_OUTPUT_HIGH, BPWM_OUTPUT_LOW, BPWM_OUTPUT_NOTHING, BPWM_OUTPUT_NOTHING);

        /* Enable BPWM Output path for BPWM0 channel 0 */
        BPWM0->POEN |= BPWM_CH_0_MASK;

        /* Enable Timer for BPWM0 channel 0 */
        BPWM0->CNTEN |= BPWM_CH_0_MASK;

        /*--------------------------------------------------------------------------------------*/
        /* Set the BPWM1 channel 2 for capture function                                         */
        /*--------------------------------------------------------------------------------------*/

        /* If input minimum frequency is 250Hz, user can calculate capture settings by follows.
           Capture clock source frequency = PLL = 50000000 in the sample code.
           (CNR+1) = Capture clock source frequency/prescaler/minimum input frequency
                   = 50000000/4/250 = 50000
           (Note: CNR is 16 bits, so if calculated value is larger than 65536, user should increase prescale value.)
           CNR = 0xFFFF
           (Note: In capture mode, user should set CNR to 0xFFFF to increase capture frequency range.)
        */

        /*Set counter as down count*/
        BPWM1->CTL1 = (BPWM1->CTL1 & ~BPWM_CTL1_CNTTYPE0_Msk) | (0x1 << BPWM_CTL1_CNTTYPE0_Pos);

        /*Set BPWM1 channel 2 Timer clock prescaler*/
        BPWM_SET_PRESCALER(BPWM1, 2, 3); // Divided by 4

        /*Set BPWM1 channel 2 Timer period*/
        BPWM_SET_CNR(BPWM1, 2, 0xFFFF);

        /* Enable capture function */
        BPWM1->CAPCTL |= BPWM_CAPCTL_CAPEN2_Msk;

        /* Enable falling capture reload */
        BPWM1->CAPCTL |= BPWM_CAPCTL_FCRLDEN2_Msk;

        /* Start */
        BPWM1->CNTEN |= BPWM_CNTEN_CNTEN0_Msk;

        /* Wait until BPWM1 channel 2 Timer start to count */
        while ((BPWM1->CNT) == 0);

        /* Enable capture input path for BPWM1 channel 2 */
        BPWM1->CAPINEN |= BPWM_CAPINEN_CAPINEN2_Msk;

        /* Capture the Input Waveform Data */
        CalPeriodTime();
        /*------------------------------------------------------------------------------------------------------------*/
        /* Stop BPWM0 channel 0 (Recommended procedure method 1)                                                      */
        /* Set BPWM Timer loaded value(Period) as 0. When BPWM internal counter(CNT) reaches to 0, disable BPWM Timer */
        /*------------------------------------------------------------------------------------------------------------*/

        /* Set BPWM0 channel 0 loaded value as 0 */
        BPWM0->PERIOD = 0;

        /* Wait until BPWM0 channel 0 Timer Stop */
        while ((BPWM0->CNT & BPWM_CNT_CNT_Msk) != 0);

        /* Disable Timer for BPWM0 channel 0 */
        BPWM0->CNTEN &= ~BPWM_CNTEN_CNTEN0_Msk;

        /* Disable BPWM Output path for BPWM0 channel 0 */
        BPWM0->POEN &= ~BPWM_CH_0_MASK;

        /*------------------------------------------------------------------------------------------------------------*/
        /* Stop BPWM1 channel 2 (Recommended procedure method 1)                                                      */
        /* Set BPWM Timer loaded value(Period) as 0. When BPWM internal counter(CNT) reaches to 0, disable BPWM Timer */
        /*------------------------------------------------------------------------------------------------------------*/

        /* Set loaded value as 0 for BPWM1 channel 2 */
        BPWM1->PERIOD = 0;

        /* Wait until BPWM1 channel 2 current counter reach to 0 */
        while ((BPWM1->CNT & BPWM_CNT_CNT_Msk) != 0);

        /* Disable Timer for BPWM1 channel 2 */
        BPWM1->CNTEN &= ~BPWM_CNTEN_CNTEN0_Msk;

        /* Disable Capture Function and Capture Input path for BPWM1 channel 2*/
        BPWM1->CAPCTL &= ~BPWM_CAPCTL_CAPEN2_Msk;
        BPWM1->CAPINEN &= ~BPWM_CAPINEN_CAPINEN2_Msk;

        /* Clear Capture Interrupt flag for BPWM1 channel 2 */
        BPWM1->CAPIF = BPWM_CAPIF_CFLIF2_Msk;

    }
}

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
