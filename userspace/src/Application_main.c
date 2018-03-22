/*************************************************************************
 > Filename   : main.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-22 18:23:17
 ************************************************************************/

#include <stdio.h>
#include "Application_LED_effect.h"

static void application_main_init(void)
{
	int i;
	// Initialize buffer for writing chip registers
	led_init_regs_buffer();

	// Initialize buffer for leds effect
	application_init_leds_info();

	// Initialize all chips config(led mod) and first effect
	for (i = 0; i < led_get_chips_number(); i++) {
		led_regs_init(i);
		application_led_effect_init(i);
	}
}

static void application_main_test()
{
	//check chip id
	application_set_led_cur_state();
	application_led_effect_interrupt_handle();
	//change imax
	application_set_led_cur_state(LEDS_EFFECT_IMAX_CHANGE);
	application_led_effect_interrupt_handle();
	//
}

static void application_led_buffer_uninit(void)
{
	//TODO  free buffers
}

static void application_main_uninit(void)
{
	application_led_buffer_uninit();
}

int main()
{
	application_main_init();
	application_effect_test();

	application_main_uninit();

	return 0;
}
