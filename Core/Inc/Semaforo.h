/*
 * Semaforo.h
 *
 *  Created on: Nov 15, 2023
 *      Author: maqui
 */

#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include "Apptypes.h"
#include "Queue.h"
#include "Port.h"
#include "Scheduler.h"

#define SEM_FREE		0
#define SEM_BUSY		1

#ifndef SEM_BASE_ERROR
#define SEM_BASE_ERROR			0x0600
#endif

#define SEM_OK 					0x00
#define SEM_ERR_UNDEFF 			SEM_BASE_ERROR | 0x00FF
#define SEM_ERR_NULL_PARAM		SEM_BASE_ERROR | 0x00FE
#define SEM_ERR_PARAM_OUT_RANGE	SEM_BASE_ERROR | 0x00FD
#define SEM_ERR_EMPTY 			SEM_BASE_ERROR | 0x00FC


typedef struct
{
	u16 Count;
	u16 Limit;
	T_QueueHandler WaitQueue;
}T_SemaforoHandler, * T_SemaforoHandler_Ptr;

u16 Semaforo_Init(T_SemaforoHandler_Ptr sem, u16 Limit);
u16 Semaforo_Take(T_SemaforoHandler_Ptr sem);
u16 Semaforo_Give(T_SemaforoHandler_Ptr sem);


#endif /* SEMAFORO_H_ */
