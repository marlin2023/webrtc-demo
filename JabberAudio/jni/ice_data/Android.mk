LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:=com_jabber_jni_Ice_Jni.c \
				ice_main.cpp \
				speex_decode.c	\
				custom_java_cb.c

LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)
LOCAL_C_INCLUDES += /home/chris/work/ffmpeg/source/speex-arm/include

LOCAL_MODULE := ice_data_transport

LOCAL_SHARED_LIBRARIES := libutils

LOCAL_LDLIBS := -fpic -D_CHRIS -Wl,-Map=test.map -g -llog  -L/home/chris/work/ffmpeg/source/speex-arm/lib  -lspeex  -Xlinker "-(" -lprotobuf  -lice-helper  -lpjnath -lpjlib-util -lpj -Xlinker "-)"  
LOCAL_LDLIBS += -L/home/chris/work/android/android-ndk-r8d/sources/cxx-stl/gnu-libstdc++/4.4.3/libs/armeabi-v7a  -lgnustl_static

LOCAL_PRELINK_MODULE := false

LOCAL_PROGUARD_ENABLED := disabled

include $(BUILD_SHARED_LIBRARY)
