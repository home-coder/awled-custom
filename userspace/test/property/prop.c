/*************************************************************************
 > Filename   : prop.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-14 15:42:25
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cutils/properties.h>
#include <pthread.h>

#define TEST	1

typedef enum{
	LEDS_EFFECT_NONE,
	LEDS_EFFECT_POWERON, //TODO
	LEDS_EFFECT_INCREASE,
	LEDS_EFFECT_DECREASE,
	LEDS_EFFECT_BRIGHT_CHANGE,
	LEDS_EFFECT_COLOR_CHANGE,
	LEDS_EFFECT_IMAX_CHANGE,
	LEDS_EFFECT_CIRCLE,
	LEDS_EFFECT_BREATH,
	LEDS_EFFECT_COLOR_BREATH,
	LEDS_EFFECT_WAKEUP, //TODO
	LEDS_EFFECT_BRODCAST_COMPLETE, //TODO
	LEDS_EFFECT_TOTAL
}APP_LED_EFFECT_ENUM;

#define msleep(x) usleep(x*1000)
pthread_cond_t handler_start = PTHREAD_COND_INITIALIZER;
pthread_mutex_t state_lock = PTHREAD_MUTEX_INITIALIZER;

static char cur_state;

typedef struct lineStack {
	char cmd_type;
	struct lineStack *next;
} lineStack;

//event stack
lineStack *stack = NULL;

static void push_cmd(char a)
{
	lineStack *line = (lineStack *) malloc(sizeof(lineStack));
	line->cmd_type = a;
	line->next = stack;
	stack = line;
}

static char pop_cmd()
{
	char tcmd = LEDS_EFFECT_NONE;
	if (stack) {
		lineStack *p = stack;
		stack = stack->next;
		printf("弹栈元素：%c \n", p->cmd_type);
		tcmd = p->cmd_type;
		free(p);
	} else {
		printf("栈内没有元素\n");
	}
	return tcmd;
}

//init.rc set property
static void listen_env_init()
{
	//first prop
	char tmp[PROPERTY_VALUE_MAX], vtmp[PROPERTY_VALUE_MAX];
	sprintf(tmp, "%d", 0);
	property_set("sys.effect.change", tmp);

	sprintf(vtmp, "%d", 0x4);
	property_set("sys.effect.value", vtmp);
	cur_state = 0;
}

#if TEST
void application_handle_cmd(char cmd)
{
	sleep(3);
	printf("handle cmd %u\n", cmd);
}
#endif

static void effect_handle(void *param)
{
	char cmd = LEDS_EFFECT_NONE;
	pthread_mutex_lock(&state_lock);
	for (;;) {
		pthread_cond_wait(&handler_start, &state_lock);
		//pop stack
		cmd = pop_cmd();
		if (cmd != LEDS_EFFECT_NONE) {
			application_handle_cmd(cmd);
		}
	}
	pthread_mutex_unlock(&state_lock);
}

static void effect_handler_thread_init()
{
	pthread_t pidl;
	pthread_create(&pidl, NULL, (void *)effect_handle, NULL);
}

static void main_listen_effect_loop()
{
	int ret = -1;
	char tmp[PROPERTY_VALUE_MAX];
	char vtmp[PROPERTY_VALUE_MAX];

	while (1) {
		ret = property_get("sys.effect.change", tmp, NULL);
		if (ret && atoi(tmp) > cur_state) {
			property_get("sys.effect.value", vtmp, NULL);
			printf("tmp = %s, vtmp = %s\n", tmp, vtmp);
			cur_state = atoi(tmp);
			//push stack
			pthread_mutex_lock(&state_lock);
			push_cmd(cur_state);
			pthread_mutex_unlock(&state_lock);
			pthread_cond_signal(&handler_start);
		}
		msleep(200);
	}
}

int main()
{
	listen_env_init();

	effect_handler_thread_init();

	main_listen_effect_loop();

	return 0;
}
