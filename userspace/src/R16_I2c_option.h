/*************************************************************************
 > Filename   : R16_I2c_option.h
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-13 15:58:19
 ************************************************************************/
#ifndef __R16_I2C_OPTION__
#define __R16_I2C_OPTION__

#include "Application_LED.h"

#define R16_I2C0_LED			"/dev/i2c-0"
#define R16_I2C1_LED			"/dev/i2c-1"
#define R16_I2C2_LED			"/dev/i2c-2"

#define AW981x_I2C_WRITE_MSG	1

struct i2c_msg {
	unsigned char addr;
	unsigned char flags;
	unsigned short len;
	unsigned char *buf;
};

struct i2c_rdwr_ioctl_data {
	struct i2c_msg *msgs;
	int nmsgs;
};

typedef struct {
	int    fd;
	struct i2c_rdwr_ioctl_data *i2c_data;
}R16_I2c_HANDLER;


extern void r16_i2c_init();
extern void r16_i2c_uninit();
extern void r16_i2c_writeregisters(BYTE, AWCHIP_REGS_ACCESS_STRUCT *, BYTE *);
#endif
