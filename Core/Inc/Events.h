/*
 * Events.h
 *
 *  Created on: Nov 18, 2023
 *      Author: maqui
 */

#ifndef INC_EVENTS_H_
#define INC_EVENTS_H_

#include "Apptypes.h"
#include "Queue.h"
#include "Scheduler.h"
#include "Port.h"

#define EVENTS_WAIT_ALL 			0x8000

#define EVENTS_WAIT_MASK			0x7FFF

#ifndef EVENTS_BASE_ERROR
#define EVENTS_BASE_ERROR			0x0700
#endif

#define EVENTS_OK 					0x00
#define EVENTS_ERR_UNDEFF 			EVENTS_BASE_ERROR | 0x00FF
#define EVENTS_ERR_NULL_PARAM		EVENTS_BASE_ERROR | 0x00FE
#define EVENTS_ERR_PARAM_OUT_RANGE	EVENTS_BASE_ERROR | 0x00FD
#define EVENTS_ERR_EMPTY 			EVENTS_BASE_ERROR | 0x00FC


typedef struct
{
	u16 Events;
	T_QueueHandler WaitQueue;

}T_EventHanlder, * T_EventHanlderPtr;

u16 Events_Init		(T_EventHanlderPtr Handler);
u16 Events_WaitAny	(T_EventHanlderPtr Handler, u16 WaitMask);
u16 Events_WaitAll	(T_EventHanlderPtr Handler, u16 WaitMask);
u16 Events_Clear	(T_EventHanlderPtr Handler, u16 WaitMask);
u16 Events_Set		(T_EventHanlderPtr Handler, u16 WaitMask);

#endif /* INC_EVENTS_H_ */
