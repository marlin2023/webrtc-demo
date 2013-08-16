package com.jabber.audio.encoder;

import java.nio.ByteBuffer;

/**
 * Created by chris on 6/19/13.
 */
public class AudioEncodeSpeex {
    static {
        try{
            System.loadLibrary("speex_encoder");
        }catch(UnsatisfiedLinkError ule){
            System.err.println("WARNING: Could not load library 12425242!");
        }
    }

    //jni
    //native function
    private native int yy_speex_encode_init();

    private native int yy_speex_encode_frame(int handle ,short []pcm_data ,ByteBuffer speexEncodedData);

    private native int yy_speex_destroy(int handle);



    public  int yySpeexEncodeInit(){

        return yy_speex_encode_init();

    }

    public int yySpeexEncodeFrame(int handle ,short []pcm_data ,ByteBuffer speexEncodedData){

        yy_speex_encode_frame(handle ,pcm_data ,speexEncodedData);
        return 0;
    }

    public int yySpeexDestroy(int handle){

        yy_speex_destroy(handle);
        return  0;
    }
}
