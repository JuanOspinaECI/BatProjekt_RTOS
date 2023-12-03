/*
 * Scheduler.h
 *
 *  Created on: Oct 1, 2023
 *      Author: JuanOsp
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "Rtos.h"
#include "Apptypes.h"

#ifndef SCHEDULER_BASE_ERROR
#define SCHEDULER_BASE_ERROR			0x0300
#endif


#define SCHEDULER_OK					0
#define SCHEDULER_ERR_UNDEF				SCHEDULER_BASE_ERROR | 0x00FF
#define SCHEDULER_ERR_NULL_PARAM		SCHEDULER_BASE_ERROR | 0x00FE

u16 Scheduler_Init();
u16 Scheduler_SetReady(T_TaskTCB_Ptr Task);
u32 Scheduler_GetNextTask(u32 ActualSp);
u16 Scheduler_CreateIdleTask();
u16 Scheduler_SetStackOverFlowTask(T_TaskTCB_Ptr Task);
void Scheduler_Start();
T_TaskTCB_Ptr Scheduler_GetActualTask();

#endif /* SCHEDULER_H_ */
