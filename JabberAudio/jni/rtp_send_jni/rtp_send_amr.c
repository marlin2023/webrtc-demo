/*
 * rtp_send_amr.c
 *
 *  Created on: May 30, 2013
 *      Author: chris
 */

#include "rtp_send_amr.h"

#include <ortp/ortp.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>

int yy_init(char *addr, int port) {

	RtpSession *session;
	ortp_init();
	ortp_scheduler_init();
	ortp_set_log_level_mask(ORTP_MESSAGE | ORTP_WARNING | ORTP_ERROR);
	session = rtp_session_new(RTP_SESSION_SENDONLY);

	rtp_session_set_scheduling_mode(session, 1);
	rtp_session_set_blocking_mode(session, 1);
	rtp_session_set_connected_mode(session, TRUE);
	rtp_session_set_remote_addr(session, addr, port);
	rtp_session_set_payload_type(session, 97); //这里设置为0，实际表示payload type为PCMU，ortp里面这个设置不光设置数字就行了，还需要avprofile中定义的payload结构，负责会报无法识别的type

#if 0
	char *ssrc;
	ssrc = getenv("SSRC");
	if (ssrc != NULL) {
		printf("using SSRC=%i.\n", atoi(ssrc));
		rtp_session_set_ssrc(session, atoi(ssrc));
	}
#endif
	return (int)session;
}

int yy_rtp_send_amr(int handle,unsigned char *buffer, int len) {

	static uint32_t user_ts = 0;
	RtpSession *session = (RtpSession *) handle;
	rtp_session_send_with_ts(session, buffer, len, user_ts);
	user_ts += 160;

	return 0;
}

int yy_exit(int handle) {

	RtpSession *session = (RtpSession *) handle;
	rtp_session_destroy(session);
	ortp_exit();
	ortp_global_stats_display();

	return 0;

}

