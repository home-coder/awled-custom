#include "Application_main.h"
#include "Application_LED_customize.h"
#include "Application_LED.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef APPLICATION_WISTRON_ENABLE

static BYTE ** leds_regs_buf = NULL;

BYTE led_get_chips_number(void)
{
	return APPLI_LED_CHIPS_NUM;
}

static const AWCHIP_LEDS_HW_STRUCT *led_get_hw_info_by_chip_id(BYTE led_chip_id)
{
	BYTE i = 0;
	const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info = NULL;

	if ( APPLI_CHIPS_NONE != led_chip_id )
	{
		for ( i = 0; i < led_get_chips_number(); i++ )
		{
			// check the chip id
			if ( led_hw_info[i].awchip_id == led_chip_id )
			{
				p_led_hw_info = &led_hw_info[i];
				break;
			}
		}
	}

	return p_led_hw_info;
}

const AWCHIP_LEDS_HW_STRUCT *led_get_hw_info_by_chip_index(BYTE led_chip_index)
{
	const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info = NULL;

	if ( led_chip_index < led_get_chips_number() )
	{
		p_led_hw_info = &led_hw_info[led_chip_index];
	}

	return p_led_hw_info;
}

AW_MCU_COMMU_ENUM led_get_commu_group(BYTE led_chip_id)
{
	AW_MCU_COMMU_ENUM commu_group = AW_MCU_COMMU_NONE;
	const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info = led_get_hw_info_by_chip_id( led_chip_id );

	if ( NULL != p_led_hw_info )
	{
		commu_group = (AW_MCU_COMMU_ENUM)(p_led_hw_info->commu_struct.device_commu);
	}
	
	return commu_group;
}

BYTE led_get_i2c_address(BYTE led_chip_id)
{
	BYTE i2c_address = 0x00;
	const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info = led_get_hw_info_by_chip_id( led_chip_id );

	if ( NULL != p_led_hw_info )
	{
		i2c_address = (AW_MCU_COMMU_ENUM)(p_led_hw_info->commu_struct.device_addr);
	}
	
	return i2c_address;
}

BOOL led_check_chip_id(BYTE chip_index)
{
	BOOL re = FALSE;
	const AWCHIP_LEDS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = led_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_led_funcs 
		&& NULL != p_hw_info->p_led_funcs->p_check_chip_id
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		re = p_hw_info->p_led_funcs->p_check_chip_id( p_hw_info->awchip_id );
	}
	
	return re;
}

void led_chip_read_reg_cfg(BYTE chip_index)
{
	const AWCHIP_LEDS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = led_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_led_funcs 
		&& NULL != p_hw_info->p_led_funcs->p_get_regs_cfg
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		p_hw_info->p_led_funcs->p_get_regs_cfg( p_hw_info->awchip_id );
	}
}

void led_regs_init(BYTE chip_index)
{
	const AWCHIP_LEDS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = led_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_led_funcs 
		&& NULL != p_hw_info->p_led_funcs->p_regs_init
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		p_hw_info->p_led_funcs->p_regs_init( p_hw_info->awchip_id );
	}
}

void led_chip_sleep(BYTE chip_index)
{
	const AWCHIP_LEDS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = led_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_led_funcs 
		&& NULL != p_hw_info->p_led_funcs->p_chip_sleep
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		p_hw_info->p_led_funcs->p_chip_sleep( p_hw_info->awchip_id );
	}
}

void led_chip_softreset(BYTE chip_index)
{
	const AWCHIP_LEDS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = led_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_led_funcs 
		&& NULL != p_hw_info->p_led_funcs->p_chip_softreset
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		p_hw_info->p_led_funcs->p_chip_softreset( p_hw_info->awchip_id );
	}
}

void led_chip_change_imax(BYTE chip_index, BYTE iMax_level)
{
	const AWCHIP_LEDS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = led_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_led_funcs 
		&& NULL != p_hw_info->p_led_funcs->p_chip_change_imax
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id )
	{
		p_hw_info->p_led_funcs->p_chip_change_imax( p_hw_info->awchip_id, iMax_level );
	}
}

void led_chip_update_data(BYTE chip_index, BYTE reg_addr, BYTE reg_num, BYTE *p_value)
{
	const AWCHIP_LEDS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = led_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_hw_info 
		&& NULL != p_hw_info->p_led_funcs 
		&& NULL != p_hw_info->p_led_funcs->p_chip_update_data
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id 
		&& NULL != p_value )
	{
		p_hw_info->p_led_funcs->p_chip_update_data( p_hw_info->awchip_id, reg_addr, reg_num, p_value );
	}
}

static BYTE ** leds_get_regs_buf(void)
{
	return leds_regs_buf;
}

static BYTE * leds_regs_buf_by_index(BYTE chip_index)
{
	const AWCHIP_LEDS_HW_STRUCT *p_leds_hw = NULL;
	BYTE * p_regs_buf = NULL;
	BYTE ** p_tmp = NULL;

	p_leds_hw = led_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_leds_hw 
		&& APPLI_CHIPS_NONE != p_leds_hw->awchip_id )
	{
		p_tmp = leds_get_regs_buf();
		if ( NULL != p_tmp )
		{
			p_regs_buf = p_tmp[chip_index];
		}
	}

	return p_regs_buf;
}

static void leds_set_regs_buf(BYTE **p_leds_regs_buf)
{
	leds_regs_buf = p_leds_regs_buf;
}

void led_init_regs_buffer(void)
{
	BYTE i = 0;
	BYTE len = 0;
	BYTE led_chips = led_get_chips_number();
	BYTE **pp_regs_buf = NULL;
	const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info = NULL;

	if ( led_chips > 0 )
	{
		len = led_chips*sizeof(BYTE *);
		pp_regs_buf = (BYTE **)Application_malloc( len );

		if ( NULL != pp_regs_buf )
		{
			memset( (void *)pp_regs_buf, 0, len);
			
			for ( i = 0; i < led_chips; i++ )
			{
				// Get the number of chip registers by chip index
				p_led_hw_info = led_get_hw_info_by_chip_index( i );
				if ( NULL != p_led_hw_info
					&& APPLI_CHIPS_NONE != p_led_hw_info->awchip_id )
				{
					len = p_led_hw_info->led_regs_info.reg_data_nums;
					pp_regs_buf[i] = (BYTE *)Application_malloc( len );
					if ( NULL != pp_regs_buf[i] )
					{
						memset( (void *)pp_regs_buf[i], 0, len );
					}
				}
			}

			// Backup the buffer array pointer
			leds_set_regs_buf( pp_regs_buf );
		}
	}
}

void led_uninit_regs_buffer(void)
{
	BYTE i = 0;
	BYTE led_chips = led_get_chips_number();
	BYTE **pp_regs_buf = leds_get_regs_buf();

	if ( led_chips > 0 && NULL != pp_regs_buf )
	{
		for ( i = 0; i < led_chips; i++ )
		{
			if ( NULL != pp_regs_buf[i] )
			{
				Application_free( (void *)pp_regs_buf[i] );
				pp_regs_buf[i] = NULL;
			}
		}
	
		Application_free( (void *)pp_regs_buf );
		pp_regs_buf = NULL;

		// clear the buffer array pointer
		leds_set_regs_buf( NULL );
	}
}

static BYTE * led_data_convert_to_regs_data(BYTE chip_index, 
			const AW_LED_STRUCT *p_led_data, const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info )
{
	BYTE i = 0;
	BYTE base_reg = 0;
	const AW_LED_STRUCT *led_regs_map = NULL;
	BYTE *p_regs_buf = NULL;

	if ( chip_index < led_get_chips_number()
		&& NULL != p_led_data
		&& NULL != p_led_hw_info )
	{
		base_reg = p_led_hw_info->led_regs_info.reg_data_base;
		led_regs_map = p_led_hw_info->led_regs_info.p_regs_map;

		// If more than one chip used to display leds,
		// we should get the correct start data of the chip by index
		p_led_data += chip_index * p_led_hw_info->led_nums;

		p_regs_buf = leds_regs_buf_by_index( chip_index );
		if ( NULL != p_regs_buf )
		{
			memset( (void *)p_regs_buf, 0, p_led_hw_info->led_regs_info.reg_data_nums );
			
			for ( i = 0; i < p_led_hw_info->led_nums; i++ )
			{
				// convert led data to register data
				p_regs_buf[led_regs_map[i].r-base_reg] = (p_led_data+i)->r;
				p_regs_buf[led_regs_map[i].g-base_reg] = (p_led_data+i)->g;
				p_regs_buf[led_regs_map[i].b-base_reg] = (p_led_data+i)->b;
			}
		}
	}
	
	return p_regs_buf;
}

void led_effect_update_data(BYTE chip_index, AW_LED_STRUCT * p_led_data, 
								const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info)
{
	BYTE *p_regs_buf = NULL;

	if ( NULL != p_led_data && NULL != p_led_hw_info )
	{
		// set each led data to correct data register of led chip
		p_regs_buf = led_data_convert_to_regs_data( chip_index, p_led_data, p_led_hw_info );
		
		// because data registers are not consecutive for leds,
		// just write all registers number used in current project
		led_chip_update_data( chip_index, 
					p_led_hw_info->led_regs_info.reg_data_base, 
					p_led_hw_info->led_regs_info.reg_data_nums, 
					p_regs_buf );
	}
}

#endif /* APPLICATION_WISTRON_ENABLE */

#ifdef __cplusplus
}
#endif /* __cplusplus */
