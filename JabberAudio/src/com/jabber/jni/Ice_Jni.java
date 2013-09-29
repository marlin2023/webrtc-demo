package com.jabber.jni;

public class Ice_Jni {

    static {
        System.loadLibrary("ice_data_transport");
    }

    /* native function */
    private native static int voice_call_init(String srvIP, int port, String jabberID);

    /* native function */
    private native static String ice_get_info(int handle, Integer size ,int no_mark);

    /* native function */
    private native static int voice_call_establish(int handle, String remoteId, String info,
            int infoSz ,int me_mark);

    /* native function */
    private native static String voice_call_destoy(int handle);

    /* native function */
    private native static int ice_send(int handle, byte[] buffer, int size);

    public static int init(String srvIP, int port, String jabberID) {
        return voice_call_init(srvIP, port, jabberID);
    }

    public static String get_info(int handle, Integer size ,int no_mark) {
        return ice_get_info(handle, size ,no_mark);
    }

    public static int establish(int handle, String remoteId, String info, int infoSz ,int me_mark) {
        return voice_call_establish(handle, remoteId, info, infoSz ,me_mark);
    }

    public static void destoy(int handle) {
        voice_call_destoy(handle);
    }

    public static void send(int handle, byte[] buffer, int size) {
        ice_send(handle, buffer, size);
    }
}
