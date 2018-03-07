#include "Application_main.h"
#include "Chip_AW9163.h"
#include "Application_KEY_customize.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef APPLICATION_WISTRON_ENABLE

static const AWCHIP_KEY_FUNCS_STRUCT key_chip_funcs = {
	aw9163_check_chipid,
	aw9163_get_regs_cfg,
	aw9163_regs_init_key,
	aw9163_enter_standby_state,
	aw9163_softreset,
	aw9163_get_key_interrupt,
};

static const GPIO_LEVEL_STRUCT key_PA7_data = {
	GPIO_Mode_Out_PP,
	GPIO_Speed_50MHz, 
	// Set PA7  --- PDN_9163 to low as default
	Bit_RESET,
};

static const GPIO_IN_FLOATING_STRUCT key_PC4_data = {
	GPIO_Mode_IN_FLOATING,
};

static const NVIC_InitTypeDef key_nvic_PC4 = {
	EXTI4_IRQn, 1, 0, ENABLE
};

static const MCU_GPIO_STRUCT key_gpios_data[] = {
	// PA7  --- PDN_9163 Out_PP
	{
		GPIO_OUT_LEVEL, 
		GPIO_GROUP_A, 
		GPIO_Pin_7,
		(void *)&key_PA7_data,
		NULL,
	},
	// PC4  --- INTN_9163  --- IN_FLOATING
	{
		GPIO_IN_FLOATING,
		GPIO_GROUP_C,
		GPIO_Pin_4,
		(void *)&key_PC4_data,
		&key_nvic_PC4,
	},
};

const AWCHIP_KEYS_HW_STRUCT key_hw_info[APPLI_KEY_CHIPS_NUM] = {
	{	
		AW9163_1, 
		4,
		{AW_MCU_I2C_1, 0x2C},
		{sizeof(key_gpios_data)/sizeof(MCU_GPIO_STRUCT), key_gpios_data,},
		&key_chip_funcs,
	},
};

#endif /* APPLICATION_WISTRON_ENABLE */

#ifdef __cplusplus
}
#endif /* __cplusplus */
