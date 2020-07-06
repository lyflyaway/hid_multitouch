/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V4.1.0
  * @date    26-May-2017
  * @brief   Descriptors for Joystick Mouse Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_desc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize 64*/
    0x83,                       /*idVendor (0x0483)*/
    0x04,
    0x10,                       /*idProduct = 0x5710*/
    0x57,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing
                                                  manufacturer */
    2,                          /*Index of string descriptor describing
                                                 product*/
    3,                          /*Index of string descriptor describing the
                                                 device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* Joystick_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
  {
/***************配置描述符***********************/
 0x09,      //bLength字段
 USB_CONFIGURATION_DESCRIPTOR_TYPE,     //bDescriptorType字段
 JOYSTICK_SIZ_CONFIG_DESC&0xff,         //wTotalLength字段
 (JOYSTICK_SIZ_CONFIG_DESC>>8)&0xff,
 0x01,                                  //bNumInterfaces字段
 0x01,                                  //bConfiguration字段
 0x00,                                  //iConfigurationz字段
 0x80,                                  //bmAttributes字段
 0x20,                                  //bMaxPower字段
/* 9 */

/*******************第一个接口描述符*********************/
 0x09,                                  //bLength字段
 USB_INTERFACE_DESCRIPTOR_TYPE,         //bDescriptorType字段
 0x00,                                  //bInterfaceNumber字段
 0x00,                                  //bAlternateSetting字段
 0x02,                                  //bNumEndpoints字段
 0x03,                                  //bInterfaceClass字段
 0x00,                                  //bInterfaceSubClass字段
 0x00,                                  //bInterfaceProtocol字段
 0x00,                                  //iConfiguration字段
/* 18 */

/******************HID描述符************************/
0x09,                                   //bLength字段
0x21,                                   //bDescriptorType字段
0x01,                                   //bcdHID字段
0x01,
0x00,                                   //bCountyCode字段
0x01,                                   //bNumDescriptors字段
0x22,                                   //bDescriptorType字段
JOYSTICK_SIZ_REPORT_DESC&0xFF,
(JOYSTICK_SIZ_REPORT_DESC>>8)&0xFF,
/* 27 */

/**********************输入端点描述符***********************/
 0x07,                                 //bLength字段
 USB_ENDPOINT_DESCRIPTOR_TYPE,         //bDescriptorType字段
 0x81,                                 //bEndpointAddress字段
 0x03,                                 //bmAttributes字段
 0x10,                                 //wMaxPacketSize字段
 0x00,
 0x0A,                                 //bInterval字段
/* 34 */

/**********************输出端点描述符***********************/
 0x07,                                 //bLength字段
 USB_ENDPOINT_DESCRIPTOR_TYPE,         //bDescriptorType字段
 0x01,                                 //bEndpointAddress字段
 0x03,                                 //bmAttributes字段
 0x10,                                 //wMaxPacketSize字段
 0x00,
 0x0A,                                 //bInterval字段
/* 41 */

  
//    0x09, /* bLength: Configuration Descriptor size */
//    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
//    JOYSTICK_SIZ_CONFIG_DESC,
//    /* wTotalLength: Bytes returned */
//    0x00,
//    0x01,         /*bNumInterfaces: 1 interface*/
//    0x01,         /*bConfigurationValue: Configuration value*/
//    0x00,         /*iConfiguration: Index of string descriptor describing
//                                     the configuration*/
//    0xE0,         /*bmAttributes: Self powered */
//    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/
//
//    /************** Descriptor of Joystick Mouse interface ****************/
//    /* 09 */
//    0x09,         /*bLength: Interface Descriptor size*/
//    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
//    0x00,         /*bInterfaceNumber: Number of Interface*/
//    0x00,         /*bAlternateSetting: Alternate setting*/
//    0x01,         /*bNumEndpoints*/
//    0x03,         /*bInterfaceClass: HID*/
//    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
//    0x02,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
//    0,            /*iInterface: Index of string descriptor*/
//    /******************** Descriptor of Joystick Mouse HID ********************/
//    /* 18 */
//    0x09,         /*bLength: HID Descriptor size*/
//    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
//    0x00,         /*bcdHID: HID Class Spec release number*/
//    0x01,
//    0x00,         /*bCountryCode: Hardware target country*/
//    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
//    0x22,         /*bDescriptorType*/
//    JOYSTICK_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
//    0x00,
//    /******************** Descriptor of Joystick Mouse endpoint ********************/
//    /* 27 */
//    0x07,          /*bLength: Endpoint Descriptor size*/
//    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/
//
//    0x81,          /*bEndpointAddress: Endpoint Address (IN)*/
//    0x03,          /*bmAttributes: Interrupt endpoint*/
//    0x04,          /*wMaxPacketSize: 4 Byte max */
//    0x00,
//    0x20,          /*bInterval: Polling Interval (32 ms)*/
//    /* 34 */
  }
  ; /* MOUSE_ConfigDescriptor */
const uint8_t Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC] =
  {
    /* 多点触控设备 */
    0x05, 0x0d,// USAGE_PAGE (Digitizers)
    0x09, 0x04,// USAGE (Touch Screen)
    0xa1, 0x01,// COLLECTION (Application)
    0x85, 0x01,//REPORTID_MTOUCH, // REPORT_ID (Touch)
    /* 8 */

    /* 第一点 */ 
    /* 手指和z轴 */
    0x09, 0x22,// USAGE (Finger)
    0xa1, 0x02,// COLLECTION (Logical)
    0x09, 0x42,// USAGE (Tip Switch)
    0x15, 0x00,// LOGICAL_MINIMUM (0)
    0x25, 0x01,// LOGICAL_MAXIMUM (1)
    0x75, 0x01,// REPORT_SIZE (1)
    0x95, 0x01,// REPORT_COUNT (1)
    0x81, 0x02,// INPUT (Data,Var,Abs)
    0x09, 0x32,// USAGE (In Range)
    0x81, 0x02,// INPUT (Data,Var,Abs)
    0x95, 0x06,// REPORT_COUNT (6)
    0x81, 0x01,// INPUT (Cnst,Ary,Abs)
    /* 32 */

     /* 手指ID */    
    0x75, 0x08,// REPORT_SIZE (8)
    0x09, 0x51,// USAGE ( Contact Identifier)
    0x25, 0xFF,//  LOGICAL_MAXIMUM (255)
    0x95, 0x01,// REPORT_COUNT (1)
    0x81, 0x02,// INPUT (Data,Var,Abs)
    /* 42 */

    /* X和Y */
    0x05, 0x01,// USAGE_PAGE (Generic Desk)
    0x16, 0x00,0x00,//HID_LogicalMinS (0)
    0x26, 0x56, 0x05,// LOGICAL_MAXIMUM (1366)
    0x75, 0x10,// REPORT_SIZE (16)
    0x55, 0x00,// UNIT_EXPONENT (0)
    0x65, 0x00,// UNIT (00)
    0x09, 0x30,// USAGE (X)
    0x36, 0x00,0x00,// PHYSICAL_MINIMUM (0)
    0x46, 0x56, 0x05,// PHYSICAL_MAXIMUM (1366)
    0x95, 0x01,// REPORT_COUNT (1)
    0x81, 0x02,// INPUT (Data,Var,Abs)
    0x26, 0x00, 0x03,// LOGICAL_MAXIMUM (4095)
    0x46, 0x00, 0x03,// PHYSICAL_MAXIMUM (4095)
    0x09, 0x31,// USAGE (Y)
    0x81, 0x02,// INPUT (Data,Var,Abs)
    0xc0,      // END_COLLECTION
    /* 79 */

    /* 第二点 */
    /* 手指和z轴 */
    0x05, 0x0d,// USAGE_PAGE (Digitizers)
    0x09, 0x22,// USAGE (Finger)
    0xa1, 0x02,// COLLECTION (Logical)
    0x09, 0x42,// USAGE (Tip Switch)
    0x15, 0x00,// LOGICAL_MINIMUM (0)
    0x25, 0x01,// LOGICAL_MAXIMUM (1)
    0x75, 0x01,// REPORT_SIZE (1)
    0x95, 0x01,// REPORT_COUNT (1)
    0x81, 0x02,// INPUT (Data,Var,Abs)
    0x09, 0x32,// USAGE (In Range)
    0x81, 0x02,// INPUT (Data,Var,Abs)
    0x95, 0x06,// REPORT_COUNT (6)
    0x81, 0x01,// INPUT (Cnst,Ary,Abs)
    /* 105 */

    /* 手指ID */
    0x75, 0x08,// REPORT_SIZE (8)
    0x09, 0x51,// USAGE ( Contact Identifier)
    0x95, 0x01,// REPORT_COUNT (1)
    0x25, 0xFF,// LOGICAL_MAXIMUM (255) 
    0x81, 0x02,// INPUT (Data,Var,Abs)
    /* 115 */

    /* X和Y */
    0x05, 0x01,// USAGE_PAGE (Generic Desk..
    0x26, 0x56, 0x05, // LOGICAL_MAXIMUM (1366)
    0x75, 0x10,// REPORT_SIZE (16)
    0x55, 0x00,// UNIT_EXPONENT (0)
    0x65, 0x00,// UNIT (0)
    0x09, 0x30,// USAGE (X)
    0x36, 0x00,0x00,// PHYSICAL_MINIMUM (0)
    0x46, 0x56, 0x05,// PHYSICAL_MAXIMUM (1366)
    0x81, 0x02,// INPUT (Data,Var,Abs)
    0x26, 0x00, 0x03,
    0x46, 0x00, 0x03,// PHYSICAL_MAXIMUM (0)
    0x09, 0x31,// USAGE (Y)
    0x81, 0x02,// INPUT (Data,Var,Abs)
    0xc0, //END_COLLECTION
    /* 147 */

    /* 实际点数 */
    0x05, 0x0d,// USAGE_PAGE (Digitizers)
    0x09, 0x54,// USAGE (Actual count)
    0x95, 0x01,// REPORT_COUNT (1)
    0x75, 0x08,// REPORT_SIZE (8)
    0x15, 0x00,// LOGICAL_MINIMUM (0)
    0x25, 0x08,// LOGICAL_MAXIMUM (8)
    0x81, 0x02,// INPUT (Data,Var,Abs)
    /* 161 */

    /* 硬件支持点数 */
    0x85, 0x02,// REPORT_ID (Feature)
    0x75, 0x08,// REPORT_SIZE (8)
    0x95, 0x01,// REPORT_COUNT (1)
    0x15, 0x01,// LOGICAL_MINIMUM (1)
    0x25, 0x08,// LOGICAL_MAXIMUM (8)
    0x09, 0x55,// USAGE(Maximum Count)
    0xB1, 0x02,// FEATURE (Data,Var,Abs)
    0xc0,       // END_COLLECTION
    /* 176 */

    /* 上位机特性设定 */
    0x09, 0x0E,// USAGE (Device Configuration)  
    0xa1, 0x01,// COLLECTION (Application)  
    0x85, 0x04,// REPORT_ID (Configuration)                
    0x09, 0x23,// USAGE (Device Settings)                
    0xa1, 0x02,// COLLECTION (logical)      
    0x09, 0x52,// USAGE (Device Mode)           
    0x15, 0x00,// LOGICAL_MINIMUM (0)        
    0x25, 0x0a,// LOGICAL_MAXIMUM (10)  
    0x75, 0x08,// REPORT_SIZE (8)           
    0x09, 0x53,// USAGE (Device Identifier) 
    0x95, 0x01,// REPORT_COUNT (2)  
    0xb1, 0x02,// FEATURE (Data,Var,Abs      
    0xc0,         // END_COLLECTION  
    0xc0,         // END_COLLECTION
    /* 202 */


//    0x05,          /*Usage Page(Generic Desktop)*/
//    0x01,
//    0x09,          /*Usage(Mouse)*/
//    0x02,
//    0xA1,          /*Collection(Logical)*/
//    0x01,
//    0x09,          /*Usage(Pointer)*/
//    0x01,
//    /* 8 */
//    0xA1,          /*Collection(Linked)*/
//    0x00,
//    0x05,          /*Usage Page(Buttons)*/
//    0x09,
//    0x19,          /*Usage Minimum(1)*/
//    0x01,
//    0x29,          /*Usage Maximum(3)*/
//    0x03,
//    /* 16 */
//    0x15,          /*Logical Minimum(0)*/
//    0x00,
//    0x25,          /*Logical Maximum(1)*/
//    0x01,
//    0x95,          /*Report Count(3)*/
//    0x03,
//    0x75,          /*Report Size(1)*/
//    0x01,
//    /* 24 */
//    0x81,          /*Input(Variable)*/
//    0x02,
//    0x95,          /*Report Count(1)*/
//    0x01,
//    0x75,          /*Report Size(5)*/
//    0x05,
//    0x81,          /*Input(Constant,Array)*/
//    0x01,
//    /* 32 */
//    0x05,          /*Usage Page(Generic Desktop)*/
//    0x01,
//    0x09,          /*Usage(X axis)*/
//    0x30,
//    0x09,          /*Usage(Y axis)*/
//    0x31,
//    0x09,          /*Usage(Wheel)*/
//    0x38,
//    /* 40 */
//    0x15,          /*Logical Minimum(-127)*/
//    0x81,
//    0x25,          /*Logical Maximum(127)*/
//    0x7F,
//    0x75,          /*Report Size(8)*/
//    0x08,
//    0x95,          /*Report Count(3)*/
//    0x03,
//    /* 48 */
//    0x81,          /*Input(Variable, Relative)*/
//    0x06,
//    0xC0,          /*End Collection*/
//    0x09,
//    0x3c,
//    0x05,
//    0xff,
//    0x09,
//    /* 56 */
//    0x01,
//    0x15,
//    0x00,
//    0x25,
//    0x01,
//    0x75,
//    0x01,
//    0x95,
//    /* 64 */
//    0x02,
//    0xb1,
//    0x22,
//    0x75,
//    0x06,
//    0x95,
//    0x01,
//    0xb1,
//    /* 72 */
//    0x01,
//    0xc0
  }
  ; /* Joystick_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] =
  {
    JOYSTICK_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] =
  {
    JOYSTICK_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const uint8_t Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] =
  {
    JOYSTICK_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'J', 0,
    'o', 0, 'y', 0, 's', 0, 't', 0, 'i', 0, 'c', 0, 'k', 0
  };
uint8_t Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] =
  {
    JOYSTICK_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

