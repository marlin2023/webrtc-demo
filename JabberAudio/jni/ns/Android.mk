LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
        noise_suppression.c \
        ns_core.c \
        fft4g.c \
        com_jabber_audio_encoder_AudioWebrtcNs.c
LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)
LOCAL_C_INCLUDES +=/home/chris/work/webrtc/source/voe/3.31/webrtc
LOCAL_C_INCLUDES +=/home/chris/work/webrtc/source/voe/3.31

LOCAL_LDLIBS := -llog
LOCAL_MODULE    := webrtc_ns
LOCAL_PRELINK_MODULE := false
LOCAL_PROGUARD_ENABLED := disabled
include $(BUILD_SHARED_LIBRARY)

#LOCAL_PATH := $(call my-dir)
#include $(CLEAR_VARS)
#LOCAL_SRC_FILES:=com_chris_yanlong_XPlayer.c	\
#LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)
#LOCAL_MODULE := libxplayer
#LOCAL_SHARED_LIBRARIES := libutils
#LOCAL_LDLIBS := -fpic  -g  -llog -lz -L/home/chris/work/ffmpeg/refs/ffmpeg-armv7/lib  -lavformat -lavcodec -lswscale -lswresample -lavutil 
#LOCAL_PRELINK_MODULE := false
#LOCAL_PROGUARD_ENABLED := disabled
#include $(BUILD_SHARED_LIBRARY)