/*************************************************************************
 > Filename   : Application_main_option.h
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-22 10:51:35
 ************************************************************************/

#ifndef __APPLICATION_MAIN_OPTION_H__
#define __APPLICATION_MAIN_OPTION_H__

#include "type.h"
#include <string.h>
#include <stdlib.h>

#ifndef LINUX_PLATFORM
#include "cutils/log.h"
#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "awledcontorl"
#define Application_debug(...)	__android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif
#else
#define APPLICATION_DEBUG
#ifdef APPLICATION_DEBUG
#define Application_debug(...)	printf(__VA_ARGS__)
#else
#define Application_debug(...)
#endif
#endif

#ifndef Application_malloc
#define Application_malloc(a)	malloc(a)
#endif

#ifndef Application_free
#define Application_free(a)	\
	do {	\
		free(a);	\
	}while( 0 )
#endif

/* UINT configuration */
#ifndef UINT
typedef unsigned int UINT;
#endif

/* INT configuration */
#ifndef INT
typedef int INT;
#endif

/* uint16_t configuration */
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#endif  /* __APPLICATION_MAIN_OPTION_H__ */
