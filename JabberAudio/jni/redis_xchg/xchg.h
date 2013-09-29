/*
 * xchg.h
 *
 *  Created on: Sep 24, 2013
 *      Author: jay
 */

#ifndef XCHG_H_
#define XCHG_H_

void* xchgEndp(const char* srvIP, int srvPort);

int putInfo(void* endp, const char* myId, void* info, int size);

int getInfo(void* endp, const char* id, void** info, int* size);

void freeEndp(void* endp);

#endif /* XCHG_H_ */
