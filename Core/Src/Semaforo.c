/*
 * Mutex.c
 *
 *  Created on: Nov 11, 2023
 *      Author:
 */

#include "Semaforo.h"


u16 Semaforo_Init(T_SemaforoHandler_Ptr sem, u16 Limit)
{
	u16 Res = SEM_ERR_NULL_PARAM;
	if(sem != NULL)
	{
		Res = SEM_ERR_PARAM_OUT_RANGE;
		if(Limit > 0)
		{
			sem->Limit = Limit;
			sem->Count = Limit;
			Res = Queue_Init(&sem->WaitQueue);
		}


	}
	return Res;
}

u16 Semaforo_Take(T_SemaforoHandler_Ptr sem)
{
	u8 CallScheduler = False;
	u16 Res = SEM_ERR_NULL_PARAM;
	if(sem != NULL)
	{
		vPortEnterCritical();

		if(sem->Count == 0)
		{
			Res = SEM_ERR_UNDEFF;
			T_TaskTCB_Ptr Task = Scheduler_GetActualTask();
			if(Task != NULL)
			{
				Res = Queue_Enqueue(&sem->WaitQueue, &Task->QueueElement,(pv)Task);
				if(Res== QUEUE_OK)
				{
					Task->TaskStatus = TASK_BLOCKED;
					CallScheduler = True;
				}
			}
		}
		else
		{
			sem->Count--;
			Res = SEM_OK;
		}

		vPortExitCritical();
		if(CallScheduler)
		{
			Port_SoftwareInterrupt();
		}
	}
	return Res;

}
u16 Semaforo_Give(T_SemaforoHandler_Ptr sem)
{
	u8 CallScheduler = False;
	u16 Res = SEM_ERR_NULL_PARAM;
	if(sem != NULL)
	{
		vPortEnterCritical();
		if(Queue_GetElementsCount(&sem->WaitQueue))
		{
			T_TaskTCB_Ptr WakeTask;
			Res = Queue_Dequeue(&sem->WaitQueue, (pv *)WakeTask);
			if(Res == QUEUE_OK)
			{
				Res = SEM_ERR_NULL_PARAM;
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
			if(sem->Count < sem->Limit)
			{
				sem->Count++;
			}

			Res = SEM_OK;
		}

		vPortExitCritical();
		if(CallScheduler)
		{
			Port_SoftwareInterrupt();
		}
	}

	return Res;

}


/*
 * Semaforo.c
 *
 *  Created on: Nov 15, 2023
 *      Author: maqui
 */


