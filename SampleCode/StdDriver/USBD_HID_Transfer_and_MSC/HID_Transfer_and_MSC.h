/******************************************************************************
 * @file     HID_Transfer_and_MSC.h
 * @brief    NUC121 series USB device header file
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __USBD_HID_MSC_H__
#define __USBD_HID_MSC_H__

/* Define the vendor id and product id */
#define USBD_VID        0x0416
#define USBD_PID        0x5020

/*!<Define HID Class Specific Request */
#define GET_REPORT          0x01
#define GET_IDLE            0x02
#define GET_PROTOCOL        0x03
#define SET_REPORT          0x09
#define SET_IDLE            0x0A
#define SET_PROTOCOL        0x0B

/*!<USB HID Interface Class protocol */
#define HID_NONE            0x00
#define HID_KEYBOARD        0x01
#define HID_MOUSE           0x02

/*!<USB HID Class Report Type */
#define HID_RPT_TYPE_INPUT      0x01
#define HID_RPT_TYPE_OUTPUT     0x02
#define HID_RPT_TYPE_FEATURE    0x03

/*-------------------------------------------------------------*/
/* Define EP maximum packet size */
#define EP0_MAX_PKT_SIZE    8
#define EP1_MAX_PKT_SIZE    EP0_MAX_PKT_SIZE
#define EP2_MAX_PKT_SIZE    64
#define EP3_MAX_PKT_SIZE    64
#define EP4_MAX_PKT_SIZE    64
#define EP5_MAX_PKT_SIZE    64

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
#define EP5_BUF_BASE        (EP4_BUF_BASE + EP4_BUF_LEN)
#define EP5_BUF_LEN         EP5_MAX_PKT_SIZE

/* Define the EP numbers */
#define INT_IN_EP_NUM       0x01
#define INT_OUT_EP_NUM      0x02
#define BULK_IN_EP_NUM      0x03
#define BULK_OUT_EP_NUM     0x04

/* Define Descriptor information */
#define HID_DEFAULT_INT_IN_INTERVAL     1
#define USBD_SELF_POWERED               0
#define USBD_REMOTE_WAKEUP              0
#define USBD_MAX_POWER                  50  /* The unit is in 2mA. ex: 50 * 2mA = 100mA */


/* HIRC trim setting:
 *    HIRC trim reference clock is from USB SOF (Start-Of-Frame) packet.
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


/*-------------------------------------------------------------*/
void HID_MSC_Init(void);
void HID_MSC_ClassRequest(void);

void EP2_Handler(void);
void EP3_Handler(void);
void EP4_Handler(void);
void EP5_Handler(void);
void HID_SetInReport(void);
void HID_GetOutReport(uint8_t *pu8EpBuf, uint32_t u32Size);

#endif  /* __USBD_HID_MSC_H__ */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
