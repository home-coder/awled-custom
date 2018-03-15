/*************************************************************************
 > Filename   : format.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-10 17:31:29
 ************************************************************************/

#include <stdio.h>

void led_chip_read_reg_cfg(BYTE chip_index)
{
	const AWCHIP_LEDS_HW_STRUCT *p_hw_info = NULL;

	p_hw_info = led_get_hw_info_by_chip_index(chip_index);
	if (NULL != p_hw_info
	    && NULL != p_hw_info->p_led_funcs
	    && NULL != p_hw_info->p_led_funcs->p_get_regs_cfg
	    && APPLI_CHIPS_NONE != p_hw_info->awchip_id) {
		p_hw_info->p_led_funcs->p_get_regs_cfg(p_hw_info->awchip_id);
	}
}
