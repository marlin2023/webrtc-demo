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

	//__android_log_print(ANDROID_LOG_INFO, TAG ,"..in audio_data_play function ");
	//Attach主线程
	if ((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK) {	//env
		__android_log_print(ANDROID_LOG_INFO, TAG ,"AttachCurrentThread() failed");
		return -1;
	}

	cls =  (*env)->GetObjectClass(env, g_obj);
	if (cls == NULL) {
		__android_log_print(ANDROID_LOG_INFO, TAG ,".111......find class is failed");
		return -1;
	}
	jmethodID mid1 = (*env)->GetMethodID(env, cls, "audioPlay", "(I[S)V"); //public void onPrepare();  ////再找类中的方法
	if (mid1 == NULL) {
		__android_log_print(ANDROID_LOG_INFO, TAG ,".......get GetStaticMethodID .......id is failed");
		return -1;
	}


	jshortArray retval = (*env)->NewShortArray(env ,audio_size);  		//????
	(*env)->SetShortArrayRegion(env ,retval,0,audio_size,audio_data);
	(*env)->CallVoidMethod(env, g_obj, mid1 ,audio_size ,retval);			///use g_obj ,but not cls ,because funtion audioPlay is not static function

	// __android_log_print(ANDROID_LOG_INFO, TAG ,"0 ,audio_size =%d ,audio_data = %p" ,audio_size ,audio_data);
	 (*env)->DeleteLocalRef(env ,retval);
	 (*env)->DeleteLocalRef(env ,cls);

	//Detach主线程
	 if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK)
	 {
		 __android_log_print(ANDROID_LOG_INFO, TAG ,"DetachCurrentThread() failed");
		 return -1;
	 }



	return 0;
}
