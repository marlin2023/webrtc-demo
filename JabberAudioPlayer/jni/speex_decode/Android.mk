LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:=com_jabber_audio_encoder_AudioDecodeSpeex.c \
				speex_decode.c

LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)
LOCAL_C_INCLUDES += /home/chris/work/ffmpeg/source/speex-arm/include

LOCAL_MODULE := speex_decoder

LOCAL_SHARED_LIBRARIES := libutils

LOCAL_LDLIBS := -fpic -D_CHRIS -llog  -L/home/chris/work/ffmpeg/source/speex-arm/lib -lspeex


LOCAL_PRELINK_MODULE := false

LOCAL_PROGUARD_ENABLED := disabled

include $(BUILD_SHARED_LIBRARY)
