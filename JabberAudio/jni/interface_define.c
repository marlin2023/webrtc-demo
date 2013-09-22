//callback function

/*
 * LogCallback function
 * @param buffer: log information
 *
 * */
static void LogCallback(const char *buffer){

}


/*
 * RxCallback function (I maintenance a buffer to store receive data)
 * @param userData: ?
 * @param buffer: 	point the receive data
 * @param size: 	receive data  size
 *
 * */
void RxCallback(void* userData, void* buffer, size_t size){

	//receive android api function to  play audio ?

	//parse rtp packet

	//decode
}

typedef struct IceControl_Union_t{
	IceEndpoint* endp;
	void *info;
	int info_size;
}icecontrol_t;
//=====================================voice call start =====================
// obtain ice_myinfo and send to remote
//1 .voice call start
/*
 *
 * */
//return handle
int voice_call_init(const char* srvIP, unsigned short port ,const char* localId){
	int ice_init(const char* srvIP, ushort_t port, LogCallback logCall);
	IceEndpoint* ice_endpoint(const char* localId, void* userData, RxCallback func);
	void* ice_myInfo(IceEndpoint* endp, uint_t* size);
	//return handle
}

//=====================================remote accept =====================
int voice_call_establish(int handle ,const char* remoteId){
	int ice_establish(IceEndpoint* endp, const char* remoteId, void* info, uint_t infoSz);
}

//=====================================voice call stop =================
void voice_call_destoy(int handle){
	//void ice_destroy(IceEndpoint* endp)
}

//---------------------------transport data ------------------------
int ice_send(IceEndpoint* endp, void* buffer, uint_t size){

}
