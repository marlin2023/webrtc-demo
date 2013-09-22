package com.jabber.jni;

public class Ice_Jni {

    static {
        System.loadLibrary("Encrypt");
    }

    /* native function */
    private native static int voice_call_init(String srvIP, int port, String jabberID);

    /* native function */
    private native static String ice_get_info(int handle, Integer size);

    /* native function */
    private native static int voice_call_establish(int handle, String remoteId, String info,
            int infoSz);

    /* native function */
    private native static String voice_call_destoy(int handle);

    /* native function */
    private native static int ice_send(int handle, byte[] buffer, int size);

    public static int init(String srvIP, int port, String jabberID) {
        return voice_call_init(srvIP, port, jabberID);
    }

    public static String get_info(int handle, Integer size) {
        return ice_get_info(handle, size);
    }

    public static int establish(int handle, String remoteId, String info, int infoSz) {
        return voice_call_establish(handle, remoteId, info, infoSz);
    }

    public static void destoy(int handle) {
        voice_call_destoy(handle);
    }

    public static void send(int handle, byte[] buffer, int size) {
        ice_send(handle, buffer, size);
    }
}
