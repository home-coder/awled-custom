#ifndef __APPLICATION_MAIN_OPTION_H__
#define __APPLICATION_MAIN_OPTION_H__

#include "type.h"
#include <string.h>
#include <stdlib.h>

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
