/*
 * Queue.c
 *
 *  Created on: Sep 24, 2023
 *      Author: maqui
 */

#include "Apptypes.h"
#include "Queue.h"
#include <stdint.h>
#include "Port.h"

u16 Queue_Init(T_QueueHandlerPtr Queue)
{
	if(Queue != NULL)
	{
		vPortEnterCritical();
		Queue->ElementsCount = 0;
		Queue->Head = NULL;
		Queue->Tail = NULL;
		vPortExitCritical();
		return QUEUE_OK;
	}
	return QUEUE_ERR_NULL_PARAM;
}

u16 Queue_GetElementsCount(T_QueueHandlerPtr Queue)
{
	if(Queue != NULL)
	{
		return Queue->ElementsCount;
	}return 0;
}


u16 Queue_Enqueue(T_QueueHandlerPtr Queue, T_QueueElementPtr Element, pv Data)
{
	if((Queue != NULL) && (Element != NULL))
		{
			vPortEnterCritical();
			Element->DataPtr = Data;
			Element->Next = NULL;
			if(Queue->Head == NULL)
			{
				Queue->Head = Element;
				Queue->Tail = Element;
				Queue->ElementsCount = 1;
			}
			else
			{
				Queue->Tail->Next = Element;
				Queue->Tail = Element;
				Queue->ElementsCount++;
			}
			vPortExitCritical();
			return QUEUE_OK;
		}return QUEUE_ERR_NULL_PARAM;
}

u16 Queue_Dequeue(T_QueueHandlerPtr Queue, pv * Data)
{
	if((Queue != NULL) && (Data != NULL))
	{
		vPortEnterCritical();
		if(Queue->Head != NULL)
		{
			*Data = Queue->Head->DataPtr;
			Queue->Head = Queue->Head->Next;
			Queue->ElementsCount--;
			if(Queue->Head == NULL)
			{
				Queue->Tail = NULL;
				Queue->ElementsCount = 0;
			}
			vPortExitCritical();
			return QUEUE_OK;
		}
		Queue->ElementsCount = 0;
		Queue->Tail = NULL;
		vPortExitCritical();
		return QUEUE_ERR_EMPTY;
	}
	return QUEUE_ERR_NULL_PARAM;
}

u16 Queue_DequeueElement(T_QueueHandlerPtr Queue, T_QueueElementPtr * Element)
{
	if((Queue != NULL) && (Element != NULL))
	{
		vPortEnterCritical();
		if(Queue->Head != NULL)
		{
			*Element = Queue->Head;
			Queue->Head = Queue->Head->Next;
			Queue->ElementsCount--;
			if(Queue->Head == NULL)
			{
				Queue->Tail = NULL;
				Queue->ElementsCount = 0;
			}
			vPortExitCritical();
			return QUEUE_OK;
		}
		Queue->ElementsCount = 0;
		Queue->Tail = NULL;
		vPortExitCritical();
		return QUEUE_ERR_EMPTY;
	}
	return QUEUE_ERR_NULL_PARAM;
}

u16 Queue_Copy(T_QueueHandlerPtr Origin, T_QueueHandlerPtr Destinado)
{
	u16 Res = QUEUE_ERR_NULL_PARAM;
	if((Origin != NULL) && (Destinado != NULL))
	{
		vPortEnterCritical();
		Destinado->Head = Origin->Head;
		Destinado->Tail = Origin->Tail;
		Destinado->ElementsCount = Origin->ElementsCount;
		vPortExitCritical(); // ******
		Res = QUEUE_OK;
	}
	return Res;
}
