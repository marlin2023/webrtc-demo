/*
 *
 * */
#include "com_jabber_audio_encoder_AudioTransJniApi.h"
#include "rtp_send_amr.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_jabber_audio_encoder_AudioTransJniApi
 * Method:    yy_init
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioTransJniApi_yy_1init
  (JNIEnv *env, jclass js, jstring addr, jint port){

	char *dest_addr = (char*)(*env)->GetStringUTFChars(env, addr, NULL);  //get destination address
	int dest_port = port;

	int yy_handle = yy_init(dest_addr, dest_port);

	//end
	(*env)->ReleaseStringUTFChars(env, addr, dest_addr);
	return (jint)yy_handle;
}

/*
 * Class:     com_jabber_audio_encoder_AudioTransJniApi
 * Method:    yy_rtp_send_amr
 * Signature: (ILjava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioTransJniApi_yy_1rtp_1send_1amr
(JNIEnv *env, jclass js, jint j_handle, jbyteArray j_byte, jint j_len){

	int handle = j_handle;
	//http://en.wikipedia.org/wiki/Java_Native_Interface
	//convert jbyteArray to char*
	jint len = (*env)->GetArrayLength(env ,j_byte);

	char* amr_buf = (char*)malloc(len);
	(*env)->GetByteArrayRegion (env ,j_byte, (jint)0, (jint)len, (jbyte*)amr_buf);

	int amr_len	= j_len;
	yy_rtp_send_amr(handle ,(unsigned char *)amr_buf ,amr_len);

	return 0;
}

/*
 * Class:     com_jabber_audio_encoder_AudioTransJniApi
 * Method:    yy_exit
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioTransJniApi_yy_1exit
  (JNIEnv *env, jclass js, jint j_handle){

		int handle = j_handle;
		yy_exit(handle);
		return 0;
}

#ifdef __cplusplus
}
#endif
