/*
 * Scheduler.c
 *
 *  Created on: Oct 1, 2023
 *      Author: maqui
 */


#include "Apptypes.h"
#include "Queue.h"
#include "Rtos.h"
#include "Port.h"
#include "Scheduler.h"



T_QueueHandler ReadyTaskQueue[RTOS_SYSTEM_PRIORITIES];
T_TaskTCB_Ptr ActualTaskPtr = NULL;
T_TaskTCB IdleTaskHandler;
u32 IdleTaskStack[RTOS_MIN_STACK_SIZE];
T_QueueHandler StackOverFlowTaskQueue;

//extern u16 Task_Create(T_TaskTCB_Ptr TCB, u16 Id, const char *Name, u8 Priority, pu32 Stack, u32 StackSize, TaskCallback Function);


extern void Task_CheckTimeBlockedTask();

//extern u32 uxCriticalNesting;

u16 Scheduler_CreateIdleTask();
u16 Scheduler_SetStackOverFlowTask(T_TaskTCB_Ptr Task);
void IdleTask();


u16 Scheduler_Init()
{
	u16 Res = SCHEDULER_OK;
	u8 i 	= 0;
	ActualTaskPtr = NULL;
	Res = Scheduler_CreateIdleTask();
	if(Res)
	{
		return Res;
	}
	for(i = 0; i < RTOS_SYSTEM_PRIORITIES; i++ )
	{
		Res = Queue_Init(&ReadyTaskQueue[i]);
		if(Res)
		{
			break;
		}
	}
	if(Res == SCHEDULER_OK)
	{
		Res = Queue_Init(&ReadyTaskQueue[i]);
	}
	return Res;
}

u16 Scheduler_SetReady(T_TaskTCB_Ptr Task)
{
	u16 Res = SCHEDULER_ERR_NULL_PARAM;
	if (Task != NULL)
	{
		if (Task->TaskPriority > RTOS_MAX_TASK_PRIORITY)
		{
			Task->TaskPriority = RTOS_MAX_TASK_PRIORITY;
		}
		Task->TaskStatus = TASK_READY;
		Res = Queue_Enqueue(&ReadyTaskQueue[Task->TaskPriority],&Task->QueueElement, Task);
		if(Res)
		{
			Task->TaskStatus = TASK_BLOCKED;
		}
	}
	return Res;
}

u32 Scheduler_GetNextTask(u32 ActualSp)
{
	u16 Priority;
	T_TaskTCB_Ptr Temp = NULL;
	if(ActualSp)
	{
		if(ActualTaskPtr != NULL)
		{
			ActualTaskPtr->TaskActualSP = ActualSp;
			if(Port_CheckStackOverFlow(ActualTaskPtr->TaskStack))
			{
				ActualTaskPtr->TaskStackOverFlow = True;
			}
			if(ActualTaskPtr != &IdleTaskHandler)
			{
				if(ActualTaskPtr->TaskStatus == TASK_RUNNING)
				{
					if(!ActualTaskPtr->TaskStackOverFlow)
					{
						Scheduler_SetReady(ActualTaskPtr);
					}
					else
					{
						Scheduler_SetStackOverFlowTask(ActualTaskPtr);
					}
				}
			}
			else
			{
				IdleTaskHandler.TaskStatus = TASK_READY;
			}

		}
		ActualTaskPtr = NULL;
	}
	for(Priority = 0; Priority < RTOS_MAX_TASK_PRIORITY; Priority++)
	{
		if(Queue_GetElementsCount(&ReadyTaskQueue[Priority]) > 0)
		{
			if(Queue_Dequeue(&ReadyTaskQueue[Priority], (pv *)&Temp) == QUEUE_OK)
			{
				ActualTaskPtr = Temp;
				break;
			}
		}
	}
	if(ActualTaskPtr == NULL)
	{
		ActualTaskPtr = &IdleTaskHandler;
	}
	ActualTaskPtr->TaskStatus = TASK_RUNNING;
	return ActualTaskPtr->TaskActualSP;
}

u16 Scheduler_CreateIdleTask()
{
	IdleTaskHandler.TaskId					= RTOS_IDLE_TASK_ID;
	IdleTaskHandler.TaskName				= RTOS_IDLE_TASK_NAME;
	IdleTaskHandler.TaskPriority 			= RTOS_SYSTEM_PRIORITIES;
	IdleTaskHandler.TaskStack				= (pu32)IdleTaskStack;
	IdleTaskHandler.TaskStackOverFlow		= False;
	IdleTaskHandler.TaskStackSize			= RTOS_MIN_STACK_SIZE;
	IdleTaskHandler.TaskFunction			= IdleTask;
	IdleTaskHandler.TaskStatus				= TASK_READY;
	IdleTaskHandler.TaskWaitEvents 		= 0;
	IdleTaskHandler.TaskActualSP			= (u32)Port_InitTaskStack(IdleTaskHandler.TaskStack, IdleTaskHandler.TaskStackSize,IdleTaskHandler.TaskFunction);
	IdleTaskHandler.QueueElement.DataPtr	= &IdleTaskHandler;
	IdleTaskHandler.QueueElement.Next 		= NULL;
	return SCHEDULER_OK;
}

u16 Scheduler_SetStackOverFlowTask(T_TaskTCB_Ptr Task)
{
	u16 Res = SCHEDULER_ERR_NULL_PARAM;
	if(Task != NULL)
	{
		if(Task->TaskPriority > RTOS_MAX_TASK_PRIORITY)
			Task->TaskPriority = RTOS_MAX_TASK_PRIORITY;

	Task->TaskStatus = TASK_BLOCKED;
	Res = Queue_Enqueue(&StackOverFlowTaskQueue,&Task->QueueElement, Task);
	}
	return Res;
}

void Scheduler_Start()
{
	u16 Res = SCHEDULER_OK;
	//Res = Scheduler_Init();
	u32 FirstSp = Scheduler_GetNextTask(0);
	Port_InitTickTimer();
	//uxCriticalNesting = 0;
	Port_RestoreFirstContext(FirstSp);


}

T_TaskTCB_Ptr Scheduler_GetActualTask()
{
	return ActualTaskPtr;
}

void IdleTask()
{
	for(;;)
	{
		Task_CheckTimeBlockedTask();
	}
}
