#include "Application_main.h"
#include "Application_init_device.h"
#include "Application_LED_interface.h"
#include "Application_LED_effect.h"
#include "Application_LED.h"
#include "Application_KEY.h"

#include "stm32f10x_gpio.h"
#include "User_Exti.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef APPLICATION_WISTRON_ENABLE
extern void DelayMs(uint16 nMs);

static MAIN_CMD_STRUCT *p_cmd_list = NULL;

// Get the first command to handle,
static MAIN_CMD_STRUCT * application_get_cmd(void)
{
	return p_cmd_list;
}

static void application_set_cmd(MAIN_CMD_STRUCT * p_cmd_pointer)
{
	p_cmd_list = p_cmd_pointer;
}

// Add a new commmand to the tail of command list
static BOOL application_push_cmd(MAIN_CMD_STRUCT *p_new_cmd)
{
	BOOL re = FALSE;
	MAIN_CMD_STRUCT *p_cmd = application_get_cmd();
	MAIN_CMD_STRUCT *p_tmp_cmd = NULL;
	
	if ( p_new_cmd != NULL )
	{
		// get the tail of command list
		while ( NULL != p_cmd )
		{
			p_tmp_cmd = p_cmd;
			p_cmd = p_cmd->p_next;
		}

		// malloc a new command buffer
		p_cmd = (MAIN_CMD_STRUCT *)Application_malloc(sizeof(MAIN_CMD_STRUCT));
		if ( NULL != p_cmd )
		{
			// if p_tmp_cmd is NULL, 
			// that means there is not any command in list 
			if ( NULL == p_tmp_cmd )
			{
				// set the new command as the header of command list
				application_set_cmd( p_cmd );
			}
			else
			{
				// set the new command as the tail of command list
				p_tmp_cmd->p_next = p_cmd;
			}

			// copy command data to the list
			re = TRUE;
			memcpy( (void *)p_cmd, (const void *)p_new_cmd, sizeof(MAIN_CMD_STRUCT) );
			p_cmd->p_next = NULL;
		}
	}

	return re;
}

// Remove the the first command from list
static void application_pop_cmd(void)
{
	MAIN_CMD_STRUCT *p_cmd_pointer = NULL;
	MAIN_CMD_STRUCT *p_tmp_cmd = NULL;

	p_cmd_pointer = application_get_cmd();
	// free the header of command list
	if ( NULL != p_cmd_pointer )
	{
		// get the next command pointer
		p_tmp_cmd = p_cmd_pointer->p_next;

		// free the current command buffer
		Application_free( p_cmd_pointer );

		// set the next command pointer 
		// to the header of command list
		application_set_cmd( p_tmp_cmd );
	}
}

static APP_FUNC_STRUCT *application_main_get_func(void)
{
	static APP_FUNC_STRUCT app_func;

	return &app_func;
}

TIM_TypeDef * application_get_effect_TIMx(void)
{
	return TIM2;
}

static void application_add_new_effect_int(void)
{
	MAIN_CMD_STRUCT cmd;
	APP_FUNC_STRUCT *p_app_func = application_main_get_func();

	Exti_SetTIMFlag(application_get_effect_TIMx(), 0);	// clear timer flag

	// go on to display the current led effection
	if ( NULL != p_app_func 
		&& NULL != p_app_func->p_app_push_cmd )
	{
		cmd.cmd_type = LED_CMD_EFFECT_INT_HANDLE;
		p_app_func->p_app_push_cmd( &cmd );
	}
}

static void application_handle_key_interrupt(void)
{
	APP_LED_EFFECT_ENUM new_led_state = LEDS_EFFECT_NONE;

	// key interrupt
	if ( Exti_GetPC4Flag() )
	{
		Exti_SetPC4Flag(0);

		// we think the first key chip is valid
		new_led_state = key_chip_handle_interrupt( 0 );
		// handle key interrupt to get new state for led
		if ( LEDS_EFFECT_NONE != new_led_state )
		{
			application_stop_timer( application_get_effect_TIMx() );

			application_set_led_cur_state( new_led_state );
			application_add_new_effect_int();
		}
	}
}

static void application_handle_timer_interrupt(void)
{
	// TIMx interrupt
	if ( Exti_GetTIMFlag(application_get_effect_TIMx()) )
	{
		application_add_new_effect_int();
	}
/*	else if ( Exti_GetTIMFlag(TIM3) )
	{
		Exti_SetTIMFlag(TIM3, 0);	// clear timer flag

		if ( NULL != p_app_func 
			&& NULL != p_app_func->p_app_push_cmd )
		{
			cmd.cmd_type = TIMER_CMD_TIM3_RESTART;
			p_app_func->p_app_push_cmd( &cmd );
			
			cmd.cmd_type = TIMER_CMD_TIM3_EXPIRED;
			p_app_func->p_app_push_cmd( &cmd );
		}
	}
	else if ( Exti_GetTIMFlag(TIM4) )
	{
		Exti_SetTIMFlag(TIM4, 0);	// clear timer flag

		if ( NULL != p_app_func 
			&& NULL != p_app_func->p_app_push_cmd )
		{
			cmd.cmd_type = TIMER_CMD_TIM4_RESTART;
			p_app_func->p_app_push_cmd( &cmd );
			
			cmd.cmd_type = TIMER_CMD_TIM4_EXPIRED;
			p_app_func->p_app_push_cmd( &cmd );
		}
	}
	else if ( Exti_GetTIMFlag(TIM5) )
	{
		Exti_SetTIMFlag(TIM5, 0);	// clear timer flag

		if ( NULL != p_app_func 
			&& NULL != p_app_func->p_app_push_cmd )
		{
			cmd.cmd_type = TIMER_CMD_TIM5_RESTART;
			p_app_func->p_app_push_cmd( &cmd );
			
			cmd.cmd_type = TIMER_CMD_TIM5_EXPIRED;
			p_app_func->p_app_push_cmd( &cmd );
		}
	}
*/
}

static void application_handle_interrupt(void)
{
	application_handle_key_interrupt();

	application_handle_timer_interrupt();
}

static void application_set_gpio_level(const MCU_GPIO_STRUCT * p_gpio_level, BYTE action)
{
	MCU_GPIO_LEVEL_STRUCT gpio_level_info;

	if ( NULL != p_gpio_level
		&& NULL != p_gpio_level->p_gpio_data
		&& GPIO_OUT_LEVEL == p_gpio_level->gpio_type )
	{
		gpio_level_info.gpio_group = p_gpio_level->gpio_group;
		gpio_level_info.gpio = p_gpio_level->gpio_pin;
		gpio_level_info.action = action;
		gpio_level_info.mode = ((GPIO_LEVEL_STRUCT *)p_gpio_level->p_gpio_data)->mode;
		gpio_level_info.speed = ((GPIO_LEVEL_STRUCT *)p_gpio_level->p_gpio_data)->speed;

		APPLICATION_ControlGPIOLEVEL( (const MCU_GPIO_LEVEL_STRUCT *)&gpio_level_info );
	}
}

static void application_set_led_gpio_level(BYTE chip_index, BYTE pin_index, BYTE action)
{
	const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info = NULL;
	const MCU_GPIO_STRUCT *p_gpio_info = NULL;

	p_led_hw_info = led_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_led_hw_info
		&& APPLI_CHIPS_NONE != p_led_hw_info->awchip_id
		&& pin_index < p_led_hw_info->pins_struct.pin_nums )
	{
		p_gpio_info = p_led_hw_info->pins_struct.p_pins_data + pin_index;
		application_set_gpio_level( p_gpio_info, action );
	}
}

static void application_set_key_gpio_level(BYTE chip_index, BYTE pin_index, BYTE action)
{
	const AWCHIP_KEYS_HW_STRUCT *p_key_hw_info = NULL;
	const MCU_GPIO_STRUCT *p_gpio_info = NULL;

	p_key_hw_info = key_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_key_hw_info
		&& APPLI_CHIPS_NONE != p_key_hw_info->awchip_id
		&& pin_index < p_key_hw_info->pins_struct.pin_nums )
	{
		p_gpio_info = p_key_hw_info->pins_struct.p_pins_data + pin_index;
		application_set_gpio_level( p_gpio_info, action );
	}
}

static void application_handle_cmd(MAIN_CMD_STRUCT * p_cmd)
{
	if ( p_cmd != NULL )
	{
		switch ( p_cmd->cmd_type )
		{
			case MAIN_CMD_NONE:
				break;

			case LED_CMD_POWER_ON:
				application_set_led_gpio_level( 0, 0, Bit_SET );
				DelayMs( 1 );
				break;
				
			case LED_CMD_POWER_OFF:
				application_set_led_gpio_level( 0, 0, Bit_RESET );
				DelayMs( 1 );
				break;
				
			case LED_CMD_CHIPID_CHECK:
				led_check_chip_id( 0 );
				break;

			case LED_CMD_REG_INIT:
				led_regs_init( 0 );
				break;
				
			case LED_CMD_REG_SLEEP:
				led_chip_sleep( 0 );
				break;

			case LED_CMD_REG_READ:
				led_chip_read_reg_cfg( 0 );
				break;
				
			case LED_CMD_EFFECT_INIT:
				application_led_effect_init( 0 );
				break;

			case LED_CMD_EFFECT_INT_HANDLE:
				application_led_effect_interrupt_handle( 0 );
				break;

			case KEY_CMD_POWER_ON:
				application_set_key_gpio_level( 0, 0, Bit_SET );
				DelayMs( 1 );
				break;

			case KEY_CMD_POWER_OFF:
				application_set_key_gpio_level( 0, 0, Bit_RESET );
				DelayMs( 1 );
				break;

			case KEY_CMD_CHIPID_CHECK:
				key_check_chip_id( 0 );
				break;

			case KEY_CMD_REG_INIT:
				key_regs_init( 0 );
				break;

			case KEY_CMD_REG_SLEEP:
				key_chip_sleep( 0 );
				break;

			case KEY_CMD_REG_SOFTRESET:
				key_chip_softreset( 0 );
				break;

			case KEY_CMD_REG_READ:
				key_chip_read_reg_cfg( 0 );
				break;

			case TIMER_CMD_TIM2_START:
			//	application_start_timer( TIM2, 2 );
				break;

			case TIMER_CMD_TIM2_RESTART:
			//	application_restart_timer( TIM2 );
				break;

			case TIMER_CMD_TIM2_EXPIRED:
			//	application_led_effect_interrupt_handle( AW981X_1 );
				break;

			case TIMER_CMD_TIM3_START:
			//	application_start_timer( TIM3, 1000 );
				break;

			case TIMER_CMD_TIM3_RESTART:
			//	application_restart_timer( TIM3 );
				break;

			case TIMER_CMD_TIM3_EXPIRED:
			//	led_effect_state = application_led_effect_random();
				break;

			case TIMER_CMD_TIM4_START:
			//	application_start_timer( TIM4, 15 );
				break;

			case TIMER_CMD_TIM4_RESTART:
			//	application_restart_timer( TIM4 );
				break;

			case TIMER_CMD_TIM4_EXPIRED:
				break;

			case TIMER_CMD_TIM5_START:
			//	application_start_timer( TIM5, 30 );
				break;

			case TIMER_CMD_TIM5_RESTART:
			//	application_restart_timer( TIM5 );
				break;

			case TIMER_CMD_TIM5_EXPIRED:
				break;
			
			default:
				break;
		}
	}
}

static void application_main_register(APP_FUNC_STRUCT *p_app_func)
{
	if ( NULL != p_app_func )
	{
		memset( (void *)p_app_func, 0, sizeof(APP_FUNC_STRUCT) );

		p_app_func->p_app_int_cmd = application_handle_interrupt;
		p_app_func->p_app_push_cmd = application_push_cmd;
		p_app_func->p_app_pop_cmd = application_pop_cmd;
		p_app_func->p_app_get_cmd = application_get_cmd;
		p_app_func->p_app_handle_cmd = application_handle_cmd;
	}
}

static void application_led_buffer_init(void)
{
	// Initialize buffer for writing chip registers
	led_init_regs_buffer();

	// Initialize buffer for leds effect
	application_init_leds_info();
}

static void application_led_buffer_uninit(void)
{
	// free buffers
	led_uninit_regs_buffer();

	application_uninit_leds_info();
}

static void application_cmd_init(MAIN_CMD_ENUM *p_init_cmd, BYTE cmd_num)
{
	BYTE i = 0;
	MAIN_CMD_STRUCT cmd;
	pAppPushCmd p_cmd_push = NULL;
	APP_FUNC_STRUCT *p_app_func = application_main_get_func();
	
	if ( NULL != p_init_cmd && cmd_num > 0 )
	{
		if ( NULL != p_app_func )
		{
			// get command push function
			p_cmd_push = p_app_func->p_app_push_cmd;
			if ( NULL != p_cmd_push )
			{
				// clear the command structure
				memset( &cmd, 0, sizeof(MAIN_CMD_STRUCT) );

				// add all commands in command list to command list
				for ( i = 0; i < cmd_num; i++ )
				{
					cmd.cmd_type = p_init_cmd[i];
					p_cmd_push( &cmd );
				}
			}
		}
	}
}

static void application_led_cmd_init(void)
{
	BYTE led_chips = led_get_chips_number();
	// we can add or remove command id to array for initialization
	MAIN_CMD_ENUM init_led_cmd[] = {
		LED_CMD_POWER_ON,
		LED_CMD_CHIPID_CHECK,
		LED_CMD_REG_INIT,
	//	LED_CMD_REG_READ,

		LED_CMD_EFFECT_INIT,
	};

	if ( led_chips > 0 )
	{
		application_cmd_init( init_led_cmd, sizeof(init_led_cmd)/sizeof(MAIN_CMD_ENUM) );
	}
}

static void application_key_buffer_init(void)
{
	// Initialize buffer for keys event
	key_init_events_buffer();
}

static void application_key_buffer_uninit(void)
{
	// free buffers
	key_uninit_events_buffer();
}

static void application_key_cmd_init(void)
{
	BYTE key_chips = key_get_chips_number();
	// we can add or remove command id to array for initialization
	MAIN_CMD_ENUM init_key_cmd[] = {
		KEY_CMD_POWER_ON,
		KEY_CMD_CHIPID_CHECK,
		KEY_CMD_REG_INIT,
	//	KEY_CMD_REG_READ,
	};
	
	if ( key_chips > 0 )
	{
		application_cmd_init( init_key_cmd, sizeof(init_key_cmd)/sizeof(MAIN_CMD_ENUM) );
	}
}

static void application_main_init(void)
{
	// Initialize device hardware configure
	application_device_init();

	// Initialize buffer for led registers refresh and effect
	application_led_buffer_init();
	// Add led initial command to list
	application_led_cmd_init();

	//Initialize buffer for keys' events
	application_key_buffer_init();
	// Add key initial command to list
	application_key_cmd_init();
}

static void application_main_uninit(void)
{
	application_led_buffer_uninit();
	application_key_buffer_uninit();
}

static void application_main_loop(void)
{
	MAIN_CMD_STRUCT *p_cmd = NULL;
	APP_FUNC_STRUCT *p_app_func = application_main_get_func();
	
	while ( 1 )
	{
		if ( NULL != p_app_func )
		{
			// handle interrupt firstly
			if ( NULL != p_app_func->p_app_int_cmd )
			{
				p_app_func->p_app_int_cmd();
			}
			
			// get command
			if ( NULL != p_app_func->p_app_get_cmd )
			{
				p_cmd = p_app_func->p_app_get_cmd();
			}

			// if there is no any command, 
			// just loop and wait for new command
			if ( NULL != p_cmd )
			{
				if ( NULL != p_app_func->p_app_handle_cmd )
				{
					p_app_func->p_app_handle_cmd( p_cmd );
				}

				// remove the command handled
				if ( NULL != p_app_func->p_app_pop_cmd )
				{
					p_app_func->p_app_pop_cmd();
				}
			}
		}
	}
}

const APP_MAIN_STRUCT app_main = {
	application_main_get_func,
	application_main_register,
	application_main_init,
	application_main_loop,
	application_main_uninit,
};

#endif /* APPLICATION_WISTRON_ENABLE */

#ifdef __cplusplus
}
#endif /* __cplusplus */
