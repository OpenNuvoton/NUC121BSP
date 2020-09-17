/****************************************************************************//**
 * @file     descriptors.c
 * @version  V3.00
 * @brief    USBD descriptors
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __DESCRIPTORS_C__
#define __DESCRIPTORS_C__

/*!<Includes */
#include "NuMicro.h"
#include "cdc_serial.h"

/*----------------------------------------------------------------------------*/
/*!<USB Device Descriptor */
const uint8_t gu8DeviceDescriptor[] = {
    LEN_DEVICE,  /* bLength */
    DESC_DEVICE, /* bDescriptorType */
#ifdef SUPPORT_LPM
    0x01, 0x02, /* bcdUSB => 0x0201 to support LPM */
#else
    0x10, 0x01, /* bcdUSB */
#endif
    0x02,             /* bDeviceClass */
    0x00,             /* bDeviceSubClass */
    0x00,             /* bDeviceProtocol */
    EP0_MAX_PKT_SIZE, /* bMaxPacketSize0 */
    /* idVendor */
    USBD_VID & 0x00FF, (USBD_VID & 0xFF00) >> 8,
    /* idProduct */
    USBD_PID & 0x00FF, (USBD_PID & 0xFF00) >> 8,
    0x00, 0x03, /* bcdDevice */
    0x01,       /* iManufacture */
    0x02,       /* iProduct */
    0x03,       /* iSerialNumber */
    0x01 /* bNumConfigurations */
};

/*!<USB Configure Descriptor */
const uint8_t gu8ConfigDescriptor[] = {
    LEN_CONFIG,  /* bLength              */
    DESC_CONFIG, /* bDescriptorType      */
    0x43, 0x00,  /* wTotalLength         */
    0x02,        /* bNumInterfaces       */
    0x01,        /* bConfigurationValue  */
    0x00,        /* iConfiguration       */
    0xC0,        /* bmAttributes         */
    0x32,        /* MaxPower             */

    /* INTERFACE descriptor */
    LEN_INTERFACE,  /* bLength              */
    DESC_INTERFACE, /* bDescriptorType      */
    0x00,           /* bInterfaceNumber     */
    0x00,           /* bAlternateSetting    */
    0x01,           /* bNumEndpoints        */
    0x02,           /* bInterfaceClass      */
    0x02,           /* bInterfaceSubClass   */
    0x01,           /* bInterfaceProtocol   */
    0x00,           /* iInterface           */

    /* Communication Class Specified INTERFACE descriptor */
    0x05,       /* Size of the descriptor, in bytes */
    0x24,       /* CS_INTERFACE descriptor type */
    0x00,       /* Header functional descriptor subtype */
    0x10, 0x01, /* Communication device compliant to the communication spec.
                   ver. 1.10 */

    /* Communication Class Specified INTERFACE descriptor */
    0x05, /* Size of the descriptor, in bytes */
    0x24, /* CS_INTERFACE descriptor type */
    0x01, /* Call management functional descriptor */
    0x00, /* BIT0: Whether device handle call management itself. */
          /* BIT1: Whether device can send/receive call management information over a
             Data Class Interface 0 */
    0x01, /* Interface number of data class interface optionally used for call
             management */

    /* Communication Class Specified INTERFACE descriptor */
    0x04, /* Size of the descriptor, in bytes */
    0x24, /* CS_INTERFACE descriptor type */
    0x02, /* Abstract control management functional descriptor subtype */
    0x00, /* bmCapabilities       */

    /* Communication Class Specified INTERFACE descriptor */
    0x05, /* bLength              */
    0x24, /* bDescriptorType: CS_INTERFACE descriptor type */
    0x06, /* bDescriptorSubType   */
    0x00, /* bMasterInterface     */
    0x01, /* bSlaveInterface0     */

    /* ENDPOINT descriptor */
    LEN_ENDPOINT,               /* bLength          */
    DESC_ENDPOINT,              /* bDescriptorType  */
    (EP_INPUT | INT_IN_EP_NUM), /* bEndpointAddress */
    EP_INT,                     /* bmAttributes     */
    EP4_MAX_PKT_SIZE, 0x00,     /* wMaxPacketSize   */
    0x01,                       /* bInterval        */

    /* INTERFACE descriptor */
    LEN_INTERFACE,  /* bLength              */
    DESC_INTERFACE, /* bDescriptorType      */
    0x01,           /* bInterfaceNumber     */
    0x00,           /* bAlternateSetting    */
    0x02,           /* bNumEndpoints        */
    0x0A,           /* bInterfaceClass      */
    0x00,           /* bInterfaceSubClass   */
    0x00,           /* bInterfaceProtocol   */
    0x00,           /* iInterface           */

    /* ENDPOINT descriptor */
    LEN_ENDPOINT,                /* bLength          */
    DESC_ENDPOINT,               /* bDescriptorType  */
    (EP_INPUT | BULK_IN_EP_NUM), /* bEndpointAddress */
    EP_BULK,                     /* bmAttributes     */
    EP2_MAX_PKT_SIZE, 0x00,      /* wMaxPacketSize   */
    0x00,                        /* bInterval        */

    /* ENDPOINT descriptor */
    LEN_ENDPOINT,                  /* bLength          */
    DESC_ENDPOINT,                 /* bDescriptorType  */
    (EP_OUTPUT | BULK_OUT_EP_NUM), /* bEndpointAddress */
    EP_BULK,                       /* bmAttributes     */
    EP3_MAX_PKT_SIZE, 0x00,        /* wMaxPacketSize   */
    0x00                           /* bInterval        */
};

#ifdef SUPPORT_LPM
/*!<USB BOS Descriptor */
const uint8_t gu8BosDescriptor[] =
{
    LEN_BOS,                         /* bLength */
    DESC_BOS,                        /* bDescriptorType */
    ((LEN_BOS + LEN_DEVCAP) & 0xFF), /* wTotalLength */
    ((LEN_BOS + LEN_DEVCAP) >> 8),   /* wTotalLength */
    0x01,                            /* bNumDevcieCaps */
    LEN_DEVCAP,                      /* bLength */
    DESC_DEVCAP,                     /* bDescriptorType */
    0x02,                            /* bDevCapabilityType, 0x02 is USB 2.0 Extension */
    0x06, 0x04, 0x00, 0x00  /* bmAttributs, 32 bits                                              */
    /* bit 0 : Reserved. Must 0.                                         */
    /* bit 1 : 1 to support LPM.                                         */
    /* bit 2 : 1 to support BSL & Alternat HIRD                          */
    /* bit 3 : 1 to recommend Baseline BESL                              */
    /* bit 4 : 1 to recommend Deep BESL                                  */
    /* bit 11:8 : Recommend Baseline BESL value. Ignore by bit3 is zero. */
    /* bit 15:12 : Recommend Deep BESL value. Ignore by bit4 is zero.    */
    /* bit 31:16 : Reserved. Must 0.                                     */
};
#endif


/*!<USB Language String Descriptor */
const uint8_t gu8StringLang[4] =
{
    4,              /* bLength */
    DESC_STRING,    /* bDescriptorType */
    0x09, 0x04
};

/*!<USB Vendor String Descriptor */
const uint8_t gu8VendorStringDesc[] =
{
    16,
    DESC_STRING,
    'N', 0, 'u', 0, 'v', 0, 'o', 0, 't', 0, 'o', 0, 'n', 0
};

/*!<USB Product String Descriptor */
const uint8_t gu8ProductStringDesc[] =
{
    32,             /* bLength          */
    DESC_STRING,    /* bDescriptorType  */
    'U', 0, 'S', 0, 'B', 0, ' ', 0, 'V', 0, 'i', 0, 'r', 0, 't', 0, 'u', 0, 'a', 0, 'l', 0, ' ', 0, 'C', 0, 'O', 0, 'M', 0
};

const uint8_t gu8StringSerial[26] =
{
    26,             // bLength
    DESC_STRING,    // bDescriptorType
    'A', 0, '0', 0, '2', 0, '0', 0, '1', 0, '4', 0, '0', 0, '9', 0, '0', 0, '3', 0, '0', 0, '5', 0
};

const uint8_t *gpu8UsbString[4] =
{
    gu8StringLang,
    gu8VendorStringDesc,
    gu8ProductStringDesc,
    gu8StringSerial
};

const S_USBD_INFO_T gsInfo =
{
    gu8DeviceDescriptor,
    gu8ConfigDescriptor,
    gpu8UsbString,
    NULL,
    NULL,
    NULL,
#ifdef SUPPORT_LPM
    gu8BosDescriptor
#endif
};

#endif  /* __DESCRIPTORS_C__ */
