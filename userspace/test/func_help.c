/*************************************************************************
 > Filename   : func_help.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-10 14:16:36
 ************************************************************************/

#include <stdio.h>

typedef enum{false, true}BOOL;
typedef unsigned char BYTE;

BOOL aw981x_check_chipid(BYTE aw981x_id)
{
	    printf("aw981x_check_chipid\n");
		return true;
}

void aw981x_get_regs_cfg(BYTE aw981x_id)
{
	printf("aw981x_get_regs_cfg\n");
}
