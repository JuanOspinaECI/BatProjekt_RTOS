/*
 * Task.c
 *
 *  Created on: Sep 24, 2023
 *      Author: JuanOsp
 */

#include <Task.h>
#include "AppTypes.h"
#include "Port.h"
#include "Rtos.h"
#include "Scheduler.h"
#include "Rtosinclude.h"

T_QueueHandler TaskTimeBlocked;

void Task_CheckTimeBlockedTask();


u16 Task_Init()
{
	return Queue_Init(&TaskTimeBlocked);

}


u16 Task_Create(T_TaskTCB_Ptr TCB, u16 Id, const char *Name, u8 Priority, pu32 Stack, u32 StackSize, TaskCallback Function)
{
	u16 Res = TASK_ERR_NULL_PARAM;
	if((TCB != NULL) && (Stack != NULL) && (Function != NULL))
	{
		Res = TASK_ERR_PARAM_OUT_RANGE;
		vPortEnterCritical();
		if(StackSize >= RTOS_MIN_STACK_SIZE)
		{
			TCB->TaskId			= Id;
			TCB->TaskName		= Name;
			if(Priority > RTOS_MAX_TASK_PRIORITY)
			{
				TCB -> TaskPriority = RTOS_MAX_TASK_PRIORITY;
			}
			else
			{
				TCB -> TaskPriority = Priority;
			}
			TCB->TaskStackOverFlow = False;
			TCB->TaskStack		= Stack;
			TCB->TaskStackSize	= StackSize;
			TCB->TaskFunction	= Function;
			TCB->TaskStatus		= TASK_READY;
			TCB->TaskWaitEvents = 0;
			TCB->TaskActualSP	= (u32)Port_InitTaskStack(TCB->TaskStack, TCB->TaskStackSize,TCB->TaskFunction);
			TCB->QueueElement.DataPtr = TCB;
			TCB->QueueElement.Next = NULL;
			TCB->TaskSleepTime = 0;
			Res = Scheduler_SetReady(TCB);
		}
		vPortExitCritical();

	}
	return Res;

}

u16 Task_Sleep(u32 SleepTimeTicks)
{
	u8 CallScheduller = False;
	u16 Res = TASK_ERR_PARAM_OUT_RANGE;
	if(SleepTimeTicks)
	{
		vPortEnterCritical();
		Res = TASK_ERR_NULL_PARAM;
		T_TaskTCB_Ptr TaskPtr = Scheduler_GetActualTask();
		if(TaskPtr != NULL)
		{
			if((TaskPtr->TaskId != RTOS_IDLE_TASK_ID)&&(TaskPtr->TaskPriority < RTOS_SYSTEM_PRIORITIES))
			{
				if(!Port_CheckStackOverFlow(TaskPtr->TaskStack))
				{
					TaskPtr->TaskSleepTime = Port_GetSystemTicks();
					TaskPtr->TaskSleepTime += SleepTimeTicks;
					TaskPtr->TaskStatus = TASK_BLOCKED;
					TaskPtr->QueueElement.DataPtr = TaskPtr;
					TaskPtr->QueueElement.Next = NULL;
					Res = Queue_Enqueue(&TaskTimeBlocked, &TaskPtr->QueueElement, TaskPtr);
					if(Res == QUEUE_OK)
					{
						CallScheduller = True;
					}
					else
					{
						TaskPtr->TaskStatus = TASK_RUNNING;
					}
				}
				else
				{
					TaskPtr->TaskStackOverFlow = True;
					Res = Scheduler_SetStackOverFlowTask(TaskPtr);
					CallScheduller = True;
				}
			}
			else
			{
				Res = TASK_ERR_UNDEF;
			}
			vPortExitCritical();
			if (CallScheduller)
			{
				Port_SoftwareInterrupt();
			}
		}
	}
	return Res;
}

void Task_CheckTimeBlockedTask()
{
	u16 Res = 0;
	unsigned long long Time;
	T_QueueHandler TempQueue;
	vPortEnterCritical();
	Time = Port_GetSystemTicks();
	Res = Queue_Init(&TempQueue);
	if(Res == QUEUE_OK)
	{
		T_TaskTCB_Ptr TaskPtr;
		while(Queue_Dequeue(&TaskTimeBlocked, (void **)&TaskPtr) == QUEUE_OK)
		{
			if(TaskPtr != NULL)
			{
				if(TaskPtr->TaskSleepTime > Time)
				{
					Res = Queue_Enqueue(&TempQueue, &TaskPtr->QueueElement, TaskPtr);
				}
				else
				{
					Scheduler_SetReady(TaskPtr);
				}
			}
		}
		Res = Queue_Copy(&TempQueue, &TaskTimeBlocked);
	}
	vPortExitCritical();
}


