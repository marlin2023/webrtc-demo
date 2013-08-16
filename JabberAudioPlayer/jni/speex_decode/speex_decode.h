#ifndef SPEEX_ENCODE_H_
#define SPEEX_ENCODE_H_

#include <speex/speex.h>

typedef struct SPEEX_DECODE_UNION{

	/*Holds the state of the decoder*/
	void *state;

    /*Holds bits so they can be read and written to by the Speex routines*/
    SpeexBits bits;

}speex_decode_union_t;

int  spx_encode_init();

int spx_encode_frame(int handle ,char *src_speex_data ,short *target_pcm_data_buffer);


int spx_destroy(int handle );

#endif //speex_encode.h
