LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= prop.c

LOCAL_MODULE:= prop


LOCAL_STATIC_LIBRARIES := libc
LOCAL_SHARED_LIBRARIES:= libcutils libutils

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
LOCAL_MODULE_TAGS := debug

include $(BUILD_EXECUTABLE)
