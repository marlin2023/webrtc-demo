/*
 * speex_encode.c
 *
 *  Created on: Jun 18, 2013
 *      Author: chris
 */

#include <speex/speex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "speex_encode.h"

///*The frame size in hardcoded for this sample code but it doesn’t have to be*/
#define FRAME_SIZE 160


int  spx_encode_init(){

    speex_encode_union_t * speex_encode_u = malloc(sizeof(speex_encode_union_t));
    if(speex_encode_u == NULL){
        printf("speex_encode_union malloc failed ...\n");
        exit(1);
    }

	/*Create a new encoder state in narrowband mode*/
   speex_encode_u->state = speex_encoder_init(&speex_nb_mode);

    /*Set the quality to 8 (15 kbps)*/
    int tmp = 8;
    speex_encoder_ctl(speex_encode_u->state, SPEEX_SET_QUALITY, &tmp);

    /*Initialization of the structure that holds the bits*/
    speex_bits_init(&speex_encode_u->bits);

    return (int )speex_encode_u;
}


int spx_encode_frame(int handle ,short *const pcm_data ,char *speex_data){
   // char cbits[200];
    float input[FRAME_SIZE];
    speex_encode_union_t * speex_encode_u = (speex_encode_union_t *)handle;

    /*Flush all the bits in the struct so we can encode a new frame*/
    speex_bits_reset(&speex_encode_u->bits);

    /*Encode the frame*/
   // memcpy(input,pcm_data,FRAME_SIZE*2);
    int i;
    for (i = 0; i < FRAME_SIZE; i++)
    			input[i] = pcm_data[i];
    speex_encode(speex_encode_u->state, input, &speex_encode_u->bits);
    /*Copy the bits to an array of char that can be written*/
//    nbBytes = speex_bits_write(&speex_encode_u->bits, cbits, 200);
    int nbBytes = speex_bits_write(&speex_encode_u->bits, speex_data, 38);
    printf("nbBytes = %d \n" ,nbBytes);
//    /*Write the size of the frame first. This is what sampledec expects but
//     it’s likely to be different in your own application*/
//    fwrite(&nbBytes, sizeof(int), 1, stdout);
//    printf("nbBytes = %d \n" ,nbBytes);
//    /*Write the compressed data*/
//    //fwrite(cbits, 1, nbBytes, stdout);
}

int spx_destroy(int handle ){
    speex_encode_union_t * speex_encode_u = (speex_encode_union_t *)handle;

    /*Destroy the encoder state*/
	speex_encoder_destroy(speex_encode_u->state);
	/*Destroy the bit-packing struct*/
	speex_bits_destroy(&speex_encode_u->bits);

	free(speex_encode_u);
}
