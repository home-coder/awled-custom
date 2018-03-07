/*************************************************************************
 > Filename   : get_led_effect.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-07 17:07:20
 ************************************************************************/

#include <stdio.h>

/* static 的应用 */

static int *static_int()
{
	static int value = 10;

	return &value;
}

int main()
{
	//static int test
	int *p_value = static_int();
	*p_value = 9;
	int *p2_value = static_int();
	printf("*p2_value = %u\n", *p2_value);

	return 0;
}
