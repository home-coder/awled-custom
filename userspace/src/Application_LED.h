#ifndef __APPLICATION_LED_H__
#define __APPLICATION_LED_H__

#include "Application_main_option.h"

typedef struct{
	BYTE r;
	BYTE g;
	BYTE b;
}AW_LED_STRUCT;

typedef struct{
	BYTE device_commu;
	BYTE device_addr;
	BYTE base_reg_addr;
	BYTE regs_num;
}AWCHIP_REGS_ACCESS_STRUCT;

#define EVENT_ID_AWCHIP_REG_READ_8BIT 0x01
#define EVENT_ID_AWCHIP_REG_WRITE_8BIT 0x02

typedef struct{
	BYTE reg_data_base;
	BYTE reg_data_end;
	BYTE reg_data_nums;
	const AW_LED_STRUCT *p_regs_map;
}LED_REGS_STRUCT;

typedef BOOL (*pAwchipCheckChipId)(BYTE);
typedef void (*pAwchipGetRegsCfg)(BYTE);
typedef void (*pAwchipRegsInit)(BYTE);
typedef void (*pAwchipSleep)(BYTE);
typedef void (*pAwchipSoftreset)(BYTE);
typedef void (*pAwchipChangeImax)(BYTE, BYTE);
typedef void (*pAwChipUpdateData)(BYTE, BYTE, BYTE, BYTE *);
typedef void (*pAwChipKeyGetInt)(BYTE, BYTE, BYTE *, BYTE *);

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
	BYTE device_commu;
	BYTE device_addr;
}AW_MCU_COMMU_STRUCT;

typedef enum{
	APPLI_CHIPS_NONE,

	AWCHIP_LED_START,
	AW9817 = AWCHIP_LED_START,
	AW9818,
	AWCHIP_LED_END,

	APPLI_TOTAL_CHIPS,
}APPLI_CHIP_ENUM;

typedef struct{
	BYTE awchip_id;
	BYTE led_nums;
	AW_MCU_COMMU_STRUCT commu_struct;
	LED_REGS_STRUCT led_regs_info;
	const AWCHIP_LED_FUNCS_STRUCT *p_led_funcs;
}AWCHIP_LEDS_HW_STRUCT;

typedef enum{
	AW_MCU_COMMU_BEGIN,
	AW_MCU_COMMU_NONE = AW_MCU_COMMU_BEGIN,
	AW_MCU_I2C_1,
	AW_MCU_I2C_2,
	AW_MCU_COMMU_MAX
}AW_MCU_COMMU_ENUM;

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

#endif  /* __APPLICATION_LED_H__ */
