#ifndef __CHIP_AW9163_H__
#define __CHIP_AW9163_H__

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern BOOL aw9163_check_chipid(BYTE aw9163_id);
extern void aw9163_get_regs_cfg(BYTE aw9163_id);
extern void aw9163_regs_init_key(BYTE aw9163_id);
extern void aw9163_enter_standby_state(BYTE aw9163_id);
extern void aw9163_softreset(BYTE aw9163_id);
extern void aw9163_get_key_interrupt(BYTE aw9163_id, BYTE key_num, BYTE *p_kisr, BYTE *p_rawst);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __CHIP_AW9163_H__ */
