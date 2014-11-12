# Copyright 2006 The Android Open Source Project
LOCAL_PATH:= $(call my-dir)

LOCAL_SRC_FILES := motion_algorithm.c

LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libc 
LOCAL_MODULE := libmotion

include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_SRC_FILES := main.c

#LOCAL_C_INCLUDES := $(KERNEL_HEADERS)
LOCAL_SHARED_LIBRARIES := libc libcutils libnetutils libmotion
LOCAL_MODULE = motion_algo
include $(BUILD_EXECUTABLE)
