/*************************************************************************
  > Filename   : main.c
  > Author     : oneface - one_face@sina.com
  > Company    : 一尊还酹江月
  > Time       : 2018-03-22 18:23:17
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "Application_LED_effect.h"

//some effect will repeat and should support be interrupted
static int timer = 0x0;
static int timer_0 = 0x1;				//LEDS_EFFECT_STARTUP
static int timer_1 = 0x1 << 1;			//LEDS_EFFECT_COMPLETE
static int timer_2 = 0x1 << 2;			//LEDS_EFFECT_AIRKISS_CONFIG
static int timer_3 = 0x1 << 3;			//LEDS_EFFECT_AIRKISS_CONNECT
static int timer_4 = 0x1 << 3;			//LEDS_EFFECT_COMMAND_SUCCESS

pthread_mutex_t state_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct _linestack {
	APP_LED_EFFECT_ENUM cmd_type;
	struct _linestack *next;
} linestack;

//event stack
linestack *stack = NULL;

typedef void (*effect_handle) ();

static void push_state_queue(APP_LED_EFFECT_ENUM state)
{
	//TODO mutex
	pthread_mutex_lock(&state_lock);
	Application_debug("push stack-state :%d\n", state);
	linestack *line = (linestack *) malloc(sizeof(linestack));
	line->cmd_type = state;
	line->next = stack;
	stack = line;
	pthread_mutex_unlock(&state_lock);
}

static APP_LED_EFFECT_ENUM pop_state_queue()
{
	//TODO mutex
	pthread_mutex_lock(&state_lock);
	Application_debug("pop ");
	APP_LED_EFFECT_ENUM state = LEDS_EFFECT_NONE;
	if (stack) {
		linestack *p = stack;
		stack = stack->next;
		state = p->cmd_type;
		Application_debug("stack-state ：%d \n", state);
		free(p);
	} else {
		Application_debug("stack null\n");
	}
	pthread_mutex_unlock(&state_lock);

	return state;
}

void clear_timer()
{
	timer &= 0;
}

static void set_timer(int timerx)
{
	timer |= timerx;
}

/*
   App call interface
*/
void send_event(APP_LED_EFFECT_ENUM state)
{
	//push
	APP_LED_EFFECT_STRUCT *p_led_effect = application_get_led_effect();
	p_led_effect->cur_state = state;
	//clear timer
	if (p_led_effect->cur_state != p_led_effect->last_state) {	//state != last state, clear
		Application_debug("clear timer\n");
		clear_timer();
		switch (state) {
			case LEDS_EFFECT_STARTUP:
				push_state_queue(LEDS_EFFECT_STARTUP);
				set_timer(timer_0);
				break;
			case LEDS_EFFECT_COMPLETE:
				push_state_queue(LEDS_EFFECT_COMPLETE);
				set_timer(timer_1);
				break;
			case LEDS_EFFECT_AIRKISS_MODE:
				push_state_queue(LEDS_EFFECT_AIRKISS_MODE);
				break;
			case LEDS_EFFECT_AIRKISS_CONFIG:
				push_state_queue(LEDS_EFFECT_AIRKISS_CONFIG);
				set_timer(timer_2);
				break;
			case LEDS_EFFECT_AIRKISS_CONNECT:
				push_state_queue(LEDS_EFFECT_AIRKISS_CONNECT);
				set_timer(timer_3);
				break;
			case LEDS_EFFECT_WAKE_UP:
				push_state_queue(LEDS_EFFECT_WAKE_UP);
				break;
			case LEDS_EFFECT_COMMAND_FAIL:
				push_state_queue(LEDS_EFFECT_COMMAND_FAIL);
				break;
			case LEDS_EFFECT_COMMAND_SUCCESS:
				push_state_queue(LEDS_EFFECT_COMMAND_SUCCESS);
				set_timer(timer_4);
				break;
			case LEDS_EFFECT_KEYMUTE:
				push_state_queue(LEDS_EFFECT_KEYMUTE);
				break;
			case LEDS_EFFECT_KEYUNMUTE:
				push_state_queue(LEDS_EFFECT_KEYUNMUTE);
				break;
			default:
				break;
		}
	}
}

static void timer_function(int interval, effect_handle e_handle_state)
{
	struct timeval temp;

	temp.tv_sec = 0;
	temp.tv_usec = interval;
	select(0, NULL, NULL, NULL, &temp);
	e_handle_state();
}

/*
 loop message queue
 */
static void handle_event(void *param)
{
	APP_LED_EFFECT_STRUCT *p_led_effect = NULL;
	APP_LED_EFFECT_ENUM state;

	Application_debug("handle event\n");
	while (1) {
		msleep(200);
		state = pop_state_queue();	// after every case-break, here will go
		if (state == LEDS_EFFECT_NONE) {
			Application_debug("state is null, continue\n");
			continue;
		}

		p_led_effect = application_get_led_effect();
		p_led_effect->last_state = state;
		// we clear all the leds if led effection change
		if (NULL != p_led_effect) {
			p_led_effect->cur_idx = 0;
			application_set_circle_nums(0);
			application_allchips_led_close_all();
		}

		switch (state) {
		case LEDS_EFFECT_STARTUP: //TODO should be in kernel
			while (timer & timer_0) {
				timer_function(500000, application_led_effect_bootm_startup);
			}
			break;
		case LEDS_EFFECT_COMPLETE:
			while (timer & timer_1) {
				timer_function(5000, application_led_effect_bootm_complete);
			}
			break;
		}
	}
}

static void application_main_event_handler()
{
	//TODO self-delete creat
	pthread_t pidl;
	int err;
	err = pthread_create(&pidl, NULL, (void *)handle_event, NULL);
	if (err != 0) {
		Application_debug("failed\n");
	}
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
	application_main_event_handler();
}

static void application_main_test()
{
	while (1) {
		send_event(LEDS_EFFECT_STARTUP);
		sleep(1);
		send_event(LEDS_EFFECT_STARTUP);
		sleep(1);
		send_event(LEDS_EFFECT_STARTUP);
		sleep(1);
		send_event(LEDS_EFFECT_STARTUP);
		sleep(1);
		send_event(LEDS_EFFECT_STARTUP);
		sleep(1);
		send_event(LEDS_EFFECT_COMPLETE);
		sleep(50);
	#if 0
		send_event(LEDS_EFFECT_AIRKISS_MODE);
		sleep(1);
		send_event(LEDS_EFFECT_AIRKISS_CONFIG);
		sleep(1);
		send_event(LEDS_EFFECT_AIRKISS_CONNECT);
		sleep(1);
		send_event(LEDS_EFFECT_WAKE_UP);
		sleep(1);
		send_event(LEDS_EFFECT_COMMAND_FAIL);
		sleep(1);
	#endif
		send_event(LEDS_EFFECT_STARTUP);
		sleep(1);
	}
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

	application_main_test();
	application_main_uninit();

	return 0;
}
