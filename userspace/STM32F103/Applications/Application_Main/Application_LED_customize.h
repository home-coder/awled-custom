#ifndef __APPLICATION_LED_CUSTOMIZE_H__
#define __APPLICATION_LED_CUSTOMIZE_H__

#include "Application_LED.h"
#include "Application_LED_interface.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define  APPLI_LED_CHIPS_NUM		1
#define  APPLI_LED_MAX_BRIGHT_LEVEL	6
#define  APPLI_LED_MAX_COLOR		7

extern const AWCHIP_LEDS_HW_STRUCT led_hw_info[];
extern const BYTE appli_led_bright_levels[];
extern const APP_COLOR_STRUCT appli_led_colors[];


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __APPLICATION_LED_CUSTOMIZE_H__ */
