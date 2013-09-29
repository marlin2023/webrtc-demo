/*
 * xchg.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: jay
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hiredis/hiredis.h"


void* xchgEndp(const char* srvIP, int srvPort)
{
	redisContext* redisCli = redisConnect(srvIP, srvPort);
	return redisCli;
}

int putInfo(void* endp, const char* myId, void* info, int size)
{
	redisContext* redisCli = (redisContext*)endp;
	redisReply* reply = (redisReply*)redisCommand(redisCli, "hset iceh_clients cli_%s %b", myId, info, size);
	if(NULL == reply){
		printf("fail to submit my info to redis, error is '%s'\n", redisCli->errstr);
		return -1;
	}
	freeReplyObject((void*)reply);
	return 0;
}

int getInfo(void* endp, const char* id, void** info, int* size)
{
	redisContext* redisCli = (redisContext*)endp;
	redisReply* reply = (redisReply*)redisCommand(redisCli, "hget iceh_clients cli_%s", id);
	if(NULL == reply){
		printf("fail to get contrary info from redis, error is '%s'\n", redisCli->errstr);
		return -1;
	}
	if(reply->type != REDIS_REPLY_STRING || reply->len == 0){
		printf("contrary info invalid\n");
		freeReplyObject((void*)reply);
		return -1;
	}

	char* buff = (char*)malloc(reply->len);
	if(!buff){
		printf("malloc failure\n");
		return -1;
	}
	memcpy(buff, reply->str, reply->len);
	*size = reply->len;
	*info = buff;

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
