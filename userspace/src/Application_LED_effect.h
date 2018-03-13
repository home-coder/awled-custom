#ifndef __APPLICATION_LED_EFFECT_H__
#define __APPLICATION_LED_EFFECT_H__

#include "Application_LED.h"

//TODO 自定义效果 led effect function
typedef enum{
	LEDS_EFFECT_NONE,
	LEDS_EFFECT_POWERON, //TODO
	LEDS_EFFECT_INCREASE,
	LEDS_EFFECT_DECREASE,
	LEDS_EFFECT_BRIGHT_CHANGE,
	LEDS_EFFECT_COLOR_CHANGE,
	LEDS_EFFECT_IMAX_CHANGE,
	LEDS_EFFECT_CIRCLE,
	LEDS_EFFECT_BREATH,
	LEDS_EFFECT_COLOR_BREATH,
	LEDS_EFFECT_WAKEUP, //TODO
	LEDS_EFFECT_BRODCAST_COMPLETE, //TODO
	LEDS_EFFECT_TOTAL
}APP_LED_EFFECT_ENUM;

typedef struct{
	APP_LED_EFFECT_ENUM last_state;
	APP_LED_EFFECT_ENUM cur_state;
	BYTE led_nums;
	BYTE bright_level;
	BYTE color_level;
	WORD cur_idx;
}APP_LED_EFFECT_STRUCT;

extern APP_LED_EFFECT_ENUM application_get_led_cur_state(void);
extern void application_set_led_cur_state(APP_LED_EFFECT_ENUM cur_state);
extern APP_LED_EFFECT_ENUM application_led_effect_random(void);
extern void application_led_effect_init(BYTE chip_index);
extern void application_led_effect_interrupt_handle(BYTE chip_index);

#endif  /* __APPLICATION_LED_EFFECT_H__ */
