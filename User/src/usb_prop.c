/**
  ******************************************************************************
  * @file    usb_prop.c
  * @author  MCD Application Team
  * @version V4.1.0
  * @date    26-May-2017
  * @brief   All processing related to Joystick Mouse Demo
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
  *      may be used to endorse or promote
  products derived from this software
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
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ProtocolValue;

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    Joystick_init,
    Joystick_Reset,
    Joystick_Status_In,
    Joystick_Status_Out,
    Joystick_Data_Setup,
    Joystick_NoData_Setup,
    Joystick_Get_Interface_Setting,
    Joystick_GetDeviceDescriptor,
    Joystick_GetConfigDescriptor,
    Joystick_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };
USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    Joystick_GetConfiguration,
    Joystick_SetConfiguration,
    Joystick_GetInterface,
    Joystick_SetInterface,
    Joystick_GetStatus,
    Joystick_ClearFeature,
    Joystick_SetEndPointFeature,
    Joystick_SetDeviceFeature,
    Joystick_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)Joystick_DeviceDescriptor,
    JOYSTICK_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)Joystick_ConfigDescriptor,
    JOYSTICK_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR Joystick_Report_Descriptor =
  {
    (uint8_t *)Joystick_ReportDescriptor,
    JOYSTICK_SIZ_REPORT_DESC
  };

ONE_DESCRIPTOR Mouse_Hid_Descriptor =
  {
    (uint8_t*)Joystick_ConfigDescriptor + JOYSTICK_OFF_HID_DESC,
    JOYSTICK_SIZ_HID_DESC
  };

ONE_DESCRIPTOR String_Descriptor[4] =
  {
    {(uint8_t*)Joystick_StringLangID, JOYSTICK_SIZ_STRING_LANGID},
    {(uint8_t*)Joystick_StringVendor, JOYSTICK_SIZ_STRING_VENDOR},
    {(uint8_t*)Joystick_StringProduct, JOYSTICK_SIZ_STRING_PRODUCT},
    {(uint8_t*)Joystick_StringSerial, JOYSTICK_SIZ_STRING_SERIAL}
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * Function Name  : Joystick_init.
  * Description    : Joystick Mouse init routine.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void Joystick_init(void)
{
  /* Update the serial number string descriptor with the data from the unique
  ID*/
//  Get_SerialNum();

  pInformation->Current_Configuration = 0;
  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  bDeviceState = UNCONNECTED;
}

/**
  * Function Name  : Joystick_Reset.
  * Description    : Joystick Mouse reset routine.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void Joystick_Reset(void)
{
  /* Set Joystick_DEVICE as not configured */
  pInformation->Current_Configuration = 0;
  pInformation->Current_Interface = 0;/*the default Interface*/

  /* Current Feature initialization */
  pInformation->Current_Feature = Joystick_ConfigDescriptor[7];
  SetBTABLE(BTABLE_ADDRESS);
  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxCount(ENDP1, 4);
  SetEPRxStatus(ENDP1, EP_RX_DIS);
  SetEPTxStatus(ENDP1, EP_TX_NAK);

  /* Set this device to response on default address */
  SetDeviceAddress(0);
  bDeviceState = ATTACHED;
}

/**
  * Function Name  : Joystick_SetConfiguration.
  * Description    : Update the device state to configured.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void Joystick_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/**
  * Function Name  : Joystick_SetConfiguration.
  * Description    : Update the device state to addressed.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void Joystick_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}
/**
  * Function Name  : Joystick_Status_In.
  * Description    : Joystick status IN routine.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void Joystick_Status_In(void)
{}

/**
  * Function Name  : Joystick_Status_Out
  * Description    : Joystick status OUT routine.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void Joystick_Status_Out (void)
{}

/**
  * Function Name  : Joystick_Data_Setup
  * Description    : Handle the data class specific requests.
  * Input          : Request Nb.
  * Output         : None.
  * Return         : USB_UNSUPPORT or USB_SUCCESS.
  */
RESULT Joystick_Data_Setup(uint8_t RequestNo)
{
  uint8_t *(*CopyRoutine)(uint16_t);

  CopyRoutine = NULL;
  if ((RequestNo == GET_DESCRIPTOR)
      && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
      && (pInformation->USBwIndex0 == 0))
  {
    if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
    {
      CopyRoutine = Joystick_GetReportDescriptor;
    }
    else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
    {
      CopyRoutine = Joystick_GetHIDDescriptor;
    }

  } /* End of GET_DESCRIPTOR */

  /*** GET_PROTOCOL ***/
  else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
           && RequestNo == GET_PROTOCOL)
  {
    CopyRoutine = Joystick_GetProtocolValue;
  }
    else if((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)))
    {
        switch(RequestNo)
        {
            case GET_REPORT:
                CopyRoutine = Joystick_GetReport;
                break;

            case SET_REPORT:
                CopyRoutine = Joystick_SetReport;
                break;

            default:
                break;
        }
    }
  
  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }
  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/**
  * Function Name  : Joystick_NoData_Setup
  * Description    : handle the no data class specific requests
  * Input          : Request Nb.
  * Output         : None.
  * Return         : USB_UNSUPPORT or USB_SUCCESS.
  */
RESULT Joystick_NoData_Setup(uint8_t RequestNo)
{
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == SET_PROTOCOL))
  {
    return Joystick_SetProtocol();
  }

  else
  {
    return USB_UNSUPPORT;
  }
}

/**
  * Function Name  : Joystick_GetDeviceDescriptor.
  * Description    : Gets the device descriptor.
  * Input          : Length
  * Output         : None.
  * Return         : The address of the device descriptor.
  */
uint8_t *Joystick_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/**
  * Function Name  : Joystick_GetConfigDescriptor.
  * Description    : Gets the configuration descriptor.
  * Input          : Length
  * Output         : None.
  * Return         : The address of the configuration descriptor.
  */
uint8_t *Joystick_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/**
  * Function Name  : Joystick_GetStringDescriptor
  * Description    : Gets the string descriptors according to the needed index
  * Input          : Length
  * Output         : None.
  * Return         : The address of the string descriptors.
  */
uint8_t *Joystick_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  if (wValue0 >= 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/**
  * Function Name  : Joystick_GetReportDescriptor.
  * Description    : Gets the HID report descriptor.
  * Input          : Length
  * Output         : None.
  * Return         : The address of the configuration descriptor.
  */
uint8_t *Joystick_GetReportDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Joystick_Report_Descriptor);
}

/**
  * Function Name  : Joystick_GetHIDDescriptor.
  * Description    : Gets the HID descriptor.
  * Input          : Length
  * Output         : None.
  * Return         : The address of the configuration descriptor.
  */
uint8_t *Joystick_GetHIDDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Mouse_Hid_Descriptor);
}

/**
  * Function Name  : Joystick_Get_Interface_Setting.
  * Description    : tests the interface and the alternate setting according to the
  *                  supported one.
  * Input          : - Interface : interface number.
  *                  - AlternateSetting : Alternate Setting number.
  * Output         : None.
  * Return         : USB_SUCCESS or USB_UNSUPPORT.
  */
RESULT Joystick_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/**
  * Function Name  : Joystick_SetProtocol
  * Description    : Joystick Set Protocol request routine.
  * Input          : None.
  * Output         : None.
  * Return         : USB SUCCESS.
  */
RESULT Joystick_SetProtocol(void)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

/**
  * Function Name  : Joystick_GetProtocolValue
  * Description    : get the protocol value
  * Input          : Length.
  * Output         : None.
  * Return         : address of the protocol value.
  */
uint8_t *Joystick_GetProtocolValue(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (uint8_t *)(&ProtocolValue);
  }
}

uint8_t Report_Buffer[2] = {0x02, 0x02}; //Report_Buffer[0]为FeatureID，
                                    //Report_Buffer[1]为最大触控点数

uint8_t *Joystick_GetReport(uint16_t Length)  //处理Get_Report
{
    if(Length == 0)
    {
        pInformation->Ctrl_Info.Usb_wLength = 2;
        return NULL;
    }
    return  &Report_Buffer[pInformation->Ctrl_Info.Usb_wOffset];
}

uint8_t *Joystick_SetReport(uint16_t Length)//处理Set_Report
{
    if(Length == 0)
    {
        pInformation->Ctrl_Info.Usb_wLength = 2;
        return NULL;
    }
    return &Report_Buffer[pInformation->Ctrl_Info.Usb_wOffset];
}

void Joystick_Send(void)
{
    //  对于本代码2点触控每个数据包由14个字节组成分别为
    //  InReport[0]     设备ID 固定值为TOUCH_REPORT_ID
    //  InReport[1]     第一点触摸情况 bit1:1有触摸 0没触摸  bit2: 1在范围内  0不在范围内
    //  InReport[2]     第一点ID  每个点在抬起之前不能改变     0~255
    //  InReport[3]     第一点X坐标低8位
    //  InReport[4]     第一点X坐标高8位
    //  InReport[5]     第一点Y坐标低8位
    //  InReport[6]     第一点Y坐标高8位
    //  InReport[7]     第二点触摸情况 bit1:1有触摸 0没触摸  bit2: 1在范围内  0不在范围内
    //  InReport[8]     第二点ID  每个点在抬起之前不能改变     0~255
    //  InReport[9]     第二点X坐标低8位
    //  InReport[10]    第二点X坐标高8位
    //  InReport[11]    第二点Y坐标低8位
    //  InReport[12]    第二点Y坐标高8位
    //  InReport[13]    此数据包中有效的点数
    static uint8_t i = 0;
    uint8_t Buffer[14];
    if(i < 20)
    {
        Buffer[0]   = 0x01;
        Buffer[1]   = 0x07;
        Buffer[2]   = 0x01;
        Buffer[3]   = (720 + i) & 0xFF;
        Buffer[4]   = (720 + i) >> 8;
        Buffer[5]   = (135 + i) & 0xFF;
        Buffer[6]   = (135 + i) >> 8;
        Buffer[7]   = 0x00;
        Buffer[8]   = 0x00;
        Buffer[9]   = 0x00;
        Buffer[10]  = 0x00;
        Buffer[11]  = 0x00;
        Buffer[12]  = 0x00; 
        Buffer[13]  = 0x01;
    }
    else if(i < 40)
    {
        Buffer[0]   = 0x01;
        Buffer[1]   = 0x06;
        Buffer[2]   = 0x01;
        Buffer[3]   = (720 + i) & 0xFF;
        Buffer[4]   = (720 + i) >> 8;
        Buffer[5]   = (135 + i) & 0xFF;
        Buffer[6]   = (135 + i) >> 8;
        Buffer[7]   = 0x00;
        Buffer[8]   = 0x00;
        Buffer[9]   = 0x00;
        Buffer[10]  = 0x00;
        Buffer[11]  = 0x00;
        Buffer[12]  = 0x00; 
        Buffer[13]  = 0x01;
    }
    else
    {
        Buffer[0]   = 0x01;
        Buffer[1]   = 0x00;
        Buffer[2]   = 0x00;
        Buffer[3]   = 0x00;
        Buffer[4]   = 0x00;
        Buffer[5]   = 0x00;
        Buffer[6]   = 0x00;
        Buffer[7]   = 0x00;
        Buffer[8]   = 0x00;
        Buffer[9]   = 0x00;
        Buffer[10]  = 0x00;
        Buffer[11]  = 0x00;
        Buffer[12]  = 0x00; 
        Buffer[13]  = 0x00;
    }
    i++;
    if(i > 60)
    {
        i = 0;
    }
    UserToPMABufferCopy(Buffer, GetEPTxAddr(ENDP1) , 14);
    SetEPTxValid(ENDP1);
    return;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
