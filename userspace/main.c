/*************************************************************************
 > Filename   : main.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-09 18:23:17
 ************************************************************************/

#include <stdio.h>

typedef enum {
	LED_CMD_POWER_ON,
	LED_CMD_CHIPID_CHECK,
	LED_CMD_REG_INIT,
	//  LED_CMD_REG_READ,

	LED_CMD_EFFECT_INIT,
	LED_CMD_TOTAL,
}MAIN_CMD_ENUM;

static void application_led_buffer_init(void)
{
#if 0
	// Initialize buffer for writing chip registers
	led_init_regs_buffer();

	// Initialize buffer for leds effect
	application_init_leds_info();
#endif
}

static void application_led_buffer_uninit(void)
{
	// free buffers
#if 0
	led_uninit_regs_buffer();

	application_uninit_leds_info();
#endif
}

static void application_main_init(void)
{
	// Initialize device hardware configure, eg: I2C configure 7bit
	//TODO application_device_init();

	// Initialize buffer for led registers refresh and effect
	application_led_buffer_init();
}

static void application_handle_cmd(MAIN_CMD_ENUM cmd)
{
	switch (cmd) {
		case LED_CMD_POWER_ON:
			break;
		case LED_CMD_CHIPID_CHECK:
			break;
		case LED_CMD_REG_INIT:
			break;
		case LED_CMD_EFFECT_INIT:
			break;
		default:
			break;
	}

}

static void application_main_loop(void)
{
	MAIN_CMD_ENUM cmd;

	for (cmd = LED_CMD_POWER_ON; cmd < LED_CMD_TOTAL; cmd++) {
		printf("loop test ...%u\n", cmd);
		application_handle_cmd(cmd);
	}

	//TODO some test case ...
}

int main()
{
	application_main_init();
	application_main_loop();

	return 0;
}
