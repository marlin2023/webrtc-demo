#include "com_jabber_audio_encoder_AudioWebrtcAecm.h"
#include <android/log.h>
#include "echo_control_mobile.h"

#define TAG "COM_JABBER_ENCODER_AUDIOWEBRTCAECM_"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioWebrtcAecm_yy_1webrtc_1aecm_1initiate
  (JNIEnv *env, jobject obj, jint samp_freq){


	int ret ;
	void* AECM_instance;

	if ((ret = WebRtcAecm_Create(&AECM_instance) )) {
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"WebRtcAecm_Create failed with error code = %d", ret);
		return ret;
	}

	if ((ret = WebRtcAecm_Init(AECM_instance, samp_freq) )) {
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"WebRtcAecm_Init failed with error code = %d", ret);
		return ret;
	}

#if 0	//config
	AecmConfig aecm_config;
	aecm_config.cngMode = AECM_TRUE;
	aecm_config.echoMode = 3;

	if(( ret = WebRtcAecm_set_config(AECM_instance, aecm_config) )){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"WebRtcAecm_set_config failed with error code = %d", ret);
		return ret;
	}
#endif

	return (jint)((int *)AECM_instance );
}

JNIEXPORT jshortArray JNICALL Java_com_jabber_audio_encoder_AudioWebrtcAecm_yy_1webrtc_1aecm_1process
  (JNIEnv *env, jobject obj, jint handle, jshortArray nearendNoisy, jshortArray nearendClean){  //nearendNoisy , nearendClean
//	if( 0!=WebRtcAec_BufferFarend(haec,farend,160) ) printf("bufferfarend error\n");
//	                if( 0!=WebRtcAec_Process(haec,nearend,NULL,output,NULL,160,100,0) ) printf("process error\n");
//
//	                fwrite(output,2,160,foutput);
	//get handle
	void* AECM_instance = (void* )handle;

	//create native shorts from java shorts
	jshort *native_nearend_noisy = (*env)->GetShortArrayElements(env, nearendNoisy, NULL);
	jshort *native_nearend_clean = (*env)->GetShortArrayElements(env, nearendClean, NULL);

	//allocate memory for output data
	jint length = (*env)->GetArrayLength(env, nearendNoisy);
	jshortArray temp = (*env)->NewShortArray(env, length);
	jshort *native_output_frame = (*env)->GetShortArrayElements(env, temp, 0);

//	int32_t WebRtcAecm_BufferFarend(void* aecmInst,
//	                                const int16_t* farend,
//	                                int16_t nrOfSamples);
	if( 0!=WebRtcAecm_BufferFarend(AECM_instance,/*farend*/NULL,160) ){  //farend value??? audio
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"WebRtcAec_BufferFarend failed");
	}

//	int32_t WebRtcAecm_Process(void* aecmInst,
//	                           const int16_t* nearendNoisy,
//	                           const int16_t* nearendClean,
//	                           int16_t* out,
//	                           int16_t nrOfSamples,
//	                           int16_t msInSndCardBuf);
	if( 0!=WebRtcAecm_Process(AECM_instance,nearendNoisy,nearendClean,output_shorts,NULL,160,100,0) ){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"WebRtcAec_Process failed ");
	}



	//convert native output to java layer output
	jshortArray output_shorts = (*env)->NewShortArray(env, length);
	(*env)->SetShortArrayRegion(env, output_shorts, 0, length, native_output_frame);

	//cleanup and return
	(*env)->ReleaseShortArrayElements(env, input_frame, native_input_frame, 0);
	(*env)->ReleaseShortArrayElements(env, temp, native_output_frame, 0);

	return output_shorts;

}

JNIEXPORT jint JNICALL Java_com_jabber_audio_encoder_AudioWebrtcAecm_yy_1webrtc_1aecm_1free
  (JNIEnv *env, jobject obj, jint handle){


}

#ifdef __cplusplus
}
#endif
