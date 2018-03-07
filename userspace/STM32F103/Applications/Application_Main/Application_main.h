#ifndef __APPLICATION_MAIN_H__
#define __APPLICATION_MAIN_H__

#include "Application_main_option.h"
#include "Application_commu_handle.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum{
	MAIN_CMD_NONE = 0,
	MAIN_CMD_START,
	
	LED_CMD_START = MAIN_CMD_START,
	LED_CMD_POWER_ON,
	LED_CMD_POWER_OFF,
	LED_CMD_CHIPID_CHECK,
	LED_CMD_REG_INIT,
	LED_CMD_REG_SLEEP,
	LED_CMD_REG_READ,

	LED_CMD_EFFECT_INIT,
	LED_CMD_EFFECT_INT_HANDLE,

	LED_CMD_TOTAL,
	LED_CMD_END = LED_CMD_START + 100,

	KEY_CMD_START,
	KEY_CMD_POWER_ON,
	KEY_CMD_POWER_OFF,
	KEY_CMD_CHIPID_CHECK,
	KEY_CMD_REG_INIT,
	KEY_CMD_REG_SLEEP,
	KEY_CMD_REG_SOFTRESET,
	KEY_CMD_REG_READ,
	
	KEY_CMD_END = KEY_CMD_START + 100,

	TIMER_CMD_START,
	TIMER_CMD_TIM2_START,
	TIMER_CMD_TIM2_RESTART,
	TIMER_CMD_TIM2_STOP,
	TIMER_CMD_TIM2_EXPIRED,

	TIMER_CMD_TIM3_START,
	TIMER_CMD_TIM3_RESTART,
	TIMER_CMD_TIM3_STOP,
	TIMER_CMD_TIM3_EXPIRED,

	TIMER_CMD_TIM4_START,
	TIMER_CMD_TIM4_RESTART,
	TIMER_CMD_TIM4_STOP,
	TIMER_CMD_TIM4_EXPIRED,

	TIMER_CMD_TIM5_START,
	TIMER_CMD_TIM5_RESTART,
	TIMER_CMD_TIM5_STOP,
	TIMER_CMD_TIM5_EXPIRED,
	TIMER_CMD_END = TIMER_CMD_START + 100,
	
	MAIN_CMD_END,
}MAIN_CMD_ENUM;
	
typedef struct CMD_STRUCT{
	MAIN_CMD_ENUM cmd_type;
	struct CMD_STRUCT *p_next;
}MAIN_CMD_STRUCT;

typedef void (*pAppInterruptCmd)(void);
typedef BOOL (*pAppPushCmd)(MAIN_CMD_STRUCT *);
typedef MAIN_CMD_STRUCT * (*pAppGetCmd)(void);
typedef void (*pAppPopCmd)(void);
typedef void (*pAppHandleCmd)(MAIN_CMD_STRUCT *);

typedef struct{
	pAppInterruptCmd p_app_int_cmd;
	pAppPushCmd p_app_push_cmd;
	pAppGetCmd p_app_get_cmd;
	pAppPopCmd p_app_pop_cmd;
	pAppHandleCmd p_app_handle_cmd;
}APP_FUNC_STRUCT;

typedef APP_FUNC_STRUCT * (*pAppMainGetFunc)(void); 
typedef void (*pAppMainRegister)(APP_FUNC_STRUCT *);
typedef void (*pAppMainInit)(void);
typedef void (*pAppMainLoop)(void);
typedef void (*pAppMainUninit)(void);

typedef struct{
	pAppMainGetFunc p_app_get_func;
	pAppMainRegister p_app_main_reg;
	pAppMainInit p_app_main_init;
	pAppMainLoop p_app_main_loop;
	pAppMainUninit p_app_main_uninit;
}APP_MAIN_STRUCT;

extern const APP_MAIN_STRUCT app_main;

typedef enum{
	APPLI_CHIPS_NONE,
	
	AWCHIP_LED_START,
	AW981X_1 = AWCHIP_LED_START,
	AWCHIP_LED_END,

	AWCHIP_KEY_START,
	AW9163_1 = AWCHIP_KEY_START,
	AWCHIP_KEY_END,
	
	APPLI_TOTAL_CHIPS,
}APPLI_CHIP_ENUM;

#define APPLICATION_ControlGPIOLEVEL(p_gpio_struct)		aw_chip_gpio_level(p_gpio_struct)

#define APPLICATION_ReadRegisters(event_id, p_reg_access, p_reg_data)	\
	do {	\
			BYTE len = 0;	\
			BYTE *m_pReadRegs = NULL;	\
			AWCHIP_REGS_ACCESS_STRUCT *p_access = (AWCHIP_REGS_ACCESS_STRUCT *)p_reg_access;	\
					\
			if ( EVENT_ID_AWCHIP_REG_READ_16BIT == event_id ) {  \
				((AWCHIP_REGS_ACCESS_STRUCT *)p_reg_access)->regs_num *= 2;  \
			}	\
					\
			len = sizeof(AWCHIP_REGS_ACCESS_STRUCT) + p_access->regs_num;	\
					\
			m_pReadRegs = (BYTE *)Application_malloc( len );	\
			if ( NULL != m_pReadRegs )	\
			{	\
				memset( (void *)m_pReadRegs, 0, len );	\
				memcpy( (void *)m_pReadRegs, (const void *)p_access, sizeof(AWCHIP_REGS_ACCESS_STRUCT) );	\
						\
				aw_chip_read_commu( (AWCHIP_REGS_ACCESS_STRUCT *)m_pReadRegs );	\
						\
				if ( EVENT_ID_AWCHIP_REG_READ_16BIT == event_id ) {  \
					BYTE i = 0; 	\
					for ( i = 0; i < p_access->regs_num; i+=2 ) { 	\
						((BYTE *)p_reg_data)[i] = ((BYTE *)(m_pReadRegs+sizeof(AWCHIP_REGS_ACCESS_STRUCT)))[i+1]; 	\
						((BYTE *)p_reg_data)[i+1] = ((BYTE *)(m_pReadRegs+sizeof(AWCHIP_REGS_ACCESS_STRUCT)))[i]; 	\
					} 	\
				}	\
				else {	\
					memcpy( (void *)p_reg_data, (const void *)(m_pReadRegs+sizeof(AWCHIP_REGS_ACCESS_STRUCT)), p_access->regs_num);	\
				}	\
						\
				free( m_pReadRegs );	\
			}	\
	}while( 0 )

#define APPLICATION_WriteRegisters(event_id, p_reg_access, p_reg_data)	\
	do {	\
			BYTE len = 0;	\
			BYTE *m_pWriteRegs = NULL;	\
			AWCHIP_REGS_ACCESS_STRUCT *p_access = (AWCHIP_REGS_ACCESS_STRUCT *)p_reg_access;	\
					\
			if ( EVENT_ID_AWCHIP_REG_WRITE_16BIT == event_id ) {  \
				p_access->regs_num *= 2;  \
			}	\
					\
			len = sizeof(AWCHIP_REGS_ACCESS_STRUCT) + p_access->regs_num;	\
					\
			m_pWriteRegs = (BYTE *)Application_malloc( len );	\
			if ( NULL != m_pWriteRegs )	\
			{	\
			memset( (void *)m_pWriteRegs, 0, len );	\
					\
			memcpy( (void *)m_pWriteRegs, (const void *)p_access, sizeof(AWCHIP_REGS_ACCESS_STRUCT) );	\
			memcpy( (void *)&m_pWriteRegs[sizeof(AWCHIP_REGS_ACCESS_STRUCT)], (const void *)p_reg_data, p_access->regs_num );	\
					\
			aw_chip_write_commu( (AWCHIP_REGS_ACCESS_STRUCT *)m_pWriteRegs );	\
					\
			free( m_pWriteRegs );	\
			}	\
	}while( 0 )

typedef BOOL (*pAwchipCheckChipId)(BYTE);
typedef void (*pAwchipGetRegsCfg)(BYTE);
typedef void (*pAwchipRegsInit)(BYTE);
typedef void (*pAwchipSleep)(BYTE);
typedef void (*pAwchipSoftreset)(BYTE);
typedef void (*pAwchipChangeImax)(BYTE, BYTE);
typedef void (*pAwChipUpdateData)(BYTE, BYTE, BYTE, BYTE *);
typedef void (*pAwChipKeyGetInt)(BYTE, BYTE, BYTE *, BYTE *);

extern TIM_TypeDef * application_get_effect_TIMx(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __APPLICATION_MAIN_H__ */
