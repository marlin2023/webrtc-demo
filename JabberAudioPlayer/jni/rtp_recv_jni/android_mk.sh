#!/bin/bash
arm-linux-androideabi-gcc -Wall -c  rtp_recv_amr.c -I/home/chris/work/rtp-rtcp/ortp-arm/include

arm-linux-androideabi-gcc -Wall -c com_jabber_audio_encoder_AudioReceiveJniApi.c -I/home/chris/work/rtp-rtcp/ortp-arm/include -I/home/test/jdk-src/jdk1.6.0_26/include -I/home/test/jdk-src/jdk1.6.0_26/include/linux

arm-linux-androideabi-gcc -shared -fPCI -o libyy_recv_jni.so *.o -L/home/chris/work/rtp-rtcp/ortp-arm/lib -lortp -llog
