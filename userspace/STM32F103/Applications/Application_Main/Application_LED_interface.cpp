#include "Application_main.h"
#include "Application_LED_customize.h"
#include "Application_LED_interface.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef APPLICATION_WISTRON_ENABLE

static APP_LEDS_INFO * app_leds_info_buf = NULL;

static APP_LEDS_INFO * application_get_leds_info(void)
{
	return app_leds_info_buf;
}

static APP_LEDS_INFO * application_get_leds_info_by_index(BYTE chip_index)
{
	const AWCHIP_LEDS_HW_STRUCT *p_hw_info = NULL;
	APP_LEDS_INFO * p_app_leds_info = NULL;
	APP_LEDS_INFO * p_tmp = NULL;
	
	p_hw_info = led_get_hw_info_by_chip_index( chip_index );
	p_tmp = application_get_leds_info();
	if ( NULL != p_hw_info 
		&& APPLI_CHIPS_NONE != p_hw_info->awchip_id
		&& NULL != p_tmp )
	{
		p_app_leds_info = &(p_tmp[chip_index]);
	}

	return p_app_leds_info;
}

static void application_set_leds_info(APP_LEDS_INFO *p_leds_info_buf)
{
	app_leds_info_buf = p_leds_info_buf;
}

BYTE application_get_max_leds(BYTE chip_index)
{
	BYTE led_nums = 0;
	const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info = NULL;

	p_led_hw_info = led_get_hw_info_by_chip_index( chip_index );
	if ( NULL != p_led_hw_info )
	{
		led_nums = p_led_hw_info->led_nums;
	}

	return led_nums;
}

void application_init_leds_info(void)
{
	BYTE i = 0;
	BYTE len = 0;
	BYTE led_chips = led_get_chips_number();
	APP_LEDS_INFO *p_app_leds_info = NULL;

	if ( led_chips > 0 )
	{
		p_app_leds_info = (APP_LEDS_INFO *)Application_malloc(sizeof(APP_LEDS_INFO)*led_chips);
		if ( NULL != p_app_leds_info )
		{
			memset( (void *)p_app_leds_info, 0, sizeof(APP_LEDS_INFO)*led_chips );

			for ( i = 0; i < led_chips; i++ )
			{
				// Get the led numbers of chip by index
				p_app_leds_info[i].led_nums = application_get_max_leds( i );
				if ( p_app_leds_info[i].led_nums > 0 )
				{
					p_app_leds_info[i].led_range.led_effect_start = 0;
					p_app_leds_info[i].led_range.led_effect_end = 0;

					// malloc the data buffer for each led
					len = p_app_leds_info[i].led_nums*sizeof(AW_LED_STRUCT);
					p_app_leds_info[i].p_led_data = (AW_LED_STRUCT *)Application_malloc( len );
					if ( NULL != p_app_leds_info[i].p_led_data )
					{
						memset( (void *)p_app_leds_info[i].p_led_data, 0, len );
					}

					// malloc the color buffer for each led
					len = p_app_leds_info[i].led_nums*sizeof(APP_COLOR_STRUCT);
					p_app_leds_info[i].p_led_color = (APP_COLOR_STRUCT *)Application_malloc( len );
					if ( NULL != p_app_leds_info[i].p_led_color )
					{
						memset( (void *)p_app_leds_info[i].p_led_color, 0, len );
					}
					
					// malloc the bright level buffer for each led
					len = p_app_leds_info[i].led_nums*sizeof(BYTE);
					p_app_leds_info[i].p_led_bright_level = (BYTE *)Application_malloc( len );
					if ( NULL != p_app_leds_info[i].p_led_bright_level )
					{
						memset( (void *)p_app_leds_info[i].p_led_bright_level, 0, len );
					}

					// malloc the brightness buffer for each led
					len = p_app_leds_info[i].led_nums*sizeof(BYTE);
					p_app_leds_info[i].p_led_brightness = (BYTE *)Application_malloc( len );
					if ( NULL != p_app_leds_info[i].p_led_brightness )
					{
						memset( (void *)p_app_leds_info[i].p_led_brightness, 0, len );
					}	
				}
			}

			// Backup the buffer array pointer of leds' infomation
			application_set_leds_info( p_app_leds_info );
		}
	}
}

void application_uninit_leds_info(void)
{
	APP_LEDS_INFO * p_app_leds_info = application_get_leds_info();
	BYTE led_chips = led_get_chips_number();
	BYTE i = 0;

	if ( led_chips > 0 && NULL != p_app_leds_info )
	{
		for ( i = 0; i < led_chips; i++ )
		{
			// free the buffer of leds' data
			if ( NULL != p_app_leds_info[i].p_led_data )
			{
				Application_free( p_app_leds_info[i].p_led_data );
				p_app_leds_info[i].p_led_data = NULL;
			}

			// free the buffer of leds' color
			if ( NULL != p_app_leds_info[i].p_led_color )
			{
				Application_free( p_app_leds_info[i].p_led_color );
				p_app_leds_info[i].p_led_color = NULL;
			}
			
			// free the buffer of leds' bright level
			if ( NULL != p_app_leds_info[i].p_led_bright_level )
			{
				Application_free( p_app_leds_info[i].p_led_bright_level );
				p_app_leds_info[i].p_led_bright_level = NULL;
			}
		
			// free the buffer of leds' brightness
			if ( NULL != p_app_leds_info[i].p_led_brightness )
			{
				Application_free( p_app_leds_info[i].p_led_brightness );
				p_app_leds_info[i].p_led_brightness = NULL;
			}
		}

		Application_free( p_app_leds_info );
		p_app_leds_info = NULL;

		// Clear the buffer array pointer of leds' information
		application_set_leds_info( NULL );
	}
}

void application_set_leds_range(BYTE chip_index, BYTE start, BYTE end)
{
	APP_LEDS_INFO * p_app_leds_info = NULL;
	BYTE led_nums = 0;

	p_app_leds_info = application_get_leds_info_by_index( chip_index );
	if ( NULL != p_app_leds_info )
	{
		led_nums = p_app_leds_info->led_nums;
		if ( start < led_nums && end < led_nums )
		{
			p_app_leds_info->led_range.led_effect_start = start;
			p_app_leds_info->led_range.led_effect_end = end;
		}
	}
}

//APP_LEDS_RAGNE * application_get_leds_range(BYTE chip_index)
//{
//	APP_LEDS_RAGNE * p_app_led_range = NULL;
//	APP_LEDS_INFO * p_app_leds_info = NULL;
	
//	p_app_leds_info = application_get_leds_info_by_index( chip_index );
//	if ( NULL != p_app_leds_info )
//	{
//		p_app_led_range = &(p_app_leds_info->led_range);
//	}
	
//	return p_app_led_range;
//}

// Real brightness of led is got from color*brightness
static BYTE application_convert_brightness_to_data( BYTE brightness, APP_COLOR_LEVEL color)
{
	BYTE led_data = 0;

	switch ( color )
	{
		case APP_COLOR_FULL:
			led_data = brightness;
			break;

		case APP_COLOR_HALF:
			led_data = brightness >> 1;
			break;

		case APP_COLOR_NONE:
			led_data = 0;
			break;

		default:
			break;
	}

	return led_data;
}

BYTE application_get_max_bright_level(void)
{
	return APPLI_LED_MAX_BRIGHT_LEVEL;
}

BYTE application_get_brightness_by_level(BYTE level)
{
	BYTE brightness = 0;
	
	if ( level < application_get_max_bright_level() )
	{
		brightness = appli_led_bright_levels[level];
	}

	return brightness;
}

static void application_set_led_data(AW_LED_STRUCT *p_led_data,
						APP_COLOR_STRUCT *p_led_color, BYTE brightness)
{
	BYTE max_level = application_get_max_bright_level() - 1;
	BYTE max_brightness = application_get_brightness_by_level(max_level);
	
	if ( NULL != p_led_data && NULL != p_led_color )
	{
		if ( brightness > max_brightness )
		{
			brightness = max_brightness;
		}

		p_led_data->r = application_convert_brightness_to_data( brightness, p_led_color->r );
		p_led_data->g = application_convert_brightness_to_data( brightness, p_led_color->g );
		p_led_data->b = application_convert_brightness_to_data( brightness, p_led_color->b );
	}
}

void application_set_led_color(BYTE chip_index, BYTE led_idx, APP_COLOR_STRUCT *p_led_color, BOOL update_led)
{
	APP_LEDS_INFO * p_app_leds_info = NULL;

	p_app_leds_info = application_get_leds_info_by_index( chip_index );
	if ( NULL != p_app_leds_info 
		&& NULL != p_led_color
		&& led_idx < p_app_leds_info->led_nums 
		&& NULL != p_app_leds_info->p_led_data 
		&& NULL != p_app_leds_info->p_led_brightness )
	{
		// set color data
		memcpy( (void *)(p_app_leds_info->p_led_color+led_idx), 
				(const void *)p_led_color,
				sizeof(APP_COLOR_STRUCT) );

		if ( update_led )
		{
			// combine brightness and color to real brightness
			application_set_led_data( p_app_leds_info->p_led_data+led_idx, 
							p_led_color, *(p_app_leds_info->p_led_brightness+led_idx) );
		}
	}
}

// set the color between led start and led end
void application_set_led_same_color(BYTE chip_index, const APP_COLOR_STRUCT *p_led_color, BOOL update_led)
{
	APP_LEDS_INFO * p_app_leds_info = NULL;
	BYTE start = 0;
	BYTE end = 0;
	BYTE led_nums = 0;
	BYTE i = 0;

	p_app_leds_info = application_get_leds_info_by_index( chip_index );
	if ( NULL != p_app_leds_info 
		&& NULL != p_app_leds_info->p_led_color
		&& NULL != p_app_leds_info->p_led_data 
		&& NULL != p_app_leds_info->p_led_brightness
		&& NULL != p_led_color )
	{
		led_nums = p_app_leds_info->led_nums;
		start = p_app_leds_info->led_range.led_effect_start;
		end = p_app_leds_info->led_range.led_effect_end;

		if ( start < led_nums && end < led_nums )
		{
			if ( start <= end )
			{
				for ( i = start; i <= end; i++ )
				{
					// set color data
					memcpy( (void *)(p_app_leds_info->p_led_color+i), 
							(const void *)p_led_color,
							sizeof(APP_COLOR_STRUCT) );

					if ( update_led )
					{
						// combine brightness and color to real brightness
						application_set_led_data( p_app_leds_info->p_led_data+i, 
								(APP_COLOR_STRUCT *)p_led_color, *(p_app_leds_info->p_led_brightness+i) );
					}
				}
			}
			else
			{
				for ( i = start; i < led_nums; i++ )
				{
					// set color data
					memcpy( (void *)(p_app_leds_info->p_led_color+i), 
							(const void *)p_led_color,
							sizeof(APP_COLOR_STRUCT) );

					if ( update_led )
					{
						// combine brightness and color to real brightness
						application_set_led_data( p_app_leds_info->p_led_data+i, 
								(APP_COLOR_STRUCT *)p_led_color, *(p_app_leds_info->p_led_brightness+i) );
					}
				}

				for ( i = 0; i <= end; i++ )
				{
					// set color data
					memcpy( (void *)(p_app_leds_info->p_led_color+i), 
							(const void *)p_led_color,
							sizeof(APP_COLOR_STRUCT) );

					if ( update_led )
					{
						// combine brightness and color to real brightness
						application_set_led_data( p_app_leds_info->p_led_data+i, 
								(APP_COLOR_STRUCT *)p_led_color, *(p_app_leds_info->p_led_brightness+i) );
					}
				}
			}
		}
	}
}

// set the color to all leds
void application_set_all_same_color(BYTE chip_index, APP_COLOR_STRUCT *p_led_color, BOOL update_led)
{
	APP_LEDS_INFO * p_app_leds_info = NULL;
	BYTE led_nums = 0;
	BYTE i = 0;

	p_app_leds_info = application_get_leds_info_by_index( chip_index );
	if ( NULL != p_app_leds_info 
		&& NULL != p_app_leds_info->p_led_data
		&& NULL != p_app_leds_info->p_led_data 
		&& NULL != p_app_leds_info->p_led_brightness
		&& NULL != p_led_color )
	{
		led_nums = p_app_leds_info->led_nums;

		// set all leds as the same color
		for ( i = 0; i <= led_nums-1; i++ )
		{
			// set color data
			memcpy( (void *)(p_app_leds_info->p_led_color+i), 
					(const void *)p_led_color,
					sizeof(APP_COLOR_STRUCT) );

			if ( update_led )
			{
				// combine brightness and color to real brightness
				application_set_led_data( p_app_leds_info->p_led_data+i, 
							p_led_color, *(p_app_leds_info->p_led_brightness+i) );
			}
		}
	}
}

BYTE application_get_max_color_number(void)
{
	return APPLI_LED_MAX_COLOR;
}

const APP_COLOR_STRUCT * application_get_led_color_by_index(BYTE idx)
{
	const APP_COLOR_STRUCT *p_color = NULL;

	if ( idx < application_get_max_color_number() )
	{
		p_color = &appli_led_colors[idx];
	}

	return p_color;
}

//APP_COLOR_STRUCT * application_get_led_color(BYTE chip_index, BYTE led_idx)
//{
//	APP_LEDS_INFO * p_app_leds_info = NULL;
//	APP_COLOR_STRUCT * p_led_color = NULL;
	
//	p_app_leds_info = application_get_leds_info_by_index( chip_index );
//	if ( NULL != p_app_leds_info 
//		&& led_idx < p_app_leds_info->led_nums )
//	{
//		p_led_color = p_app_leds_info->p_led_color+led_idx;
//	}

//	return p_led_color;
//}

void application_set_led_bright_level(BYTE chip_index, BYTE led_idx, BYTE bright_level, BOOL update_led)
{
	APP_LEDS_INFO * p_app_leds_info = NULL;
	
	p_app_leds_info = application_get_leds_info_by_index( chip_index );
	if ( NULL != p_app_leds_info 
		&& NULL != p_app_leds_info->p_led_bright_level 
		&& NULL != p_app_leds_info->p_led_brightness
		&& NULL != p_app_leds_info->p_led_color 
		&& NULL != p_app_leds_info->p_led_data
		&& led_idx < p_app_leds_info->led_nums
		&& bright_level < application_get_max_bright_level() )
	{
		// set bright level data
		p_app_leds_info->p_led_bright_level[led_idx] = bright_level;
		// set brightness data by level
		p_app_leds_info->p_led_brightness[led_idx] = application_get_brightness_by_level(bright_level);

		if ( update_led )
		{
			// combine brightness and color to real brightness
			application_set_led_data( p_app_leds_info->p_led_data+led_idx, 
						p_app_leds_info->p_led_color+led_idx, *(p_app_leds_info->p_led_brightness+led_idx) );
		}
	}
}

// set the bright level between led start and led end
void application_set_led_same_bright_level(BYTE chip_index, BYTE bright_level, BOOL update_led)
{
	APP_LEDS_INFO * p_app_leds_info = NULL;
	BYTE start = 0;
	BYTE end = 0;
	BYTE led_nums = 0;
	BYTE i = 0;
	BYTE brightness = 0;

	p_app_leds_info = application_get_leds_info_by_index( chip_index );
	if ( NULL != p_app_leds_info 
		&& NULL != p_app_leds_info->p_led_bright_level 
		&& NULL != p_app_leds_info->p_led_brightness
		&& NULL != p_app_leds_info->p_led_color 
		&& NULL != p_app_leds_info->p_led_data
		&& bright_level < application_get_max_bright_level() )
	{
		led_nums = p_app_leds_info->led_nums;
		start = p_app_leds_info->led_range.led_effect_start;
		end = p_app_leds_info->led_range.led_effect_end;
		// get brightness data by level
		brightness = application_get_brightness_by_level(bright_level);

		if ( start < led_nums && end < led_nums )
		{
			if ( start <= end )
			{
				for ( i = start; i <= end; i++ )
				{
					p_app_leds_info->p_led_bright_level[i] = bright_level;
					p_app_leds_info->p_led_brightness[i] = brightness;

					if ( update_led )
					{
						// combine brightness and color to real brightness
						application_set_led_data( p_app_leds_info->p_led_data+i, 
										p_app_leds_info->p_led_color+i, brightness );
					}
				}
			}
			else
			{
				for ( i = start; i < led_nums; i++ )
				{
					p_app_leds_info->p_led_bright_level[i] = bright_level;
					p_app_leds_info->p_led_brightness[i] = brightness;

					if ( update_led )
					{
						// combine brightness and color to real brightness
						application_set_led_data( p_app_leds_info->p_led_data+i, 
										p_app_leds_info->p_led_color+i, brightness );
					}
				}

				for ( i = 0; i <= end; i++ )
				{
					p_app_leds_info->p_led_bright_level[i] = bright_level;
					p_app_leds_info->p_led_brightness[i] = brightness;

					if ( update_led )
					{
						// combine brightness and color to real brightness
						application_set_led_data( p_app_leds_info->p_led_data+i, 
										p_app_leds_info->p_led_color+i, brightness );
					}
				}
			}
		}
	}
}

// set the bright level to all leds
void application_set_all_same_bright_level(BYTE chip_index, BYTE bright_level, BOOL update_led)
{
	APP_LEDS_INFO * p_app_leds_info = NULL;
	BYTE led_nums = 0;
	BYTE i = 0;
	BYTE brightness = 0;
	
	p_app_leds_info = application_get_leds_info_by_index( chip_index );
	if ( NULL != p_app_leds_info 
		&& NULL != p_app_leds_info->p_led_bright_level 
		&& NULL != p_app_leds_info->p_led_brightness
		&& NULL != p_app_leds_info->p_led_color 
		&& NULL != p_app_leds_info->p_led_data
		&& bright_level < application_get_max_bright_level() )
	{
		led_nums = p_app_leds_info->led_nums;

		// get brightness data by level
		brightness = application_get_brightness_by_level(bright_level);

		// set all leds as the same brightness
		for ( i = 0; i <= led_nums-1; i++ )
		{
			p_app_leds_info->p_led_bright_level[i] = bright_level;
			p_app_leds_info->p_led_brightness[i] = brightness;

			if ( update_led )
			{
				// combine brightness and color to real brightness
				application_set_led_data( p_app_leds_info->p_led_data+i, 
								p_app_leds_info->p_led_color+i, brightness );
			}
		}
	}
}

//BYTE application_get_led_bright_level(BYTE chip_index, BYTE led_idx)
//{
//	APP_LEDS_INFO * p_app_leds_info = NULL;
//	BYTE bright_level = 0;

//	p_app_leds_info = application_get_leds_info_by_index( chip_index );
//	if ( NULL != p_app_leds_info 
//		&& led_idx < p_app_leds_info->led_nums
//		&& NULL != p_app_leds_info->p_led_bright_level )
//	{
//		bright_level = p_app_leds_info->p_led_bright_level[led_idx];
//	}

//	return bright_level;
//}

void application_set_led_brightness(BYTE chip_index, BYTE led_idx, BYTE brightness, BOOL update_led)
{
	APP_LEDS_INFO * p_app_leds_info = NULL;
	
	p_app_leds_info = application_get_leds_info_by_index( chip_index );
	if ( NULL != p_app_leds_info 
		&& NULL != p_app_leds_info->p_led_brightness
		&& NULL != p_app_leds_info->p_led_color 
 		&& NULL != p_app_leds_info->p_led_data )
	{
		p_app_leds_info->p_led_brightness[led_idx] = brightness;

		if ( update_led )
		{
			// combine brightness and color to real brightness
			application_set_led_data( p_app_leds_info->p_led_data+led_idx, 
							p_app_leds_info->p_led_color+led_idx, brightness );
		}
	}
}

// set the brightness between led start and led end
void application_set_led_same_brightness(BYTE chip_index, BYTE brightness, BOOL update_led)
{
	APP_LEDS_INFO * p_app_leds_info = NULL;
	BYTE start = 0;
	BYTE end = 0;
	BYTE led_nums = 0;
	BYTE i = 0;
	
	p_app_leds_info = application_get_leds_info_by_index( chip_index );
	if ( NULL != p_app_leds_info 
		&& NULL != p_app_leds_info->p_led_brightness
		&& NULL != p_app_leds_info->p_led_color 
 		&& NULL != p_app_leds_info->p_led_data )
	{
		led_nums = p_app_leds_info->led_nums;
		start = p_app_leds_info->led_range.led_effect_start;
		end = p_app_leds_info->led_range.led_effect_end;

		// just set the leds from start to end
		if ( start < led_nums && end < led_nums )
		{
			if ( start <= end )
			{
				for ( i = start; i <= end; i++ )
				{
					p_app_leds_info->p_led_brightness[i] = brightness;

					if ( update_led )
					{
						// combine brightness and color to real brightness
						application_set_led_data( p_app_leds_info->p_led_data+i, 
										p_app_leds_info->p_led_color+i, brightness );
					}
				}
			}
			else
			{
				for ( i = start; i < led_nums; i++ )
				{
					p_app_leds_info->p_led_brightness[i] = brightness;

					if ( update_led )
					{
						// combine brightness and color to real brightness
						application_set_led_data( p_app_leds_info->p_led_data+i, 
										p_app_leds_info->p_led_color+i, brightness );
					}
				}

				for ( i = 0; i <= end; i++ )
				{
					p_app_leds_info->p_led_brightness[i] = brightness;

					if ( update_led )
					{
						// combine brightness and color to real brightness
						application_set_led_data( p_app_leds_info->p_led_data+i, 
										p_app_leds_info->p_led_color+i, brightness );
					}
				}
			}
		}
	}
}

// set the brightness to all leds
void application_set_all_same_brightness(BYTE chip_index, BYTE brightness, BOOL update_led)
{
	APP_LEDS_INFO * p_app_leds_info = NULL;
	BYTE led_nums = 0;
	BYTE i = 0;

	p_app_leds_info = application_get_leds_info_by_index( chip_index );
	if ( NULL != p_app_leds_info 
		&& NULL != p_app_leds_info->p_led_brightness
		&& NULL != p_app_leds_info->p_led_color 
 		&& NULL != p_app_leds_info->p_led_data )
	{
		led_nums = p_app_leds_info->led_nums;

		// set all leds as the same brightness
		for ( i = 0; i <= led_nums-1; i++ )
		{
			p_app_leds_info->p_led_brightness[i] = brightness;

			if ( update_led )
			{
				// combine brightness and color to real brightness
				application_set_led_data( p_app_leds_info->p_led_data+i, 
								p_app_leds_info->p_led_color+i, brightness );
			}
		}
	}
}

//BYTE application_get_led_brightness(BYTE chip_index, BYTE led_idx)
//{
//	APP_LEDS_INFO * p_app_leds_info = NULL;
//	BYTE brightness = 0;

//	p_app_leds_info = application_get_leds_info_by_index( chip_index );
//	if ( NULL != p_app_leds_info 
//		&& led_idx < p_app_leds_info->led_nums
//		&& NULL != p_app_leds_info->p_led_brightness )
//	{
//		brightness = p_app_leds_info->p_led_brightness[led_idx];
//	}

//	return brightness;
//}

void application_led_effect_update(BYTE chip_index)
{
	APP_LEDS_INFO * p_led_info = NULL;
	const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info = NULL;

	// get hardware information
	p_led_hw_info = led_get_hw_info_by_chip_index( chip_index );
	// get leds data buffer
	p_led_info = application_get_leds_info_by_index( chip_index );
	if ( NULL != p_led_hw_info && NULL != p_led_info )
	{
		// update leds data
		led_effect_update_data( chip_index, p_led_info->p_led_data, p_led_hw_info );
	}
}

#endif /* APPLICATION_WISTRON_ENABLE */

#ifdef __cplusplus
}
#endif /* __cplusplus */
