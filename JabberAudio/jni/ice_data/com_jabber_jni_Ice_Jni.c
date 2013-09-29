#include <stdio.h>
#include <string.h>

#include "android/log.h"
#include "com_jabber_jni_Ice_Jni.h"
#include "ice_main.h"
#include "custom_java_cb.h"

#ifdef __cplusplus
extern "C" {
#endif
#define TAG "COM_JABBER_JNI_ICE_C"

//由java调用来建立JNI环境
void setJNIEnv(JNIEnv* env, jclass cls1) {
	//保存全局JVM以便在子线程中使用
	(*env)->GetJavaVM(env, &g_jvm);
	//不能直接赋值(g_obj = obj)
	jclass cls = (*env)->FindClass(env, "com/jabber/audio/encoder/Jabber");
	__android_log_print(ANDROID_LOG_INFO, TAG ,"..after FindClass");
	if (cls == NULL) {
		__android_log_print(ANDROID_LOG_INFO, TAG, ".in init function ,.find class is failed");
		return;
	}else{
		__android_log_print(ANDROID_LOG_INFO, TAG, ".in init function ,.find class success");
	}

	jmethodID mid1 = (*env)->GetMethodID(env, cls, "<init>", "()V");
	if (mid1 == NULL) {
		__android_log_print(ANDROID_LOG_INFO, TAG ,"111.......construct function id is failed");
	}
	jobject obj = (*env)->NewObject(env ,cls ,mid1 ,NULL);
	g_obj = ((*env)->NewGlobalRef(env, obj));

	__android_log_print(ANDROID_LOG_INFO, TAG ,"after..construct function id is success");
}


// convert the binary data  to hexadecimal data
static void data_to_hex(const unsigned char *src, int s, char * dest , int lowercase){
	int i;
	const char hex_table_uc[16] = { '0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'A', 'B',
		'C', 'D', 'E', 'F' };
	const char hex_table_lc[16] = { '0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f' };
	const char *hex_table = lowercase ? hex_table_lc : hex_table_uc;

	for(i = 0; i < s; i++) {
		dest[2*i]   = hex_table[src[i] >> 4];
		dest[2*i+1] = hex_table[src[i] & 0xF];
	}

}
//convert the hexadecimal data to binary data
static void hex_to_data(const char* src, int s, char * dest, int lowercase){
	int i, j;
	unsigned low, high;
	if(s%2 != 0)
		return ;
	const char hex_table_uc[16] = { '0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'A', 'B',
		'C', 'D', 'E', 'F' };
	const char hex_table_lc[16] = { '0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f' };
	const char* hex_table = lowercase ? hex_table_lc : hex_table_uc;
	for(i = 0; i < s; i++){
		for(j = 0; j < 16; j++){
			if(src[i] == hex_table[j]){
				if(!(i%2))
					high = j;
				else{
					low = j;
					//rs.push_back(char(high*16 + low));
					dest[i/2] = (char)(high*16 + low);
				}
				break;
			}
		}
		continue;
	}
}

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
//#include <execinfo.h>
static void sig_handle(	int sig_no){

	if(sig_no == SIGSEGV ){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"SIGSEGV received");
		printf("SIGSEGV received.\n");
//		void * array[25]; /* 25 层，太够了 : )，你也可以自己设定个其他值 */
//
//		int nSize = backtrace(array, sizeof(array) / sizeof(array[0]));
//
//		int i;
//		for (i = nSize - 3; i >= 2; i--) { /* 头尾几个地址不必输出，看官要是好奇，输出来看看就知道了 */
//
//			/* 修正array使其指向正在执行的代码 */
//
//			__android_log_print(ANDROID_LOG_ERROR ,TAG ,"SIGSEGV catched when running code at %x\n", (char*)array[i] - 1);
//
//		}


	}else if(sig_no == SIGFPE){			//Floating-point operation exception
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"SIGFPE received");
		printf("SIGFPE received.\n");

	}else if(sig_no == SIGILL){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"SIGILL received.");
		printf("SIGILL received.\n");

	}else if(sig_no == SIGBUS){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"SIGBUS received.");
		printf("SIGBUS received.\n");

	}else if(sig_no == SIGSYS){
		printf("SIGSYS received.\n");
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"SIGSYS received.");

	}else if(sig_no == SIGABRT){
		printf("SIGABRT received.\n");
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"SIGABRT received.");

	}

}


JNIEXPORT jint JNICALL Java_com_jabber_jni_Ice_1Jni_voice_1call_1init
  (JNIEnv *env, jclass jcs, jstring jsrv_ip, jint jport, jstring jabber_id){

	setJNIEnv(env, jcs);

//	jclass cls = (*env)->FindClass(env, "com/jabber/audio/encoder/Jabber");
//	__android_log_print(ANDROID_LOG_INFO, TAG ,"..after FindClass");
//	if (cls == NULL) {
//		__android_log_print(ANDROID_LOG_INFO, TAG,
//				".in init function ,.find class is failed");
//		return 0;
//	}else{
//		__android_log_print(ANDROID_LOG_INFO, TAG,
//						".in init function ,.find class success");
//		return 0;
//	}

#if 0
    struct sigaction newAct;
    newAct.sa_handler = sig_handle;	//signal handle function
    newAct.sa_flags = SA_RESETHAND;//SA_RESTART;  //这里注意了。

    sigemptyset(&newAct.sa_mask);
    if(-1 == sigaction(SIGSEGV, &newAct, NULL)){
		printf("set signal environment for SIGSEGV exception, error is '%s'", strerror(errno));
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"set signal environment for SIGSEGV exception, error is '%s'", strerror(errno));
		exit(1);
    }
    if(-1 == sigaction(SIGFPE, &newAct, NULL)){
    	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"set signal environment for SIGFPE exception, error is '%s'", strerror(errno));
		printf("set signal environment for SIGFPE exception, error is '%s'", strerror(errno));
		exit(1);
    }
    if(-1 == sigaction(SIGILL, &newAct, NULL)){
    	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"set signal environment for SIGILL exception, error is '%s'", strerror(errno));
		exit(1);
    }
    if(-1 == sigaction(SIGBUS, &newAct, NULL)){
    	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"set signal environment for SIGBUS exception, error is '%s'", strerror(errno));
		exit(1);
    }
    if(-1 == sigaction(SIGSYS, &newAct, NULL)){
    	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"set signal environment for SIGSYS exception, error is '%s'", strerror(errno));
		exit(1);
    }
    if(-1 == sigaction(SIGABRT, &newAct, NULL)){
    	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"set signal environment for SIGABRT exception, error is '%s'", strerror(errno));
		exit(1);
    }
#endif

	const char * srv_ip = (*env)->GetStringUTFChars(env ,jsrv_ip ,0);
	const char * local_jabber_id = (*env)->GetStringUTFChars(env ,jabber_id ,0);
	unsigned short port = jport;

	int handle = voice_call_init(srv_ip ,port ,local_jabber_id);
	if (handle == 0){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"voice_call_init failed ");
		return 0;
	}

	(*env)->ReleaseStringUTFChars(env, jsrv_ip, srv_ip);
	(*env)->ReleaseStringUTFChars(env, jabber_id, local_jabber_id);

	__android_log_print(ANDROID_LOG_INFO ,TAG ,"after voice_call_init  function");
	return (jint)handle;

}
/*
 * in function Java_com_jabber_jni_Ice_1Jni_ice_1get_1info
 * I need  convert the binary data  to hexadecimal data
 * */
JNIEXPORT jstring JNICALL Java_com_jabber_jni_Ice_1Jni_ice_1get_1info
  (JNIEnv *env, jclass jcs, jint jhandle, jobject jsize){

	/*first ,get jsize*/
	jclass c;
	jfieldID id;
	c = (*env)->FindClass(env ,"java/lang/Integer");
	if (c == NULL) {
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"FindClass failed");
		return NULL;
	}

	id = (*env)->GetFieldID(env ,c, "value", "I");
	if (id == NULL) {
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"GetFiledID failed");
		return NULL;
	}

	jint size_tmp = (*env)->GetIntField(env ,jsize, id);

	int handle = jhandle;
	unsigned int ice_info_size = size_tmp ;//jsize;

	__android_log_print(ANDROID_LOG_INFO ,TAG ,"before ice_get_info");
	unsigned char *ice_info = (unsigned char *)ice_get_info(handle ,&ice_info_size);

	//filling string with char *
	char *info_tmp = (char *)malloc(ice_info_size*2 + 1);
	info_tmp[ice_info_size*2] = '\0';
	// convert the binary data  to hexadecimal data
	data_to_hex(ice_info, ice_info_size, info_tmp ,1);
	__android_log_print(ANDROID_LOG_INFO ,TAG ,"ice_get_info ,ice_info_size =%d" ,ice_info_size * 2);
	__android_log_print(ANDROID_LOG_INFO ,TAG ,"ice_get_info ,ice_info:%s" ,info_tmp);

	jstring jstrBuf = (*env)->NewStringUTF(env, info_tmp);   ///?????

	free(info_tmp);
	 (*env)->SetIntField(env ,jsize, id, ice_info_size * 2);
	__android_log_print(ANDROID_LOG_INFO ,TAG ,"after free info_tmp ,after ice_get_info ,ice_info_size =%d" ,ice_info_size * 2);
//	(*env)->DeleteLocalRef( jstr);
	return jstrBuf;
}

/*
 * in function Java_com_jabber_jni_Ice_1Jni_voice_1call_1establish
 * I need convert the hexadecimal data to binary data
 * */
JNIEXPORT jint JNICALL Java_com_jabber_jni_Ice_1Jni_voice_1call_1establish
  (JNIEnv *env, jclass jcs, jint jhandle, jstring jremote_id, jstring jinfo, jint jinfo_size){
	int handle = jhandle;
	if(handle == 0){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"handle invalid ,in voice_call_establish");
		return -1;
	}
	const char * remote_id = (*env)->GetStringUTFChars(env, jremote_id, 0);
	const char * farend_ice_info = (*env)->GetStringUTFChars(env, jinfo, 0);
	//
	unsigned int info_size = jinfo_size ;

	//
	char *ice_info_tmp = (char *)malloc(info_size /2 + 1 );
	ice_info_tmp[info_size /2] = '\0';

	hex_to_data(farend_ice_info, info_size, ice_info_tmp, 1);

	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"farend_ice_info :%s" ,farend_ice_info);
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"before in voice_call_establish ,info_size =%d" ,info_size /2);
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"1............................");
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"2............................");
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"3............................");
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"4............................");
	int ret = voice_call_establish(handle ,remote_id ,ice_info_tmp, info_size /2);
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"1............................");
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"2............................");
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"3............................");
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"4............................");
	if(ret != 0){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"voice_call_establish failed");
		return -1;
	}

	(*env)->ReleaseStringUTFChars(env, jremote_id, remote_id);
	(*env)->ReleaseStringUTFChars(env, jinfo, farend_ice_info);
}

JNIEXPORT jstring JNICALL Java_com_jabber_jni_Ice_1Jni_voice_1call_1destoy
  (JNIEnv *env, jclass jcs, jint jhandle){
	int handle = jhandle;
	if(handle == 0){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"handle invalid ,in voice_call_destoy");
		return NULL;
	}
	voice_call_destoy(handle);
}

JNIEXPORT jint JNICALL Java_com_jabber_jni_Ice_1Jni_ice_1send
  (JNIEnv *env, jclass jcs, jint jhandle, jbyteArray jbuffer, jint jsize){
	int handle = jhandle;
	if(handle == 0){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"handle invalid ,in ice_send_data");
		return -1;
	}


	char *voice_data = (char *)((*env)-> GetByteArrayElements(env ,jbuffer ,NULL));   //????
	unsigned voice_data_size = jsize;
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"before ice_send_data in com_jabber_jni_ice_JNI ,size =%d" ,voice_data_size);
	ice_send_data(handle, voice_data, voice_data_size);

	(*env)->ReleaseByteArrayElements(env ,jbuffer ,voice_data ,0);
//	(*env)->ReleaseShortArrayElements(env ,pcm_data ,pcm ,0);
//	void Release<PrimitiveType>ArrayElements(JNIEnv *env,
//	ArrayType array, NativeType *elems, jint mode);
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"after ice_send_data in com_jabber_jni_ice_JNI");



}

#ifdef __cplusplus
}
#endif

