/*
 * Events.c
 *
 *  Created on: Nov 18, 2023
 *      Author: maqui
 */

#include "Events.h"

u16 Events_Init(T_EventHanlderPtr Handler)
{
	u16 Res = EVENTS_ERR_NULL_PARAM;
	if(Handler != NULL)
	{
		Handler->Events = 0;
		Res = Queue_Init(&Handler->WaitQueue);
	}
	return Res;
}
u16 Events_WaitAny(T_EventHanlderPtr Handler, u16 WaitMask)
{
	u16 Res = EVENTS_ERR_NULL_PARAM;
	u8 CallScheduler = False;
	if(Handler != NULL)
	{
		WaitMask &= EVENTS_WAIT_MASK;
		if(WaitMask)
		{
			vPortEnterCritical();
			if((Handler->Events & WaitMask) == 0)
			{
				T_TaskTCB_Ptr Task = Scheduler_GetActualTask();
				if(Task != NULL)
				{
					Task->TaskWaitEvents 	= WaitMask;
					Task->TaskStatus 		= TASK_BLOCKED;
					Res = Queue_Enqueue(&Handler->WaitQueue, &Task->QueueElement, Task);
					if (Res != QUEUE_OK)
					{
						Task->TaskStatus 		= TASK_RUNNING;
					}
					else
					{
						CallScheduler = True;
					}
				}
			}
			else
			{
				Res = EVENTS_OK;
			}
			vPortExitCritical();
			if(CallScheduler)
			{
				Port_SoftwareInterrupt();
			}
		}
		else
		{
			Res = EVENTS_OK;
		}
	}
	return Res;
}

u16 Events_WaitAll(T_EventHanlderPtr Handler, u16 WaitMask)
{
	u16 Res = EVENTS_ERR_NULL_PARAM;
	u8 CallScheduler = False;
	if(Handler != NULL)
	{
		WaitMask &= EVENTS_WAIT_MASK;
		if(WaitMask)
		{
			vPortEnterCritical();
			if((Handler->Events & WaitMask) != WaitMask)
			{
				T_TaskTCB_Ptr Task = Scheduler_GetActualTask();
				if(Task != NULL)
				{
					WaitMask |= EVENTS_WAIT_ALL;
					Task->TaskWaitEvents 	= WaitMask;
					Task->TaskStatus 		= TASK_BLOCKED;
					Res = Queue_Enqueue(&Handler->WaitQueue, &Task->QueueElement, Task);
					if (Res != QUEUE_OK)
					{
						Task->TaskStatus 	= TASK_RUNNING;
					}
					else
					{
						CallScheduler = True;
					}
				}
			}
			else
			{
				Res = EVENTS_OK;
			}
			vPortExitCritical();
			if(CallScheduler)
			{
				Port_SoftwareInterrupt();
			}
		}
		else
		{
			Res = EVENTS_OK;
		}
	}
	return Res;
}

u16 Events_Clear(T_EventHanlderPtr Handler, u16 WaitMask)
{
	u16 Res = EVENTS_ERR_NULL_PARAM;
	if(Handler != NULL)
	{
		vPortEnterCritical();
		Handler->Events &= ~WaitMask;
		Handler->Events &= EVENTS_WAIT_MASK;
		Res = EVENTS_OK;
		vPortExitCritical();
	}
	return Res;
}

u16 Events_Set(T_EventHanlderPtr Handler, u16 WaitMask)
{
	u16 Res = EVENTS_ERR_NULL_PARAM;
	u8 CallScheduler = False;
	if(Handler != NULL)
	{
		if(WaitMask)
		{
			T_QueueHandler Temp;
			T_TaskTCB_Ptr Task;
			T_TaskTCB_Ptr ActualTask;
			vPortEnterCritical();
			Handler->Events |= WaitMask;
			Res = Queue_Init(&Temp);
			if(Res == QUEUE_OK)
			{
				ActualTask = Scheduler_GetActualTask();
				while(Queue_GetElementsCount(&Handler->WaitQueue))
				{
					if(Queue_Dequeue(&Handler->WaitQueue,(pv *)&Task)==QUEUE_OK)
					{
						if(Task->TaskWaitEvents & EVENTS_WAIT_ALL)
						{
							if((Handler->Events & (Task->TaskWaitEvents & EVENTS_WAIT_MASK)) == (Task->TaskWaitEvents & EVENTS_WAIT_ALL) )
							{
								if(Scheduler_SetReady(Task) != SCHEDULER_OK )
								{
									Task->TaskWaitEvents = 0;
									if(ActualTask != NULL)
									{
										if(ActualTask->TaskPriority > Task->TaskPriority)
										{
											CallScheduler = True;
										}
									}
								}
								else
								{
									Queue_Enqueue(&Temp, &Task->QueueElement, Task);
								}
							}
							else
							{
								Queue_Enqueue(&Temp, &Task->QueueElement, Task);
							}
						}
						else
						{
							if(Handler->Events & (Task->TaskWaitEvents & EVENTS_WAIT_MASK))
							{
								if(Scheduler_SetReady(Task) == SCHEDULER_OK)
								{
									Task->TaskWaitEvents = 0;
									if(ActualTask != NULL)
									{
										if(ActualTask->TaskPriority > Task->TaskPriority)
										{
											CallScheduler = True;
										}
									}
								}
								else
								{
									Queue_Enqueue(&Temp, &Task->QueueElement, Task);
								}
							}
							else
							{
								Queue_Enqueue(&Temp, &Task->QueueElement, Task);
							}
						}
					}
				}
				Queue_Copy(&Temp, &Handler->WaitQueue);
			}
			vPortExitCritical();
			if(CallScheduler)
			{
				Port_SoftwareInterrupt();
			}
		}
		else
		{
			Res = EVENTS_OK;
		}

	}
	return Res;
}
