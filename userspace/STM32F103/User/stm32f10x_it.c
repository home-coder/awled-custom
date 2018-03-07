/**
  ******************************************************************************
  * @file    stm32f10x_it.c
  * $Author: mll $
  * $Revision: 00 $
  * $Date:: 2015-10-15  #$
  * @brief   中断函数定义.
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_it.h"

#include "Application_main_option.h"
#ifdef APPLICATION_USB_ENABLE
#include "usb_lib.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#endif
#include "UserProgram.h"
#include "User_Exti.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            			Cortex-M3 处理器的相关服务函数                        */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	nTime--;
}

/******************************************************************************/
/*                 STM32F10x 外设 中断 服务函数                               */
/*  增加需要的外设中断函数在下面。中断的函数名字都已经在startup_stm32f10x_xx.s*/
/*  的文件中定义好了，请参照它来写。                                          */
/******************************************************************************/
/**
  * @brief  USB中断处理函数
  * @note	该中断函数只有STM32F105和STM32F107系列芯片才有这个中断
  * @param  None
  * @retval None
  */
#ifndef STM32F10X_CL
void USB_LP_CAN1_RX0_IRQHandler(void)
{
#ifdef APPLICATION_USB_ENABLE
  USB_Istr();
#endif
}
#endif /* STM32F10X_CL */
/**
  * @brief  This function handles OTG WakeUp interrupt request.
  * @note	None
  * @param  None
  * @retval None
  */
void OTG_FS_WKUP_IRQHandler(void)
{
#ifdef APPLICATION_USB_ENABLE
  /* Initiate external resume sequence (1 step) */
  Resume(RESUME_EXTERNAL);  
#endif
}
/**
  * @brief  This function handles USB-On-The-Go FS global interrupt request.
  * @note	None
  * @param  None
  * @retval None
  */
#ifdef STM32F10X_CL
void OTG_FS_IRQHandler(void)
{
  STM32_PCD_OTG_ISR_Handler(); 
}
#endif /* STM32F10X_CL */

//外部中断0响应函数
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		// PB0
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == RESET)
		{
			Exti_SetPB0Flag(1);
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

//外部中断4响应函数
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		// PC4
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4) == RESET)
		{
			Exti_SetPC4Flag(1);
		}
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

//外部中断12响应函数
void EXTI15_10_IRQHandler(void)
{
	BYTE level = RESET;
	
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		// PC12
		level = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12);
		if( level == RESET)
		{
			// when key pressed, enable timer
			TIM_Cmd(TIM2, ENABLE);
			
			Exti_SetPC12Flag(1);
		}
		else if ( level == SET)
		{
			// when key released, disable timer
			TIM_Cmd(TIM2, DISABLE);
			
			Exti_SetPC12Flag(2);
		}
		
		// PB12
		level = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
		
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
}

static void TIM_IRQHandler(TIM_TypeDef * TIMx)
{
	if ( TIM_GetITStatus(TIMx, TIM_IT_Update) != RESET )
	{
		TIM_ClearITPendingBit(TIMx , TIM_FLAG_Update);

		// when timer overflow, disable it
		TIM_Cmd(TIMx, DISABLE);	
		
		Exti_SetTIMFlag(TIMx, 1);	
	}
}

void TIM2_IRQHandler(void)
{
	TIM_IRQHandler( TIM2 );
}

void TIM3_IRQHandler(void)
{
	TIM_IRQHandler( TIM3 );
}

void TIM4_IRQHandler(void)
{
	TIM_IRQHandler( TIM4 );
}

void TIM5_IRQHandler(void)
{
	TIM_IRQHandler( TIM5 );
}


/*********************************END OF FILE**********************************/
