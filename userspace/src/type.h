/*************************************************************************
 > Filename   : type.h
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-22 10:53:47
 ************************************************************************/

#ifndef __TYPE_H__
#define __TYPE_H__

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

#ifndef BYTE
typedef unsigned char  BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef DWORD
typedef unsigned long  DWORD;
#endif

#ifndef BOOL
typedef unsigned int   BOOL;
#endif

#ifndef int8
typedef signed char	int8;
#endif

#ifndef int16
typedef signed short	int16;
#endif

#ifndef int32
typedef signed int		int32;
#endif

#ifndef uint8
typedef unsigned char 	uint8;
#endif

#ifndef uint16
typedef unsigned short 	uint16;
#endif

#ifndef uint32
typedef unsigned int  	uint32;
#endif

#ifndef u8
typedef unsigned char       u8;
#endif

//#ifndef uc32
//typedef unsigned long  const    uc32;  /* Read Only */
//#endif

#ifndef uc16
typedef unsigned short const    uc16;  /* Read Only */
#endif

#ifndef uc8
typedef unsigned char  const    uc8;   /* Read Only */
#endif

//#ifndef uc32
//typedef volatile unsigned long      vu32;
//#endif

#ifndef vu16
typedef volatile unsigned short     vu16;
#endif

#ifndef vu8
typedef volatile unsigned char      vu8;
#endif

#endif  /* __TYPE_H__ */
