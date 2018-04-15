/*************************************************************************
 > Filename   : test_app.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-04-13 10:03:21
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#define DEV_NAME    "/dev/xlj-aw9818"

#define IOCTL
#ifdef IOCTL            //same as app
typedef enum {
	LEDS_EFFECT_NONE = 0,
	LEDS_EFFECT_STARTUP,    /*board power on */
	LEDS_EFFECT_COMPLETE,   /*board startup over */
	LEDS_EFFECT_AIRKISS_MODE,   /*in airkiss mode */
	LEDS_EFFECT_AIRKISS_CONFIG, /*config airkiss */
	LEDS_EFFECT_AIRKISS_CONNECT,    /*airkiss connected */
	LEDS_EFFECT_WAKE_UP,    /*voice wake up */
	LEDS_EFFECT_COMMAND_FAIL,   /*voice command failed */
	LEDS_EFFECT_COMMAND_SUCCESS,    /*voice command success */
	LEDS_EFFECT_KEYMUTE,    /*key mute record */
	LEDS_EFFECT_KEYUNMUTE,  /*key cancel mute record */
	LEDS_EFFECT_BRIGHT_CHANGE,
	LEDS_EFFECT_COLOR_CHANGE,
	LEDS_EFFECT_IMAX_CHANGE,
	LEDS_EFFECT_TOTAL
} LED_EFFECT_ENUM;

#define AW9818_IOC_MAGIC 'm'    //定义类型
#define AW9818_LEDS_EFFECT_NONE	            _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_NONE, int)
#define AW9818_LEDS_EFFECT_STARTUP          _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_STARTUP, int)
#define AW9818_LEDS_EFFECT_COMPLETE         _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_COMPLETE, int)
#define AW9818_LEDS_EFFECT_AIRKISS_MODE     _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_AIRKISS_MODE, int)
#define AW9818_LEDS_EFFECT_AIRKISS_CONFIG   _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_AIRKISS_CONFIG, int)
#define AW9818_LEDS_EFFECT_AIRKISS_CONNECT  _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_AIRKISS_CONNECT, int)
#define AW9818_LEDS_EFFECT_WAKE_UP          _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_WAKE_UP, int)
#define AW9818_LEDS_EFFECT_COMMAND_FAIL     _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_COMMAND_FAIL, int)
#define AW9818_LEDS_EFFECT_COMMAND_SUCCESS  _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_COMMAND_SUCCESS, int)
#define AW9818_LEDS_EFFECT_KEYMUTE          _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_KEYMUTE, int)
#define AW9818_LEDS_EFFECT_KEYUNMUTE        _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_KEYUNMUTE, int)
#endif

int main()
{
	int fd, ret = -1;
	char cmd;
#if 1	
	fd = open(DEV_NAME, O_RDWR);
	if (fd < 0) {
		printf("open failed\n");
		exit(-1);
	}
#endif
	while (1) {
		printf("\n=============================================================================================\n"
				"a			LEDS_EFFECT_NONE = 0, \n"
				"b			LEDS_EFFECT_STARTUP,    /*board power on */ \n"
				"c			LEDS_EFFECT_COMPLETE,   /*board startup over */ \n"
				"d			LEDS_EFFECT_AIRKISS_MODE,   /*in airkiss mode */ \n"
				"e			LEDS_EFFECT_AIRKISS_CONFIG, /*config airkiss */ \n"
				"f			LEDS_EFFECT_AIRKISS_CONNECT,    /*airkiss connected */ \n"
				"g			LEDS_EFFECT_WAKE_UP,    /*voice wake up */ \n"
				"h			LEDS_EFFECT_COMMAND_FAIL,   /*voice command failed */ \n"
				"i			LEDS_EFFECT_COMMAND_SUCCESS,    /*voice command success */ \n"
				"j			LEDS_EFFECT_KEYMUTE,    /*key mute record */ \n"
				"k			LEDS_EFFECT_KEYUNMUTE,  /*key cancel mute record */ \n"
				"l			LEDS_EFFECT_BRIGHT_CHANGE, \n"
				"m			LEDS_EFFECT_COLOR_CHANGE, \n"
				"n			LEDS_EFFECT_IMAX_CHANGE, \n"
				"o			LEDS_EFFECT_TOTAL \n"
				"===============================================================================================\n\n");
		printf("please input test case:");
		cmd = getchar();
		getchar();
		//printf("%c", cmd);
		if (cmd == 'q') {
			printf("quit now\n");
			exit(-1);
		}

		switch (cmd) {
			case 'a':
				ret = ioctl(fd, AW9818_LEDS_EFFECT_NONE, NULL);
				break;
			case 'b':
				ret = ioctl(fd, AW9818_LEDS_EFFECT_STARTUP, NULL);
				break;
			case 'c':
				ret = ioctl(fd, AW9818_LEDS_EFFECT_COMPLETE, NULL);
				break;
			case 'd':
				ret = ioctl(fd, AW9818_LEDS_EFFECT_AIRKISS_MODE, NULL);
				break;
			case 'e':
				ret = ioctl(fd, AW9818_LEDS_EFFECT_AIRKISS_CONFIG, NULL);
				break;
			case 'f':
				ret = ioctl(fd, AW9818_LEDS_EFFECT_AIRKISS_CONNECT, NULL);
				break;
			case 'g':
				printf("input a oreitation:");
				cmd = getchar();
				ret = ioctl(fd, AW9818_LEDS_EFFECT_WAKE_UP, cmd);
				break;
			case 'h':
				ret = ioctl(fd, AW9818_LEDS_EFFECT_COMMAND_FAIL, NULL);
				break;
			case 'i':
				ret = ioctl(fd, AW9818_LEDS_EFFECT_COMMAND_SUCCESS, NULL);
				break;
			case 'j':
				ret = ioctl(fd, AW9818_LEDS_EFFECT_KEYMUTE, NULL);
				break;
			case 'k':
				ret = ioctl(fd, AW9818_LEDS_EFFECT_KEYUNMUTE, NULL);
				break;
			default:
				printf("not support\n");
				break;
		}
	}

	return 0;
}
