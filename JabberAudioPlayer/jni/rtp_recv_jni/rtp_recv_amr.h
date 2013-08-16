/*
 * rtp_recv_amr.h
 *
 *  Created on: Jun 6, 2013
 *      Author: chris
 */

#ifndef RTP_RECV_AMR_H_
#define RTP_RECV_AMR_H_


/*
 * init function
 * */
int yy_init(int port);

/*
 *  recv data buf
 *
 * */
char * yy_rtp_recv_amr(int handle ,unsigned char * speex_buffer);
//JNIEXPORT jbyteArray JNICALL Java_com_jabber_audio_encoder_AudioReceiveJniApi_yy_1rtp_1receive_1amr

/*
 * yy_exit
 * */
int yy_exit(int handle);

#endif /* RTP_RECV_AMR_H_ */
