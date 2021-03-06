#ifndef __APPLICATION_LED_EFFECT_H__
#define __APPLICATION_LED_EFFECT_H__

#include "Application_LED.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// led effect function
typedef enum{
	LEDS_EFFECT_NONE,
	LEDS_EFFECT_INCREASE,
	LEDS_EFFECT_DECREASE,
	LEDS_EFFECT_BRIGHT_CHANGE,
	LEDS_EFFECT_COLOR_CHANGE,
	LEDS_EFFECT_IMAX_CHANGE,
	LEDS_EFFECT_CIRCLE,
	LEDS_EFFECT_BREATH,
	LEDS_EFFECT_COLOR_BREATH,
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


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __APPLICATION_LED_EFFECT_H__ */
