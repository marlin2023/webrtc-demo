/*
 * ice.h
 *
 *  Created on: Jul 1, 2013
 *      Author: jay
 */

#ifndef ICE_H_
#define ICE_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

typedef unsigned short ushort_t;

typedef unsigned int uint_t;

typedef unsigned long ulong_t;

typedef struct _IceEndpoint IceEndpoint;

/**
 * buffer :formatted log info
 */
typedef void (*LogCallback)(const char *buffer);

/** callback definition for receiving data
 *
 * chnId :channel ID for the receiving data
 * userData :private data which is set in ice_endpoint
 * buffer: memory buffer which holds application data
 * size :size for memory buffer
 */
typedef void (*RxCallback)(IceEndpoint* endp, uint_t chnId, void* userData, void* buffer, size_t size);


/** init ice context
 *
 * srvIP :STUN and TURN server IP
 * port :STUN and TURN server port
 * logCall :callback for log output
 * logLevel :log level, should be 1 ~ 6
 */
int ice_init(const char* srvIP, ushort_t port, LogCallback logCall, int logLevel = 4);

/**
 * reverse operation for ice_init
 */
void ice_shutdown(void);

/** get a ice endpoint instance for transfer data
 *
 *localId :the jabber ID for local XMPP client
 *userData :private data for RxCallback
 *chnCnt :channel count for an ice session
 *func :callback for data receiving
 */
IceEndpoint* ice_endpoint(const char* localId, void* userData, uint_t chnCnt, RxCallback func);

IceEndpoint* ice_recycle(IceEndpoint* endp);

/**
 * size :local endpoint info size
 * what return is the buffer which holds local endpoint info
 */
void* ice_myInfo(IceEndpoint* endp, uint_t* size);

/**
 * remoteId :the jabber ID for remote peer
 * info :the endpoint info for remote peer
 * infoSz :the endpoint info size for remote peer
 */
int ice_establish(IceEndpoint* endp, const char* remoteId, void* info, uint_t infoSz);

/**
 * chnId :channel ID which is used for sending data, should start from 1
 * buffer :the memory buffer which holds user data
 * size :size for memory buffer
 */
int ice_send(IceEndpoint* endp, uint_t chnId, void* buffer, uint_t size);

/** destroy a endpoint instance
 *
 * reverse operation for ice_endpoint
 */
void ice_destroy(IceEndpoint* endp);

#endif /* ICE_H_ */
