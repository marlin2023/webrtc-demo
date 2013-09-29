#include "com_jabber_audio_encoder_AudioEncodeSpeex.h"
#include "speex_encode.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <android/log.h>
#define TAG	"COM_AUDIO_ENCODER_AUDIOENCODESpeex_C"

/*
 * Class:     com_jabber_audio_encoder_AudioEncodeSpeex
 * Method:    yy_speex_encode_init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioEncodeSpeex_yy_1speex_1encode_1init
  (JNIEnv *env, jobject obj){

    int speex_handle = spx_encode_init();
}


JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioEncodeSpeex_yy_1speex_1encode_1frame
  (JNIEnv *env, jobject obj, jint j_handle, jshortArray pcm_data, jobject  speex_data){

    /*  handle */
    int speex_handle = j_handle;

    /*  convert jshortArray to short * */
    int num_samples = (*env)->GetArrayLength(env ,pcm_data);

    jshort *pcm = (*env)->GetShortArrayElements(env ,pcm_data , 0);

    /*	下面这句话 就可以在 JNI中也操作这块内存了	*/
	jbyte *directBuffer = (*env)->GetDirectBufferAddress(env, speex_data);

    spx_encode_frame(speex_handle ,(short *)pcm ,(char * )directBuffer);
    __android_log_print(ANDROID_LOG_ERROR ,TAG ,"after spx_encode_frame ,in encode audio data");

    (*env)->ReleaseShortArrayElements(env ,pcm_data ,pcm ,0);
  }


JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioEncodeSpeex_yy_1speex_1destroy
  (JNIEnv *env, jobject obj, jint j_handle){


  }

#ifdef __cplusplus
}
#endif
