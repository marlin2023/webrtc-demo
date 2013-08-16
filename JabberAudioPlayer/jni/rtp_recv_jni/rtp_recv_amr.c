/*
 * rtp_recv_amr.c
 *
 *  Created on: Jun 6, 2013
 *      Author: chris
 */
#include <ortp/ortp.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>

#include "rtp_recv_amr.h"

#define SPEEX_FRAME_SIZE		38

int yy_init( int port) {

	RtpSession *session;
	ortp_init();
	ortp_scheduler_init();
	ortp_set_log_level_mask(
			ORTP_DEBUG | ORTP_MESSAGE | ORTP_WARNING | ORTP_ERROR);
	session = rtp_session_new(RTP_SESSION_RECVONLY);

	rtp_session_set_scheduling_mode(session, 1);
	rtp_session_set_blocking_mode(session, 1);
	rtp_session_set_local_addr(session, "0.0.0.0", port);
	rtp_session_set_connected_mode(session, TRUE);
	rtp_session_set_symmetric_rtp(session, TRUE);
//	rtp_session_enable_adaptive_jitter_compensation(session,adapt);
//	rtp_session_set_jitter_compensation(session,jittcomp);
	rtp_session_set_payload_type(session, 0);

	return (int) session;
}

extern FILE *outfile;
char * yy_rtp_recv_amr(int handle ,unsigned char * speex_buffer) {

	RtpSession *session = (RtpSession *) handle;
	int have_more = 1;
	static uint32_t user_ts = 0;
//	static unsigned char *buffer = NULL;
//	if (user_ts == 0) {
//		buffer = (unsigned char *)malloc(13);
//	}
	int stream_received = 0;
	__android_log_print(ANDROID_LOG_INFO,"zhangyanlong", "before rtp_session_recv_with_ts");
	printf("before rtp_session_recv_with_ts \n");
	while(1){

		have_more=1;
		while (have_more){
			int err = rtp_session_recv_with_ts(session, speex_buffer /*buffer*/, SPEEX_FRAME_SIZE, user_ts,
								&have_more);
			printf("have_more = %d ,err = %d  \n"  ,have_more ,err);
			if (err >0) stream_received=1;
			/* this is to avoid to write to disk some silence before the first RTP packet is returned*/
			if ((stream_received) && (err>0)) {
//				size_t ret = fwrite(buffer,1,err,outfile);
//				int ii;
//				for(ii =0 ; ii < 32 ; ii ++)
//					printf("%x " ,buffer[ii]);
//				printf("\n");
//				__android_log_print(ANDROID_LOG_INFO, "zhangyanlong",
//						"%x ,%x ,%x ,%x ,%x", buffer[0], buffer[1], buffer[2],
//						buffer[3], buffer[4]);
//				printf("%x ,%x ,%x ,%x ,%x \n", buffer[0], buffer[1], buffer[2],
//						buffer[3], buffer[4]);
//				printf("stream_recv = %d \n" ,err);
				__android_log_print(ANDROID_LOG_INFO,"zhangyanlong", "stream_recv = %d \n" ,err);
				__android_log_print(ANDROID_LOG_INFO, "zhangyanlong",
						"...................return buffer");
				user_ts+=160;
				return (char* )speex_buffer;
			}
			//printf("...\n");
		}
		user_ts+=160;

	}
		//ortp_message("Receiving packet.");
//	return null;
}

int yy_exit(int handle) {

	RtpSession *session = (RtpSession *) handle;
	rtp_session_destroy(session);
	ortp_exit();

	ortp_global_stats_display();
	return 0;
}

