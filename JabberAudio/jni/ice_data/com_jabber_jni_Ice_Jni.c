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
void setJNIEnv(JNIEnv* env, jobject obj) {
	//保存全局JVM以便在子线程中使用
	(*env)->GetJavaVM(env, &g_jvm);
	//不能直接赋值(g_obj = obj)
	g_obj = (*env)->NewGlobalRef(env, obj);
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

JNIEXPORT jint JNICALL Java_com_jabber_jni_Ice_1Jni_voice_1call_1init
  (JNIEnv *env, jclass jcs, jstring jsrv_ip, jint jport, jstring jabber_id){

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
	char *info_tmp = (char *)malloc(ice_info_size*2);
	// convert the binary data  to hexadecimal data
	data_to_hex(ice_info, ice_info_size, info_tmp ,1);

	jstring jstrBuf = (*env)->NewStringUTF(env, info_tmp);

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
	char *ice_info_tmp = (char *)malloc(info_size /2 );
	hex_to_data(farend_ice_info, info_size, ice_info_tmp, 1);

	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"before in voice_call_establish ,info_size =%d" ,info_size /2);
	int ret = voice_call_establish(handle ,remote_id ,ice_info_tmp, info_size /2);
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

	char *voice_data = (char *)((*env)-> GetByteArrayElements(env ,jbuffer ,NULL));
	unsigned voice_data_size = jsize;
	ice_send_data(handle, voice_data, voice_data_size);

}

#ifdef __cplusplus
}
#endif

