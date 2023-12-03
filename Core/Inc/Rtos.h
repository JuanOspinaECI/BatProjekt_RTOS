/*
 * Rtos.h
 *
 *  Created on: Oct 1, 2023
 *      Author: maqui
 */

#ifndef RTOS_H_
#define RTOS_H_

#include "Apptypes.h"
#include "Queue.h"


typedef void (*TaskCallback)(void);


typedef enum TaskStatus
{
	TASK_BLOCKED = 0,
	TASK_READY,
	TASK_RUNNING
}T_TaskStatus, *T_TaskStatus_Ptr;

typedef struct
{
	const char *TaskName;
	u16 TaskId;
	T_TaskStatus TaskStatus;
	u8 TaskStackOverFlow;
	u8 TaskPriority;
	pu32 TaskStack;
	u32 TaskStackSize;
	u32 TaskActualSP;
	u16 TaskWaitEvents;
	TaskCallback TaskFunction;
	T_QueueElement QueueElement;
	unsigned long long TaskSleepTime;
}T_TaskTCB, *T_TaskTCB_Ptr;

#define RTOS_SYSTEM_PRIORITIES		16
#define RTOS_MAX_TASK_PRIORITY 		RTOS_SYSTEM_PRIORITIES-1
#define RTOS_MIN_STACK_SIZE 		128
#define RTOS_IDLE_TASK_ID			0xFFFF
#define RTOS_IDLE_TASK_NAME			"IDLE_TASK"

#endif /* RTOS_H_ */
