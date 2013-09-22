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

#include "ice.h"
#include "ice_main.h"

extern "C"
{
#include "speex_decode.h"
#include "custom_java_cb.h"
}
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
static void recv_data_cb(IceEndpoint* endp, uint_t chnId, void* userData, void* buffer, size_t size){
	//1.receive audio rtp packet (buffer)
	int speex_decode_handle = *((int *)userData);

	//2.unpack rtp packet
	char *speex_audio_data = (char*)buffer ;
	speex_audio_data += 12; //rtp packet header size is 12 bytes

	short decoded_audio[160]={0};
	//3.decode speex data
	spx_decode_frame(speex_decode_handle,(char *)speex_audio_data ,(short *)decoded_audio);
	//4.play pcm data(call back java interface)
	audio_data_play(g_obj ,160,decoded_audio);
}

int voice_call_init(const char* srvIP, unsigned short port ,const char* localId){

	//call ice_init
	//int ice_init(const char* srvIP, ushort_t port, LogCallback logCall);
	ice_init(srvIP, port, log_cb ,5);

	//localId :the jabber ID for local XMPP client
	//speex_decode_handle
	int speex_decode_handle = spx_decode_init();
		//IceEndpoint* ice_endpoint(const char* localId, void* userData, uint_t chnCnt, RxCallback func);
	IceEndpoint* ice_handle = ice_endpoint(localId, &speex_decode_handle, RTP_CHAN_ID_ONLY, recv_data_cb); //userData :private data for RxCallback

	//return handle
}
//
void* ice_get_info(int handle, uint_t* size){

	IceEndpoint* ice_handle = (IceEndpoint* )handle;

	return ice_myInfo(ice_handle, size);
}

int voice_call_establish(int handle ,const char* remoteId ,void* info, uint_t infoSz){
	IceEndpoint* ice_handle = (IceEndpoint* )handle;
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
