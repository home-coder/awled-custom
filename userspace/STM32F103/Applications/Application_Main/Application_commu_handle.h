#ifndef __APPLICATION_COMMU_HANDLE_H__
#define __APPLICATION_COMMU_HANDLE_H__

#include "Application_GPIO.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* MODULE_ID_AWCHIP_REG_CTRL Begin */

typedef enum{
	EVENT_ID_AWCHIP_REG_CTRL_BEGIN,
	EVENT_ID_AWCHIP_REG_READ_8BIT = 0x01,
	EVENT_ID_AWCHIP_READ_8BIT_RSP = 0x02,
	EVENT_ID_AWCHIP_REG_WRITE_8BIT = 0x08,
	EVENT_ID_AWCHIP_WRITE_8BIT_RSP = 0x09,
	EVENT_ID_AWCHIP_REG_READ_16BIT = 0x11,
	EVENT_ID_AWCHIP_READ_16BIT_RSP = 0x12,
	EVENT_ID_AWCHIP_REG_WRITE_16BIT = 0x18,
	EVENT_ID_AWCHIP_WRITE_16BIT_RSP = 0x19,
	EVENT_ID_AWCHIP_READ_REG_ERROR = 0x30,
	EVENT_ID_AWCHIP_REG_CTRL_MAX = 0xFF
}AWCHIP_REG_CTRL_EVENT_ID_ENUM;

/* MODULE_ID_AWCHIP_REG_CTRL End */
	 
typedef enum{
	AW_MCU_COMMU_BEGIN,
	AW_MCU_COMMU_NONE = AW_MCU_COMMU_BEGIN,
	AW_MCU_I2C_1,
	AW_MCU_I2C_2,
	AW_MCU_SPI_1,
	AW_MCU_SPI_2,
	AW_MCU_SPI_3,
	AW_MCU_COMMU_MAX
}AW_MCU_COMMU_ENUM;

typedef struct{
	BYTE device_commu;
	BYTE device_addr;
}AW_MCU_COMMU_STRUCT;

typedef struct{
	BYTE device_commu;
	BYTE device_addr;
	BYTE base_reg_addr;
	BYTE regs_num;
}AWCHIP_REGS_ACCESS_STRUCT;

extern BYTE aw_chip_read_commu(AWCHIP_REGS_ACCESS_STRUCT *p_aw_regs_access);
extern BYTE aw_chip_write_commu(AWCHIP_REGS_ACCESS_STRUCT *p_aw_regs_access);
extern GPIO_TypeDef * aw_chip_gpio_get_group(BYTE group);
extern BOOL aw_chip_gpio_level(const MCU_GPIO_LEVEL_STRUCT *p_aw_gpio_level);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __APPLICATION_COMMU_HANDLE_H__ */
