/*
 * speex_encode.h
 *
 *  Created on: Jun 19, 2013
 *      Author: chris
 */

#ifndef SPEEX_ENCODE_H_
#define SPEEX_ENCODE_H_

#include <speex/speex.h>
typedef struct SPEEX_ENCODE_UNION{

	/*Holds the state of the encoder*/
	void *state;

    /*Holds bits so they can be read and written to by the Speex routines*/
    SpeexBits bits;

}speex_encode_union_t;

int  spx_encode_init();

int spx_encode_frame(int handle ,short * const pcm_data ,char *speex_data);

int spx_destroy(int handle );

#endif /* SPEEX_ENCODE_H_ */
