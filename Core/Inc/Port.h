/*
 * Port.h
 *
 *  Created on: Sep 24, 2023
 *      Author: JuanOSp
 */

#ifndef PORT_H_
#define PORT_H_

#include "Apptypes.h"

#define Port_SoftwareInterrupt()										\
{														\
	/* Set a PendSV to request a context switch. 	*/	\
	portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;		\
														\
	/* Barriers are normaly not required but do ensure the code is completely \
	 * Withthin the specified behavior for the architecture. */				\
	__asm volatile ("dsb" ::: "memory");				\
	__asm volatile ("isb");								\
}

#define portNVIC_INT_CTRL_REG 		(*( ( volatile u32 * ) 0xe000ed04 ) )
#define portNVIC_PENDSVSET_BIT		( 1UL << 28UL )

#define RCC_APB2ENR					(( ( volatile u32 * ) 0xe40021018 ) )
#define RCC_APB2ENR_IOPC_EN			( 1UL << 4UL )

#define GPIOC_CRL					(*( ( volatile u32 * ) 0x40011000 ) )
#define GPIOC_CRH					(*( ( volatile u32 * ) 0x40011004 ) )
#define GPIOC_CRH_MODER13			( 3UL << 20UL )
#define GPIOC_CRH_CNF13				( 3UL << 22UL )
#define GPIOC_BSRR 					(*( ( volatile u32 * ) 0x40011010 ) )
#define GPIOC_BSRR_BS13				( 1UL << 13UL )
#define GPIOC_BSRR_BR13				( 1UL << 29UL )

/* Critical section management. */
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

//u32 uxCriticalNesting;

#define portSET_INTERRUPT_MASK_FROM_ISR() 			ulPortRaiseBASEPRI()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR( x )		vPortSetBASEPRI( x )
#define portDISABLE_INTERRUPTS()					vPortRaiseBASEPRI()
#define portENABLE_INTERRUPTS()						vPortSetBASEPRI( 0 )
#define portENTER_CRITICAL() 						vPortEnterCritical()
#define portEXIT_CRITICAL()							vPortExitCritical()
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 		63

/* --------------------------------------------------------- */

#define portINLINE				__inline

#ifndef portFORCE_INLINE
	#define portFORCE_INLINE 	inline __attribute__( ( always_inline ) )
#endif



void Port_InitTickTimer();
u8 Port_CheckStackOverFlow(pu32 Stack);
void Port_InitClearStack(pu32 Stack, u32 Size);
pu32 Port_InitTaskStack(pu32 Stack, u32 size, void *Function);
void Port_RestoreFirstContext(u32 Sp);
unsigned long long  Port_GetSystemTicks();


#endif /* PORT_H_ */
