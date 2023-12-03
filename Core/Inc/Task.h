/*
 * task.h
 *
 *  Created on: Sep 24, 2023
 *      Author: maqui
 */

#ifndef TASK_H_
#define TASK_H_

#include "Apptypes.h"
#include "Queue.h"
#include "Rtos.h"

#ifndef TASK_BASE_ERROR
#define TASK_BASE_ERROR			0x0400
#endif


#define TASK_OK						0
#define TASK_ERR_UNDEF				TASK_BASE_ERROR | 0x00FF
#define TASK_ERR_NULL_PARAM			TASK_BASE_ERROR | 0x00FE
#define TASK_ERR_PARAM_OUT_RANGE	TASK_BASE_ERROR | 0x00FD




	// Definiciones de funciones de modulo


u16 Task_Init();
u16 Task_Create (T_TaskTCB_Ptr TCB, u16 Id, const char * Name, u8 Priority, pu32 Stack, u32 StackSize, TaskCallback Function);
u16 Task_Sleep(u32 SleepTimeTicks);


#endif /* TASK_H_ */
