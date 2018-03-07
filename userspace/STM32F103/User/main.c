/*******************************************************************************
* @file    main.c
* $Author: Hu Bo $
* $Revision: 00 $
* $Date:: 2017-09-12  #$
* @brief   Ö÷º¯Êý
*******************************************************************************/

#include "Application_main.h"

int main(void)
{
	const APP_MAIN_STRUCT * const p_app_array = &app_main;

	if ( NULL != p_app_array->p_app_main_reg 
		&& NULL != p_app_array->p_app_get_func )
	{
		p_app_array->p_app_main_reg( p_app_array->p_app_get_func() );
	}
	
	if ( NULL != p_app_array->p_app_main_init )
	{
		p_app_array->p_app_main_init();
	}
	
	if ( NULL != p_app_array->p_app_main_loop )
	{
		p_app_array->p_app_main_loop();
	}

	if ( NULL != p_app_array->p_app_main_uninit )
	{
		p_app_array->p_app_main_uninit();
	}
}


