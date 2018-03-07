#include "Application_main.h"
#include "Application_KEY_customize.h"
#include "Application_KEY.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef APPLICATION_WISTRON_ENABLE

static AWCHIP_KEY_EVENT_STRUCT ** keys_events_buf = NULL;

BYTE key_get_chips_number(void)
{
	return APPLI_KEY_CHIPS_NUM;
}

static const AWCHIP_KEYS_HW_STRUCT *key_get_hw_info_by_chip_id(BYTE key_chip_id)
{
	BYTE i = 0;
	const AWCHIP_KEYS_HW_STRUCT *p_key_hw_info = NULL;

	if ( APPLI_CHIPS_NONE != key_chip_id )
	{
		for ( i = 0; i < key_get_chips_number(); i++ )
		{
			// check the chip id
			if ( key_hw_info[i].awchip_id == key_chip_id )
			{
				p_key_hw_info = &key_hw_info[i];
				break;
			}
		}
	}

	return p_key_hw_info;
}

const AWCHIP_KEYS_HW_STRUCT *key_get_hw_info_by_chip_index(BYTE key_chip_index)
{
	const AWCHIP_KEYS_HW_STRUCT *p_key_hw_info = NULL;

	if ( key_chip_index < key_get_chips_number() )
	{
		p_key_hw_info = &key_hw_info[key_chip_index];
	}

	return p_key_hw_info;
}

AW_MCU_COMMU_ENUM key_get_commu_group(BYTE key_chip_id)
{
	AW_MCU_COMMU_ENUM commu_group = AW_MCU_COMMU_NONE;
	const AWCHIP_KEYS_HW_STRUCT *p_key_info = key_get_hw_info_by_chip_id( key_chip_id );

	if ( NULL != p_key_info )
	{
		commu_group = (AW_MCU_COMMU_ENUM)(p_key_info->commu_struct.device_commu);
	}
	
	return commu_group;
}

BYTE key_get_i2c_address(BYTE key_chip_id)
{
	BYTE i2c_address = 0x00;
	const AWCHIP_KEYS_HW_STRUCT *p_key_info = key_get_hw_info_by_chip_id( key_chip_id );

	if ( NULL != p_key_info )
	{
		i2c_address = p_key_info->commu_struct.device_addr;
	}
	
	return i2c_address;
}

static AWCHIP_KEY_EVENT_STRUCT ** keys_get_events_buf(void)
{
	return keys_events_buf;
}

static void keys_set_events_buf(AWCHIP_KEY_EVENT_STRUCT **p_keys_events_buf)
{
	keys_events_buf = p_keys_events_buf;
}

static AWCHIP_KEY_EVENT_STRUCT * keys_events_buf_by_index(BYTE chip_index)
{
	const AWCHIP_KEYS_HW_STRUCT *p_key_hw_info = NULL;
	AWCHIP_KEY_EVENT_STRUCT * p_events_buf = NULL;
	AWCHIP_KEY_EVENT_STRUCT ** p_tmp = NULL;

	p_key_hw_info = key_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_key_hw_info 
		&& APPLI_CHIPS_NONE != p_key_hw_info->awchip_id )
	{
		p_tmp = keys_get_events_buf();
		if ( NULL != p_tmp )
		{
			p_events_buf = p_tmp[chip_index];
		}
	}

	return p_events_buf;
}

void key_init_events_buffer(void)
{
	BYTE i = 0;
	BYTE len = 0;
	BYTE key_nums = 0;
	BYTE key_chips = key_get_chips_number();
	AWCHIP_KEY_EVENT_STRUCT **pp_events_buf = NULL;
	const AWCHIP_KEYS_HW_STRUCT *p_keys_hw = NULL;
	
	if ( key_chips > 0 )
	{
		len = key_chips*sizeof(AWCHIP_KEY_EVENT_STRUCT *);
		pp_events_buf = (AWCHIP_KEY_EVENT_STRUCT **)Application_malloc( len );
		if ( NULL != pp_events_buf )
		{
			memset( (void *)pp_events_buf, 0, len);
			
			for ( i = 0; i < key_chips; i++ )
			{
				len = sizeof(AWCHIP_KEY_EVENT_STRUCT);
				pp_events_buf[i] = (AWCHIP_KEY_EVENT_STRUCT *)Application_malloc( len );
				if ( NULL != pp_events_buf[i] )
				{
					memset( (void *)pp_events_buf[i], 0, len );
				}

				p_keys_hw = key_get_hw_info_by_chip_index( i );
				if ( NULL != p_keys_hw 
					&& APPLI_CHIPS_NONE != p_keys_hw->awchip_id )
				{
					// Get the key numbers by chip index
					key_nums = p_keys_hw->key_nums;

					// malloc the buffers according to the key numbers
					len = key_nums * sizeof(BYTE);
					pp_events_buf[i]->p_kisr = (BYTE *)Application_malloc( len );
					if ( NULL != pp_events_buf[i]->p_kisr )
					{
						memset( (void *)(pp_events_buf[i]->p_kisr), 0, len );
					}

					len = key_nums * sizeof(BYTE);
					pp_events_buf[i]->p_rawst = (BYTE *)Application_malloc( len );
					if ( NULL != pp_events_buf[i]->p_rawst )
					{
						memset( (void *)(pp_events_buf[i]->p_rawst), 0, len );
					}

					len = key_nums * sizeof(AWCHIP_KEY_EVENT_ENUM);
					pp_events_buf[i]->p_event = (AWCHIP_KEY_EVENT_ENUM *)Application_malloc( len );
					if ( NULL != pp_events_buf[i]->p_event )
					{
						memset( (void *)(pp_events_buf[i]->p_event), 0, len );
					}
				}
			}

			// Backup the buffer array pointer
			keys_set_events_buf( pp_events_buf );
		}
	}
}

void key_uninit_events_buffer(void)
{
	BYTE i = 0;
	BYTE key_chips = key_get_chips_number();
	AWCHIP_KEY_EVENT_STRUCT **pp_events_buf = keys_get_events_buf();

	if ( key_chips > 0 && NULL != pp_events_buf )
	{
		for ( i = 0; i < key_chips; i++ )
		{
			if ( NULL != pp_events_buf[i] )
			{
				if ( NULL != pp_events_buf[i]->p_kisr )
				{
					Application_free( (void *)pp_events_buf[i]->p_kisr );
					pp_events_buf[i]->p_kisr = NULL;
				}

				if ( NULL != pp_events_buf[i]->p_rawst )
				{
					Application_free( (void *)pp_events_buf[i]->p_rawst );
					pp_events_buf[i]->p_rawst = NULL;
				}

				if ( NULL != pp_events_buf[i]->p_event )
				{
					Application_free( (void *)pp_events_buf[i]->p_event );
					pp_events_buf[i]->p_event = NULL;
				}
			
				Application_free( (void *)pp_events_buf[i] );
				pp_events_buf[i] = NULL;
			}
		}
	
		Application_free( (void *)pp_events_buf );
		pp_events_buf = NULL;

		// clear the buffer array pointer
		keys_set_events_buf( NULL );
	}
}

BOOL key_check_chip_id(BYTE chip_index)
{
	BOOL re = FALSE;
	const AWCHIP_KEYS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = key_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_key_funcs 
		&& NULL != p_hw_info->p_key_funcs->p_check_chip_id
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		re = p_hw_info->p_key_funcs->p_check_chip_id( p_hw_info->awchip_id );
	}
	
	return re;
}

void key_regs_init(BYTE chip_index)
{
	const AWCHIP_KEYS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = key_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_key_funcs 
		&& NULL != p_hw_info->p_key_funcs->p_regs_init
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		p_hw_info->p_key_funcs->p_regs_init( p_hw_info->awchip_id );
	}
}

void key_chip_sleep(BYTE chip_index)
{
	const AWCHIP_KEYS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = key_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_key_funcs 
		&& NULL != p_hw_info->p_key_funcs->p_chip_sleep
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		p_hw_info->p_key_funcs->p_chip_sleep( p_hw_info->awchip_id );
	}
}

void key_chip_softreset(BYTE chip_index)
{
	const AWCHIP_KEYS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = key_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_key_funcs 
		&& NULL != p_hw_info->p_key_funcs->p_chip_softreset
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		p_hw_info->p_key_funcs->p_chip_softreset( p_hw_info->awchip_id );
	}
}

void key_chip_read_reg_cfg(BYTE chip_index)
{
	const AWCHIP_KEYS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = key_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_key_funcs 
		&& NULL != p_hw_info->p_key_funcs->p_get_regs_cfg
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		p_hw_info->p_key_funcs->p_get_regs_cfg( p_hw_info->awchip_id );
	}
}

static void key_chip_get_key_interrupt(BYTE chip_index, BYTE key_nums, BYTE *p_kisr, BYTE *p_rawst)
{
	const AWCHIP_KEYS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = key_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_key_funcs 
		&& NULL != p_hw_info->p_key_funcs->p_chip_key_get_int
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		p_hw_info->p_key_funcs->p_chip_key_get_int( p_hw_info->awchip_id, key_nums, p_kisr, p_rawst );
	}
}

static APP_LED_EFFECT_ENUM key_chip_get_event_by_interrupt(BYTE key_nums, AWCHIP_KEY_EVENT_STRUCT *p_key_struct)
{
	APP_LED_EFFECT_ENUM new_led_state = LEDS_EFFECT_NONE;
	BYTE i = 0;
	BYTE int_key = key_nums;
	BYTE pressed = 0;
	BYTE released = 0;

	if ( key_nums > 0 && NULL != p_key_struct )
	{
		for ( i = 0; i < key_nums; i++ )
		{
			if ( 0 != p_key_struct->p_kisr[i] )
			{
				int_key = i;
				break;
			}
		}

		if ( int_key < key_nums )
		{
			if ( 0 == p_key_struct->p_rawst[int_key] )
			{
				p_key_struct->p_event[int_key] = KEY_EVENT_RELEASE;

				pressed = 0;
				released = 0;
				for ( i = 0; i < key_nums; i++ )
				{
					if ( (0 != p_key_struct->p_rawst[i])
						&& (KEY_EVENT_PRESS == p_key_struct->p_event[i]) )
					{
						pressed++;
					}
					else if ( (0 == p_key_struct->p_rawst[i])
						&& (KEY_EVENT_RELEASE == p_key_struct->p_event[i]) )
					{
						released++;
					}
				}

				if ( 0 == pressed && 1 == released )
				{
					switch ( int_key )
					{
						case 0:
							new_led_state = LEDS_EFFECT_COLOR_CHANGE;
							break;

						case 1:
							new_led_state = LEDS_EFFECT_IMAX_CHANGE;
							break;

						case 2:
							new_led_state = LEDS_EFFECT_BRIGHT_CHANGE;
							break;

						case 3:
							new_led_state = LEDS_EFFECT_INCREASE;
							break;

						default:
							break;
					}
				}
			}
			else
			{
				p_key_struct->p_event[int_key] = KEY_EVENT_PRESS;

				if ( (0 != p_key_struct->p_rawst[3])
					&& (KEY_EVENT_PRESS == p_key_struct->p_event[3]) )
				{
					pressed = 0;
					for ( i = 0; i < key_nums; i++ )
					{
						if ( (i != 3)
							&& (0 != p_key_struct->p_rawst[i])
							&& (KEY_EVENT_PRESS == p_key_struct->p_event[i]) )
						{
							pressed++;
						}
					}

					if ( 1 == pressed )
					{
						if ( KEY_EVENT_PRESS == p_key_struct->p_event[0] )
						{
							new_led_state = LEDS_EFFECT_CIRCLE;
						}
						else if ( KEY_EVENT_PRESS == p_key_struct->p_event[1] )
						{
							new_led_state = LEDS_EFFECT_BREATH;
						}
						else if ( KEY_EVENT_PRESS == p_key_struct->p_event[2] )
						{
							new_led_state = LEDS_EFFECT_COLOR_BREATH;
						}
					}
				}
			}

			for ( i = 0; i < key_nums; i++ )
			{
				if ( (int_key != i)
					&& (0 == p_key_struct->p_rawst[i])
					&& (KEY_EVENT_RELEASE == p_key_struct->p_event[i]) )
				{
					p_key_struct->p_event[i] = KEY_EVENT_NONE;
				}
			}
		}
	}

	return new_led_state;
}

APP_LED_EFFECT_ENUM key_chip_handle_interrupt(BYTE chip_index)
{
	APP_LED_EFFECT_ENUM new_led_state = LEDS_EFFECT_NONE;
	const AWCHIP_KEYS_HW_STRUCT *p_key_info = NULL;
	AWCHIP_KEY_EVENT_STRUCT *p_key_event = NULL;
	
	p_key_info = key_get_hw_info_by_chip_index( chip_index );
	p_key_event = keys_events_buf_by_index( chip_index );
	if ( NULL != p_key_info
		&& APPLI_CHIPS_NONE != p_key_info->awchip_id 
		&& p_key_info->key_nums > 0 
		&& NULL != p_key_event
		&& NULL != p_key_event->p_kisr 
		&& NULL != p_key_event->p_rawst )
	{
		memset( p_key_event->p_kisr, 0, p_key_info->key_nums );
		memset( p_key_event->p_rawst, 0, p_key_info->key_nums );

		key_chip_get_key_interrupt( chip_index, p_key_info->key_nums, 
						p_key_event->p_kisr, p_key_event->p_rawst );
		new_led_state = key_chip_get_event_by_interrupt( p_key_info->key_nums, p_key_event );
	}

	return new_led_state;
}

#endif /* APPLICATION_WISTRON_ENABLE */

#ifdef __cplusplus
}
#endif /* __cplusplus */
