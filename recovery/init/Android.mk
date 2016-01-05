LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_STATIC_LIBRARIES := \
    libc \
    libext2_blkid \
    libext2_uuid

LOCAL_C_INCLUDES := \
    bionic/libc/bionic \
    external/e2fsprogs/lib \
    system/core/init

LOCAL_CFLAGS := \
    -Wall \
    -DANDROID_TARGET=\"$(TARGET_BOARD_PLATFORM)\"

LOCAL_SRC_FILES := init_fireball.cpp

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libinit_fireball
include $(BUILD_STATIC_LIBRARY)
