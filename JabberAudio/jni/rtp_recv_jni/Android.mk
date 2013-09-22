LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:=com_jabber_audio_encoder_AudioReceiveJniApi.c \
				rtp_recv_amr.c

LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)
LOCAL_C_INCLUDES += /home/chris/work/rtp-rtcp/ortp-arm/include

LOCAL_MODULE := rtp_recv_speex

LOCAL_SHARED_LIBRARIES := libutils

LOCAL_LDLIBS := -fpic -D_CHRIS -llog  -L/home/chris/work/rtp-rtcp/ortp-arm/lib -lortp


LOCAL_PRELINK_MODULE := false

LOCAL_PROGUARD_ENABLED := disabled

include $(BUILD_SHARED_LIBRARY)
