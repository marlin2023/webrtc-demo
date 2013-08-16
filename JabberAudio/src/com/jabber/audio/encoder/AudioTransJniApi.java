package com.jabber.audio.encoder;

/**
 * Created by chris on 6/19/13.
 */
public class AudioTransJniApi {

    //loadLibrary
    static {
        try{
            System.loadLibrary("rtp_send_speex");

        }catch(UnsatisfiedLinkError ule){

            System.err.println("WARNING: Could not load library rtp_send_amr!");
        }

    }

    // jni
    //native function
    private native int yy_init(String addr,int port);

    private native int yy_rtp_send_amr(int handle ,byte[] audio_data ,int audio_len);

    private native int yy_exit(int handle);


    public int yyInit(String addr,int port){


        return yy_init(addr,port);
    }

    public int yyRtpSendAmr(int handle ,byte[] audio_data ,int audio_len){

        yy_rtp_send_amr( handle , audio_data ,audio_len);

        return 0;
    }

    public int yyExit(int handle){

        yy_exit(handle);
        return 0;
    }
}
