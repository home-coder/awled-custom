/*************************************************************************
 > Filename   : Application_LED_effect.h
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-22 10:46:09
 ************************************************************************/

#ifndef __APPLICATION_LED_EFFECT_H__
#define __APPLICATION_LED_EFFECT_H__

#include "Application_LED.h"

typedef enum{
	LEDS_EFFECT_NONE = 0,
	LEDS_EFFECT_STARTUP,					/*board power on*/
	LEDS_EFFECT_COMPLETE,					/*board startup over*/
	LEDS_EFFECT_AIRKISS_MODE,				/*in airkiss mode*/
	LEDS_EFFECT_AIRKISS_CONFIG,				/*config airkiss*/
	LEDS_EFFECT_AIRKISS_CONNECT,			/*airkiss connected*/
	LEDS_EFFECT_WAKE_UP,					/*voice wake up*/
	LEDS_EFFECT_COMMAND_FAIL,				/*voice command failed*/
	LEDS_EFFECT_COMMAND_SUCCESS,			/*voice command success*/
	LEDS_EFFECT_KEYMUTE,					/*key mute record*/
	LEDS_EFFECT_KEYUNMUTE,					/*key cancel mute record*/
	LEDS_EFFECT_BRIGHT_CHANGE,
	LEDS_EFFECT_COLOR_CHANGE,
	LEDS_EFFECT_IMAX_CHANGE,
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

extern APP_LED_EFFECT_STRUCT* application_get_led_effect(void);
extern APP_LED_EFFECT_ENUM application_get_led_cur_state(void);
extern void application_set_led_cur_state(APP_LED_EFFECT_ENUM cur_state);
extern APP_LED_EFFECT_ENUM application_led_effect_random(void);
extern void application_led_effect_init(BYTE chip_index);
extern void application_allchips_led_close_all();
extern void application_led_effect_bootm_startup();
extern void application_led_effect_bootm_complete();
extern void application_led_effect_interrupt_handle();

#endif  /* __APPLICATION_LED_EFFECT_H__ */
