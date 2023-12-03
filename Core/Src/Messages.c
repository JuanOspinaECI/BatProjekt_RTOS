/*
 * Messages.c
 *
 *  Created on: Nov 25, 2023
 *      Author: maqui
 */

#include "Messages.h"

u16 Message_Init(T_MessageHandlerPtr Handler, u16 MsgNumber, u16 MsgSize, pu8 MsgPoll)
{
	u16 Res = MESSAGE_ERR_NULL_PARAM;
	if((Handler != NULL) && (MsgPoll != NULL))
	{
		Res = MESSAGE_ERR_PARAM_OUT_RANGE;
		if((MsgNumber > 0) && (MsgSize > 0))
		{
			vPortEnterCritical();
			u16 i;
			pu8 PollDataPtr;
			T_QueueElementPtr Element;

			Handler->MsgNumber 	= MsgNumber;
			Handler->MsgSize 	= MsgSize;
			Handler->MsgPollPtr = MsgPoll;
			Element 			= (T_QueueElementPtr)MsgPoll;
			PollDataPtr 		= MsgPoll + (sizeof(T_QueueElement) * Handler->MsgNumber);

			Res = Queue_Init(&Handler->MsgTxWaitQueue);
			if(Res != QUEUE_OK) {return Res; vPortExitCritical();}
			Res = Queue_Init(&Handler->MsgRxWaitQueue);
			if(Res != QUEUE_OK) {return Res; vPortExitCritical();}
			Res =  Queue_Init(&Handler->MsgFreeQueue);
			if(Res != QUEUE_OK) {return Res; vPortExitCritical();}
			Res = Queue_Init(&Handler->MsgUsedQueue);
			if(Res != QUEUE_OK) {return Res; vPortExitCritical();}
			for(i = 0; i < Handler->MsgNumber; i++)
			{
				Element->Next = NULL;
				Element->DataPtr = PollDataPtr;
				Res = Queue_Enqueue(&Handler->MsgFreeQueue, Element, PollDataPtr);

				if(Res != QUEUE_OK) break;

				Element += 1;
				PollDataPtr += Handler->MsgSize;
			}
			vPortExitCritical();
		}
	}
	return Res;
}

u16 Message_GetFree(T_MessageHandlerPtr Handler)
{
	u16 Count = 0;
	if(Handler != NULL)
	{
		vPortEnterCritical();
		Count = Queue_GetElementsCount(&Handler->MsgFreeQueue);
		vPortExitCritical();
	}
	return Count;
}
u16 Message_GetUsed(T_MessageHandlerPtr Handler)
{
	u16 Count = 0;
	if(Handler != NULL)
	{
		vPortEnterCritical();
		Count = Queue_GetElementsCount(&Handler->MsgUsedQueue);
		vPortExitCritical();
	}
	return Count;
}

u16 Message_Send(T_MessageHandlerPtr Handler , pv Msg)
{
	u16 Res = MESSAGE_ERR_NULL_PARAM;
	u8 CallScheduler = False;
	if((Handler != NULL) && (Msg != NULL) )
	{
		T_QueueElementPtr Element;
		T_TaskTCB_Ptr ActualTask;
		vPortEnterCritical();
		ActualTask = Scheduler_GetActualTask();
		if (ActualTask != NULL)
		{
			while(Queue_DequeueElement(&Handler->MsgFreeQueue, &Element) != QUEUE_OK)
			{
				Res = Queue_Enqueue(&Handler->MsgTxWaitQueue, &ActualTask->QueueElement, ActualTask);
				if(Res == QUEUE_OK)
				{
					ActualTask->TaskStatus = TASK_BLOCKED;
					vPortExitCritical();
					Port_SoftwareInterrupt();
				}
				else
				{
					vPortExitCritical();
					Task_Sleep(10);
				}
				vPortEnterCritical();
			}
			memcpy(Element->DataPtr, Msg, Handler->MsgSize);
			Res = Queue_Enqueue(&Handler->MsgFreeQueue, Element, Element->DataPtr);
			if(Res == QUEUE_OK)
			{
				if(Queue_GetElementsCount(&Handler->MsgRxWaitQueue) > 0)
				{
					T_TaskTCB_Ptr WakeTask;
					Res = Queue_Dequeue(&Handler->MsgRxWaitQueue,(pv *) &WakeTask);
					if((Res == QUEUE_OK) && (WakeTask != NULL))
					{
						Res = Scheduler_SetReady(WakeTask);
						if(Res == SCHEDULER_OK)
						{
							if((ActualTask->TaskPriority > WakeTask->TaskPriority))
							{
								CallScheduler = True;
							}
						}
					}
				}
			}
		}

		vPortExitCritical();
		if(CallScheduler)
		{
			Port_SoftwareInterrupt();
		}

	}
	return Res;
}

u16 Message_Rec(T_MessageHandlerPtr Handler , pv Msg)
{
	u16 Res = MESSAGE_ERR_NULL_PARAM;
	u8 CallScheduler = False;
	if((Handler != NULL) && (Msg != NULL) )
	{
		T_QueueElementPtr Element;
		T_TaskTCB_Ptr ActualTask;
		vPortEnterCritical();
		ActualTask = Scheduler_GetActualTask();
		if (ActualTask != NULL)
		{
			while(Queue_DequeueElement(&Handler->MsgUsedQueue, &Element) != QUEUE_OK)
			{
				Res = Queue_Enqueue(&Handler->MsgRxWaitQueue, &ActualTask->QueueElement, ActualTask);
				if(Res == QUEUE_OK)
				{
					ActualTask->TaskStatus = TASK_BLOCKED;
					vPortExitCritical();
					Port_SoftwareInterrupt();
				}
				else
				{
					vPortExitCritical();
					Task_Sleep(10);
				}
				vPortEnterCritical();
			}
			memcpy(Element->DataPtr, Msg, Handler->MsgSize);
			Res = Queue_Enqueue(&Handler->MsgFreeQueue, Element, Element->DataPtr);
			if(Res == QUEUE_OK)
			{
				if(Queue_GetElementsCount(&Handler->MsgTxWaitQueue) > 0)
				{
					T_TaskTCB_Ptr WakeTask;
					Res = Queue_Dequeue(&Handler->MsgTxWaitQueue,(pv *) &WakeTask);
					if((Res == QUEUE_OK) && (WakeTask != NULL))
					{
						Res = Scheduler_SetReady(WakeTask);
						if(Res == SCHEDULER_OK)
						{
							if((ActualTask->TaskPriority > WakeTask->TaskPriority))
							{
								CallScheduler = True;
							}
						}
					}
				}
			}
		}

		vPortExitCritical();
		if(CallScheduler)
		{
			Port_SoftwareInterrupt();
		}

	}
	return Res;
}
