LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libsdl
LOCAL_SRC_FILES := libsdl.so
include $(PREBUILT_SHARED_LIBRARY)
