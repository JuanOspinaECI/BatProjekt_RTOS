/*
 * Apptypes.h
 *
 *  Created on: Sep 24, 2023
 *      Author: maqui
 */

#ifndef APPTYPES_H_
#define APPTYPES_H_

#define MAIN_BASE_ERROR 		0x0100
#define QUEUE_BASE_ERROR		0x0200
#define SCHEDULER_BASE_ERROR	0x0300
#define TASK_BASE_ERROR			0x0400
#define MUTEX_BASE_ERROR		0x0500
#define SEM_BASE_ERROR			0x0600
#define EVENTS_BASE_ERROR		0x0700
#define MESSAGE_BASE_ERROR		0x0800

#ifndef f
	typedef float f;
#endif

#ifndef d
	typedef double d;
#endif

#ifndef c
	typedef char c;
#endif

#ifndef int
	typedef int i;
#endif

#ifndef pf
	typedef float * pf;
#endif

#ifndef pd
	typedef double * pd;
#endif

#ifndef v
	typedef void v;
#endif

#ifndef u8
	typedef unsigned char u8;
#endif

#ifndef u16
	typedef unsigned short u16;
#endif

#ifndef u32
	typedef unsigned int u32;
#endif

#ifndef s8
	typedef signed char s8;
#endif

#ifndef s16
	typedef signed short s16;
#endif

#ifndef s32
	typedef signed int s32;
#endif

#ifndef pv
	typedef void * pv;
#endif

#ifndef pu8
	typedef unsigned char * pu8;
#endif

#ifndef pu16
	typedef unsigned short * pu16;
#endif

#ifndef pu32
	typedef unsigned int * pu32;
#endif

#ifndef ps8
	typedef signed char * ps8;
#endif

#ifndef ps16
	typedef signed short * ps16;
#endif

#ifndef ps32
	typedef signed int * ps32;
#endif

#ifndef _PTR_
	typedef void* _PTR_;
#endif

#ifndef False
#define False 0
#endif

#ifndef True
#define True 1
#endif

#ifndef NULL
#define NULL 0
#endif

//typedef signed long  s32;
//typedef signed short s16;
//typedef signed char  s8;
//
//typedef volatile signed long  vs32;
//typedef volatile signed short vs16;
//typedef volatile signed char  vs8;
//
//typedef unsigned long  u32;
//typedef unsigned short u16;
//typedef unsigned char  u8;
//
//typedef unsigned long  const uc32;  /* Read Only */
//typedef unsigned short const uc16;  /* Read Only */
//typedef unsigned char  const uc8;   /* Read Only */
//
//typedef volatile unsigned long  vu32;
//typedef volatile unsigned short vu16;
//typedef volatile unsigned char  vu8;
//
//typedef volatile unsigned long  const vuc32;  /* Read Only */
//typedef volatile unsigned short const vuc16;  /* Read Only */
//typedef volatile unsigned char  const vuc8;   /* Read Only */
//
//typedef enum {FALSE = 0, TRUE = !FALSE} bool;
//
//typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
//
//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
//#define IS_FUNCTIONAL_STATE(STATE) ((STATE == DISABLE) || (STATE == ENABLE))
//
//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
//
//#define U8_MAX     ((u8)255)
//#define S8_MAX     ((s8)127)
//#define S8_MIN     ((s8)-128)
//#define U16_MAX    ((u16)65535u)
//#define S16_MAX    ((s16)32767)
//#define S16_MIN    ((s16)-32768)
//#define U32_MAX    ((u32)4294967295uL)
//#define S32_MAX    ((s32)2147483647)
//#define S32_MIN    ((s32)2147483648uL)


#endif /* APPTYPES_H_ */
