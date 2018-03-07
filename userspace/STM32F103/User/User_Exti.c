#include "User_Exti.h"
#include "stm32f10x_exti.h"

static BYTE m_PC4flag = 0;			//PC4外部中断标志位
static BYTE m_PB0flag = 0;			//PB0外部中断标志位
static BYTE m_PC12flag = 0;			//PC12外部中断标志位

static BYTE m_TIM2flag = 0;			//TIM2外部中断标志位
static BYTE m_TIM3flag = 0;			//TIM3外部中断标志位
static BYTE m_TIM4flag = 0;			//TIM4外部中断标志位
static BYTE m_TIM5flag = 0;			//TIM5外部中断标志位


//************************************
//函数：外部中断引脚PB0/PC4检测初始化
//参数：无
//返回值：无
//************************************
void Exti_PB0Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	
	EXTI_Init(&EXTI_InitStruct);
}


void Exti_PC4Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	
	EXTI_Init(&EXTI_InitStruct);
}


void Exti_PC12Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line12;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	
	EXTI_Init(&EXTI_InitStruct);
}

//************************************
//函数：禁用外部中断PB1检测
//参数：无
//返回值：无
//************************************
void Exti_PC4Dis(void)
{
	EXTI->IMR &= ~EXTI_Line4;
}

//************************************
//函数：启用外部中断PB1检测
//参数：无
//返回值：无
//************************************
void Exti_PC4En(void)
{
	EXTI->IMR |= EXTI_Line4;
}

void Exti_SetPC4Flag(BYTE flag)
{
	m_PC4flag = flag;
}

BYTE Exti_GetPC4Flag(void)
{
	return m_PC4flag;
}

void Exti_SetPB0Flag(BYTE flag)
{
	m_PB0flag = flag;
}

BYTE Exti_GetPB0Flag(void)
{
	return m_PB0flag;
}

void Exti_SetPC12Flag(BYTE flag)
{
	m_PC12flag = flag;
}

BYTE Exti_GetPC12Flag(void)
{
	return m_PC12flag;
}

void Exti_SetTIMFlag(TIM_TypeDef * TIMx, BYTE flag)
{
	if ( TIM2 == TIMx )
	{
		m_TIM2flag = flag;
	}
	else if ( TIM3 == TIMx )
	{
		m_TIM3flag = flag;
	}
	else if ( TIM4 == TIMx )
	{
		m_TIM4flag = flag;
	}
	else if ( TIM5 == TIMx )
	{
		m_TIM5flag = flag;
	}
}

BYTE Exti_GetTIMFlag(TIM_TypeDef * TIMx)
{
	BYTE flag = 0;
	
	if ( TIM2 == TIMx )
	{
		flag = m_TIM2flag;
	}
	else if ( TIM3 == TIMx )
	{
		flag = m_TIM3flag;
	}
	else if ( TIM4 == TIMx )
	{
		flag = m_TIM4flag;
	}
	else if ( TIM5 == TIMx )
	{
		flag = m_TIM5flag;
	}

	return flag;
}

