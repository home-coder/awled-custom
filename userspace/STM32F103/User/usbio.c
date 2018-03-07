/**
  ******************************************************************************
  * @file    usbio.c
  * $Author: wdluo $
  * $Revision: 67 $
  * $Date:: 2012-08-15 19:00:29 +0800 #$
  * @brief   USB上层函数.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Application_main_option.h"
#ifdef APPLICATION_USB_ENABLE
#include "usbio.h"
#include "usb_desc.h"
#include "usb_lib.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

  
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  通过USB发送数据
  * @param  data 数据存储首地址
  * @param  dataNum 发送的数据字节数
  * @retval 发送的字节数
  */
uint32_t USB_SendData(uint8_t *data,uint32_t dataNum)
{
	#ifndef STM32F10X_CL
	//将数据通过USB发送出去
	UserToPMABufferCopy(data, ENDP2_TXADDR, dataNum);
	SetEPTxCount(ENDP2, REPORT_COUNT);
	SetEPTxValid(ENDP2);
	#else
	USB_SIL_Write(EP2_IN, data, dataNum);
	#endif
	return dataNum;  
}
#endif

/*********************************END OF FILE**********************************/

