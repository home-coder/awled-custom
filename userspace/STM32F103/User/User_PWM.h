#ifndef _USER_PWM_H_
#define _USER_PWM_H_

#include "type.h"




enum _PWM_FRETYPE_
{
	PWM_2Hz,
	PWM_4Hz,
	PWM_8Hz,
	PWM_16Hz,
	PWM_32Hz,
	PWM_64Hz,
	PWM_125Hz,
	PWM_250Hz,
	PWM_500Hz,
	PWM_1kHz,
	PWM_2kHz,
	PWM_4kHz,
	PWM_6kHz,
	PWM_9kHz,
	PWM_12kHz,
	PWM_18kHz,
	PWM_24kHz,
	PWM_36kHz,
};

void TIM1_CH1_PWM_Config(BYTE Fretype,uint16 duty);
void TIM2_CH2_PWM_Config(BYTE Fretype,uint16 duty);
void TIM3_CH3_PWM_Config(BYTE Fretype,uint16 duty);
void TIM4_CH3_PWM_Config(BYTE Fretype,uint16 duty);
void TIM5_CH4_PWM_Config(BYTE Fretype,uint16 duty);

#endif

