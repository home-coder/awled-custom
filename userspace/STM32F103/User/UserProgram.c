#include "UserProgram.h"
#include "User_PWM.h"

// �δ�ʱ��ȫ�ֱ���
uint16 nTime = 0;


//*****************************************
//������1US����ʱ
//������nUs -- ��ʱʱ�䷶Χ1
//����ֵ����
//*****************************************
void Delay1Us(void)
{
	BYTE i;

	for(i = 0; i< 4; i++);
}


//*****************************************
//���������ڵ���2US����ʱ
//������nUs -- ��ʱʱ�䷶Χ2-65535
//����ֵ����
//*****************************************
void DelayUs(uint16 nUs)
{
	nTime = nUs-2;
	SysTick_Config(72);
	while(nTime);
	SysTick->CTRL &= 0xfffe;
}


//*****************************************
//������MS����ʱ
//������nMs -- ��ʱʱ�䷶Χ1-65535
//����ֵ����
//*****************************************
void DelayMs(uint16 nMs)
{
	nTime = nMs;
	SysTick_Config(72000);
	while(nTime);
	SysTick->CTRL &= 0xfffe;
}

//*****************************************
//������S����ʱ
//������ns -- ��ʱʱ�䷶Χ1-65535
//����ֵ����
//*****************************************
void Delays(uint16 ns)
{
	while(ns--)
	{
		DelayMs(1000);
	}
}

//*********************************************
//�������������Ź�������
//������pin_num -- ���ź�
//		pin_mode-- ����ģʽ
//		param	-- ���ܲ�������
//����ֵ����
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
//������PA8���Ź�������
//������pin_mode-- ����ģʽ
//		param	-- ���ܲ�������
//����ֵ����
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
//������PA8��������ߵ͵�ƽ
//������flag-- ��ƽ
//����ֵ����
//*********************************************
void PA8_Out_Level(BYTE flag)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM1,DISABLE);
	
	//PA8	-- 	�������
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
//������PA8�����������
//������num		-- �������
//		timetype-- ����ʱ�䵥λ0 - us;1 - ms; 2 - s
//		Hwidth	-- ����ߵ�ƽ���
//		Lwidth	-- ����͵�ƽ���
//		endflag	-- �����β��ƽ
//����ֵ����
//*********************************************
void PA8_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag)
{
	BYTE i = 0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM1,DISABLE);
	
	//PA8	-- 	�������
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
//������PA1���Ź�������
//������pin_mode-- ����ģʽ
//		param	-- ���ܲ�������
//����ֵ����
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
//������PA1��������ߵ͵�ƽ
//������flag-- ��ƽ
//����ֵ����
//*********************************************
void PA1_Out_Level(BYTE flag)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM2,DISABLE);
	
	//PA1	-- 	�������
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
//������PA1�����������
//������num		-- �������
//		timetype-- ����ʱ�䵥λ0 - us;1 - ms; 2 - s
//		Hwidth	-- ����ߵ�ƽ���ms
//		Lwidth	-- ����͵�ƽ���ms
//		endflag	-- �����β��ƽ
//����ֵ����
//*********************************************
void PA1_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag)
{
	BYTE i = 0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM2,DISABLE);
	
	//PA1	-- 	�������
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
//������PB0���Ź�������
//������pin_mode-- ����ģʽ
//		param	-- ���ܲ�������
//����ֵ����
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
//������PB0��������ߵ͵�ƽ
//������flag-- ��ƽ
//����ֵ����
//*********************************************
void PB0_Out_Level(BYTE flag)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM3,DISABLE);
	
	//PB0	-- 	�������
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
//������PB0�����������
//������num		-- �������
//		timetype-- ����ʱ�䵥λ0 - us;1 - ms; 2 - s
//		Hwidth	-- ����ߵ�ƽ���ms
//		Lwidth	-- ����͵�ƽ���ms
//		endflag	-- �����β��ƽ
//����ֵ����
//*********************************************
void PB0_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag)
{
	BYTE i = 0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM3,DISABLE);
	
	//PB0	-- 	�������
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
//������PB8���Ź�������
//������pin_mode-- ����ģʽ
//		param	-- ���ܲ�������
//����ֵ����
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
//������PB8��������ߵ͵�ƽ
//������flag-- ��ƽ
//����ֵ����
//*********************************************
void PB8_Out_Level(BYTE flag)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM4,DISABLE);
	
	//PB8	-- 	�������
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
//������PB8�����������
//������num		-- �������
//		timetype-- ����ʱ�䵥λ0 - us;1 - ms; 2 - s
//		Hwidth	-- ����ߵ�ƽ���ms
//		Lwidth	-- ����͵�ƽ���ms
//		endflag	-- �����β��ƽ
//����ֵ����
//*********************************************
void PB8_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag)
{
	BYTE i = 0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM4,DISABLE);
	
	//PB8	-- 	�������
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
//������PA3���Ź�������
//������pin_mode-- ����ģʽ
//		param	-- ���ܲ�������
//����ֵ����
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
//������PA3��������ߵ͵�ƽ
//������flag-- ��ƽ
//����ֵ����
//*********************************************
void PA3_Out_Level(BYTE flag)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM5,DISABLE);
	
	//PA3	-- 	�������
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
//������PA3�����������
//������num		-- �������
//		timetype-- ����ʱ�䵥λ0 - us;1 - ms; 2 - s
//		Hwidth	-- ����ߵ�ƽ���ms
//		Lwidth	-- ����͵�ƽ���ms
//		endflag	-- �����β��ƽ
//����ֵ����
//*********************************************
void PA3_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag)
{
	BYTE i = 0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_Cmd(TIM5,DISABLE);
	
	//PA0	-- 	�������
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

