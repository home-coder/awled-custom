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
	LED_CMD_EFFECT_INT_HANDLE,
	LED_CMD_TOTAL,
} MAIN_CMD_ENUM;

static void application_led_buffer_uninit(void)
{
	//TODO  free buffers
}

static void application_main_init(void)
{
	// Initialize buffer for writing chip registers
	led_init_regs_buffer();

	// Initialize buffer for leds effect
	application_init_leds_info();

	// Initialize all chips config and first effect
	for (i = 0; i < led_get_chips_number(); i++) {
		led_regs_init(i);
		application_led_effect_init(i);
	}
}

static void application_main_uninit(void)
{
	application_led_buffer_uninit();
}

static void application_handle_cmd(MAIN_CMD_ENUM cmd)
{
	//TODO some test case effect
	switch (cmd) {
	case LED_CMD_POWER_ON:
		break;
	case LED_CMD_CHIPID_CHECK:
		led_check_chip_id(0);
		break;
	case LED_CMD_EFFECT_INT_HANDLE:
		application_led_effect_interrupt_handle();
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
	while (1) {
		//effect->state

		application_handle_cmd(LED_CMD_EFFECT_INT_HANDLE);
	}
}

/*
   TODO 重新整理思路，做成一个底层服务，用于监听和处理各处的事件
 
 */
int main()
{
	application_main_init();
	application_main_loop();

	application_main_uninit();

	return 0;
}
