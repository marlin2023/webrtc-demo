LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
        aecm_core.c \
        complex_bit_reverse.c \
        complex_fft.c	\
        delay_estimator_wrapper.c	\
        delay_estimator.c	\
        division_operations.c	\
        echo_control_mobile.c	\
        min_max_operations.c	\
        randomization_functions.c	\
        ring_buffer.c	\
        spl_sqrt_floor.c \
        spl_init.c	\
        real_fft.c	\
        cross_correlation.c	\
        downsample_fast.c	\
        vector_scaling_operations.c
        
LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)
LOCAL_C_INCLUDES +=/home/chris/work/webrtc/source/voe/3.31
LOCAL_C_INCLUDES +=/home/chris/work/webrtc/source/voe/3.31/webrtc 
 
LOCAL_CFLAGS := -DWEBRTC_ANDROID

LOCAL_MODULE    := webrtc_aecm
LOCAL_PRELINK_MODULE := false
LOCAL_PROGUARD_ENABLED := disabled
include $(BUILD_SHARED_LIBRARY)
