/*
 * Queue.h
 *
 *  Created on: Sep 24, 2023
 *      Author: maqui
 */



#ifndef QUEUE_H_
#define QUEUE_H_

#include "Apptypes.h"

#ifndef QUEUE_BASE_ERROR
#define QUEUE_BASE_ERROR			0x0200
#endif

#define QUEUE_OK 					0x00
#define QUEUE_ERR_UNDEFF 			QUEUE_BASE_ERROR | 0x00FF
#define QUEUE_ERR_NULL_PARAM		QUEUE_BASE_ERROR | 0x00FE
#define QUEUE_ERR_PARAM_OUT_RANGE	QUEUE_BASE_ERROR | 0x00FD
#define QUEUE_ERR_EMPTY 			QUEUE_BASE_ERROR | 0x00FC


typedef struct _queue_element
{
	pv DataPtr;
	struct _queue_element * Next;
}T_QueueElement, *T_QueueElementPtr;

typedef struct
{
	T_QueueElementPtr Head;
	T_QueueElementPtr Tail;
	u16 ElementsCount;
}T_QueueHandler, *T_QueueHandlerPtr;

u16 Queue_Init(T_QueueHandlerPtr Queue);
u16 Queue_GetElementsCount(T_QueueHandlerPtr Queue);
u16 Queue_Enqueue(T_QueueHandlerPtr Queue, T_QueueElementPtr Element, pv Data);
u16 Queue_Dequeue(T_QueueHandlerPtr Queue, pv * Data);
u16 Queue_DequeueElement(T_QueueHandlerPtr Queue, T_QueueElementPtr * Element);
u16 Queue_Copy(T_QueueHandlerPtr Origin, T_QueueHandlerPtr Destinado);



#endif /* QUEUE_H_ */
