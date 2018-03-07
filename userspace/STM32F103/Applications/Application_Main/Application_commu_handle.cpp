#include "User_IIC.h"
#include "Application_commu_handle.h"
#include "Application_GPIO.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

BYTE aw_chip_read_commu(AWCHIP_REGS_ACCESS_STRUCT *p_aw_regs_access)
{
	BOOL re = FALSE;

	if ( NULL != p_aw_regs_access )
	{
		switch ( p_aw_regs_access->device_commu )
		{
			case AW_MCU_I2C_1:
				re = IIC1_ReadBytes( p_aw_regs_access->device_addr, 
										p_aw_regs_access->base_reg_addr, 
										p_aw_regs_access->regs_num, 
										(BYTE *)p_aw_regs_access+sizeof(AWCHIP_REGS_ACCESS_STRUCT) );
				break;
			
			case AW_MCU_I2C_2:
				re = IIC2_ReadBytes( p_aw_regs_access->device_addr, 
										p_aw_regs_access->base_reg_addr, 
										p_aw_regs_access->regs_num, 
										(BYTE *)p_aw_regs_access+sizeof(AWCHIP_REGS_ACCESS_STRUCT) );
				break;
			
			case AW_MCU_SPI_1:
				break;
			
			case AW_MCU_SPI_2:
				break;
			
			case AW_MCU_SPI_3:
				break;
			
			default:
				break;
		}
	}
	
	return re;
}

BYTE aw_chip_write_commu(AWCHIP_REGS_ACCESS_STRUCT *p_aw_regs_access)
{
	BOOL re = FALSE;

	if ( NULL != p_aw_regs_access )
	{
		switch ( p_aw_regs_access->device_commu )
		{
			case AW_MCU_I2C_1:
				re = IIC1_WriteBytes( p_aw_regs_access->device_addr, 
										p_aw_regs_access->base_reg_addr, 
										p_aw_regs_access->regs_num, 
										(BYTE *)p_aw_regs_access+sizeof(AWCHIP_REGS_ACCESS_STRUCT) );
				break;
			
			case AW_MCU_I2C_2:
				re = IIC2_WriteBytes( p_aw_regs_access->device_addr, 
										p_aw_regs_access->base_reg_addr, 
										p_aw_regs_access->regs_num, 
										(BYTE *)p_aw_regs_access+sizeof(AWCHIP_REGS_ACCESS_STRUCT) );
				break;
			
			case AW_MCU_SPI_1:
				break;
			
			case AW_MCU_SPI_2:
				break;
			
			case AW_MCU_SPI_3:
				break;
			
			default:
				break;
		}
	}
	
	return re;
}

GPIO_TypeDef * aw_chip_gpio_get_group(BYTE group)
{
	GPIO_TypeDef *p_gpio_group = (GPIO_TypeDef *)NULL;

	if ( group < GPIO_GROUP_TOTAL )
	{
		switch ( group )
		{
			case GPIO_GROUP_A:
				p_gpio_group = GPIOA;
				break;

			case GPIO_GROUP_B:
				p_gpio_group = GPIOB;
				break;

			case GPIO_GROUP_C:
				p_gpio_group = GPIOC;
				break;

			case GPIO_GROUP_D:
				p_gpio_group = GPIOD;
				break;

			case GPIO_GROUP_E:
				p_gpio_group = GPIOE;
				break;

			case GPIO_GROUP_F:
				p_gpio_group = GPIOF;
				break;

			case GPIO_GROUP_G:
				p_gpio_group = GPIOG;
				break;

			default:
				break;
		}
	}
	
	return p_gpio_group;
}

BOOL aw_chip_gpio_level(const MCU_GPIO_LEVEL_STRUCT *p_aw_gpio_level)
{
	BOOL re = FALSE;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_TypeDef *p_gpio_group = (GPIO_TypeDef *)NULL;

	if ( NULL != p_aw_gpio_level )
	{
		GPIO_InitStructure.GPIO_Pin = p_aw_gpio_level->gpio; 	 	
		GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)p_aw_gpio_level->mode;
		GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)p_aw_gpio_level->speed;

		p_gpio_group = aw_chip_gpio_get_group( p_aw_gpio_level->gpio_group );
		if ( NULL != p_gpio_group )
		{
			GPIO_Init( p_gpio_group, &GPIO_InitStructure );
			
			if ( p_aw_gpio_level->action )
			{
				GPIO_SetBits( p_gpio_group, p_aw_gpio_level->gpio ); 
			}
			else
			{
				GPIO_ResetBits( p_gpio_group, p_aw_gpio_level->gpio );
			}
			
			re = TRUE;
		}
	}

	return re;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
