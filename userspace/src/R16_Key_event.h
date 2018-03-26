/*************************************************************************
 > Filename   : R16_Key_event.h
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-26 16:37:13
 ************************************************************************/

#ifndef __R16_KEY_EVENT_H__
#define __R16_KEY_EVENT_H__

//key node
#define  KEY_EVENT_DEV0_NAME    "/dev/input/event0"
#define  KEY_EVENT_DEV1_NAME    "/dev/input/event1"

//TODO key code: mute vol+ vol-

extern void key_event_listen(void);

#endif /* __R16_KEY_EVENT_H__ *
