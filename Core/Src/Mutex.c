/*
 * Mutex.c
 *
 *  Created on: Nov 11, 2023
 *      Author:
 */

#include "Mutex.h"


u16 Mutex_Init(T_MutexHandler_Ptr mutex)
{
	u16 Res = MUTEX_ERR_NULL_PARAM;
	if(mutex != NULL)
	{
		mutex->Status = MUTEX_FREE;
		Res = Queue_Init(&mutex->WaitQueue);

	}
	return Res;
}

u16 Mutex_Take(T_MutexHandler_Ptr mutex)
{
	u8 CallScheduler = False;
	u16 Res = MUTEX_ERR_NULL_PARAM;
	if(mutex != NULL)
	{
		vPortEnterCritical();

		if(mutex->Status != MUTEX_FREE)
		{
			Res = MUTEX_ERR_UNDEFF;
			T_TaskTCB_Ptr Task = Scheduler_GetActualTask();
			if(Task != NULL)
			{
				Res = Queue_Enqueue(&mutex->WaitQueue, &Task->QueueElement,(pv)Task);
				if(Res== QUEUE_OK)
				{
					Task->TaskStatus = TASK_BLOCKED;
					CallScheduler = True;
				}
			}
		}
		else
		{
			mutex->Status = MUTEX_BUSY;
			Res = MUTEX_OK;
		}

		vPortExitCritical();
		if(CallScheduler)
		{
			Port_SoftwareInterrupt();
		}
	}
	return Res;

}
u16 Mutex_Give(T_MutexHandler_Ptr mutex)
{
	u8 CallScheduler = False;
	u16 Res = MUTEX_ERR_NULL_PARAM;
	if(mutex != NULL)
	{
		vPortEnterCritical();
		if(Queue_GetElementsCount(&mutex->WaitQueue))
		{
			T_TaskTCB_Ptr WakeTask;
			Res = Queue_Dequeue(&mutex->WaitQueue, (pv *)WakeTask);
			if(Res == QUEUE_OK)
			{
				Res = MUTEX_ERR_NULL_PARAM;
				if(WakeTask != NULL)
				{
					Res = Scheduler_SetReady(WakeTask);
					if(Res == SCHEDULER_OK)
					{
						T_TaskTCB_Ptr ThisTask = Scheduler_GetActualTask();
						if(ThisTask != NULL)
						{
							if(ThisTask->TaskPriority > WakeTask->TaskPriority)
							{
								CallScheduler = True;
							}
						}
					}
				}
			}
		}
		else
		{
			mutex->Status = MUTEX_FREE;
			Res = MUTEX_OK;
		}

		vPortExitCritical();
		if(CallScheduler)
		{
			Port_SoftwareInterrupt();
		}
	}

	return Res;

}


