/*
 * xchg.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: jay
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>
#include "hiredis/hiredis.h"
#define TAG "XCHG_C"

void* xchgEndp(const char* srvIP, int srvPort)
{
	redisContext* redisCli = redisConnect(srvIP, srvPort);
	if(redisCli != NULL){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"init fail, error is '%s' ,srvIP = '%s' ,port = '%d'\n", redisCli->errstr , srvIP ,srvPort);
	}

	return redisCli;
}

int putInfo(void* endp, const char* myId, void* info, int size)
{
	redisContext* redisCli = (redisContext*)endp;
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"0 ,redisCli = %p" ,redisCli);
	redisReply* reply = (redisReply*)redisCommand(redisCli, "hset iceh_clients cli_%s %b", myId, info, size);
	if(NULL == reply){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"fail to submit my info to redis , error is '%s'\n", redisCli->errstr);
		printf("fail to submit my info to redis\n");
		return -1;
	}
	freeReplyObject((void*)reply);
	return 0;
}

int getInfo(void* endp, const char* id, void** info, int* size)
{
	redisContext* redisCli = (redisContext*)endp;
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"0 ,redisCli = %p" ,redisCli);
	redisReply* reply = (redisReply*)redisCommand(redisCli, "hget iceh_clients cli_%s", id);
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"1");
	if(NULL == reply){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"fail to get contrary info from redis, error is '%s'\n", redisCli->errstr );
		printf("fail to get contrary info from redis\n");
		return -1;
	}
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"2");
	if(reply->type != REDIS_REPLY_STRING || reply->len == 0){
		__android_log_print(ANDROID_LOG_ERROR ,TAG ,"contrary info invalid");
		printf("contrary info invalid\n");
		freeReplyObject((void*)reply);
		return -1;
	}

	char* buff = (char*)malloc(reply->len);
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"buff address =%p" ,buff);
	if(!buff){
		printf("malloc failure\n");
		return -1;
	}
	memcpy(buff, reply->str, reply->len);
	*size = reply->len;
	*info = buff;
	__android_log_print(ANDROID_LOG_ERROR ,TAG ,"*info address =%p" ,*info);
	freeReplyObject((void*)reply);
	return 0;
}

void freeEndp(void* endp)
{
	if(endp){
		redisContext* redisCli = (redisContext*)endp;
		redisFree(redisCli);
	}
}
