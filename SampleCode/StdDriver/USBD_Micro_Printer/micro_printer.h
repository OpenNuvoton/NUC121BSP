/****************************************************************************//**
 * @file     micro_printer.h
 * @version  V3.00
 * @brief    NUC121 series USB micro printer header file
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __USBD_PRINTER_H__
#define __USBD_PRINTER_H__

/* HIRC trim setting:
 *    HIRC trim reference clock is USB signal.
 *    HIRC trim operation is keep going if clock is inaccuracy.
 *    HIRC Trim retry count limitation is 512 loops.
 *    Trim value calculation is based on average difference in 4 clocks of reference clock.
 *    Enable HIRC auto trim function and trim HIRC to 48 MHz.
 */
#define DEFAULT_HIRC_TRIM_SETTING    ((0x1ul<<SYS_IRCTCTL_REFCKSEL_Pos)| \
                                      (0x0ul<<SYS_IRCTCTL_CESTOPEN_Pos)| \
                                      (0x3ul<<SYS_IRCTCTL_RETRYCNT_Pos)| \
                                      (0x0ul<<SYS_IRCTCTL_LOOPSEL_Pos) | \
                                      (0x2ul<<SYS_IRCTCTL_FREQSEL_Pos))


/* Define the vendor id and product id */
#define USBD_VID        0x0416
#define USBD_PID        0xAABB

/*!<Define CDC Class Specific Request */
#define SET_LINE_CODE           0x20
#define GET_LINE_CODE           0x21
#define SET_CONTROL_LINE_STATE  0x22

/*-------------------------------------------------------------*/
/* Define EP maximum packet size */
#define EP0_MAX_PKT_SIZE    64
#define EP1_MAX_PKT_SIZE    EP0_MAX_PKT_SIZE
#define EP2_MAX_PKT_SIZE    64
#define EP3_MAX_PKT_SIZE    64
#define EP4_MAX_PKT_SIZE    8

#define SETUP_BUF_BASE      0
#define SETUP_BUF_LEN       8
#define EP0_BUF_BASE        (SETUP_BUF_BASE + SETUP_BUF_LEN)
#define EP0_BUF_LEN         EP0_MAX_PKT_SIZE
#define EP1_BUF_BASE        (SETUP_BUF_BASE + SETUP_BUF_LEN)
#define EP1_BUF_LEN         EP1_MAX_PKT_SIZE
#define EP2_BUF_BASE        (EP1_BUF_BASE + EP1_BUF_LEN)
#define EP2_BUF_LEN         EP2_MAX_PKT_SIZE
#define EP3_BUF_BASE        (EP2_BUF_BASE + EP2_BUF_LEN)
#define EP3_BUF_LEN         EP3_MAX_PKT_SIZE
#define EP4_BUF_BASE        (EP3_BUF_BASE + EP3_BUF_LEN)
#define EP4_BUF_LEN         EP4_MAX_PKT_SIZE

/* Define the interrupt In EP number */
#define BULK_IN_EP_NUM      0x01
#define BULK_OUT_EP_NUM     0x02
#define INT_IN_EP_NUM       0x03


/* Define Descriptor information */
#define USBD_SELF_POWERED               0
#define USBD_REMOTE_WAKEUP              0
#define USBD_MAX_POWER                  50  /* The unit is in 2mA. ex: 50 * 2mA = 100mA */

/************************************************/
#define  GET_PORT_STATUS           0x01


/*-------------------------------------------------------------*/
void PTR_Init(void);
void PTR_ClassRequest(void);
void PTR_Data_Receive(void);

#endif  /* __USBD_PRINTER_H_ */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
