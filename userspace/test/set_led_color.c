/*************************************************************************
 > Filename   : set_led_color.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-07 14:46:35
 ************************************************************************/

#include <stdio.h>

APP_LEDS_INFO *application_get_leds_info_by_index(BYTE chip_index)
{
	return &app_leds_info_buf[chip_index];

}

void application_set_led_color(BYTE chip_index, BYTE led_idx, APP_COLOR_STRUCT *p_led_color, BOOL update_led)
{
	APP_LEDS_INFO *p_app_led_info = NULL;

	p_app_led_info = application_get_leds_info_by_index(chip_index);

}
