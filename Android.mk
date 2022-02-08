# Copyright 2008 The Android Open Source Project
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libUartTest
LOCAL_SRC_FILES := UartTest.c
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(call all-subdir-java-files)
LOCAL_MODULE := UartTest
include $(BUILD_JAVA_LIBRARY)
