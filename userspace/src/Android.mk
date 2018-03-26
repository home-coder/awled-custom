LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= Application_LED.c \
				  Application_LED_customize.c \
				  Application_LED_effect.c \
				  Application_LED_interface.c \
				  Application_main.c \
				  Chip_AW981x.c \
				  R16_I2c_option.c

LOCAL_MODULE := libawledcontrol
LOCAL_SHARED_LIBRARIES:= libcutils libutils
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
#
#Application_LED.c
#Application_LED_customize.c
#Application_LED_customize.h
#Application_LED_effect.c
#Application_LED_effect.h
#Application_LED.h
#Application_LED_interface.c
#Application_LED_interface.h
#Application_main.c
#Application_main_option.h
#Chip_AW981x.c
#Chip_AW981x.h
#R16_I2c_option.c
#R16_I2c_option.h
#type.h
