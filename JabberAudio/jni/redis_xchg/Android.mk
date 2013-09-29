LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:=xchg.cpp \
				hiredis/async.c	\
				hiredis/dict.c	\
				hiredis/hiredis.c	\
				hiredis/net.c	\
				hiredis/sds.c

LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)
LOCAL_C_INCLUDES += /home/chris/work/webrtc-demo/JabberAudio/jni/redis_xchg/hiredis

LOCAL_MODULE := xchg_mod

LOCAL_SHARED_LIBRARIES := libutils

LOCAL_LDLIBS := -fpic -D_CHRIS -llog  


LOCAL_PRELINK_MODULE := false

LOCAL_PROGUARD_ENABLED := disabled

include $(BUILD_SHARED_LIBRARY)
