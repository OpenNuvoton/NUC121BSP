/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief    Show the usage of GPIO interrupt function.
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

/**
 * @brief       PortA/PortB IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The PortA/PortB default IRQ, declared in startup_NUC121.s.
 */
void GPAB_IRQHandler(void)
{
    /* To check if PB.3 interrupt occurred */
    if (PB->INTSRC & BIT3)
    {
        PB->INTSRC = BIT3;
        printf("PB.3 INT occurred.\n");
    }
    else
    {
        /* Un-expected interrupt. Just clear all PORTA, PORTB interrupts */
        PA->INTSRC = (PA->INTSRC);
        PB->INTSRC = (PB->INTSRC);
        printf("Un-expected interrupts.\n");
    }
}

/**
 * @brief       PortC/PortD/PortE/PortF IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The PortC/PortD/PortE/PortF default IRQ, declared in startup_NUC121.s.
 */
void GPCDEF_IRQHandler(void)
{
    /* To check if PC.4 interrupt occurred */
    if (PC->INTSRC & BIT4)
    {
        PC->INTSRC = BIT4;
        printf("PC.4 INT occurred.\n");
    }
    else
    {
        /* Un-expected interrupt. Just clear all PORTC, PORTD, PORTE and PORTF interrupts */
        PC->INTSRC = (PC->INTSRC);
        PD->INTSRC = (PD->INTSRC);
        PE->INTSRC = (PE->INTSRC);
        PF->INTSRC = (PF->INTSRC);
        printf("Un-expected interrupts.\n");
    }
}

void SYS_Init(void)
{

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable HIRC clock (Internal RC 48MHz) */
    CLK->PWRCTL |= CLK_PWRCTL_HIRCEN_Msk;

    /* Wait for HIRC clock ready */
    while (!(CLK->STATUS & CLK_STATUS_HIRCSTB_Msk));

    /* Select HCLK clock source as HIRC and HCLK clock divider as 1 */
    CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLKSEL_Msk)) | CLK_CLKSEL0_HCLKSEL_HIRC;
    CLK->CLKDIV0 = (CLK->CLKDIV0 & (~CLK_CLKDIV0_HCLKDIV_Msk)) | CLK_CLKDIV0_HCLK(1);

    /* Enable UART module clock */
    CLK->APBCLK0 |= CLK_APBCLK0_UART0CKEN_Msk;

    /* Select UART module clock source as HIRC/2 and UART module clock divider as 1 */
    CLK->CLKSEL1 = (CLK->CLKSEL1 & (~CLK_CLKSEL1_UARTSEL_Msk)) | CLK_CLKSEL1_UARTSEL_HIRC_DIV2;
    CLK->CLKDIV0 = (CLK->CLKDIV0 & (~CLK_CLKDIV0_UARTDIV_Msk)) | CLK_CLKDIV0_UART(1);

    /* Update System Core Clock */
    PllClock        = 0;                 // PLL
    SystemCoreClock = __HIRC;            // HCLK
    CyclesPerUs     = __HIRC / 1000000;  // For CLK_SysTickDelay()

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set PB multi-function pins for UART0 RXD(PB.0) and TXD(PB.1) */
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB0MFP_Msk)) | SYS_GPB_MFPL_PB0MFP_UART0_RXD;
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB1MFP_Msk)) | SYS_GPB_MFPL_PB1MFP_UART0_TXD;

}

void UART0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 */
    SYS->IPRST1 |=  SYS_IPRST1_UART0RST_Msk;
    SYS->IPRST1 &= ~SYS_IPRST1_UART0RST_Msk;

    /* Configure UART0 and set UART0 baud rate */
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_MODE2_DIVIDER(__HIRC_DIV2, 115200);
    UART0->LINE = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART0 for printf */
    UART0_Init();

    printf("\n\nCPU @ %d Hz\n", SystemCoreClock);
    printf("+------------------------------------------------+\n");
    printf("|    GPIO PB.3 and PC.4 Interrupt Sample Code    |\n");
    printf("+------------------------------------------------+\n\n");

    /*-----------------------------------------------------------------------------------------------------*/
    /* GPIO Interrupt Function Test                                                                        */
    /*-----------------------------------------------------------------------------------------------------*/
    printf("PB.3 and PC.4 are used to test interrupt ......\n");

    /* Configure PB.3 as Input mode and enable interrupt by rising edge trigger */
    PB->MODE = (PB->MODE & (~GPIO_MODE_MODE3_Msk)) | (GPIO_MODE_INPUT << GPIO_MODE_MODE3_Pos);
    PB->INTTYPE |= (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE3_Pos);
    PB->INTEN |= GPIO_INTEN_RHIEN3_Msk;
    NVIC_EnableIRQ(GPAB_IRQn);

    /*  Configure PC.4 as Quasi-bidirection mode and enable interrupt by falling edge trigger */
    PC->MODE = (PC->MODE & (~GPIO_MODE_MODE4_Msk)) | (GPIO_MODE_QUASI << GPIO_MODE_MODE4_Pos);
    PC->INTTYPE |= (GPIO_INTTYPE_EDGE << GPIO_INTTYPE_TYPE4_Pos);
    PC->INTEN |= GPIO_INTEN_FLIEN4_Msk;
    NVIC_EnableIRQ(GPCDEF_IRQn);

    /* Enable interrupt de-bounce function and select de-bounce sampling cycle time is 1024 clocks of LIRC clock */
    GPIO->DBCTL = (GPIO_DBCTL_ICLK_ON | GPIO_DBCTL_DBCLKSRC_LIRC | GPIO_DBCTL_DBCLKSEL_1024);
    PB->DBEN |= BIT3;
    PC->DBEN |= BIT4;

    /* Waiting for interrupts */
    while (1);
}

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
