#ifndef __CHIP_AW981X_H__
#define __CHIP_AW981X_H__

#include "type.h"

#define  AW981X_IMAX_MASK		(0xCF)
#define  AW981X_IMAX_10MA		(0x00)
#define  AW981X_IMAX_20MA		(0x10)
#define  AW981X_IMAX_30MA		(0x20)
#define  AW981X_IMAX_40MA		(0x30)

#define  AW981X_DEFAULT_IMAX	AW981X_IMAX_10MA

extern BOOL aw981x_check_chipid(BYTE aw981x_id);
extern void aw981x_get_regs_cfg(BYTE aw981x_id);
extern void aw981x_regs_init_led(BYTE aw981x_id);
extern void aw981x_disable_chip(BYTE aw981x_id);
extern void aw981x_enable_chip(BYTE aw981x_id);
extern void aw981x_softreset(BYTE aw981x_id);
extern void aw981x_change_Imax(BYTE aw981x_id, BYTE iMax_level);
extern void aw981x_update_led_data(BYTE aw981x_id, BYTE reg_addr, BYTE reg_num, BYTE *p_value);

#endif  /* __CHIP_AW981X_H__ */
