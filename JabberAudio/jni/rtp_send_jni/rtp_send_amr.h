/*
 * rtp_send_amr.h
 *
 *  Created on: May 30, 2013
 *      Author: chris
 */

#ifndef RTP_SEND_AMR_H_
#define RTP_SEND_AMR_H_


/*
 * init function
 * */
int yy_init(char *addr ,int port);

/*
 *  send data buf
 *  @handle ,the yy_init return value
 *  @buffer ,you want to send  data pointer
 *  @len    ,the size of you send data
 *
 * */
//int yy_rtp_send_amr(int handle ,char *buffer ,int len);
int yy_rtp_send_amr(int handle ,unsigned char *buffer ,int len);


/*
 * yy_exit
 * */
int yy_exit(int handle);
#endif /* RTP_SEND_AMR_H_ */
