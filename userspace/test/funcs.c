/*************************************************************************
 > Filename   : funcs.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-10 14:12:07
 ************************************************************************/

#include <stdio.h>

typedef enum{false, true}BOOL;
typedef unsigned char BYTE;

extern BOOL aw981x_check_chipid(BYTE aw981x_id);
extern void aw981x_get_regs_cfg(BYTE aw981x_id);
extern void aw981x_regs_init_led(BYTE aw981x_id);
extern void aw981x_disable_chip(BYTE aw981x_id);
extern void aw981x_enable_chip(BYTE aw981x_id);
extern void aw981x_softreset(BYTE aw981x_id);
extern void aw981x_change_Imax(BYTE aw981x_id, BYTE iMax_level);
extern void aw981x_update_led_data(BYTE aw981x_id, BYTE reg_addr, BYTE reg_num, BYTE *p_value);

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

#if 0
BOOL aw981x_check_chipid(BYTE aw981x_id)
{
	printf("aw981x_check_chipid\n");
	return true;
}
#endif

#if 1
static const AWCHIP_LED_FUNCS_STRUCT led_chip_funcs = {
	aw981x_check_chipid,
	aw981x_get_regs_cfg,
	aw981x_regs_init_led,
	aw981x_disable_chip,
	aw981x_softreset,
	aw981x_change_Imax,
	aw981x_update_led_data,
};
#endif
int main()
{
#if 0
	aw981x_check_chipid(8);
#endif
	return 0;
}
