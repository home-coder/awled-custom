#include "User_PWM.h"
#include "stm32f10x_tim.h"



//*********************************************
//函数：PA8/TIM1_CH1配置为PWM输出
//参数：Fretype	--	PWM频率类型 (_PWM_FRE_)
//		Duty	--  占空比
//返回值：无
//*********************************************
void TIM1_CH1_PWM_Config(BYTE Fretype,uint16 duty)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	//初始化TIM1的时间基数单位
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//初始化TIM1的外设
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	//PA8 --TIM1_CH1		-- 复用推挽输出		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;	
	
	switch(Fretype)
	{
		case PWM_2Hz:
		{	
			TIM_TimeBaseInitStruct.TIM_Prescaler = 36000 - 1;
		}
		break;
		case PWM_4Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 18000 - 1;
		}
		break;
		case PWM_8Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 9000 - 1;
		}
		break;
		case PWM_16Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 4500 - 1;
		}
		break;
		case PWM_32Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 2250 - 1;
		}
		break;
		case PWM_64Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 1125 - 1;
		}
		break;
		case PWM_125Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 576 - 1;
		}
		break;
		case PWM_250Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 288 - 1;
		}
		break;
		case PWM_500Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 144 - 1;
		}
		break;
		case PWM_1kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
		}
		break;
		case PWM_2kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 36 - 1;
		}
		break;
		case PWM_4kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 18 - 1;
		}
		break;
		case PWM_6kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 12 - 1;
		}
		break;
		case PWM_9kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 8 - 1;
		}
		break;
		case PWM_12kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 6 - 1;
		}
		break;
		case PWM_18kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 4 - 1;
		}
		break;
		case PWM_24kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 3 - 1;
		}
		break;
		case PWM_36kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 2 - 1;
		}
		break;
		default:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 36000 - 1;
		}
		break;
	}
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = duty;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);	
	TIM_Cmd(TIM1,ENABLE);	
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

//*********************************************
//函数：PA1/TIM2_CH2配置为PWM输出
//参数：Fretype	--	PWM频率类型 (_PWM_FRE_)
//		Duty	--  占空比
//返回值：无
//*********************************************
void TIM2_CH2_PWM_Config(BYTE Fretype,uint16 duty)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	//初始化TIM2的时间基数单位
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//初始化TIM2的外设
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	//PA1 --TIM2_CH2		-- 复用推挽输出		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;	
	
	switch(Fretype)
	{
		case PWM_2Hz:
		{	
			TIM_TimeBaseInitStruct.TIM_Prescaler = 36000 - 1;
		}
		break;
		case PWM_4Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 18000 - 1;
		}
		break;
		case PWM_8Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 9000 - 1;
		}
		break;
		case PWM_16Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 4500 - 1;
		}
		break;
		case PWM_32Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 2250 - 1;
		}
		break;
		case PWM_64Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 1125 - 1;
		}
		break;
		case PWM_125Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 576 - 1;
		}
		break;
		case PWM_250Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 288 - 1;
		}
		break;
		case PWM_500Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 144 - 1;
		}
		break;
		case PWM_1kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
		}
		break;
		case PWM_2kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 36 - 1;
		}
		break;
		case PWM_4kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 18 - 1;
		}
		break;
		case PWM_6kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 12 - 1;
		}
		break;
		case PWM_9kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 8 - 1;
		}
		break;
		case PWM_12kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 6 - 1;
		}
		break;
		case PWM_18kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 4 - 1;
		}
		break;
		case PWM_24kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 3 - 1;
		}
		break;
		case PWM_36kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 2 - 1;
		}
		break;
		default:
			break;
	}
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = duty;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2,ENABLE);	
	TIM_Cmd(TIM2,ENABLE);
}

//*********************************************
//函数：PB0/TIM3_CH3配置为PWM输出
//参数：Fretype	--	PWM频率类型 (_PWM_FRE_)
//		Duty	--  占空比
//返回值：无
//*********************************************
void TIM3_CH3_PWM_Config(BYTE Fretype,uint16 duty)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	//初始化TIM3的时间基数单位
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//初始化TIM3的外设
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	//PB0 --TIM3_CH3		-- 复用推挽输出		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;	
	
	switch(Fretype)
	{
		case PWM_2Hz:
		{	
			TIM_TimeBaseInitStruct.TIM_Prescaler = 36000 - 1;
		}
		break;
		case PWM_4Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 18000 - 1;
		}
		break;
		case PWM_8Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 9000 - 1;
		}
		break;
		case PWM_16Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 4500 - 1;
		}
		break;
		case PWM_32Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 2250 - 1;
		}
		break;
		case PWM_64Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 1125 - 1;
		}
		break;
		case PWM_125Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 576 - 1;
		}
		break;
		case PWM_250Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 288 - 1;
		}
		break;
		case PWM_500Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 144 - 1;
		}
		break;
		case PWM_1kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
		}
		break;
		case PWM_2kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 36 - 1;
		}
		break;
		case PWM_4kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 18 - 1;
		}
		break;
		case PWM_6kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 12 - 1;
		}
		break;
		case PWM_9kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 8 - 1;
		}
		break;
		case PWM_12kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 6 - 1;
		}
		break;
		case PWM_18kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 4 - 1;
		}
		break;
		case PWM_24kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 3 - 1;
		}
		break;
		case PWM_36kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 2 - 1;
		}
		break;
		default:
			break;
	}
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = duty;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);	
	TIM_Cmd(TIM3,ENABLE);
}

//*********************************************
//函数：PB8/TIM4_CH3配置为PWM输出
//参数：Fretype	--	PWM频率类型 (_PWM_FRE_)
//		Duty	--  占空比
//返回值：无
//*********************************************
void TIM4_CH3_PWM_Config(BYTE Fretype,uint16 duty)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	//初始化TIM4的时间基数单位
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//初始化TIM4的外设
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	//PB8 --TIM4_CH3		-- 复用推挽输出		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;	
	
	switch(Fretype)
	{
		case PWM_2Hz:
		{	
			TIM_TimeBaseInitStruct.TIM_Prescaler = 36000 - 1;
		}
		break;
		case PWM_4Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 18000 - 1;
		}
		break;
		case PWM_8Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 9000 - 1;
		}
		break;
		case PWM_16Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 4500 - 1;
		}
		break;
		case PWM_32Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 2250 - 1;
		}
		break;
		case PWM_64Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 1125 - 1;
		}
		break;
		case PWM_125Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 576 - 1;
		}
		break;
		case PWM_250Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 288 - 1;
		}
		break;
		case PWM_500Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 144 - 1;
		}
		break;
		case PWM_1kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
		}
		break;
		case PWM_2kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 36 - 1;
		}
		break;
		case PWM_4kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 18 - 1;
		}
		break;
		case PWM_6kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 12 - 1;
		}
		break;
		case PWM_9kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 8 - 1;
		}
		break;
		case PWM_12kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 6 - 1;
		}
		break;
		case PWM_18kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 4 - 1;
		}
		break;
		case PWM_24kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 3 - 1;
		}
		break;
		case PWM_36kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 2 - 1;
		}
		break;
		default:
			break;
	}
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = duty;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);	
	TIM_Cmd(TIM4,ENABLE);
}

//*********************************************
//函数：PA3/TIM5_CH4配置为PWM输出
//参数：Fretype		--	PWM频率类型 (_PWM_FRE_)
//		Duty	--  占空比
//返回值：无
//*********************************************
void TIM5_CH4_PWM_Config(BYTE Fretype,uint16 duty)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	//初始化TIM5的时间基数单位
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//初始化TIM5的外设
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	//PA3 --TIM5_CH4		-- 复用推挽输出		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;	
	
	switch(Fretype)
	{
		case PWM_2Hz:
		{	
			TIM_TimeBaseInitStruct.TIM_Prescaler = 36000 - 1;
		}
		break;
		case PWM_4Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 18000 - 1;
		}
		break;
		case PWM_8Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 9000 - 1;
		}
		break;
		case PWM_16Hz:
		{		
			TIM_TimeBaseInitStruct.TIM_Prescaler = 4500 - 1;
		}
		break;
		case PWM_32Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 2250 - 1;
		}
		break;
		case PWM_64Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 1125 - 1;
		}
		break;
		case PWM_125Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 576 - 1;
		}
		break;
		case PWM_250Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 288 - 1;
		}
		break;
		case PWM_500Hz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 144 - 1;
		}
		break;
		case PWM_1kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
		}
		break;
		case PWM_2kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 36 - 1;
		}
		break;
		case PWM_4kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 18 - 1;
		}
		break;
		case PWM_6kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 12 - 1;
		}
		break;
		case PWM_9kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 8 - 1;
		}
		break;
		case PWM_12kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 6 - 1;
		}
		break;
		case PWM_18kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 4 - 1;
		}
		break;
		case PWM_24kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 3 - 1;
		}
		break;
		case PWM_36kHz:
		{
			TIM_TimeBaseInitStruct.TIM_Prescaler = 2 - 1;
		}
		break;
		default:
			break;
	}
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = duty;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM5,&TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig(TIM5,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM5,ENABLE);	
	TIM_Cmd(TIM5,ENABLE);
}



