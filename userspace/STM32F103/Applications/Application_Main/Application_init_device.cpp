#include "stm32f10x_gpio.h"
#include "User_IIC.h"
#include "User_Exti.h"
#include "Application_main.h"
#include "Application_KEY.h"
#include "Application_LED.h"
#include "Application_init_device.h"

#ifdef APPLICATION_USB_ENABLE
#include "usb_core.h"
#include "usb_init.h"
extern void Set_USBClock(void);
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef APPLICATION_WISTRON_ENABLE

static void application_rcc_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 | RCC_APB1Periph_I2C2 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5, ENABLE );

	//关闭PA15引脚的JTAG功能
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

#ifdef APPLICATION_ADC_ENABLE
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12MHZ
#endif
}

static void application_set_gpio_data(const MCU_GPIO_STRUCT *p_gpio_info)
{
	GPIO_TypeDef *p_gpio_group = NULL;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_LEVEL_STRUCT *p_gpio_level = NULL;
	GPIO_IN_FLOATING_STRUCT *p_gpio_in_floating = NULL;
	
	if ( NULL != p_gpio_info
		&& NULL != p_gpio_info->p_gpio_data )
	{
		p_gpio_group = aw_chip_gpio_get_group( p_gpio_info->gpio_group );
		if ( NULL != p_gpio_group )
		{
			GPIO_InitStructure.GPIO_Pin = p_gpio_info->gpio_pin;
			switch ( p_gpio_info->gpio_type )
			{
				case GPIO_OUT_LEVEL:
					p_gpio_level = (GPIO_LEVEL_STRUCT *)p_gpio_info->p_gpio_data;
					GPIO_InitStructure.GPIO_Mode = p_gpio_level->mode;
					GPIO_InitStructure.GPIO_Speed = p_gpio_level->speed;

					GPIO_Init( p_gpio_group, &GPIO_InitStructure );
					if ( p_gpio_level->action )
					{
						GPIO_SetBits( p_gpio_group, p_gpio_info->gpio_pin );
					}
					else
					{
						GPIO_ResetBits( p_gpio_group, p_gpio_info->gpio_pin );
					}
					break;

				case GPIO_IN_FLOATING:
					p_gpio_in_floating = (GPIO_IN_FLOATING_STRUCT *)p_gpio_info->p_gpio_data;
					GPIO_InitStructure.GPIO_Mode = p_gpio_in_floating->mode;

					GPIO_Init( p_gpio_group, &GPIO_InitStructure );
					break;

				default:
					break;
			}
		}
	}
}

static void application_led_gpios_init(void)
{
	BYTE chip_index = 0;
	BYTE gpio_index = 0;
	const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info = NULL;

	for ( chip_index = 0; chip_index < led_get_chips_number(); chip_index++ )
	{
		p_led_hw_info = led_get_hw_info_by_chip_index( chip_index );
		if ( NULL != p_led_hw_info 
			&& APPLI_CHIPS_NONE != p_led_hw_info->awchip_id )
		{
			for ( gpio_index = 0; gpio_index < p_led_hw_info->pins_struct.pin_nums; gpio_index++ )
			{
				application_set_gpio_data( p_led_hw_info->pins_struct.p_pins_data+gpio_index );
			}
		}
	}
}

static void application_key_gpios_init(void)
{
	BYTE chip_index = 0;
	BYTE gpio_index = 0;
	const AWCHIP_KEYS_HW_STRUCT *p_key_hw_info = NULL;

	for ( chip_index = 0; chip_index < key_get_chips_number(); chip_index++ )
	{
		p_key_hw_info = key_get_hw_info_by_chip_index( chip_index );
		if ( NULL != p_key_hw_info 
			&& APPLI_CHIPS_NONE != p_key_hw_info->awchip_id )
		{
			for ( gpio_index = 0; gpio_index < p_key_hw_info->pins_struct.pin_nums; gpio_index++ )
			{
				application_set_gpio_data( p_key_hw_info->pins_struct.p_pins_data+gpio_index );
			}
		}
	}
}

static void application_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	//PB6	-- IIC1_SCL	--	复用开漏输出
	//PB7	-- IIC1_SDA	--	复用开漏输出
	//PB10	-- IIC2_SCL	--	复用开漏输出
	//PB11	-- IIC2_SDA	--	复用开漏输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	// PA6  --- SHDN_9818  --- 推挽输出
	application_led_gpios_init();

	// PA7  --- PDN_9163  --- 推挽输出
	// PC4  --- INTN_9163  --- 上拉输入
	application_key_gpios_init();

#ifdef APPLICATION_USB_ENABLE
	//PA2	-- USB_CTRL 	-- 	推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	//USB_CTRL = 1 
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
#endif

#ifdef APPLICATION_ADC_ENABLE
	/* Configure PA.0 (ADC Channel) as analog input -------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
}

static void application_I2C1_init(void)
{
	IIC1_Init();
}

static void application_I2C2_init(void)
{
	IIC2_Init();
}

static void application_I2C_init(void)
{
	application_I2C1_init();
	application_I2C2_init();
}

// Initialize external interrupt struct
// Such as GPIO group, GPIO pin, line, line command, mode, trigger
static void application_ext_interrupt_init(void)
{
	Exti_PC4Init();
}

static void application_usb_nvic_init(void)
{
#ifdef APPLICATION_USB_ENABLE
	static const NVIC_InitTypeDef usb_nvic = { 
		USB_LP_CAN1_RX0_IRQn, //USB中断选通
		0, //抢占优先级
		0, //子优先级
		ENABLE, //使能中断控制
	};

	NVIC_Init( &usb_nvic );
#endif
}

static void application_key_nvic_init(void)
{
	BYTE chip_index = 0;
	BYTE gpio_index = 0;
	const AWCHIP_KEYS_HW_STRUCT *p_key_hw_info = NULL;
	NVIC_InitTypeDef *p_nvic = NULL;

	for ( chip_index = 0; chip_index < key_get_chips_number(); chip_index++ )
	{
		p_key_hw_info = key_get_hw_info_by_chip_index( chip_index );
		if ( NULL != p_key_hw_info 
			&& APPLI_CHIPS_NONE != p_key_hw_info->awchip_id )
		{
			for ( gpio_index = 0; gpio_index < p_key_hw_info->pins_struct.pin_nums; gpio_index++ )
			{
				p_nvic = (NVIC_InitTypeDef *)(p_key_hw_info->pins_struct.p_pins_data[gpio_index].p_nvic_data);
				if ( NULL != p_nvic )
				{
					NVIC_Init( p_nvic );
				}
			}
		}
	}
}

static void application_TIMx_nvic_init(void)
{
	static const NVIC_InitTypeDef timx_nvic[] = {
		{ 
			TIM2_IRQn, //TIM2中断选通
			2, //抢占优先级
			0, //子优先级
			ENABLE, //使能中断控制
		},
	//	{ 
	//		TIM3_IRQn, //TIM3中断选通
	//		2, //抢占优先级
	//		1, //子优先级
	//		ENABLE, //使能中断控制
	//	},
	//	{ 
	//		TIM4_IRQn, //TIM4中断选通
	//		2, //抢占优先级
	//		2, //子优先级
	//		ENABLE, //使能中断控制
	//	},
	//	{ 
	//		TIM5_IRQn, //TIM5中断选通
	//		2, //抢占优先级
	//		3, //子优先级
	//		ENABLE, //使能中断控制
	//	},
	};
	BYTE i = 0;

	for ( i = 0; i < sizeof(timx_nvic)/sizeof(NVIC_InitTypeDef); i++ )
	{
		NVIC_Init( (NVIC_InitTypeDef *)(timx_nvic+i) );
	}
}

// Initialize external interrupt priority
// Such as priority group, IRQ channel, Preemption Priority, SubPriority, command
static void application_nvic_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the USB interrupt */
	application_usb_nvic_init();

	//Key -- PC4 下降沿外部中断
	application_key_nvic_init();

	//TIMx 中断
	application_TIMx_nvic_init();
}

static void application_set_timer(TIM_TypeDef * TIMx, uint16_t msDelay)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

	// which has been enabled in RCCInit()
	//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM2定时器时钟使能

	TIM_DeInit(TIMx); //复位TIM2定时器
	TIM_TimeBaseStructure.TIM_Period=msDelay-1; //定时器周期
	TIM_TimeBaseStructure.TIM_Prescaler=36000-1; //预分频数
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //TIM2时钟分频，为1表示不分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //定时器计数为向上计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //
	
	TIM_ClearFlag(TIMx, TIM_FLAG_Update); //清除定时器2的溢出标志位
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE); //使能定时器2溢出中断

	//	TIM_Cmd(TIM2, ENABLE); //定时器2使能
	//void TIM2_IRQHandler(void) defined in stm32f10x_it.c
}

void application_start_timer(TIM_TypeDef * TIMx, uint16_t msDelay)
{
	application_set_timer( TIMx, msDelay );

	TIM_Cmd( TIMx, ENABLE );
}

void application_restart_timer(TIM_TypeDef * TIMx)
{
	TIM_Cmd( TIMx, ENABLE );
}

void application_stop_timer(TIM_TypeDef * TIMx)
{
	TIM_Cmd( TIMx, DISABLE );
}

static void application_timer_init(void)
{
//	application_set_timer( TIM2, 100 );
//	application_set_timer( TIM3, 50 );
//	application_set_timer( TIM4, 15 );
//	application_set_timer( TIM5, 30 );
}

static void application_usb_init(void)
{
#ifdef APPLICATION_USB_ENABLE
/* Enable USB */
	Set_USBClock();
	USB_Init();	
	USB_ENCONNECT();
#endif
}

static void application_adc_configuration(void)
{
#ifdef APPLICATION_ADC_ENABLE

	ADC_InitTypeDef ADC_InitStructure;
	
	//ADC_DeInit(ADC1);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* Enable ADC1 reset calibration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

#endif /* APPLICATION_ADC_ENABLE */
}

#ifdef APPLICATION_ADC_ENABLE
static WORD application_get_adc(BYTE ch)
{
	WORD DataValue = 0;
	/* ADC1 regular channel14 configuration */ 
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);

	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	/* Test if the ADC1 EOC flag is set or not */ 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

	/*Returns the ADC1 Master data value of the last converted channel*/
	DataValue = ADC_GetConversionValue(ADC1); 

	return DataValue;	
}
#endif /* APPLICATION_ADC_ENABLE */

uint32 application_get_seed(void)
{
	uint32 seed = 0;
#ifdef APPLICATION_ADC_ENABLE
	uint32 i = 0;
	WORD temp = 0;
	
	seed = 0;
	for ( i = 0; i < 16; i++ )
	{
		temp = application_get_adc(ADC_Channel_0);
		
		temp &= 0x0001;
		
		seed |= temp << i;
	}
#endif /* APPLICATION_ADC_ENABLE */

	return seed;
}

void application_device_init(void)
{
	application_rcc_init();
	application_gpio_init();
	application_I2C_init();
	application_ext_interrupt_init();
	application_nvic_init();
	application_timer_init();
	application_usb_init();
	application_adc_configuration();
}

#endif /* APPLICATION_WISTRON_ENABLE */

#ifdef __cplusplus
}
#endif /* __cplusplus */
