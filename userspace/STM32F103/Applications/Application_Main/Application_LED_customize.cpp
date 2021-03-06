#include "Application_main.h"
#include "Chip_AW981x.h"
#include "Application_LED_customize.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef APPLICATION_WISTRON_ENABLE

static const AWCHIP_LED_FUNCS_STRUCT led_chip_funcs = {
	aw981x_check_chipid,
	aw981x_get_regs_cfg,
	aw981x_regs_init_led,
	aw981x_disable_chip,
	aw981x_softreset,
	aw981x_change_Imax,
	aw981x_update_led_data,
};

static const AW_LED_STRUCT led_aw981x_1_regs_map[] = 
{
	{0x33, 0x32, 0x31}, // U7
	{0x1D, 0x1C, 0x1B}, // U18
	{0x28, 0x27, 0x26}, // U8
	{0x12, 0x11, 0x10}, // U19
	{0x3E, 0x3D, 0x3C}, // U6
	{0x49, 0x48, 0x47}, // U5
	{0x4F, 0x4E, 0x4D}, // U13
	{0x44, 0x43, 0x42}, // U14
	{0x39, 0x38, 0x37}, // U15
	{0x2E, 0x2D, 0x2C}, // U16
	{0x23, 0x22, 0x21}, // U22
	{0x18, 0x17, 0x16}, // U23
	{0x15, 0x14, 0x13}, // U21
	{0x20, 0x1F, 0x1E}, // U20
	{0x2B, 0x2A, 0x29}, // U12
	{0x36, 0x35, 0x34}, // U9
	{0x41, 0x40, 0x3F}, // U11
	{0x4C, 0x4B, 0x4A}, // U10
};

static const GPIO_LEVEL_STRUCT led_PA6_data = {
	GPIO_Mode_Out_PP,
	GPIO_Speed_50MHz, 
	// Set PA6  --- SHDN_9818 to low as default
	Bit_RESET,
};

static const MCU_GPIO_STRUCT led_gpios_data[] = {
	// PA6  --- SHDN_981X Out_PP
	{
		GPIO_OUT_LEVEL, 
		GPIO_GROUP_A, 
		GPIO_Pin_6,
		(void *)&led_PA6_data,
		NULL,
	},
};

const AWCHIP_LEDS_HW_STRUCT led_hw_info[APPLI_LED_CHIPS_NUM] = {
	{	
		AW981X_1, 
		18, 
		{AW_MCU_I2C_1, 0x3A},
		// base data register is 0x10 in AW981x
		// and the last used data register is 0x4F in current chip
		{0x10, 0x4F, (0x4F-0x10+1), led_aw981x_1_regs_map},
		{sizeof(led_gpios_data)/sizeof(MCU_GPIO_STRUCT), led_gpios_data},
		&led_chip_funcs,
	},
};

const BYTE appli_led_bright_levels[APPLI_LED_MAX_BRIGHT_LEVEL] = {
	0x03, 0x06, 0x0a, 0x14, 0x28, 0x3F,
};

const APP_COLOR_STRUCT appli_led_colors[APPLI_LED_MAX_COLOR] = {
	{APP_COLOR_FULL, APP_COLOR_NONE, APP_COLOR_NONE}, // red
	{APP_COLOR_FULL, APP_COLOR_HALF, APP_COLOR_NONE}, // orange
	{APP_COLOR_FULL, APP_COLOR_FULL, APP_COLOR_NONE}, // yellow
	{APP_COLOR_NONE, APP_COLOR_FULL, APP_COLOR_NONE}, // green
	{APP_COLOR_NONE, APP_COLOR_NONE, APP_COLOR_FULL}, // blue
	{APP_COLOR_NONE, APP_COLOR_FULL, APP_COLOR_FULL}, //indigo
	{APP_COLOR_FULL, APP_COLOR_NONE, APP_COLOR_FULL}, //purple
};

#endif /* APPLICATION_WISTRON_ENABLE */

#ifdef __cplusplus
}
#endif /* __cplusplus */
