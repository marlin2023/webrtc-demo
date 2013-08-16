package com.jabber.audio.encoder;

/**
 * Created by chris on 6/19/13.
 */
public class AudioWebrtcNs {

    //loadLibrary
    static {
        try{
            System.loadLibrary("webrtc_ns");

        }catch(UnsatisfiedLinkError ule){

            System.err.println("WARNING: Could not load library webrtc_ns!");
        }

    }

    // jni
    //native function
    private native int yy_webrtc_ns_initiate(int sample_rate);

    private native short[] yy_webrtc_ns_process(int handle ,short[] in_data );

    private native int yy_webrtc_ns_free(int handle);


    public int yyWebRtcNsInitiate(int sample_rate){


        return yy_webrtc_ns_initiate(sample_rate);
    }

    public short []yyWebRtcNsProcess(int handle ,short[] in_data ){

    	return yy_webrtc_ns_process( handle , in_data );

        
    }

    public int yyWebRtcNsFree(int handle){

    	yy_webrtc_ns_free(handle);
        return 0;
    }
}
