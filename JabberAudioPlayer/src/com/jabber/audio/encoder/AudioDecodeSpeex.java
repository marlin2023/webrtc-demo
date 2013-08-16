package com.jabber.audio.encoder;

import java.nio.ByteBuffer;


public class AudioDecodeSpeex {
    static {
        try{
            System.loadLibrary("speex_decoder");
        }catch(UnsatisfiedLinkError ule){
            System.err.println("WARNING: Could not load library speex_decoder!");
        }
    }

    //jni
    //native function
    private native int spx_decode_init();

    private native short [] spx_decode_frame(int handle ,byte []src_speex_data );

    private native int spx_destroy(int handle);

    //public interface
    public int spxDecodeInit(){
    	return spx_decode_init();
    }
    
    public short [] spxDecodeFrame(int handle ,byte [] src_speex_data /* ,short []target_pcm_data_buffer*/){
    	return spx_decode_frame(handle ,src_speex_data /*,target_pcm_data_buffer*/);
    }
    
    public int spxDestroy(int handle){
    	spx_destroy(handle);
    	return 0;
    }
}
