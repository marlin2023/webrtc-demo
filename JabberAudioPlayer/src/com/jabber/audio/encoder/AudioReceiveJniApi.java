package com.jabber.audio.encoder;

import java.nio.ByteBuffer;

public class AudioReceiveJniApi {
	
	//loadLibrary
	static{
		
		try{
			System.loadLibrary("rtp_recv_speex");
		}catch(UnsatisfiedLinkError ule){
			 System.err.println("WARNING: Could not load library rtp_recv_speex!");
		}
	}
	
	//native function 
	private native int yy_init(int port);
	
	private native int yy_rtp_receive_speex(int data_num ,ByteBuffer speexData);

	private native int yy_exit(int handle);

	
	public int yyInit(int port) {
		return yy_init(port);
	}
	
	
	public int yyRtpReceiveSpeex(int data_num ,ByteBuffer speexData){
		yy_rtp_receive_speex(data_num ,speexData);
		return 0;
	}
	
	
	public int yyExit(int handle){
		return yy_exit(handle);
	}
	
	
}
