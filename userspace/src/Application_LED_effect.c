#include "Application_LED_interface.h"
#include "Application_LED_effect.h"

//#include <stdlib.h> 
#include "Chip_AW981x.h"

static const APP_COLOR_STRUCT breath_colors[] = {
	{APP_COLOR_FULL, APP_COLOR_NONE, APP_COLOR_NONE},
	{APP_COLOR_NONE, APP_COLOR_FULL, APP_COLOR_NONE},
	{APP_COLOR_NONE, APP_COLOR_NONE, APP_COLOR_FULL},
	{APP_COLOR_FULL, APP_COLOR_FULL, APP_COLOR_FULL},
};

static const BYTE breath_color_nums = sizeof(breath_colors) / sizeof(APP_COLOR_STRUCT);
static const BYTE gamma_brightness[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 2, 2, 2, 3,
	3, 3, 4, 4, 4, 5, 5, 5,
	6, 6, 7, 7, 8, 9, 10, 11,
	11, 12, 13, 14, 15, 16, 17, 18,
	19, 20, 21, 22, 23, 24, 26, 27,
	28, 29, 31, 32, 33, 35, 36, 38,
	39, 41, 42, 44, 45, 47, 48, 50,
	52, 54, 55, 57, 59, 61, 62, 63,
};

static const BYTE gamma_levels = sizeof(gamma_brightness) / sizeof(BYTE);

static WORD application_refresh_nums = 0;

static const BYTE chip_iMax[] = {
	AW981X_IMAX_10MA,
	AW981X_IMAX_20MA,
	AW981X_IMAX_30MA,
	AW981X_IMAX_40MA,
};

static BYTE application_iMax = 0;

static void application_set_refresh_nums(WORD refresh_nums)
{
	application_refresh_nums = refresh_nums;
}

static WORD application_get_refresh_nums(void)
{
	return application_refresh_nums;
}

// led effect begin
static APP_LED_EFFECT_STRUCT *application_get_led_effect(void)
{
	static APP_LED_EFFECT_STRUCT led_effect;

	return &led_effect;
}

APP_LED_EFFECT_ENUM application_get_led_cur_state(void)
{
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();
	APP_LED_EFFECT_ENUM cur_state = LEDS_EFFECT_NONE;

	if (NULL != p_led_effect) {
		cur_state = p_led_effect->cur_state;
	}

	return cur_state;
}

void application_set_led_cur_state(APP_LED_EFFECT_ENUM cur_state)
{
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();

	if (NULL != p_led_effect) {
		p_led_effect->cur_state = cur_state;
	}
}

// generate random effection for test only
APP_LED_EFFECT_ENUM application_led_effect_random(void)
{
	APP_LED_EFFECT_ENUM led_effect = LEDS_EFFECT_NONE;

//      srand(application_get_seed());
//      led_effect = (APP_LED_EFFECT_ENUM)(rand()%(LEDS_EFFECT_TOTAL-1)+1);

	return led_effect;
}

static void application_allchips_set_all_same_bright_level(int8 brightness, BOOL update_led)
{
	BYTE chip_index;

	for (chip_index = 0; chip_index < led_get_chips_number(); chip_index++) {
		application_set_all_same_bright_level(chip_index, brightness, update_led);
	}
}

static void application_allchips_set_all_same_color(APP_COLOR_STRUCT *color, update_led)
{
	BYTE chip_index;

	for (chip_index = 0; chip_index < led_get_chips_number(); chip_index++) {
		application_set_all_same_color(chip_index, color, TRUE);
	}
}

static void application_allchips_led_effect_update()
{
	BYTE chip_index;

	for (chip_index = 0; chip_index < led_get_chips_number(); chip_index++) {
		application_led_effect_update(chip_index);
	}
}

// when we change the effection, we should clear all leds firstly
static void application_led_close_all(BYTE chip_index)
{
	APP_COLOR_STRUCT color = { APP_COLOR_NONE, APP_COLOR_NONE, APP_COLOR_NONE };

	// clear all bright level and color data
	application_set_all_same_bright_level(chip_index, 0, FALSE);
	application_set_all_same_color(chip_index, &color, TRUE);
}

static void application_allchips_led_close_all()
{
	BYTE chip_index;

	for (chip_index = 0; chip_index < led_get_chips_number(); chip_index++) {
		application_led_close_all(chip_index);
	}
}

static BYTE application_allchips_get_max_leds()
{
	BYTE chip_index, led_nums = 0;

	for (chip_index = 0; chip_index < led_get_chips_number(); chip_index++) {
		led_nums += application_get_max_leds(chip_index);
	}

	return led_nums;
}

void application_led_effect_init(BYTE chip_index)
{
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();
	const AWCHIP_LEDS_HW_STRUCT *p_leds_hw = NULL;

	p_leds_hw = led_get_hw_info_by_chip_index(chip_index);
	if (NULL != p_leds_hw && APPLI_CHIPS_NONE != p_leds_hw->awchip_id && NULL != p_led_effect) {
		application_led_close_all(chip_index);
		application_set_leds_range(chip_index, 0, 0);

		memset(p_led_effect, 0, sizeof(APP_LED_EFFECT_STRUCT));
	}
}

static void application_led_effect_increase(BYTE chip_index)
{
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();
	BYTE effect_led_nums = 0;
	BYTE led_nums = 0;

	led_nums = application_get_max_leds(chip_index);
	if (NULL != p_led_effect && led_nums > 0) {
		effect_led_nums = p_led_effect->led_nums;
		if (effect_led_nums < led_nums) {
			effect_led_nums++;
		} else {
			application_led_close_all(chip_index);
			effect_led_nums = 1;
		}
		p_led_effect->led_nums = effect_led_nums;

		// if current is different from last state
		// all leds will be colse firstly in application_led_close_all()
		if ((effect_led_nums <= led_nums)
		    || (p_led_effect->last_state != p_led_effect->cur_state)) {
			application_set_leds_range(chip_index, 0, effect_led_nums - 1);
			application_set_led_same_bright_level(chip_index, p_led_effect->bright_level, FALSE);
			application_set_led_same_color(chip_index, application_get_led_color_by_index(p_led_effect->color_level), TRUE);
		}
	}
}

static void application_led_effect_decrease(BYTE chip_index)
{
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();
	BYTE effect_led_nums = 0;
	APP_COLOR_STRUCT color = { APP_COLOR_NONE, APP_COLOR_NONE, APP_COLOR_NONE };

	if (NULL != p_led_effect) {
		effect_led_nums = p_led_effect->led_nums;
		if (effect_led_nums > 0) {
			// set the bright level of last led to 0
			application_set_led_bright_level(chip_index, effect_led_nums - 1, 0, FALSE);
			// set the color of last led to NONE
			application_set_led_color(chip_index, effect_led_nums - 1, &color, TRUE);

			effect_led_nums--;
			p_led_effect->led_nums = effect_led_nums;

			if (effect_led_nums > 0) {
				// set the new led range
				application_set_leds_range(chip_index, 0, effect_led_nums - 1);
				application_set_led_same_bright_level(chip_index, p_led_effect->bright_level, FALSE);
				application_set_led_same_color(chip_index, application_get_led_color_by_index(p_led_effect->color_level), TRUE);
			}
		}
	}
}

static void application_led_effect_change_bright(BYTE chip_index)
{
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();

	if (NULL != p_led_effect) {
		// if there is not any led on, 
		// we set led number to 1 to display bright change
		if (0 == p_led_effect->led_nums) {
			p_led_effect->led_nums = 1;
		}

		application_set_leds_range(chip_index, 0, p_led_effect->led_nums - 1);

		p_led_effect->bright_level++;
		p_led_effect->bright_level %= application_get_max_bright_level();

		// color and brightness data have been clear when led effection changed
		// set the color data agin
		application_set_led_same_color(chip_index, application_get_led_color_by_index(p_led_effect->color_level), FALSE);
		// set the brightness again
		application_set_led_same_bright_level(chip_index, p_led_effect->bright_level, TRUE);
	}
}

static void application_led_effect_change_color(BYTE chip_index)
{
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();

	if (NULL != p_led_effect) {
		// if there is not any led on, 
		// we set led number to 1 to display color change
		if (0 == p_led_effect->led_nums) {
			p_led_effect->led_nums = 1;
		}

		application_set_leds_range(chip_index, 0, p_led_effect->led_nums - 1);

		p_led_effect->color_level++;
		p_led_effect->color_level %= application_get_max_color_number();

		// color and brightness data have been clear when led effection changed
		// set the brightness again
		application_set_led_same_bright_level(chip_index, p_led_effect->bright_level, FALSE);
		// set the color data agin
		application_set_led_same_color(chip_index, application_get_led_color_by_index(p_led_effect->color_level), TRUE);
	}
}

static void application_led_effect_change_iMax(BYTE chip_index)
{
	BYTE iMax_nums = sizeof(chip_iMax) / sizeof(BYTE);

	application_iMax++;
	application_iMax %= iMax_nums;

	led_chip_change_imax(chip_index, chip_iMax[application_iMax]);
}

// led move with the same color
static void application_led_effect_move(BYTE chip_index, BYTE start, BYTE len, APP_LEDS_MOVE_DIR dir)
{
	APP_COLOR_STRUCT color = { APP_COLOR_NONE, APP_COLOR_NONE, APP_COLOR_NONE };
	BYTE led_nums = 0;
	BYTE i = 0;
	BYTE end = 0;
	int8 brightness = 0;
	int8 low_brightness = 0;

	led_nums = application_get_max_leds(chip_index);
	if (led_nums > 0 && start < led_nums && len < led_nums) {
		// clear all the leds firstly
		application_set_all_same_color(chip_index, &color, TRUE);

		if (LEDS_MOVE_CW == dir) {
			// if move is clockwise, end led increases
			end = (start + len - 1) % led_nums;
			application_set_leds_range(chip_index, start, end);
		} else if (LEDS_MOVE_ANTICW == dir) {
			// if move is anti-clockwise, end led decreases
			end = (start + led_nums + 1 - len) % led_nums;
			application_set_leds_range(chip_index, end, start);
		}

		for (i = 0; i < len; i++) {
			if (LEDS_MOVE_CW == dir) {
				end = (start + i) % led_nums;
			} else if (LEDS_MOVE_ANTICW == dir) {
				end = (start + led_nums - i) % led_nums;
			}

			brightness = application_get_brightness_by_level(i);
			if (i > 0) {
				low_brightness = application_get_brightness_by_level(i - 1);

				brightness -= application_get_refresh_nums() / 4;
				if (brightness < low_brightness) {
					brightness = low_brightness;
				}
			}

			application_set_led_brightness(chip_index, end, brightness, FALSE);
		}

		// set the color of leds
		color.b = APP_COLOR_FULL;
		application_set_led_same_color(chip_index, &color, TRUE);
	}
}

static void application_led_effect_circle(BYTE chip_index)
{
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();
	APP_LEDS_MOVE_DIR dir = LEDS_MOVE_CW;
	BYTE led_nums = 0;
	WORD start = 0;
	WORD max_refresh_nums = 25;
	WORD refresh_nums = 0;

	if (NULL != p_led_effect) {
		start = p_led_effect->cur_idx;
		application_led_effect_move(chip_index, start, application_get_max_bright_level(), dir);

		refresh_nums = application_get_refresh_nums();
		refresh_nums++;
		if (refresh_nums >= max_refresh_nums) {
			led_nums = application_get_max_leds(chip_index);
			if (led_nums > 0) {
				if (LEDS_MOVE_CW == dir) {
					start++;
				} else if (LEDS_MOVE_ANTICW == dir) {
					start += led_nums - 1;
				}

				start %= led_nums;

				p_led_effect->cur_idx = start;
			}
		}

		refresh_nums %= max_refresh_nums;
		application_set_refresh_nums(refresh_nums);
	}
}

static void application_led_effect_light_up(BYTE chip_index, BYTE brightness, const APP_COLOR_STRUCT * p_color)
{
	BYTE i = 0;
	BYTE led_nums = 0;

	led_nums = application_get_max_leds(chip_index);
	if (led_nums > 0 && NULL != p_color) {
		// clear the data of all leds firstly
		application_set_all_same_brightness(chip_index, 0, FALSE);
		for (i = 0; i < led_nums; i += 2) {
			// set the bright level
			application_set_led_brightness(chip_index, i, brightness, FALSE);
			// set the color
			application_set_led_color(chip_index, i, (APP_COLOR_STRUCT *) p_color, TRUE);
		}
	}
}

static void application_led_effect_breath(BYTE chip_index)
{
	BYTE color_idx = 0;
	BYTE brightness = 0;
	WORD bright_idx = 0;
	WORD max_idx = 0;
	WORD refresh_nums = 0;
	const APP_COLOR_STRUCT *p_color = NULL;
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();

	if (NULL != p_led_effect) {
		refresh_nums = application_get_refresh_nums();

		// use p_led_effect->cur_idx to calculate brightness and color
		max_idx = 2 * gamma_levels;
		color_idx = p_led_effect->cur_idx / max_idx;
		p_color = &breath_colors[color_idx];

		bright_idx = p_led_effect->cur_idx % max_idx;

		if (bright_idx < gamma_levels) {
			brightness = gamma_brightness[bright_idx];
		} else {
			brightness = gamma_brightness[max_idx - bright_idx - 1];
		}

		// light up leds
		application_led_effect_light_up(chip_index, brightness, p_color);

		refresh_nums++;
		if (refresh_nums > 1) {
			p_led_effect->cur_idx++;
			max_idx *= breath_color_nums;
			p_led_effect->cur_idx %= max_idx;

			refresh_nums = 0;
		}

		application_set_refresh_nums(refresh_nums);
	}
}

static void application_led_effect_color_breath(BYTE chip_index)
{
	static BYTE refresh_nums = 0;
	BYTE i = 0;
	BYTE j = 0;
	BYTE max_leds = 0;
	BYTE start = 0;
	BYTE end = 0;
	BYTE bright_level = 0;
	BYTE max_color_nums = application_get_max_color_number();
	BYTE max_bright_level = application_get_max_bright_level();
	BYTE max_steps = max_bright_level * max_color_nums;
	BOOL decrease = FALSE;
	WORD cur_steps = 0;
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();

	if (NULL != p_led_effect) {
		max_leds = application_get_max_leds(chip_index);
		if (max_leds > 0) {
			// get the current step from backup index
			cur_steps = p_led_effect->cur_idx;

			// set the leds range
			start = max_leds - (max_color_nums >> 1);
			end = (max_color_nums - 1) >> 1;
			application_set_leds_range(chip_index, start, end);

			// clear all the bright data firstly
			application_set_all_same_brightness(chip_index, 0, FALSE);

			// get the bright index
			j = cur_steps / max_bright_level;
			// the current led is ramping down
			if (j >= max_color_nums) {
				j = max_color_nums - 1 - j % max_color_nums;
				decrease = TRUE;
			}

			for (i = 0; i < max_color_nums; i++) {
				// if the color by led index 
				application_set_led_color(chip_index, (start + i + max_leds) % max_leds,
							  (APP_COLOR_STRUCT *) application_get_led_color_by_index(i), FALSE);

				// set the bright level of all the leds to the max bright level
				// whose index are less than ramping one 
				if (j > i) {
					bright_level = max_bright_level - 1;
				}
				// ramping led
				else if (j == i) {
					// ramping down
					if (decrease) {
						bright_level = (max_bright_level - 1) - (cur_steps % max_bright_level);
					}
					// ramping up
					else {
						bright_level = cur_steps % max_bright_level;
					}
				}

				if (j >= i) {
					application_set_led_bright_level(chip_index, (start + i + max_leds) % max_leds, bright_level, TRUE);
				}
				// close all other leds
				else {
					application_set_led_brightness(chip_index, (start + i + max_leds) % max_leds, 0, TRUE);
				}
			}

			refresh_nums++;
			if (refresh_nums > 5) {
				refresh_nums = 0;
				cur_steps++;
			}
			cur_steps %= max_steps << 1;

			// backup current breath step
			p_led_effect->cur_idx = cur_steps;
		}
	}
}

/*
 一圈浅蓝色的灯亮起，
 浅红色的灯以 彗星 模式转动
 */
void application_led_effect_bootm_startup()
{
	APP_COLOR_STRUCT yellow = {APP_COLOR_FULL, APP_COLOR_FULL, APP_COLOR_NONE};
	APP_COLOR_STRUCT black  = {APP_COLOR_NONE, APP_COLOR_NONE, APP_COLOR_NONE};
	APP_LEDS_MOVE_DIR dir = LEDS_MOVE_CW;
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();
	WORD start = 0;
	BYTE len = application_get_max_bright_level();
	BYTE led_nums = 0;

	if (NULL == p_led_effect) {
		printf("p_led_effect is not exsit\n");
		return ;
	}

	//start = p_led_effect->cur_idx; 
	start = 0;
	led_nums = application_allchips_get_max_leds();
	half_pos = led_nums / 2;
	while (p_led_effect->last_state == p_led_effect->cur_state) {
		if (led_nums>0 && start<led_nums && len<led_nums) {
			application_set_all_same_color(&black, TRUE);	

			//set bright increase. set color yellow
			for (i = 0; i < len; i++) {
				if (LEDS_MOVE_CW == dir) {
					end = (start+i) % led_nums;
				}
				brightness = application_get_max_bright_level(i);
				if (end < half_pos) {
					application_set_led_brightness(0, end, brightness, FALSE);	
					application_set_led_color(0, end, &yellow, TRUE);
				} else {
					application_set_led_brightness(1, end % half_pos, brightness, FALSE);
					application_set_led_color(1, end, &yellow, TRUE);
				}
			}
		}
		application_allchips_led_effect_update();
		//sleep
		msleep(300);
		start++ %= led_nums;
	}

}

/*
 一圈白色的灯亮起，
 浅蓝色的灯以 彗星 模式转动
 */
void application_led_effect_bootm_complete()
{
	APP_COLOR_STRUCT yellow = {APP_COLOR_FULL, APP_COLOR_FULL, APP_COLOR_NONE};
	APP_COLOR_STRUCT white  = {APP_COLOR_FULL, APP_COLOR_FULL, APP_COLOR_FULL};
	APP_LEDS_MOVE_DIR dir = LEDS_MOVE_CW;
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();
	WORD start = 0;
	BYTE len = application_get_max_bright_level();
	BYTE led_nums = 0;

	if (NULL == p_led_effect) {
		printf("p_led_effect is not exsit\n");
		return ;
	}

	//start = p_led_effect->cur_idx;
	start = 0;
	led_nums = application_allchips_get_max_leds();
	half_pos = led_nums / 2;
	while (p_led_effect->last_state == p_led_effect->cur_state) {
		if (led_nums>0 && start<led_nums && len<led_nums) {
			application_set_all_same_color(&white, TRUE);

			//set bright increase. set color yellow
			for (i = 0; i < len; i++) {
				if (LEDS_MOVE_CW == dir) {
					end = (start+i) % led_nums;
				}
				brightness = application_get_max_bright_level(i);
				if (end < half_pos) {
					application_set_led_brightness(0, end, brightness, FALSE);
					application_set_led_color(0, end, &yellow, TRUE);
				} else {
					application_set_led_brightness(1, end % half_pos, brightness, FALSE);
					application_set_led_color(1, end, &yellow, TRUE);
				}
			}
		}
		application_allchips_led_effect_update();
		//sleep
		msleep(300);
		start++ %= led_nums;
	}

}

/*
 一圈橙色的灯亮起
 */
void application_led_effect_airkiss_mode()
{
	APP_COLOR_STRUCT color = {APP_COLOR_FULL, APP_COLOR_HALF, APP_COLOR_NONE};
	BYTE max_bright_level = application_get_max_bright_level();
	int8 brightness = application_get_brightness_by_level(max_bright_level);

	// set all bright level and color data

	application_allchips_set_all_same_bright_level(brightness, FALSE);
	application_allchips_set_all_same_color(&color, TRUE);

	application_allchips_led_effect_update();
}

/*
 浅黄色的灯以 彗星 模式转动
 */
void application_led_effect_airkiss_config()
{
	APP_COLOR_STRUCT yellow = {APP_COLOR_FULL, APP_COLOR_FULL, APP_COLOR_NONE};
	APP_COLOR_STRUCT black  = {APP_COLOR_NONE, APP_COLOR_NONE, APP_COLOR_NONE};
	APP_LEDS_MOVE_DIR dir = LEDS_MOVE_CW;
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();
	WORD start = 0;
	BYTE len = application_get_max_bright_level();
	BYTE led_nums = 0;

	if (NULL == p_led_effect) {
		printf("p_led_effect is not exsit\n");
		return ;
	}

	//start = p_led_effect->cur_idx; 
	start = 0;
	led_nums = application_allchips_get_max_leds();
	half_pos = led_nums / 2;
	while (p_led_effect->last_state == p_led_effect->cur_state) {
		if (led_nums>0 && start<led_nums && len<led_nums) {
			application_set_all_same_color(&black, TRUE);	

			//set bright increase. set color yellow
			for (i = 0; i < len; i++) {
				if (LEDS_MOVE_CW == dir) {
					end = (start+i) % led_nums;
				}
				brightness = application_get_max_bright_level(i);
				if (end < half_pos) {
					application_set_led_brightness(0, end, brightness, FALSE);	
					application_set_led_color(0, end, &yellow, TRUE);
				} else {
					application_set_led_brightness(1, end % half_pos, brightness, FALSE);
					application_set_led_color(1, end, &yellow, TRUE);
				}
			}
		}
		application_allchips_led_effect_update();
		//sleep
		msleep(300);
		start++ %= led_nums;
	}
}

/*
 联网成功后，所有灯都熄灭
 */
void application_led_effect_airkiss_connect()
{
	application_allchips_led_close_all();
	application_allchips_led_effect_update();
}

/*
 一圈蓝色灯亮起，发声方向处变为白色
 */
void application_led_effect_wake_up()
{
	//direction:property

}

/*
 一圈蓝色灯闪烁一下后，全部熄灭
 */
void application_led_effect_command_fail()
{
	APP_COLOR_STRUCT blue = {APP_COLOR_NONE, APP_COLOR_NONE, APP_COLOR_FULL};
	BYTE max_bright_level = application_get_max_bright_level();
	int8 brightness = application_get_brightness_by_level(max_bright_level);

	// set all bright level and color data

	application_allchips_set_all_same_bright_level(brightness, FALSE);
	application_allchips_set_all_same_color(&blue, TRUE);

	application_allchips_led_effect_update();

	msleep(1500);

	application_allchips_led_close_all();
	application_allchips_led_effect_update();
}

/*
 一圈灯由蓝色和浅蓝色交替变换
 */
void application_led_effect_command_success()
{
	//breath

}

/*
 一圈灯变成红色
 */
void application_led_effect_keymute()
{
	APP_COLOR_STRUCT red = {APP_COLOR_FULL, APP_COLOR_NONE, APP_COLOR_NONE};
	BYTE max_bright_level = application_get_max_bright_level();
	int8 brightness = application_get_brightness_by_level(max_bright_level);

	// set all bright level and color data

	application_allchips_set_all_same_bright_level(brightness, FALSE);
	application_allchips_set_all_same_color(&red, TRUE);

	application_allchips_led_effect_update();

}

void application_led_effect_interrupt_handle()
{
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();

	// we clear all the leds if led effection change
	if (NULL != p_led_effect && p_led_effect->last_state != p_led_effect->cur_state) {
		p_led_effect->cur_idx = 0;
		application_led_close_all(chip_index);
	}

	switch (p_led_effect->cur_state) {
	case LEDS_EFFECT_STARTUP:
		application_led_effect_bootm_startup();
		break;

	case LEDS_EFFECT_COMPLETE:
		application_led_effect_bootm_complete();
		break;

	case LEDS_EFFECT_AIRKISS_MODE:
		application_led_effect_airkiss_mode();
		break;

	case LEDS_EFFECT_AIRKISS_CONFIG:
		application_led_effect_airkiss_config();
		break;

	case LEDS_EFFECT_AIRKISS_CONNECT:
		application_led_effect_airkiss_connect();
		break;

	case LEDS_EFFECT_WAKE_UP:
		application_led_effect_wake_up();
		break;

	case LEDS_EFFECT_COMMAND_FAIL:
		application_led_effect_command_fail();
		break;

	case LEDS_EFFECT_COMMAND_SUCCESS:
		application_led_effect_command_success();
		break;

	case LEDS_EFFECT_KEYMUTE:
		application_led_effect_keymute();
		break;

	case LEDS_EFFECT_INCREASE:
		application_led_effect_increase(chip_index);
		break;

	case LEDS_EFFECT_DECREASE:
		application_led_effect_decrease(chip_index);
		break;

	case LEDS_EFFECT_BRIGHT_CHANGE:
		application_led_effect_change_bright(chip_index);
		break;

	case LEDS_EFFECT_COLOR_CHANGE:
		application_led_effect_change_color(chip_index);
		break;

	case LEDS_EFFECT_IMAX_CHANGE:
		application_led_effect_change_iMax(chip_index);
		break;

	case LEDS_EFFECT_CIRCLE:
		if (p_led_effect->last_state != p_led_effect->cur_state) {
			// clear refresh nums
			application_set_refresh_nums(0);
			// set the timer and start it
			//TODO application_start_timer( application_get_effect_TIMx(), 15 );
		} else {
			// just restart TIMx for circle
			//TODO application_restart_timer( application_get_effect_TIMx() );
		}
		application_led_effect_circle(chip_index);
		break;

	case LEDS_EFFECT_BREATH:
		if (p_led_effect->last_state != p_led_effect->cur_state) {
			// clear refresh nums
			application_set_refresh_nums(0);
			// set the timer and start it
			//TODO application_start_timer( application_get_effect_TIMx(), 15 );
		} else {
			// just restart TIMx for breath
			//TODO application_restart_timer( application_get_effect_TIMx() );
		}
		application_led_effect_breath(chip_index);
		break;

	case LEDS_EFFECT_COLOR_BREATH:
		if (p_led_effect->last_state != p_led_effect->cur_state) {
			// set the timer and start it
			//TODO application_start_timer( application_get_effect_TIMx(), 15 );
		} else {
			// just restart TIMx for breath
			//TODO application_restart_timer( application_get_effect_TIMx() );
		}
		application_led_effect_color_breath(chip_index);
		break;

	default:
		break;
	}

	// Update led effection
	if (LEDS_EFFECT_IMAX_CHANGE != p_led_effect->cur_state) {
		application_led_effect_update(chip_index);
	}
	// set the last effection state
	p_led_effect->last_state = p_led_effect->cur_state;
}

// led effect end
