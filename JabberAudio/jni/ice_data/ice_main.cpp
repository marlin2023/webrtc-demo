/*
 * ice.c
 *
 *  Created on: Sep 2, 2013
 *      Author: chris
 */
//#ifdef __cplusplus
//extern "C" {  // 即使这是一个C++程序，下列这个函数的实现也要以C约定的风格来搞！
//#endif
#include <stdio.h>
#include <android/log.h>
#include <time.h>

#include "ice.h"
#include "ice_main.h"

extern "C"
{
#include "speex_decode.h"
#include "custom_java_cb.h"
}
#define TAG  "ICE_MAIN_CPP"
/*
 * LogCallback function: use to manager log printed by ice library
 * @param buffer: log information
 *
 * */
static void log_cb(const char *buffer){
	//here ,I use android log print to display
	__android_log_print(ANDROID_LOG_INFO, "ICE", buffer);
}

/*
 * RxCallback function
 * @param userData: ?
 * @param buffer: 	point the receive data
 * @param size: 	receive data  size
 *
 * */
FILE *fp_speex_recv;

static void recv_data_cb(IceEndpoint* endp, uint_t chnId, void* userData, void* buffer, size_t size){
	struct timeval tv;
	double time_start;
	double time_end;
	double difftime;
	gettimeofday(&tv ,NULL);
	time_start =tv.tv_sec * 1000000 + tv.tv_usec;

	//__android_log_print(ANDROID_LOG_ERROR ,TAG ,"in recv_data_cb function ,userData = %p" ,userData);
	if(size != 50){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"in recv_data_cb function ,size = %d" ,size);
	}
	//1.receive audio rtp packet (buffer)
	int speex_decode_handle = *((int *)userData);

	//2.unpack rtp packet
	char *speex_audio_data = (char*)buffer ;
	speex_audio_data += 12; //rtp packet header size is 12 bytes

	short decoded_audio[160]={0};
	//3.decode speex data
	//write speex encode data
//
	fwrite(speex_audio_data ,1 ,38 ,fp_speex_recv);
////	//__android_log_print(ANDROID_LOG_ERROR ,TAG ,"before spx_decode_frame  ,buffer data address =%p ,speex_decode_handle =%d" ,buffer ,speex_decode_handle);
	spx_decode_frame(speex_decode_handle,(char *)speex_audio_data ,(short *)decoded_audio);
	//4.play pcm data(call back java interface)
	//__android_log_print(ANDROID_LOG_ERROR ,TAG ,"after spx_decode_frame ");
	audio_data_play(g_obj ,160,decoded_audio);
//	//__android_log_print(ANDROID_LOG_ERROR ,TAG ,"after audio_data_play ");

	gettimeofday(&tv ,NULL);
	time_end =tv.tv_sec * 1000000 + tv.tv_usec;  //单位是 微妙
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"## recv data size =%d ,recv diff time :%.5f \n" , size ,(double)(time_end - time_start)/1000000);
}

int voice_call_init(const char* srvIP, unsigned short port ,const char* localId){

	fp_speex_recv = fopen("/sdcard/speex_recv.spx" ,"w+");
	if(fp_speex_recv == NULL){
		__android_log_print(ANDROID_LOG_ERROR ,"ICE_MAIN" ,"open sdcard/speex.spx file failed");
	}

	//call ice_init
	//int ice_init(const char* srvIP, ushort_t port, LogCallback logCall);
	ice_init(srvIP, port, log_cb ,5);
//	ice_init(srvIP, port, log_cb ,3);  //set ice log level

	//localId :the jabber ID for local XMPP client
	__android_log_print(ANDROID_LOG_ERROR ,"ICE_MAIN" ,"before spx_decode_init function ");

	int *speex_decode_handle = (int *)malloc(1 * sizeof(int));
	if(speex_decode_handle == NULL){
		__android_log_print(ANDROID_LOG_ERROR ,"ICE_MAIN" ,"speex_decode malloc failed" );
		return 0;
	}

	//speex_decode_handle
	*speex_decode_handle = spx_decode_init();
	__android_log_print(ANDROID_LOG_ERROR ,"ICE_MAIN" ,"after spx_decode_init function  ,*speex_decode_handle = %d" ,*speex_decode_handle);
	if(*speex_decode_handle == 0){
		__android_log_print(ANDROID_LOG_ERROR ,"ICE_MAIN" ,"speex_decode_handle init failed");
		return 0;
	}
		//IceEndpoint* ice_endpoint(const char* localId, void* userData, uint_t chnCnt, RxCallback func);
	__android_log_print(ANDROID_LOG_ERROR ,"ICE_MAIN" ,"before ice_endpoint function ,*speex_decode_handle =%d ,address =%p" ,*speex_decode_handle ,speex_decode_handle);
	IceEndpoint* ice_handle = ice_endpoint(localId, speex_decode_handle, RTP_CHAN_ID_ONLY, recv_data_cb); //userData :private data for RxCallback

	if(ice_handle == NULL){
		__android_log_print(ANDROID_LOG_ERROR ,"ICE_MAIN" ,"ice_endpoint failed ,localId =%s" ,localId);
		return 0;
	}
	//return handle
	return (int)ice_handle;
}
//
void* ice_get_info(int handle, uint_t* size){

	IceEndpoint* ice_handle = (IceEndpoint* )handle;

	return ice_myInfo(ice_handle, size);
}

int voice_call_establish(int handle ,const char* remoteId ,void* info, uint_t infoSz){
	IceEndpoint* ice_handle = (IceEndpoint* )handle;

//	__android_log_print(ANDROID_LOG_ERROR ,"ICE_MAIN" ,"" ,localId);
	ice_establish(ice_handle, remoteId, info, infoSz);
}

void voice_call_destoy(int handle){
	IceEndpoint* ice_handle = (IceEndpoint* )handle;
	ice_destroy(ice_handle);
}

int ice_send_data(int handle, void* buffer, uint_t size){
	IceEndpoint* ice_handle = (IceEndpoint* )handle;
	ice_send(ice_handle, RTP_CHAN_ID_ONLY,  buffer,  size);
}
