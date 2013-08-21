package com.jabber.audio.encoder;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.os.Looper;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.jabber.audio.encoder.AudioEncodeSpeex;
import com.jabber.audio.encoder.AudioTransJniApi;
import com.jabber.audio.encoder.AudioWebrtcNs;

import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.nio.ByteBuffer;
import java.util.Enumeration;

public class MainActivity extends Activity {
    Button btnStart ,btnStop ,btnExit;
    int recBufSize;
    AudioRecord audioRecord;
    boolean isRecording = false;//是否录放的标记
    static final int sampleRateInHz = 8000;
    static final int channelConfiguration = AudioFormat.CHANNEL_CONFIGURATION_MONO;
    static final int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
    
    
    //---play
	boolean isPlaying = false;
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
		
        recBufSize = AudioRecord.getMinBufferSize(sampleRateInHz,
                channelConfiguration, audioFormat);

        audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRateInHz,
                channelConfiguration, audioFormat, recBufSize);


        btnStart = (Button)this.findViewById(R.id.record_start);
        btnStart.setOnClickListener(new StartRecord());

        btnStop = (Button)this.findViewById(R.id.record_stop);
        btnStop.setOnClickListener(new StopRecord());
        
        //new file 
        File path1 = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/yanlong");
        path1.mkdirs();
        try{
        	recordingFile = File.createTempFile("recording", ".spx" ,path1);
        }catch(IOException e1){
        	throw new RuntimeException("sdafjjjjjjjjjj" , e1);
        }
        
        try {
			dos = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(recordingFile)));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
        
        //-----------------------------play audio data -------------------
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

	}

	//get local ip address
	public String getLocalIpAddress() {  
        try {  
            for (Enumeration<NetworkInterface> en = NetworkInterface  
                    .getNetworkInterfaces(); en.hasMoreElements();) {  
                NetworkInterface intf = en.nextElement();  
                for (Enumeration<InetAddress> enumIpAddr = intf  
                        .getInetAddresses(); enumIpAddr.hasMoreElements();) {  
                    InetAddress inetAddress = enumIpAddr.nextElement();  
                    if (!inetAddress.isLoopbackAddress() && !inetAddress.isLinkLocalAddress()) {  
                        return inetAddress.getHostAddress().toString();  
                        
                    }  
                }  
            }  
        } catch (SocketException ex) {  
            Log.e("WifiPreference IpAddress", ex.toString());  
        }  
        return null;  
    }  
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}


    //触发的事件
    class StartRecord implements View.OnClickListener{  //开始录音

        @Override
        public void onClick(View view) {
            //开始录音
            isRecording = true;
            new RecordPlayThread().start();// 开一条线程边录边放

            //
			//开始播放
			isPlaying = true;
			new AudioPlayThread().start();// 开一条线程放
        }
    }

    //录音线程‘
    class RecordPlayThread extends Thread {
        @Override
        public void run() {
            super.run();
            Looper.prepare();   //
            try{

                ByteBuffer speexEncodedData = ByteBuffer.allocateDirect(38);  //a frame size 38
                int SPEEX_FRAME_SZIE = 160;
                short[] buffer = new short[SPEEX_FRAME_SZIE];     //这个数组用于保存每次从mic 读取的数据的大小
                short[] webrtc_ns_buffer = new short[SPEEX_FRAME_SZIE];	//save audio data denoised by webrtc????
                short[] webrtc_aecm_buffer = new short[SPEEX_FRAME_SZIE];	//save audio data denoised by webrtc????
                Log.e("chris_magic", "LOCAL IP ADDRESS:  " + getLocalIpAddress());
                Log.e("chris_magic", "...recBufSize = " + SPEEX_FRAME_SZIE);   //768/2?
                audioRecord.startRecording();   //开始录音
                //init webrtc_voice  ns moudle
                AudioWebrtcNs audioWebrtcNs = new AudioWebrtcNs();
                int webrtc_ns_handle = audioWebrtcNs.yyWebRtcNsInitiate(8000);
                Log.e("chris_magic" ,"webrtc_ns_handle = " + webrtc_ns_handle );
                
                //init webrtc_voice  aecm moudle
                AudioWebrtcAecm audioWebrtcAecm = new AudioWebrtcAecm();
                int webrtc_aecm_handle = audioWebrtcAecm.yyWebRtcAcemInitiate(8000);
                Log.e("chris_magic" ,"webrtc_aecm_handle = " + webrtc_aecm_handle );
                
                
                 //   初始化speex编码信息
                AudioEncodeSpeex audioEncodeSpeex1 = new AudioEncodeSpeex();
                int speexHandle = audioEncodeSpeex1.yySpeexEncodeInit();
                //初始化传输信息
                AudioTransJniApi audioTransJniApi1 = new AudioTransJniApi();
//                int rtpHandle = audioTransJniApi1.yyInit("192.168.1.89" ,6226);  //接收端ip
//                int rtpHandle = audioTransJniApi1.yyInit("192.168.0.177" ,6226);  //接收端ip
                int rtpHandle = audioTransJniApi1.yyInit("192.168.0.132" ,6226);  //接收端ip
//                int rtpHandle = audioTransJniApi1.yyInit("192.168.0.104" ,6226);  //接收端ip

                int ii;
                byte []temp = new byte[320]; 
                while(isRecording){
                    //从mic 保存数据到缓冲区
                    int bufferReadResult = audioRecord.read(buffer, 0,          //不断的从AudioRecord读取音频到buffer 数组中
                            SPEEX_FRAME_SZIE);
                    Log.e("chris_magic" ,"bufferReadResult = " + bufferReadResult );
                    if(bufferReadResult != SPEEX_FRAME_SZIE){
                        Log.e("chris_magic" ,"CHRIS_MAGIC_YANLONG....... " + "bufferReadResult = " + bufferReadResult );
                    }

//***********************    webrtc ns moudle 		*************************************************//                    
                    webrtc_ns_buffer = (audioWebrtcNs.yyWebRtcNsProcess(webrtc_ns_handle, buffer)).clone();
//                    for(ii = 0; ii < 160 ; ii ++){
//                    	temp[2*ii] = (byte)(webrtc_ns_buffer[ii] & 0xff);
//                    	temp[2*ii + 1] = (byte)((webrtc_ns_buffer[ii] >> 8) & 0xff);                    	
//                    }
//
//                    
//                    dos.write(temp,0 ,320);
//                    dos.flush();
                    
//***********************    webrtc aecm moudle 		*************************************************//
                    webrtc_aecm_buffer = audioWebrtcAecm.yyWebRtcAcemProcess(webrtc_aecm_handle, buffer, webrtc_ns_buffer);
                    
                    for(ii = 0; ii < 160 ; ii ++){
                    	temp[2*ii] = (byte)(webrtc_aecm_buffer[ii] & 0xff);
                    	temp[2*ii + 1] = (byte)((webrtc_aecm_buffer[ii] >> 8) & 0xff);                    	
                    }

                    dos.write(temp,0 ,320);
                    dos.flush();          
//***********************    speex encode moudle 		*************************************************//                    
                    //这里要维护一个队列。 拼凑每次编码一帧数据大小。
                    //开始编码，每次编码320字节信息
//                    audioEncodeSpeex1.yySpeexEncodeFrame(speexHandle ,buffer ,speexEncodedData);
                    audioEncodeSpeex1.yySpeexEncodeFrame(speexHandle ,webrtc_ns_buffer ,speexEncodedData);

//***********************    speex encoded data transport moudle 		*************************************************//                    
                    audioTransJniApi1.yyRtpSendAmr(rtpHandle ,speexEncodedData.array() ,38);
                    
                  //  dos.write(speexEncodedData.array());
                   // dos.flush();
                }
                audioRecord.stop();


            }catch (Throwable t){

                Toast.makeText(MainActivity.this, t.getMessage(), 1000);
            }
            Looper.loop();
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
			}
		}
		
	}
    //停止录音
    class StopRecord implements View.OnClickListener{

        @Override
        public void onClick(View view) {
            isRecording = false;
            isPlaying = false;
        }
    }
	
}
