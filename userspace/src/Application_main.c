/*************************************************************************
  > Filename   : main.c
  > Author     : oneface - one_face@sina.com
  > Company    : 一尊还酹江月
  > Time       : 2018-03-22 18:23:17
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cutils/properties.h>
#include <pthread.h>

#include "Application_LED_effect.h"

pthread_mutex_t state_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct _linestack {
	APP_LED_EFFECT_ENUM cmd_type;
	struct _linestack *next;
} linestack;

//event stack
linestack *stack = NULL;

static void push_state_queue(APP_LED_EFFECT_ENUM state)
{
	linestack *line = (linestack *) malloc(sizeof(linestack));
	line->cmd_type = state;
	line->next = stack;
	stack = line;
}

static APP_LED_EFFECT_ENUM pop_state_queue()
{
	APP_LED_EFFECT_ENUM state = LEDS_EFFECT_NONE;
	if (stack) {
		linestack *p = stack;
		stack = stack->next;
		printf("stack null：%c \n", p->cmd_type);
		state = p->cmd_type;
		free(p);
	} else {
		printf("stack null\n");
	}
	return state;
}

/*
   App call interface
*/
void listen_event(APP_LED_EFFECT_ENUM state)
{
	//TODO mutex
	//push
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();
	p_led_effect->cur_state = state;
	switch (state) {
	case:
	case:
	case:
	case:
		break;
	case LEDS_EFFECT_STARTUP:
		//clear timer
		push_state_queue();
	}
}

/*
 loop message queue
 */
static void handler_event()
{
	//TODO mutex
	//pop
	while (1) {
		pop_state_queue();
	}
}

static void application_main_handler_event()
{
	//creat

}

static void application_main_init(void)
{
	int i;
	// Initialize buffer for writing chip registers
	led_init_regs_buffer();

	// Initialize buffer for leds effect
	application_init_leds_info();

	// Initialize all chips config(led mod) and first effect
	for (i = 0; i < led_get_chips_number(); i++) {
		led_regs_init(i);
		application_led_effect_init(i);
	}

	//creat event handler pthread outof main thread
	application_main_handler_event();
}

static void application_main_test()
{
	//check chip id
	application_set_led_cur_state();
	application_led_effect_interrupt_handle();
	//change imax
	application_set_led_cur_state(LEDS_EFFECT_IMAX_CHANGE);
	application_led_effect_interrupt_handle();
	//
}

static void application_led_buffer_uninit(void)
{
	//TODO  free buffers
}

static void application_main_uninit(void)
{
	application_led_buffer_uninit();
}

int main()
{
	application_main_init();
	application_effect_test();

	application_main_uninit();

	return 0;
}
