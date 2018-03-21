#include <stdio.h>
#include "Application_LED.h"
#include "Chip_AW981x.h"

static const BYTE aw981x_chipid_reg = 0x00;
static const BYTE aw981x_sleep_reg = 0x01;
static const BYTE aw981x_reset_reg = 0x02;
static const BYTE aw981x_config_reg = 0x03;
static const BYTE aw981x_update_reg = 0x04;

static const BYTE aw981x_chipid_value = 0x18;

static const BYTE aw981x_en_value = 0x00;
static const BYTE aw981x_dis_value = 0x80;

static const BYTE aw981x_reset_value = 0x01;

// Set Imax to 10mA, and select led matrix mode
static const BYTE aw981x_config_value = (0x02 | AW981X_DEFAULT_IMAX);

static const BYTE aw981x_update_value = 0x01;

static void aw981x_read_register(BYTE aw981x_id, BYTE reg_addr, BYTE reg_num, BYTE * p_value)
{
	AWCHIP_REGS_ACCESS_STRUCT reg_access_data;
	BYTE *p_regs_buf = NULL;

	if (aw981x_id >= AWCHIP_LED_START && aw981x_id < AWCHIP_LED_END) {
		// if data buffer is NULL, malloc a buffer for reading
		if (NULL == p_value) {
			p_regs_buf = (BYTE *) Application_malloc(reg_num);
			if (NULL != p_regs_buf) {
				memset((void *)p_regs_buf, 0, reg_num);
			}
		} else {
			p_regs_buf = p_value;
		}

		reg_access_data.device_commu = led_get_commu_group(aw981x_id);
		reg_access_data.device_addr = led_get_i2c_address(aw981x_id);
		reg_access_data.base_reg_addr = reg_addr;
		reg_access_data.regs_num = reg_num;

		//TODO APPLICATION_ReadRegisters( EVENT_ID_AWCHIP_REG_READ_8BIT, &reg_access_data, p_regs_buf );

		// if data buffer is NULL, 
		// we just read it to new buffer and will not use it.
		// sometimes we can use jtag to check the registers for debug
		if (NULL == p_value) {
			Application_free(p_regs_buf);
			p_regs_buf = NULL;
		} else {
			memcpy((void *)p_value, (const void *)p_regs_buf, reg_num);
		}
	}
}

BOOL aw981x_check_chipid(BYTE aw981x_id)
{
	BOOL re = FALSE;
	BYTE chip_id = 0;

	aw981x_read_register(aw981x_id, aw981x_chipid_reg, 1, &chip_id);
	if (aw981x_chipid_value == chip_id) {
		re = TRUE;
	}

	return re;
}

void aw981x_get_regs_cfg(BYTE aw981x_id)
{
	// just read 5 registers to check configure
	aw981x_read_register(aw981x_id, aw981x_chipid_reg, 5, NULL);
}

static void aw981x_write_register_ex(BYTE aw981x_id, BYTE reg_addr, BYTE reg_num, BYTE * p_value)
{
	AWCHIP_REGS_ACCESS_STRUCT reg_access_data;

	if (aw981x_id >= AWCHIP_LED_START && aw981x_id < AWCHIP_LED_END && NULL != p_value) {
		reg_access_data.device_commu = led_get_commu_group(aw981x_id);
		reg_access_data.device_addr = led_get_i2c_address(aw981x_id);
		reg_access_data.base_reg_addr = reg_addr;
		reg_access_data.regs_num = reg_num;
#ifdef DEBUG
		int i;
		printf("aw981x_id:%d, device_commu:%d, device_addr:%x, base_reg_addr:%x, regs_num:%d\n", aw981x_id, reg_access_data.device_commu,
		       reg_access_data.device_addr, reg_access_data.base_reg_addr, reg_access_data.regs_num);
		for (i = 0; i < reg_num; i++) {
			printf("(%d,%x)", i, *(p_value + i));
			if (0 != i && 0 == i % 3) {
				printf("\n");
			}
		}
#endif
		//TODO APPLICATION_WriteRegisters( EVENT_ID_AWCHIP_REG_WRITE_8BIT, &reg_access_data, p_value );
	}
}

static void aw981x_write_register(BYTE aw981x_id, BYTE reg_addr, BYTE value)
{
	aw981x_write_register_ex(aw981x_id, reg_addr, 1, &value);
}

void aw981x_regs_init_led(BYTE aw981x_id)
{
	// reset chip
	aw981x_write_register(aw981x_id, aw981x_reset_reg, aw981x_reset_value);

	// configure Imax and mode
	aw981x_write_register(aw981x_id, aw981x_config_reg, aw981x_config_value);

	// enable chip
	aw981x_write_register(aw981x_id, aw981x_sleep_reg, aw981x_en_value);
}

void aw981x_disable_chip(BYTE aw981x_id)
{
	aw981x_write_register(aw981x_id, aw981x_sleep_reg, aw981x_dis_value);	// Enter Sleep Mode
}

void aw981x_enable_chip(BYTE aw981x_id)
{
	aw981x_write_register(aw981x_id, aw981x_sleep_reg, aw981x_en_value);	// Enable Normal Mode
}

void aw981x_softreset(BYTE aw981x_id)
{
	aw981x_write_register(aw981x_id, aw981x_reset_reg, aw981x_reset_value);
}

void aw981x_change_Imax(BYTE aw981x_id, BYTE iMax_level)
{
	BYTE config = aw981x_config_value;

	config &= AW981X_IMAX_MASK;
	config |= iMax_level;

	// configure Imax and mode
	aw981x_write_register(aw981x_id, aw981x_config_reg, config);
}

void aw981x_update_led_data(BYTE aw981x_id, BYTE reg_addr, BYTE reg_num, BYTE * p_value)
{
	if (NULL != p_value) {
		// Update led data to data registers
		aw981x_write_register_ex(aw981x_id, reg_addr, reg_num, p_value);

		// Update the chip if we change the data registers
		aw981x_write_register(aw981x_id, aw981x_update_reg, aw981x_update_value);
	}
}
