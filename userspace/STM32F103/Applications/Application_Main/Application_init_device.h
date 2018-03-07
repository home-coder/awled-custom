#ifndef __APPLICATION_INIT_DEVICE_H__
#define __APPLICATION_INIT_DEVICE_H__

#include "stm32f10x.h"
#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern void application_device_init(void);
extern void application_start_timer(TIM_TypeDef * TIMx, uint16_t msDelay);
extern void application_restart_timer(TIM_TypeDef * TIMx);
extern void application_stop_timer(TIM_TypeDef * TIMx);
extern uint32 application_get_seed(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __APPLICATION_INIT_DEVICE_H__ */
