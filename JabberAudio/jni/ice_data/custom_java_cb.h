#ifndef __CUSTOME_JAVA_CB_H
#define __CUSTOME_JAVA_CB_H


#include <jni.h>

//global variable
JavaVM *g_jvm ;
jobject g_obj;


int audio_data_play(jobject obj ,int audio_size ,short * audio_data);

#endif
