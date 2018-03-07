#ifndef _USERPROGRAM_H_
#define _USERPROGRAM_H_

#include "type.h"
#include "stm32f10x_gpio.h"


// 滴答时钟全局变量
extern uint16 nTime;

//特殊操作的引脚定义
enum _PIN_NUM_
{
	PIN_PA8,
	PIN_PA1,
	PIN_PB0,
	PIN_PB8,
	PIN_PA3,
};

//特殊操作的引脚功能
enum _PIN_MODE_
{
	MODE_LEVEL,
	MODE_PULSE,
	MODE_PWM,
};


//USB_CTRL
#define USB_ENCONNECT()	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
#define USB_DISCONNECT()GPIO_SetBits(GPIOA,GPIO_Pin_2);


extern void Delay1Us(void);
extern void DelayUs(uint16 nUs);
extern void DelayMs(uint16 nMs);
extern void Delays(uint16 ns);
extern void Special_Pin_Configuration(BYTE pin_num,BYTE pin_mode,BYTE * param);
extern void PA8_Out_Configuration(BYTE pin_mode,BYTE * param);
extern void PA8_Out_Level(BYTE flag);
extern void PA8_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag);
extern void PA1_Out_Configuration(BYTE pin_mode,BYTE * param);
extern void PA1_Out_Level(BYTE flag);
extern void PA1_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag);
extern void PB0_Out_Configuration(BYTE pin_mode,BYTE * param);
extern void PB0_Out_Level(BYTE flag);
extern void PB0_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag);
extern void PB8_Out_Configuration(BYTE pin_mode,BYTE * param);
extern void PB8_Out_Level(BYTE flag);
extern void PB8_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag);
extern void PA3_Out_Configuration(BYTE pin_mode,BYTE * param);
extern void PA3_Out_Level(BYTE flag);
extern void PA3_Out_Pulse(BYTE num,BYTE timetype,uint16 Hwidth,uint16 Lwidth,BYTE endflag);

#endif
