/*************************************************************************
 > Filename   : Application_LED_interface.h
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-22 10:50:40
 ************************************************************************/

#ifndef __APPLICATION_LED_INTERFACE_H__
#define __APPLICATION_LED_INTERFACE_H__

#include "Application_LED.h"

typedef struct{
	BYTE led_effect_start;
	BYTE led_effect_end;
}APP_LEDS_RAGNE;

typedef enum{
	APP_COLOR_NONE,
	APP_COLOR_HALF,
	APP_COLOR_FULL,
}APP_COLOR_LEVEL;

typedef struct{
	APP_COLOR_LEVEL r;
	APP_COLOR_LEVEL g;
	APP_COLOR_LEVEL b;
}APP_COLOR_STRUCT;

typedef struct{
	BYTE led_nums;
	APP_LEDS_RAGNE led_range;
	AW_LED_STRUCT *p_led_data;
	APP_COLOR_STRUCT *p_led_color;
	BYTE *p_led_bright_level;
	BYTE *p_led_brightness;
}APP_LEDS_INFO;

typedef enum{
	LEDS_MOVE_CW, // clockwise
	LEDS_MOVE_ANTICW, // anticlockwise
}APP_LEDS_MOVE_DIR;

extern BYTE application_get_max_leds(BYTE chip_index);
extern void application_init_leds_info(void);
extern void application_uninit_leds_info(void);
extern void application_set_leds_range(BYTE chip_index, BYTE start, BYTE end);
extern BYTE application_get_max_bright_level(void);
extern void application_set_led_color(BYTE chip_index, BYTE led_idx, APP_COLOR_STRUCT *p_led_color, BOOL update_led);
extern void application_set_led_same_color(BYTE chip_index, const APP_COLOR_STRUCT *p_led_color, BOOL update_led);
extern void application_set_all_same_color(BYTE chip_index, APP_COLOR_STRUCT *p_led_color, BOOL update_led);
extern BYTE application_get_max_color_number(void);
extern const APP_COLOR_STRUCT * application_get_led_color_by_index(BYTE idx);
//extern APP_COLOR_STRUCT * application_get_led_color(BYTE chip_index, BYTE led_idx);
extern BYTE application_get_brightness_by_level(BYTE level);
extern void application_set_led_bright_level(BYTE chip_index, BYTE led_idx, BYTE bright_level, BOOL update_led);
extern void application_set_led_same_bright_level(BYTE chip_index, BYTE bright_level, BOOL update_led);
extern void application_set_all_same_bright_level(BYTE chip_index, BYTE bright_level, BOOL update_led);
//extern BYTE application_get_led_bright_level(BYTE chip_index, BYTE led_idx);
extern void application_set_led_brightness(BYTE chip_index, BYTE led_idx, BYTE brightness, BOOL update_led);
extern void application_set_led_same_brightness(BYTE chip_index, BYTE brightness, BOOL update_led);
extern void application_set_all_same_brightness(BYTE chip_index, BYTE brightness, BOOL update_led);
//extern BYTE application_get_led_brightness(BYTE chip_index, BYTE led_idx);
extern void application_led_effect_update(BYTE chip_index);

#endif  /* __APPLICATION_LED_INTERFACE_H__ */
