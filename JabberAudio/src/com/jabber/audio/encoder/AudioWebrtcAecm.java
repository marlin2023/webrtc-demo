package com.jabber.audio.encoder;

public class AudioWebrtcAecm {
	   //loadLibrary
    static {
        try{
            System.loadLibrary("webrtc_aecm");

        }catch(UnsatisfiedLinkError ule){

            System.err.println("WARNING: Could not load library webrtc_aecm!");
        }

    }

    // jni
    //native function
    private native int yy_webrtc_aecm_initiate(int samp_freq);

    private native short[] yy_webrtc_aecm_process(int handle ,short[] nearendNoisy ,short[] nearendClean);

    private native int yy_webrtc_aecm_free(int handle);


    public int yyWebRtcAcemInitiate(int samp_freq){


        return yy_webrtc_aecm_initiate(samp_freq);
    }

    public short []yyWebRtcAcemProcess(int handle ,short[] nearendNoisy ,short[] nearendClean ){

    	return yy_webrtc_aecm_process( handle , nearendNoisy , nearendClean);

        
    }

    public int yyWebRtcAcemFree(int handle){

    	yy_webrtc_aecm_free(handle);
        return 0;
    }
}
