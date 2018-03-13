/*************************************************************************
 > Filename   : R16_I2c_option.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-10 16:57:59
 ************************************************************************/

#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "R16_I2c_option.h"

#include "Application_LED.h"

static const BYTE *r16_i2c_node[] = {
#ifdef R16_I2C1_LED
	R16_I2C1_LED,
#endif

#ifdef R16_I2C2_LED
	R16_I2C2_LED,
#endif

#ifdef R16_I2C3_LED
	R16_I2C3_LED,
#endif
};

static R16_I2c_HANDLER **r16_i2c_handler = NULL;

static R16_I2c_HANDLER* r16_i2c_get_handler_by_index(BYTE i2c_index)
{
	R16_I2c_HANDLER *p_handler = NULL;
	R16_I2c_HANDLER **p_temp = r16_i2c_handler;

	if (NULL != p_temp) {
		p_handler = p_temp[i2c_index];
		if (-1 != p_handler->fd) {
			return p_handler;
		}
	}

	return NULL;
}

static void r16_i2c_set_handler_buf(R16_I2c_HANDLER **pp_r16_i2c_handler)
{
	if (NULL != pp_r16_i2c_handler) {
		r16_i2c_handler = pp_r16_i2c_handler;
	}
}

/*
	eg: reg[0x0, 0x01, 0x02, ...] to set data[0x12, 0x33, 0x42, ...]
 */
/*
   reg_access_data.device_commu = led_get_commu_group(aw981x_id);
   reg_access_data.device_addr = led_get_i2c_address(aw981x_id);
   reg_access_data.base_reg_addr = reg_addr;
   reg_access_data.regs_num = reg_num;

 */
void r16_i2c_writeregisters(BYTE mode, AWCHIP_REGS_ACCESS_STRUCT *reg_access_data, BYTE *p_value )
{

}

void r16_i2c_init()
{
	int i;
	R16_I2c_HANDLER **pp_r16_i2c_handler = NULL;

	for (i = 0; i < sizeof(r16_i2c_node)/sizeof(r16_i2c_node[0]); i++) {
		pp_r16_i2c_handler[i] = (R16_I2c_HANDLER *)malloc(sizeof(R16_I2c_HANDLER*));
		if (NULL != pp_r16_i2c_handler[i]) {
			pp_r16_i2c_handler[i]->fd = open(r16_i2c_node[i], O_RDWR);
			if (-1 == pp_r16_i2c_handler[i]->fd) {
				printf("open %s failed\n", r16_i2c_node[i]);
			}
			pp_r16_i2c_handler[i]->i2c_data = (struct i2c_rdwr_ioctl_data *)malloc(sizeof(struct i2c_rdwr_ioctl_data));
		}
	}

	r16_i2c_set_handler_buf(pp_r16_i2c_handler);

}

void r16_i2c_uninit()
{

}
