#include "UserProgram.h"
#include "User_PWM.h"

// 滴答时钟全局变量
uint16 nTime = 0;


//*****************************************
//函数：1US级延时
//参数：nUs -- 延时时间范围1
//返回值：无
//*****************************************
void Delay1Us(void)
{
	BYTE i;

	for(i = 0; i< 4; i++);
}


//*****************************************
//函数：大于等于2US级延时
//参数：nUs -- 延时时间范围2-65535
//返回值：无
//*****************************************
void DelayUs(uint16 nUs)
{
	nTime = nUs-2;
	SysTick_Config(72);
	while(nTime);
	SysTick->CTRL &= 0xfffe;
}


//*****************************************
//函数：MS级延时
//参数：nMs -- 延时时间范围1-65535
//返回值：无
//*****************************************
void DelayMs(uint16 nMs)
{
	nTime = nMs;
	SysTick_Config(72000);
	while(nTime);
	SysTick->CTRL &= 0xfffe;
}

//*****************************************
//函数：S级延时
//参数：ns -- 延时时间范围1-65535
//返回值：无
//*****************************************
void Delays(uint16 ns)
{
	while(ns--)
	{
		DelayMs(1000);
	}
}

//*********************************************
//函数：特殊引脚功能配置
//参数：pin_num -- 引脚号
//		pin_mode-- 引脚模式
//		param	-- 功能参数数组
//返回值：无
//*********************************************
void Special_Pin_Configuration(BYTE pin_num,BYTE pin_mode,BYTE * param)
{
	switch(pin_num)
	{
#include <stm32f10x.h>
		case PIN_PA8:
		{
			PA8_Out_Configuration(pin_mode,param);
		}
		break;
		case PIN_PA1:
		{
			PA1_Out_Configuration(pin_mode,param);
		}
		break;
		case PIN_PB0:
		{
			PB0_Out_Configuration(pin_mode,param);
		}
		break;
		case PIN_PB8:
		{
			PB8_Out_Configuration(pin_mode,param);
		}
		break;
		case PIN_PA3:
		{
			PA3_Out_Configuration(pin_mode,param);
		}
		break;
		
	}
}

//*********************************************
//函数：PA8引脚功能配置
//参数：pin_mode-- 引脚模式
//		param	-- 功能参数数组
//返回值：无
//*********************************************
void PA8_Out_Configuration(BYTE pin_mode,BYTE * param)
{
	switch(pin_mode)
	{
		case MODE_LEVEL:
		{
			PA8_Out_Level(param[0]);
		}
		break;
		case MODE_PULSE:
		{
			PA8_Out_Pulse(param[0],param[1],(uint16)((param[2] << 8) | param[3]),(uint16)((param[4] << 8) | param[5]),param[6]);
		}
		break;
		case MODE_PWM:
		{
			TIM1_CH1_PWM_Config(param[0],(uint16)((param[1] << 8) | param[2]));
		}
		break;
	}
}

//*********************************************
//函数：PA8引脚输出高低电平
//参数：flag-- 电平
//返回值：无
//*********************************************
void PA8_Out_Level(BYTE flag)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM1,DISABLE);
	
	//PA8	-- 	推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	if(flag)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_8); 
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_8); 
	}
}

//*********************************************
//函数：PA8引脚输出脉冲
//参数：num		-- 脉冲个数
//		timetype-- 脉冲时间单位0 - us;1 - ms; 2 - s
//		Hwidth	-- 脉冲高电平宽度
//		Lwidth	-- 脉冲低电平宽度
//		endflag	-- 脉冲结尾电平
//返回值：无
//*********************************************
void PA8_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag)
{
	BYTE i = 0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM1,DISABLE);
	
	//PA8	-- 	推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_8); 
	switch(timetype)
	{
		case 0:
		{
			
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_8); 
				if(Hwidth < 0x02)
				{
					Delay1Us();
				}
				else
				{
					DelayUs(Hwidth);
				}
				GPIO_ResetBits(GPIOA,GPIO_Pin_8); 
				if(Lwidth < 0x02)
				{
					Delay1Us();
				}
				else
				{
					DelayUs(Lwidth);
				}
			}
		}
		break;
		case 1:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_8); 
				DelayMs(Hwidth);
				GPIO_ResetBits(GPIOA,GPIO_Pin_8); 
				DelayMs(Lwidth);
			}
		}
		break;
		case 2:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_8); 
				Delays(Hwidth);
				GPIO_ResetBits(GPIOA,GPIO_Pin_8); 
				Delays(Lwidth);
			}
		}
		break;	
	}
	if(endflag)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_8); 
	}
}

//*********************************************
//函数：PA1引脚功能配置
//参数：pin_mode-- 引脚模式
//		param	-- 功能参数数组
//返回值：无
//*********************************************
void PA1_Out_Configuration(BYTE pin_mode,BYTE * param)
{
	switch(pin_mode)
	{
		case MODE_LEVEL:
		{
			PA1_Out_Level(param[0]);
		}
		break;
		case MODE_PULSE:
		{
			PA1_Out_Pulse(param[0],param[1],(uint16)((param[2] << 8) | param[3]),(uint16)((param[4] << 8) | param[5]),param[6]);
		}
		break;
		case MODE_PWM:
		{
			TIM2_CH2_PWM_Config(param[0],(uint16)((param[1] << 8) | param[2]));
		}
		break;
	}
}

//*********************************************
//函数：PA1引脚输出高低电平
//参数：flag-- 电平
//返回值：无
//*********************************************
void PA1_Out_Level(BYTE flag)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM2,DISABLE);
	
	//PA1	-- 	推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	if(flag)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_1); 
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
	}
}

//*********************************************
//函数：PA1引脚输出脉冲
//参数：num		-- 脉冲个数
//		timetype-- 脉冲时间单位0 - us;1 - ms; 2 - s
//		Hwidth	-- 脉冲高电平宽度ms
//		Lwidth	-- 脉冲低电平宽度ms
//		endflag	-- 脉冲结尾电平
//返回值：无
//*********************************************
void PA1_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag)
{
	BYTE i = 0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM2,DISABLE);
	
	//PA1	-- 	推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
	switch(timetype)
	{
		case 0:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_1); 
				if(Hwidth < 0x02)
				{
					Delay1Us();
				}
				else
				{
					DelayUs(Hwidth);
				}
				GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
				if(Lwidth < 0x02)
				{
					Delay1Us();
				}
				else
				{
					DelayUs(Lwidth);
				}
			}
		}
		break;
		case 1:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_1); 
				DelayMs(Hwidth);
				GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
				DelayMs(Lwidth);
			}
		}
		break;
		case 2:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_1); 
				Delays(Hwidth);
				GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
				Delays(Lwidth);
			}
		}
		break;	
	}
	if(endflag)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_1); 
	}
}

//*********************************************
//函数：PB0引脚功能配置
//参数：pin_mode-- 引脚模式
//		param	-- 功能参数数组
//返回值：无
//*********************************************
void PB0_Out_Configuration(BYTE pin_mode,BYTE * param)
{
	switch(pin_mode)
	{
		case MODE_LEVEL:
		{
			PB0_Out_Level(param[0]);
		}
		break;
		case MODE_PULSE:
		{
			PB0_Out_Pulse(param[0],param[1],(uint16)((param[2] << 8) | param[3]),(uint16)((param[4] << 8) | param[5]),param[6]);
		}
		break;
		case MODE_PWM:
		{
			TIM3_CH3_PWM_Config(param[0],(uint16)((param[1] << 8) | param[2]));
		}
		break;
	}
}

//*********************************************
//函数：PB0引脚输出高低电平
//参数：flag-- 电平
//返回值：无
//*********************************************
void PB0_Out_Level(BYTE flag)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM3,DISABLE);
	
	//PB0	-- 	推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
	
	if(flag)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_0); 
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0); 
	}
}

//*********************************************
//函数：PB0引脚输出脉冲
//参数：num		-- 脉冲个数
//		timetype-- 脉冲时间单位0 - us;1 - ms; 2 - s
//		Hwidth	-- 脉冲高电平宽度ms
//		Lwidth	-- 脉冲低电平宽度ms
//		endflag	-- 脉冲结尾电平
//返回值：无
//*********************************************
void PB0_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag)
{
	BYTE i = 0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM3,DISABLE);
	
	//PB0	-- 	推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0); 
	switch(timetype)
	{
		case 0:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_0); 
				if(Hwidth < 0x02)
				{
					Delay1Us();
				}
				else
				{
					DelayUs(Hwidth);
				}
				GPIO_ResetBits(GPIOB,GPIO_Pin_0); 
				if(Hwidth < 0x02)
				{
					Delay1Us();
				}
				else
				{
					DelayUs(Lwidth);
				}
			}
		}
		break;
		case 1:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_0); 
				DelayMs(Hwidth);
				GPIO_ResetBits(GPIOB,GPIO_Pin_0); 
				DelayMs(Lwidth);
			}
		}
		break;
		case 2:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_0); 
				Delays(Hwidth);
				GPIO_ResetBits(GPIOB,GPIO_Pin_0); 
				Delays(Lwidth);
			}
		}
		break;
	}
	if(endflag)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_0); 
	}
}

//*********************************************
//函数：PB8引脚功能配置
//参数：pin_mode-- 引脚模式
//		param	-- 功能参数数组
//返回值：无
//*********************************************
void PB8_Out_Configuration(BYTE pin_mode,BYTE * param)
{
	switch(pin_mode)
	{
		case MODE_LEVEL:
		{
			PB8_Out_Level(param[0]);
		}
		break;
		case MODE_PULSE:
		{
			PB8_Out_Pulse(param[0],param[1],(uint16)((param[2] << 8) | param[3]),(uint16)((param[4] << 8) | param[5]),param[6]);
		}
		break;
		case MODE_PWM:
		{
			TIM4_CH3_PWM_Config(param[0],(uint16)((param[1] << 8) | param[2]));
		}
		break;
	}
}

//*********************************************
//函数：PB8引脚输出高低电平
//参数：flag-- 电平
//返回值：无
//*********************************************
void PB8_Out_Level(BYTE flag)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM4,DISABLE);
	
	//PB8	-- 	推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
	
	if(flag)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_8); 
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
	}
}

//*********************************************
//函数：PB8引脚输出脉冲
//参数：num		-- 脉冲个数
//		timetype-- 脉冲时间单位0 - us;1 - ms; 2 - s
//		Hwidth	-- 脉冲高电平宽度ms
//		Lwidth	-- 脉冲低电平宽度ms
//		endflag	-- 脉冲结尾电平
//返回值：无
//*********************************************
void PB8_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag)
{
	BYTE i = 0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM4,DISABLE);
	
	//PB8	-- 	推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
	switch(timetype)
	{
		case 0:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_8);
				if(Hwidth < 0x02)
				{
					Delay1Us();
				}
				else
				{
					DelayUs(Hwidth);
				}
				GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
				if(Lwidth < 0x02)
				{
					Delay1Us();
				}
				else
				{
					DelayUs(Lwidth);
				}
			}
		}
		break;
		case 1:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_8); 
				DelayMs(Hwidth);
				GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
				DelayMs(Lwidth);
			}
		}
		break;
		case 2:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_8); 
				Delays(Hwidth);
				GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
				Delays(Lwidth);
			}
		}
		break;
	}

	if(endflag)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_8); 
	}
}

//*********************************************
//函数：PA3引脚功能配置
//参数：pin_mode-- 引脚模式
//		param	-- 功能参数数组
//返回值：无
//*********************************************
void PA3_Out_Configuration(BYTE pin_mode,BYTE * param)
{
	switch(pin_mode)
	{
		case MODE_LEVEL:
		{
			PA3_Out_Level(param[0]);
		}
		break;
		case MODE_PULSE:
		{
			PA3_Out_Pulse(param[0],param[1],(uint16)((param[2] << 8) | param[3]),(uint16)((param[4] << 8) | param[5]),param[6]);
		}
		break;
		case MODE_PWM:
		{
			TIM5_CH4_PWM_Config(param[0],(uint16)((param[1] << 8) | param[2]));
		}
		break;
	}
}

//*********************************************
//函数：PA3引脚输出高低电平
//参数：flag-- 电平
//返回值：无
//*********************************************
void PA3_Out_Level(BYTE flag)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM5,DISABLE);
	
	//PA3	-- 	推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	if(flag)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_3); 
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_3); 
	}
}

//*********************************************
//函数：PA3引脚输出脉冲
//参数：num		-- 脉冲个数
//		timetype-- 脉冲时间单位0 - us;1 - ms; 2 - s
//		Hwidth	-- 脉冲高电平宽度ms
//		Lwidth	-- 脉冲低电平宽度ms
//		endflag	-- 脉冲结尾电平
//返回值：无
//*********************************************
void PA3_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag)
{
	BYTE i = 0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM5,DISABLE);
	
	//PA0	-- 	推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_3); 
	switch(timetype)
	{
		case 0:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_3); 
				if(Hwidth < 0x02)
				{
					Delay1Us();
				}
				else
				{
					DelayUs(Hwidth);
				}
				GPIO_ResetBits(GPIOA,GPIO_Pin_3); 
				if(Lwidth < 0x02)
				{
					Delay1Us();
				}
				else
				{
					DelayUs(Lwidth);
				}
			}
		}
		break;
		case 1:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_3); 
				DelayMs(Hwidth);
				GPIO_ResetBits(GPIOA,GPIO_Pin_3); 
				DelayMs(Lwidth);
			}
		}
		break;
		case 2:
		{
			for(i = 0; i < num; i++)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_3); 
				Delays(Hwidth);
				GPIO_ResetBits(GPIOA,GPIO_Pin_3); 
				Delays(Lwidth);
			}
		}
		break;
	}
	
	if(endflag)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_3); 
	}
}

