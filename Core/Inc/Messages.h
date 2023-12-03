/*
 * Messages.h
 *
 *  Created on: Nov 25, 2023
 *      Author: maqui
 */

#ifndef INC_MESSAGES_H_
#define INC_MESSAGES_H_


#ifndef MESSAGE_BASE_ERROR
#define MESSAGE_BASE_ERROR			0x0800
#endif

#include "Apptypes.h"
#include "Queue.h"
#include "Port.h"
#include "Task.h"
#include "stdlib.h"
#include "Scheduler.h"
#include "String.h"


#define MESSAGE_OK 					0x00
#define MESSAGE_ERR_UNDEFF 			MESSAGE_BASE_ERROR | 0x00FF
#define MESSAGE_ERR_NULL_PARAM		MESSAGE_BASE_ERROR | 0x00FE
#define MESSAGE_ERR_PARAM_OUT_RANGE	MESSAGE_BASE_ERROR | 0x00FD
#define MESSAGE_ERR_EMPTY 			MESSAGE_BASE_ERROR | 0x00FC

#define MESSAGE_GET_POLL_SIZE(x,y)	( ( sizeof(T_QueueElement) * x ) + (x * y) )


typedef struct
{
	u16 MsgNumber;
	u16 MsgSize;
	pu8 MsgPollPtr;
	T_QueueHandler MsgTxWaitQueue;
	T_QueueHandler MsgRxWaitQueue;
	T_QueueHandler MsgFreeQueue;
	T_QueueHandler MsgUsedQueue;
}T_MessageHandler, * T_MessageHandlerPtr;

u16 Message_Init(T_MessageHandlerPtr Handler, u16 MsgNumber, u16 MsgSize, pu8 MsgPoll);
u16 Message_GetFree(T_MessageHandlerPtr Handler);
u16 Message_GetUsed(T_MessageHandlerPtr Handler);
u16 Message_Send(T_MessageHandlerPtr Handler , pv Msg);
u16 Message_Rec(T_MessageHandlerPtr Handler , pv Msg);

#endif /* INC_MESSAGES_H_ */
