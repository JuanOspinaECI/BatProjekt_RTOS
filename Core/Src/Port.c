/*
 * Port.c
 *
 *  Created on: Sep 24, 2023
 *      Author: JuanOsp
 *
 */


#include "AppTypes.h"
#include "Port.h"

#define SYSTEM_CLOCK		8000000
#define TICK_CLOCK			1000
#define STACK_CHECK_VALUE 	0xAAAAAAAA
#define STACK_CHECK_SIZE	20

//u32 SaveContex();
u32 PortActualSp = 0;
static unsigned long long  PortSystemTicks = 0;
u32 static uxCriticalNesting = 0xaaaaaaaa;
/*u8 Stack1[256];
u8 Stack2[256];
u8 StackUsed = 0;
pu32 Stack1 = 0;
pu32 Stack2 = 0;*/

/* ----------------------------------------------------------*/

	//portFORCE_INLINE static long xPortIsInsideInterrupt( void )
inline __attribute__( ( always_inline ) ) static u8 xPortIsInsideInterrupt( void )
	{
		u32 ulCurrentInterrupt;
		u8 xReturn;

		 /* Obtain the number of the currently executing interrupt. */
		 asm volatile ( "mrs %0, ipsr": "=r" ( ulCurrentInterrupt ):: "memory" );

		 if( ulCurrentInterrupt == 0)
		 {
			 xReturn = False;
		 }
		 else
		 {
			 xReturn = True;

		 }

		return xReturn;
	}

/* ------------------------------------------------------------------ */

portFORCE_INLINE static void vPortRaiseBASEPRI( void )
	{
		u32 ulNewBASEPRI;

		__asm volatile
		(
				"	MOV %0, %1											\n"\
				"	msr basepri, %0										\n"\
				"	isb													\n"\
				"	dsb													\n"\
				:	"=r" (ulNewBASEPRI) : "i" ( configMAX_SYSCALL_INTERRUPT_PRIORITY ) : "memory"
		);
	}

void vPortEnterCritical( void )
{
	//if(!xPortIsInsideInterrupt())
	{
		__asm volatile ("cpsid i" : : : "memory");
		uxCriticalNesting++;
	}
}
/*-----------------------------------------------------------*/

portFORCE_INLINE static u32 ulPortRaiseBASEPRI()
{
	u32 ulOriginalBASEPRI, ulNewBASEPRI;

	__asm volatile
	(
			" mrs %0, basepri								\n"\
			" mov %1, %2									\n"\
			" msr basepri, 1%								\n"\
			" isb											\n"\
			" dsb											\n"\
			: "=r" ( ulOriginalBASEPRI ), "=r" ( ulNewBASEPRI ) : "i" (configMAX_SYSCALL_INTERRUPT_PRIORITY) : "memory"
	);

	return ulOriginalBASEPRI;
}

	portFORCE_INLINE static void vPortSetBASEPRI ( u32 ulNewMaskValue )
	{
		__asm volatile
		(
				"	msr basepri, %0 "::"r" ( ulNewMaskValue ) : "memory"
		);
	}

void vPortExitCritical( void )
{
	//if(!xPortIsInsideInterrupt())
	{
		if(uxCriticalNesting)
		{
			uxCriticalNesting--;
		}
		if( uxCriticalNesting == 0 )
		{
			__asm volatile ("cpsie i" : : : "memory");
		}
	}
}
/*-----------------------------------------------------------*/


extern u32 Scheduler_GetNextTask(u32 ActualSp);

void Port_InitTickTimer() {

	//Stack1 = Port_InitTaskStack((pu32)StackBuff1, 256, Task1);
	//Stack2 = Port_InitTaskStack((pu32)StackBuff1, 256, Task1);

	/////////////////////////////////////////////////

	pu32 pSTK_CTRL = (pu32)0xE000E010;
	pu32 pSTK_LOAD = (pu32)0xE000E014;

	u32 TickCountValue = (SYSTEM_CLOCK/TICK_CLOCK) - 1;

	*pSTK_LOAD &= ~(0x00FFFFFF);

	*pSTK_LOAD |= TickCountValue;

	*pSTK_CTRL |= (1 << 2 ); // Fuente reloj de la CPU

	*pSTK_CTRL |= (1 << 1);  // Habilitamos interrupcion del Systick

	*pSTK_CTRL |= (1 << 0);  //Habilitamos Systick

	///////////////////////////////

	// restore context
}

u8 Port_CheckStackOverFlow(pu32 Stack)
{
	u8 Res = False;
	u32 Index = STACK_CHECK_SIZE;
	while(Index)
	{
		Index--;
		if(Stack[Index] != STACK_CHECK_VALUE)
		{
			Res = True;
			break;
		}
	}
	return Res;
}

void Port_InitClearStack(pu32 Stack, u32 Size)
{
	while(Size)
	{
		*Stack = STACK_CHECK_VALUE;
		Stack++;
		Size--;
	}
}

pu32 Port_InitTaskStack(pu32 Stack, u32 Size, void *Function)
{
	Port_InitClearStack(Stack, Size);
	Stack += (Size); // dividido 4
	Stack--;
	*Stack = 0x01000000; // PSR
	Stack--;
	*Stack = (u32)Function & 0xfffffffeUL; //PC
	*Stack |= 0x00000001;
	Stack--;
	*Stack = (u32)Function & 0xfffffffeUL; //LR
	//*Stack |= 0x00000001;
	Stack--;
	*Stack = 0; // R12
	Stack--;
	*Stack = 0; // R3
	Stack--;
	*Stack = 0; // R2
	Stack--;
	*Stack = 0; // R1
	Stack--;
	*Stack = 0; // R0
	Stack--;
	*Stack = 0xFFFFFFF9; // LR
	Stack--;
	*Stack = 0; // R7
	Stack--;
	*Stack = 0; // R11
	Stack--;
	*Stack = 0; // R10
	Stack--;
	*Stack = 0; // R9
	Stack--;
	*Stack = 0; // R8
	Stack--;
	*Stack = 0; // R7
	Stack--;
	*Stack = 0; // R6
	Stack--;
	*Stack = 0; // R5
	Stack--;
	*Stack = 0; // R4
	//Stack--;// no se requiere porque ne stm siempre se apunta al ultimo elemento ubicado en el stack
	return Stack;
}

void Port_RestoreFirstContext(u32 Sp)
{
	uxCriticalNesting = 0;
	vPortExitCritical();
	__asm volatile("MOV R10,%0" :: "r"(Sp));
	__asm volatile("MSR MSP,R10");
	__asm volatile("POP {R4-R11}");
	__asm volatile("POP {R7,LR}");
	__asm volatile("POP {R0-R3}");
	__asm volatile("POP {R12}");
	__asm volatile("POP {LR}");
	__asm volatile("POP {LR}");
	__asm volatile("POP {R10}");
	__asm volatile("MSR PSR,R10");
	__asm volatile("MOV R10,#0");
	__asm volatile("BX LR");

}

unsigned long long  Port_GetSystemTicks()
{
	return PortSystemTicks;
}

void Task1(void)
{
	u32 Count = 0;
	for(;;)
	{
		if(Count < 1000000000)
		{
			Count++;
		}
		else
		{
			Count = 0;
		}
	}
}

void Task2(void)
{
	for(;;){}
}



__attribute__( ( always_inline) ) static inline void Port_SaveContex(void)
{
	__asm volatile("PUSH {R7,LR}");			//Incluir R7 si la interrupcion es naked
	__asm volatile("PUSH {R4-R11}");
	__asm volatile("MRS R10,MSP");
	__asm volatile("MOV %0,R10": "=r"(PortActualSp));
}

__attribute__( ( always_inline) ) static inline void Port_RestoreContex(void)
{
	__asm volatile("MOV R10,%0" : : "r"(PortActualSp));
	__asm volatile("MSR MSP, R10");
	__asm volatile("POP {R4-R11}");			//R7 incluido por la int naked
	__asm volatile("NOP");					//Incluir si la interrupcion es naked
	__asm volatile("POP {R7,PC}");			//Incluir si la interrupcion es naked
}

__attribute__ ( ( naked ) ) void SysTick_Handler(void)
{
	//HAL_IncTick();

	Port_SaveContex();

	PortSystemTicks++;

	PortActualSp = Scheduler_GetNextTask(PortActualSp);

	Port_RestoreContex();

}

__attribute__ ( ( naked ) ) void PendSV_Handler( void )
{

	Port_SaveContex();

	PortActualSp = Scheduler_GetNextTask(PortActualSp);

	Port_RestoreContex();

}

