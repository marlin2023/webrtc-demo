/*
 * com_jabber_audio_encoder_AudioReceiveJniApi.c
 *
 *  Created on: Jun 6, 2013
 *      Author: chris
 */

#include "com_jabber_audio_encoder_AudioDecodeSpeex.h"
#include "speex_decode.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_jabber_audio_encoder_AudioDecodeSpeex
 * Method:    spx_encode_init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioDecodeSpeex_spx_1decode_1init
  (JNIEnv *env, jobject obj){

	int yy_handle = spx_encode_init();

	return (jint)yy_handle;
}

/*
 * Class:     com_jabber_audio_encoder_AudioDecodeSpeex
 * Method:    spx_encode_frame
 * Signature: (I[B[S)I
 */
JNIEXPORT jshortArray JNICALL Java_com_jabber_audio_encoder_AudioDecodeSpeex_spx_1decode_1frame
  (JNIEnv *env , jobject obj, jint j_handle, jbyteArray speex_data){

	/*  handle */
	int speex_handle = j_handle;
   /*  convert jbyteArray to byte * */
	int sppex_data_num = (*env)->GetArrayLength(env ,speex_data);
	jbyte *encoded_data = (*env)->GetByteArrayElements(env ,speex_data , 0);

	//short buffer
	jshortArray result = (*env)->NewShortArray(env ,160);
	jshort tmp[160];
	spx_encode_frame(speex_handle,(char *)encoded_data ,(short *)tmp);

//	int spx_encode_frame(int handle ,char *src_speex_data ,short *target_pcm_data_buffer)
	(*env)->SetShortArrayRegion(env ,result,0,160,tmp);
	(*env)->ReleaseByteArrayElements(env ,speex_data ,encoded_data ,0);

	return result;
}

/*
 * Class:     com_jabber_audio_encoder_AudioDecodeSpeex
 * Method:    spx_destroy
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioDecodeSpeex_spx_1destroy
  (JNIEnv *env, jobject obj, jint j_handle){

}


#ifdef __cplusplus
}
#endif


