/*
 * ice_main.h
 *
 *  Created on: Sep 2, 2013
 *      Author: chris
 */

#ifndef ICE_MAIN_H_
#define ICE_MAIN_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int uint_t;

#define RTP_CHAN_ID_ONLY			1
#define RTCP_CHAN_ID_WITH			2
/*
 * voice call start
 * srvIP[in] 		:STUN and TURN server IP
 * port [in]		:STUN and TURN server port
 * localId[in] 		:the jabber ID for local XMPP client
 *
 * return value :return handle
 * */
int voice_call_init(const char* srvIP, unsigned short port ,const char* localId);




/*
 * get local ice info
 *
 *	handle[in] 			:the return value of the voice_call_init
 *	size[in|out]		:the size of my local ice info in bytes
 *
 *	return value 		:the pointer point the local ice information
 * */
void* ice_get_info(int handle, uint_t* size);



/*
 * ice connect establish
 *
 * handle [in]			:the return value of the voice_call_init
 * remoteId[in]			:the jabber ID for remoteId XMPP client
 * info[in]				:standard Remote local ice info[return by the function ice_get_value]
 * infoSz[in]			:the size the Remote ice information
 *
 * return value 		:success return 0;
 * */
int voice_call_establish(int handle ,const char* remoteId ,void* info, uint_t infoSz);



/*
 *voice call stop
 *
 * handle [in]			:the return value of the voice_call_init
 * */
void voice_call_destoy(int handle);



/*
 *transport data
 * handle [in]			:the return value of the voice_call_init
 * buffer [in]			:the data you want to send
 * size [in]			:the size of the data
 *
 * return value:data that have already sent.
 * */
int ice_send_data(int handle, void* buffer, uint_t size);

#ifdef __cplusplus
}
#endif
#endif
