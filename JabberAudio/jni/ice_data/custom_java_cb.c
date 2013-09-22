#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include "custom_java_cb.h"

#define TAG "CUSTOM_JAVA_CB_C"
int audio_data_play(jobject obj ,int audio_size ,short * audio_data){
	//**** call java function onPrepare , to mean ready ok//
	JNIEnv *env;
	jclass cls;
	jmethodID mid;

	//Attach主线程
	if ((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK) {	//env
		//LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
		return -1;
	}

	cls =  (*env)->GetObjectClass(env, obj);
	if (cls == NULL) {
		__android_log_print(ANDROID_LOG_INFO, TAG ,".......find class is failed");
		return -1;
	}

	mid = (*env)->GetMethodID(env, cls, "audioPlay", "(I[S)V"); //public void onPrepare();
	if (mid == NULL) {
		__android_log_print(ANDROID_LOG_INFO, TAG ,".......get method .......id is failed");
		return -1;
	}

//	(*env)->CallVoidMethod(env, obj, mid);   //the second parameter value?
	jshortArray retval = (*env)->NewShortArray(env ,audio_size);
	(*env)->SetShortArrayRegion(env ,retval,0,audio_size,audio_data);
	(*env)->CallVoidMethod(env, obj, mid ,audio_size ,retval);
	 //Detach主线程
	 if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK)
	 {
		 __android_log_print(ANDROID_LOG_INFO, TAG ,"DetachCurrentThread() failed");
//		 LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
	 }
	return 0;
}
