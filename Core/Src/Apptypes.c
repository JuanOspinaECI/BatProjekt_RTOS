/*
 * Apptypes.c
 *
 *  Created on: Sep 24, 2023
 *      Author: JuanOsp
 */

#include "Apptypes.h"


/*u16 IntAdd(T_Int * Int, u16 Dato, u8 Asig)
{
	u16 Temp = 0;
	Temp = Int->Low;
	Temp += (Int->High *256);
	Temp += Dato;
	if(Asig)
	{
		SetInt(Int,Temp);
	}
	return Temp;
}


u16 IntSub (T_Int * Int, u16 Dato, u8 Asig)
{
	u16 Temp = 0;
	Temp = Int->Low;
	Temp += (Int->High *256);
	Temp -= Dato;
	if(Asig)
	{
		SetInt(Int,Temp);
	}
	return Temp;
}

u16 GetInt(T_Int * Int)
{
	u16 Temp = 0;
	Temp = Int->Low;
	Temp += (Int->High *256);
	return Temp;
}

void SetFloat(T_Float * Float, f Dato)
{
	pu8 Pt;
	Pt = (pu8)&Dato;
	Float -> Float.High.High = *Pt;
	Pt++;
	Float -> Float.High.Low = *Pt;
	Pt++;
	Float -> Float.Low.High = *Pt;
	Pt++;
	Float -> Float.Low.Low = *Pt;
	Pt++;
}


u32 LongSub(T_Long * Long, u32 Dato, u8 Asig)
{
	u32 Temp 	=	 0;
	Temp 		=	 Long->Low.Low;
	Temp 		+=	 (Long->Low.High *256);
	Temp 		+=	 (Long->High.Low *512);
	Temp 		+=	 (Long->High.High *1024);
	Temp 		-=	 Dato;
	if(Asig)
	{
		SetLong(Long,temp);
	}
	return Temp;
}

void SetLong(T_Long * Long, u32 Dato)
{
	Long->Low.Low	= (u8)(Dato & 0x000000FF);
	Long->Low.High	= (u8)((Dato >> 8)& 0x000000FF);
	Long->High.Low	= (u8)((Dato >> 16)& 0x000000FF);
	Long->High.High	= (u8)((Dato >> 24)& 0x000000FF);
}

s32 GetLongS(T_Long * Long)
{
	s32 Temp = 0;
	Temp	=	(u32)(Long->Low.Low & 0x000000FF);
	Temp	|=	(u32)((Long->Low.High << 8 )& 0x0000FF00);
	Temp	|=	(u32)(((u32)Long->High.Low << 16) & 0x00FF0000);
	Temp	|=	(u32)(((u32)Long->High.High << 32) & 0xFF000000);
	return Temp;
}
*/
