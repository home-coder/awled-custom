#ifndef __APPLICATION_LED_H__
#define __APPLICATION_LED_H__

#include "Application_main_option.h"
#include "Application_commu_handle.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct{
	BYTE r;
	BYTE g;
	BYTE b;
}AW_LED_STRUCT;

typedef struct{
	BYTE reg_data_base;
	BYTE reg_data_end;
	BYTE reg_data_nums;
	const AW_LED_STRUCT *p_regs_map;
}LED_REGS_STRUCT;

typedef struct{
	pAwchipCheckChipId p_check_chip_id;
	pAwchipGetRegsCfg p_get_regs_cfg;
	pAwchipRegsInit p_regs_init;
	pAwchipSleep p_chip_sleep;
	pAwchipSoftreset p_chip_softreset;
	pAwchipChangeImax p_chip_change_imax;
	pAwChipUpdateData p_chip_update_data;
}AWCHIP_LED_FUNCS_STRUCT;

typedef struct{
	BYTE awchip_id;
	BYTE led_nums;
	AW_MCU_COMMU_STRUCT commu_struct;
	LED_REGS_STRUCT led_regs_info;
	MCU_PINS_STRUCT pins_struct;
	const AWCHIP_LED_FUNCS_STRUCT *p_led_funcs;
}AWCHIP_LEDS_HW_STRUCT;

extern BYTE led_get_chips_number(void);
extern const AWCHIP_LEDS_HW_STRUCT *led_get_hw_info_by_chip_index(BYTE led_chip_index);
extern AW_MCU_COMMU_ENUM led_get_commu_group(BYTE led_chip_id);
extern BYTE led_get_i2c_address(BYTE led_chip_id);
extern BOOL led_check_chip_id(BYTE chip_index);
extern void led_chip_read_reg_cfg(BYTE chip_index);
extern void led_regs_init(BYTE chip_index);
extern void led_chip_sleep(BYTE chip_index);
extern void led_chip_softreset(BYTE chip_index);
extern void led_chip_change_imax(BYTE chip_index, BYTE iMax_level);
extern void led_init_regs_buffer(void);
extern void led_uninit_regs_buffer(void);
extern void led_effect_update_data(BYTE led_chip_id, AW_LED_STRUCT * p_led_data, 
								const AWCHIP_LEDS_HW_STRUCT *p_led_hw_info);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __APPLICATION_LED_H__ */
