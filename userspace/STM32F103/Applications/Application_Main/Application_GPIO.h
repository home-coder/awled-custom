#ifndef __APPLICATION_GPIO_H__
#define __APPLICATION_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f10x_gpio.h"

typedef enum{
	GPIO_GROUP_A,
	GPIO_GROUP_B,
	GPIO_GROUP_C,
	GPIO_GROUP_D,
	GPIO_GROUP_E,
	GPIO_GROUP_F,
	GPIO_GROUP_G,
	GPIO_GROUP_TOTAL,
}MCU_GPIO_GROUP_ENUM;

typedef struct{
	WORD gpio;
	BYTE action;
	BYTE mode;
	BYTE speed;
	BYTE gpio_group;
}MCU_GPIO_LEVEL_STRUCT;

typedef struct{
	BYTE re;
	BYTE gpio_group;
	BYTE action;
	BYTE reserve;
	WORD gpio;
}MCU_GPIO_LEVEL_RSP_STRUCT;

typedef enum{
	GPIO_OUT_LEVEL,
	GPIO_OUT_PWM,
	GPIO_OUT_PULSE,
	GPIO_IN_FLOATING,
}MCU_GPIO_TYPE_ENUM;

typedef struct
{
	GPIOMode_TypeDef mode;
	GPIOSpeed_TypeDef speed;
	BYTE action;
}GPIO_LEVEL_STRUCT;

typedef struct
{
	GPIOMode_TypeDef mode;
}GPIO_IN_FLOATING_STRUCT;

typedef struct{
	BYTE gpio_type;
	BYTE gpio_group;
	WORD gpio_pin;
	const void *p_gpio_data;
	const NVIC_InitTypeDef *p_nvic_data;
}MCU_GPIO_STRUCT;

typedef struct{
	BYTE pin_nums;
	const MCU_GPIO_STRUCT *p_pins_data;
}MCU_PINS_STRUCT;

extern void aw_protocol_set_gpio_group(BYTE group);
extern void aw_protocol_set_gpio_level_pin(WORD pin);
extern void aw_protocol_set_gpio_level_action(BitAction action);
extern void aw_protocol_set_gpio_level_mode(GPIOMode_TypeDef mode);
extern void aw_protocol_set_gpio_level_speed(GPIOSpeed_TypeDef speed);
extern MCU_GPIO_LEVEL_STRUCT * aw_protocol_get_gpio_level(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __APPLICATION_GPIO_H__ */
