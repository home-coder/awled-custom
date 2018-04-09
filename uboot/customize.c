/*************************************************************************
 > Filename   : customize.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-04-02 13:59:23
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LED_EFFECT_CASE
#define LED_EFFECT_CASE
#endif

typedef unsigned char byte;
#define LED_CHIP_NUMS	2

//bright value
typedef struct {
	byte r;
	byte g;
	byte b;
} ledreg_data;

//color level
typedef enum {
	LED_COLOR_NONE = 0,
	LED_COLOR_HALF,
	LED_COLOR_FULL,
} LED_COLOR_LEVEL;

//--think
typedef struct {
	LED_COLOR_LEVEL r;
	LED_COLOR_LEVEL g;
	LED_COLOR_LEVEL b;
} ledcolor_info;

typedef struct {
	byte led_nums;
	ledreg_data *p_ledreg_data;
	ledcolor_info *p_ledcolor_info;
} ledif_info;

//hw--
typedef enum {
	AW9818_1 = 0x3A,
	AW9818_2 = 0x3B,
} CHIP_ID;

typedef struct {
	CHIP_ID chip_id;
	byte led_nums;
	const ledreg_data *ledreg_map;
} ledhw_info;
ledif_info *led_intf = NULL;

typedef enum {
	ELECTRIC_10mA = (0 << 4),
	ELECTRIC_20mA = (1 << 4),
	ELECTRIC_30mA = (2 << 4),
	ELECTRIC_40mA = (3 << 4),
} ELECTRIC;

#define AW981X_DEFAULT_IMAX    (ELECTRIC_40mA)

static const byte aw981x_sleep_reg = 0x01;
static const byte aw981x_reset_reg = 0x02;
static const byte aw981x_config_reg = 0x03;
static const byte aw981x_update_reg = 0x04;

static const byte aw981x_en_value = 0x00;
static const byte aw981x_dis_value = 0x80;

static const byte aw981x_reset_value = 0x01;

// Set default-imax to 10mA, and select led matrix mode
static const byte aw981x_config_value = (0x02 | AW981X_DEFAULT_IMAX);

static const byte aw981x_update_value = 0x01;

static const byte gamma_brightness[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//9
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//19
	1, 1, 1, 1, 2, 2, 2, 3,	//27
	3, 3, 4, 4, 4, 5, 5, 5,	//35
	6, 6, 7, 7, 8, 9, 10, 11, //43
	11, 12, 13, 14, 15, 16, 17, 18, //51
	19, 20, 21, 22, 23, 24, 26, 27, //59
	28, 29, 31, 32, 33, 35, 36, 38, //67
	39, 41, 42, 44, 45, 47, 48, 50, //75
	52, 54, 55, 57, 59, 61, 62, 63, //83
};

static const byte led_bright_level[] = {
	10, 26, 42, 52, 60, 82,
};

typedef enum {
	none = 0,
	red,
	orange,
	yellow,
	green,
	blue,
	indigo,
	purple,
	LED_MAX_COLOR
} LED_COLOR;

typedef struct {
	byte cur_idx;
	byte bright_level;
	ledcolor_info background;
	ledcolor_info foward;
} ledeffect_info;
ledeffect_info *led_effect = NULL;

const ledcolor_info led_colors[LED_MAX_COLOR] = {
	{LED_COLOR_NONE, LED_COLOR_NONE, LED_COLOR_NONE},	// none
	{LED_COLOR_FULL, LED_COLOR_NONE, LED_COLOR_NONE},	// red
	{LED_COLOR_FULL, LED_COLOR_HALF, LED_COLOR_NONE},	// orange
	{LED_COLOR_FULL, LED_COLOR_FULL, LED_COLOR_NONE},	// yellow
	{LED_COLOR_NONE, LED_COLOR_FULL, LED_COLOR_NONE},	// green
	{LED_COLOR_NONE, LED_COLOR_NONE, LED_COLOR_FULL},	// blue
	{LED_COLOR_NONE, LED_COLOR_FULL, LED_COLOR_FULL},	//indigo
	{LED_COLOR_FULL, LED_COLOR_NONE, LED_COLOR_FULL},	//purple
};

const ledreg_data ledreg_map_9818_1[] = {
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
};

const ledreg_data ledreg_map_9818_2[] = {
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x0, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
};

const ledhw_info ledhw_info_struct[LED_CHIP_NUMS] = {
	{
	 AW9818_1,
	 18,
	 ledreg_map_9818_1,
	 },
	{
	 AW9818_2,
	 18,
	 ledreg_map_9818_2,
	 },
};

static const ledhw_info *get_ledhw_info(byte i)
{
	return &ledhw_info_struct[i];
}

static byte get_led_nums(byte i)
{
	const ledhw_info *p_ledhw_info = get_ledhw_info(i);
	return p_ledhw_info->led_nums;
}

static void set_ledif_info(ledif_info * ledif_info_struct)
{
	led_intf = ledif_info_struct;
}

static void ledif_info_init()
{
	byte i, len;

	ledif_info *ledif_info_struct = (ledif_info *) malloc(sizeof(ledif_info) * LED_CHIP_NUMS);
	if (NULL != ledif_info_struct) {
		for (i = 0; i < LED_CHIP_NUMS; i++) {
			ledif_info_struct[i].led_nums = get_led_nums(i);
			len = sizeof(ledreg_data) * (ledif_info_struct[i].led_nums);
			ledif_info_struct[i].p_ledreg_data = (ledreg_data *) malloc(len);
			if (NULL != ledif_info_struct[i].p_ledreg_data) {
				memset((void *)ledif_info_struct[i].p_ledreg_data, 0, len);
			} else {
				printf("ledif_info_struct[%d].p_ledreg_data is NULL\n", i);
			}
			len = sizeof(ledcolor_info) * (ledif_info_struct[i].led_nums);
			ledif_info_struct[i].p_ledcolor_info = (ledcolor_info *) malloc(len);
			if (NULL != ledif_info_struct[i].p_ledcolor_info) {
				memset((void *)ledif_info_struct[i].p_ledcolor_info, 0, len);
			} else {
				printf("ledif_info_struct[%d].p_ledcolor_info is NULL\n", i);
			}
		}
	}

	set_ledif_info(ledif_info_struct);
}

static void set_ledeffect_info(ledeffect_info * ledeffect_info_struct)
{
	led_effect = ledeffect_info_struct;
}

static ledeffect_info *get_led_effect()
{
	return led_effect;
}

static void ledeffect_info_init()
{
	ledeffect_info *ledeffect_info_struct = NULL;

	ledeffect_info_struct = (ledeffect_info *) malloc(sizeof(ledeffect_info));
	if (ledeffect_info_struct) {
		ledeffect_info_struct->cur_idx = 0;
		ledeffect_info_struct->background = led_colors[none];
		ledeffect_info_struct->foward = led_colors[none];
		ledeffect_info_struct->bright_level = led_bright_level[0];
		set_ledeffect_info(ledeffect_info_struct);
	}

	set_ledeffect_info(ledeffect_info_struct);
}

//TODO i2c function
static void aw981x_write_register(CHIP_ID chip_id, byte reg, byte data)
{
	printf("id 0x%x, reg 0x%x, data 0x%x\n", chip_id, reg, data);
}

static void aw981x_enable_chip()
{
	byte aw981x_id;

	for (aw981x_id = 0; aw981x_id < LED_CHIP_NUMS; aw981x_id++) {
		// enable chip
		aw981x_write_register(aw981x_id, aw981x_sleep_reg, aw981x_en_value);
	}
}

static void led_default_setup()
{
	printf("%s\n", __func__);
	byte aw981x_id;

	for (aw981x_id = 0; aw981x_id < LED_CHIP_NUMS; aw981x_id++) {
		// reset chip
		aw981x_write_register(aw981x_id, aw981x_reset_reg, aw981x_reset_value);
		// configure Imax and mode
		aw981x_write_register(aw981x_id, aw981x_config_reg, aw981x_config_value);
	}

	aw981x_enable_chip();
}

static byte convert_data(byte brightness, LED_COLOR_LEVEL color)
{
	switch (color) {
	case LED_COLOR_NONE:
		brightness &= 0;
		break;
	case LED_COLOR_HALF:
		brightness <<= 1;
		break;
	case LED_COLOR_FULL:
		brightness = brightness;
		break;
	default:
		break;
	}

	return brightness;
}

static void led_update_data(byte led_index, ledreg_data * p_ledreg_data)
{
	const ledhw_info *p_ledhw_info = NULL;
	const ledreg_data *lreg = NULL;

	//get regmap(r,g,b) and data(r,g,b)
	if (led_index < get_led_nums(0)) {
		p_ledhw_info = get_ledhw_info(0);
	} else {
		p_ledhw_info = get_ledhw_info(1);
		led_index -= get_led_nums(0);
	}
	lreg = p_ledhw_info->ledreg_map + led_index;

	aw981x_write_register(p_ledhw_info->chip_id, lreg->r, p_ledreg_data->r);
	aw981x_write_register(p_ledhw_info->chip_id, lreg->g, p_ledreg_data->g);
	aw981x_write_register(p_ledhw_info->chip_id, lreg->b, p_ledreg_data->b);
}

static void led_set_bright_color(byte led_index, byte brightness, const ledcolor_info color)
{
	ledif_info *ledif_info_struct = NULL;

	if (led_index < get_led_nums(0)) {
		ledif_info_struct = &led_intf[0];
	} else {
		ledif_info_struct = &led_intf[1];
	}

	//get brightness by level
	brightness = gamma_brightness[brightness];

	if (NULL != ledif_info_struct && NULL != ledif_info_struct->p_ledreg_data) {
		ledif_info_struct->p_ledreg_data->r = convert_data(brightness, color.r);
		ledif_info_struct->p_ledreg_data->g = convert_data(brightness, color.g);
		ledif_info_struct->p_ledreg_data->b = convert_data(brightness, color.b);

		led_update_data(led_index, ledif_info_struct->p_ledreg_data);
	} else if (ledif_info_struct == NULL) {
		printf("some struct is NULL\n");
	}

}

static void led_set_all_bright_color(byte led_nums, byte brightness, const ledcolor_info background)
{
	byte i;
	for (i = 0; i < led_nums; i++) {
		printf("led %u, led_set_bright_color\n", i);
		led_set_bright_color(i, brightness, background);
	}
}

static void led_set_comet(byte cur_idx, const ledcolor_info foward)
{
	byte comet_nums, i;

	comet_nums = sizeof(led_bright_level)/sizeof(led_bright_level[0]);
	for (i = 0; i < comet_nums; i++) {
		led_set_bright_color(cur_idx++, led_bright_level[i], foward);
	}

}

static void effect_comet_function(byte led_nums, byte cur_idx, byte brightness, const ledcolor_info background,  const ledcolor_info foward)
{
	//set all background
	led_set_all_bright_color(led_nums, brightness, background);

	//set comet
	led_set_comet(cur_idx, foward);

	//enable
	aw981x_enable_chip();
}

static void led_effect_startup()
{
	printf("%s\n", __func__);
	byte led_nums, comet_nums;
	ledeffect_info *p_led_effect = get_led_effect();

	if (NULL != p_led_effect) {
		led_nums = get_led_nums(0) + get_led_nums(1);
		comet_nums = sizeof(led_bright_level)/sizeof(led_bright_level[0]);

		p_led_effect->background = led_colors[none];
		p_led_effect->foward = led_colors[yellow];
		p_led_effect->cur_idx = 0;
		p_led_effect->bright_level = led_bright_level[comet_nums - 1];//故意设置背景色比comet最大值暗一些

		while (p_led_effect->cur_idx < led_nums - comet_nums) {
			effect_comet_function(led_nums, p_led_effect->cur_idx, p_led_effect->bright_level, p_led_effect->background, p_led_effect->foward);
			p_led_effect->cur_idx++;
		}
	}
}

/*
 *TODO at last, change main to android-system call
 */
int main()
{
	led_default_setup();
	ledif_info_init();
	ledeffect_info_init();

#ifdef LED_EFFECT_CASE
	//TODO think individually-mode and matrix-mode, 统还是独完全参看led_default_setup()中的设置

	led_effect_startup();
#endif
	return 0;
}
