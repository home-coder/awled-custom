#ifndef _USER_EXTI_H_
#define _USER_EXTI_H_

#include "type.h"
#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void Exti_PC4Init(void);
void Exti_PC4Dis(void);
void Exti_P4En(void);

void Exti_SetPC4Flag(BYTE flag);
BYTE Exti_GetPC4Flag(void);

void Exti_PB0Init(void);

void Exti_SetPB0Flag(BYTE flag);
BYTE Exti_GetPB0Flag(void);
	
void Exti_PC12Init(void);

void Exti_SetPC12Flag(BYTE flag);
BYTE Exti_GetPC12Flag(void);
	
void Exti_SetTIMFlag(TIM_TypeDef * TIMx, BYTE flag);
BYTE Exti_GetTIMFlag(TIM_TypeDef * TIMx);

#ifdef __cplusplus
}
#endif /* __cplusplus */
	
#endif

