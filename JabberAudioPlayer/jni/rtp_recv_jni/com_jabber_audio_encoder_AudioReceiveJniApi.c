/*
 * com_jabber_audio_encoder_AudioReceiveJniApi.c
 *
 *  Created on: Jun 6, 2013
 *      Author: chris
 */

#include "com_jabber_audio_encoder_AudioReceiveJniApi.h"
#include "rtp_recv_amr.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_jabber_audio_encoder_AudioReceiveJniApi
 * Method:    yy_init
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioReceiveJniApi_yy_1init
  (JNIEnv *env, jclass js, jint port){

	int yy_handle = yy_init(port);

	return (jint)yy_handle;
}

/*
 * Class:     com_jabber_audio_encoder_AudioReceiveJniApi
 * Method:    yy_rtp_receive_amr
 * Signature: (I)[B
 */
JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioReceiveJniApi_yy_1rtp_1receive_1speex
  (JNIEnv *env , jclass js, jint j_handle ,jobject speexData){

	int handle = j_handle;
//
//	jbyteArray result = NULL;
//	jclass cls = (*env)->FindClass(env, "[B"); //如果可以这句话可以不要。。
//	result = (*env)->NewByteArray(env, 13);

	/*	下面这句话 就可以在 JNI中也操作这块内存了	*/
	jbyte *directBuffer = (*env)->GetDirectBufferAddress(env, speexData);

	yy_rtp_recv_amr( handle ,(unsigned char * )directBuffer);
//	char *recv_data = yy_rtp_recv_amr( handle);		//jbyte*  -> char *
	//char * yy_rtp_recv_amr(int handle)
//	(*env)->SetByteArrayRegion(env, result, 0, 13, (jbyte*)recv_data);
	//return result;
	return 0;
}

/*
 * Class:     com_jabber_audio_encoder_AudioReceiveJniApi
 * Method:    yy_exit
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioReceiveJniApi_yy_1exit
	(JNIEnv *env, jclass js, jint j_handle){
	int handle = j_handle;
	yy_exit(handle);
	return 0;
}

#ifdef __cplusplus
}
#endif


