/*************************************************************************
 > Filename   : breath.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-16 15:31:58
 ************************************************************************/

#include <stdio.h>

enum {APP_COLOR_NONE, APP_COLOR_FULL};

typedef struct {
	char r;
	char g;
	char b;
} APP_COLOR_STRUCT;

static const APP_COLOR_STRUCT breath_colors[] = {
	{APP_COLOR_FULL, APP_COLOR_NONE, APP_COLOR_NONE}, //r
	{APP_COLOR_NONE, APP_COLOR_FULL, APP_COLOR_NONE}, //g
	{APP_COLOR_NONE, APP_COLOR_NONE, APP_COLOR_FULL}, //b
};

static const char breath_color_nums = sizeof(breath_colors) / sizeof(APP_COLOR_STRUCT);
static const char gamma_brightness[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 2, 2, 2, 3,
	3, 3, 4, 4, 4, 5, 5, 5,
	6, 6, 7, 7, 8, 9, 10, 11,
	11, 12, 13, 14, 15, 16, 17, 18,
	19, 20, 21, 22, 23, 24, 26, 27,
	28, 29, 31, 32, 33, 35, 36, 38,
	39, 41, 42, 44, 45, 47, 48, 50,
	52, 54, 55, 57, 59, 61, 62, 63,
};

static const char gamma_levels = sizeof(gamma_brightness) / sizeof(char);
struct {
	//TODO
	int cur_idx;
}p_led_effect;

static void breath_amlogic(int step)
{
	const APP_COLOR_STRUCT *cur_color = NULL;
	char bright, index;

	index = p_led_effect.cur_idx % gamma_levels;
	bright = gamma_brightness[index];

	cur_color = &breath_colors[p_led_effect.cur_idx / gamma_levels % (breath_color_nums+1)];
	
	printf("step:%d....color:(r%d-g%d-b%d).....bright:%d (index=%d)\n", step, cur_color->r, cur_color->g, cur_color->b, bright, index);
	p_led_effect.cur_idx++;
}

int main()
{
	int i;

	p_led_effect.cur_idx = gamma_brightness[0];
	for (i = 0; i < 200; i++) { //TEST timer = cpu 200, 最终要用select模拟定时器进行转化 还是使用睡眠while msleep
		breath_amlogic(i);	
	}

	return 0;
}
