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
#ifdef R16_I2C0_LED
	R16_I2C0_LED,
#endif

#ifdef R16_I2C1_LED
	R16_I2C1_LED,
#endif

#ifdef R16_I2C2_LED
	R16_I2C2_LED,
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
void r16_i2c_writeregisters(BYTE mode, AWCHIP_REGS_ACCESS_STRUCT *reg_access_data, BYTE *p_value )
{
	int fd = -1, i;
	BYTE retry = 2, ret = -1;
	BYTE dcommu, base_reg, reg_num;
	R16_I2c_HANDLER *p_i2c_handler = NULL;

	if (NULL == reg_access_data) {
		printf("reg_access_data is NULL\n");
		return ;
	}

	dcommu = reg_access_data->device_commu;
	p_i2c_handler = r16_i2c_get_handler_by_index(dcommu);
	if (NULL != p_i2c_handler) {
		fd = p_i2c_handler->fd;
		(p_i2c_handler->i2c_data.msgs[0]).addr = reg_access_data->device_addr;
		base_reg = reg_access_data->base_reg_addr;
		reg_num = reg_access_data->regs_num;

		(p_i2c_handler->i2c_data.msgs[0]).buf = (unsigned char *)malloc(2);
		p_i2c_handler->i2c_data.nmsgs = 1;
		(p_i2c_handler->i2c_data.msgs[0]).len = 1;
		(p_i2c_handler->i2c_data.msgs[0]).flags = 0;
		switch (mode) {
			case EVENT_ID_AWCHIP_REG_READ_8BIT:
				for (i = 0; i < reg_num; i++) {
					while (retry > 0) {
						(p_i2c_handler->i2c_data.msgs[0]).buf[0] = base_reg + i;
						(p_i2c_handler->i2c_data.msgs[0]).buf[1] = *(p_value + i);
						ret = ioctl(fd, I2C_RDWR, (unsigned long)p_i2c_handler);
						if (ret < 0) {
							perror("ioctl write time error");
							retry--;
							continue;
						}
						break;
					}
				}
				break;
				//EVENT_ID_AWCHIP_REG_READ_16BIT:
				//unuse
			default:
				break;
		}

		free((p_i2c_handler->i2c_data.msgs[0]).buf);
	}

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
		}
	}

	r16_i2c_set_handler_buf(pp_r16_i2c_handler);

}

void r16_i2c_uninit()
{

}
