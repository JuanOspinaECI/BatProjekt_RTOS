/*
 * Mutex.h
 *
 *  Created on: Nov 11, 2023
 *      Author:
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include "Apptypes.h"
#include "Queue.h"
#include "Port.h"
#include "Scheduler.h"

#define MUTEX_FREE		0
#define MUTEX_BUSY		1

#ifndef MUTEX_BASE_ERROR
#define MUTEX_BASE_ERROR			0x0500
#endif

#define MUTEX_OK 					0x00
#define MUTEX_ERR_UNDEFF 			MUTEX_BASE_ERROR | 0x00FF
#define MUTEX_ERR_NULL_PARAM		MUTEX_BASE_ERROR | 0x00FE
#define MUTEX_ERR_PARAM_OUT_RANGE	MUTEX_BASE_ERROR | 0x00FD
#define MUTEX_ERR_EMPTY 			MUTEX_BASE_ERROR | 0x00FC

typedef struct
{
	u8 Status;
	T_QueueHandler WaitQueue;


}T_MutexHandler, * T_MutexHandler_Ptr;

u16 Mutex_Init(T_MutexHandler_Ptr mutex);
u16 Mutex_Take(T_MutexHandler_Ptr mutex);
u16 Mutex_Give(T_MutexHandler_Ptr mutex);

#endif /* MUTEX_H_ */
