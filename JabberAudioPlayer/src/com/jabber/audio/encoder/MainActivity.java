package com.jabber.audio.encoder;

import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ShortBuffer;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {
	 
	boolean isPlaying = false;
	Button btnStart ,btnStop ,btnExit;
	
	//audioTrack 
	AudioTrack audioTrack;
	int bufferSize;
	byte[] outBuffer;
	int frequency = 8000;
	
    //File
    File recordingFile;
    File path1 ;
    DataOutputStream dos ;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		
		//get button 
		btnStart = (Button)this.findViewById(R.id.btn_start_play);
		btnStart.setOnClickListener(new startPlay());
		
		btnStop = (Button)this.findViewById(R.id.btn_stop_play);
		btnStop.setOnClickListener(new stopPlay());
		
		//do audioTrack work
		bufferSize = AudioTrack.getMinBufferSize(frequency,
				AudioFormat.CHANNEL_CONFIGURATION_MONO,  
				AudioFormat.ENCODING_PCM_16BIT);
		//init audioTrack
		audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,
				frequency,
				AudioFormat.CHANNEL_CONFIGURATION_MONO, 
				AudioFormat.ENCODING_PCM_16BIT, bufferSize,
				AudioTrack.MODE_STREAM);


        //new file 
        File path1 = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/yanlong");
        path1.mkdirs();
        try{
        	recordingFile = File.createTempFile("recording", ".spx_recv" ,path1);
        }catch(IOException e1){
        	throw new RuntimeException("sdafjjjjjjjjjj" , e1);
        }
        
        try {
			dos = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(recordingFile)));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	
	 //触发的事件
	class startPlay implements View.OnClickListener{
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			//开始播放
			isPlaying = true;
			new AudioPlayThread().start();// 开一条线程放
		}
	}
	
	//音频播放线程
	class AudioPlayThread extends Thread{

		@Override
		public void run() {
			// TODO Auto-generated method stub
			super.run();
			
			//init speex
			AudioDecodeSpeex audioDecodeSpeex1 = new AudioDecodeSpeex();
            int speexHandle = audioDecodeSpeex1.spxDecodeInit();
			
            //init ortp 
            AudioReceiveJniApi audioReceiveJniApi1 = new AudioReceiveJniApi();
            int rtpHandle = audioReceiveJniApi1.yyInit(6226);

            //start audioTrack
			audioTrack.play();
			short []pcmPlayBuffer = new short[160];
			ByteBuffer speexData = ByteBuffer.allocateDirect(38);  //a frame size 38
			while(isPlaying){
				//receive audio
				audioReceiveJniApi1.yyRtpReceiveSpeex(rtpHandle ,speexData); //上层收到数据直接放在speexData中
				//write file
				try {
					dos.write(speexData.array());
					dos.flush();
					Log.e("chris_magic", "write speex data");
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				//decode audio 
				pcmPlayBuffer = audioDecodeSpeex1.spxDecodeFrame(speexHandle, speexData.array());
				//play audio
				audioTrack.write(pcmPlayBuffer, 0, 160);
				audioTrack.w
			}
		}
		
	}
	
	class stopPlay implements View.OnClickListener{

		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			 //停止播放
            isPlaying = false;
		}
		
	}
	
}
