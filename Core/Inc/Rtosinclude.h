/*
 * Rtosinclude.h
 *
 *  Created on: Oct 8, 2023
 *      Author: Juan Osp
 */

#ifndef RTOSINCLUDE_H_
#define RTOSINCLUDE_H_

#include "Apptypes.h"
#include "Rtos.h"
#include "Task.h"
#include "Scheduler.h"
#include "Port.h"
#include "Messages.h"


#define RTOS_Start()		Scheduler_Start()
#define RTOS_Init()			\
{							\
	Scheduler_Init();		\
	Task_Init();			\
}

//extern void vPortEnterCritical( void );
//extern void vPortExitCritical( void );



#endif /* RTOSINCLUD1E_H_ */
