/*************************************************************************
 > Filename   : R16_I2c_option.h
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-13 15:58:19
 ************************************************************************/
#ifndef __R16_I2C_OPTION__
#define __R16_I2C_OPTION__

#include "Application_LED.h"

#define I2C_RETRIES 0x0701  /* number of times a device address should
							                      be polled when not acknowledging */
#define I2C_TIMEOUT 0x0702  /* set timeout in units of 10 ms */
/* NOTE: Slave address is 7 or 10 bits, but 10-bit addresses
    * are NOT supported! (due to code brokenness)
	 */
#define I2C_SLAVE   0x0703  /* Use this slave address */
#define I2C_SLAVE_FORCE 0x0706  /* Use this slave address, even if it
								                      is already in use by a driver! */
#define I2C_TENBIT  0x0704  /* 0 for 7 bit addrs, != 0 for 10 bit */
#define I2C_FUNCS   0x0705  /* Get the adapter functionality mask */
#define I2C_RDWR    0x0707  /* Combined R/W transfer (one STOP only) */
#define I2C_PEC     0x0708  /* != 0 to use PEC with SMBus */
#define I2C_SMBUS   0x0720  /* SMBus transfer */
#define I2C_M_TEN 0x0010
#define I2C_M_RD 0x0001


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
	struct i2c_rdwr_ioctl_data i2c_data;
}R16_I2c_HANDLER;


extern void r16_i2c_init();
extern void r16_i2c_uninit();
extern void r16_i2c_writeregisters(BYTE, AWCHIP_REGS_ACCESS_STRUCT *, BYTE *);
#endif
