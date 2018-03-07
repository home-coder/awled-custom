#ifndef __APPLICATION_KEY_H__
#define __APPLICATION_KEY_H__

#include "Application_main_option.h"
#include "Application_commu_handle.h"
#include "Application_LED_effect.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum{
	KEY_EVENT_NONE,
	KEY_EVENT_PRESS,
	KEY_EVENT_LONG_PRESS,
	KEY_EVENT_DOUBLE_CLICK,
	KEY_EVENT_RELEASE,
	KEY_EVENTS_TOTAL
}AWCHIP_KEY_EVENT_ENUM;

typedef struct{
	BYTE *p_kisr;
	BYTE *p_rawst;
	AWCHIP_KEY_EVENT_ENUM *p_event;
}AWCHIP_KEY_EVENT_STRUCT;

typedef struct{
	pAwchipCheckChipId p_check_chip_id;
	pAwchipGetRegsCfg p_get_regs_cfg;
	pAwchipRegsInit p_regs_init;
	pAwchipSleep p_chip_sleep;
	pAwchipSoftreset p_chip_softreset;
	pAwChipKeyGetInt p_chip_key_get_int;
}AWCHIP_KEY_FUNCS_STRUCT;

typedef struct{
	BYTE awchip_id;
	BYTE key_nums;
	AW_MCU_COMMU_STRUCT commu_struct;
	MCU_PINS_STRUCT pins_struct;
	const AWCHIP_KEY_FUNCS_STRUCT *p_key_funcs;
}AWCHIP_KEYS_HW_STRUCT;

extern BYTE key_get_chips_number(void);
extern const AWCHIP_KEYS_HW_STRUCT *key_get_hw_info_by_chip_index(BYTE key_chip_index);
extern AW_MCU_COMMU_ENUM key_get_commu_group(BYTE key_chip_id);
extern BYTE key_get_i2c_address(BYTE key_chip_id);
extern void key_init_events_buffer(void);
extern void key_uninit_events_buffer(void);
extern BOOL key_check_chip_id(BYTE chip_index);
extern void key_regs_init(BYTE key_chip_id);
extern void key_chip_sleep(BYTE chip_index);
extern void key_chip_softreset(BYTE chip_index);
extern void key_chip_read_reg_cfg(BYTE chip_index);
extern APP_LED_EFFECT_ENUM key_chip_handle_interrupt(BYTE chip_index);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __APPLICATION_KEY_H__ */
