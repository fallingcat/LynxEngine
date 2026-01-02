//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2003/06/22
//  Last Update : 
//
//###########################################################################

LYNXEXTERN LYNXREAL							gCos[];
LYNXEXTERN LYNXREAL							gSin[];	
LYNXEXTERN LYNXMATRIX						gIdentityM;
LYNXEXTERN int								gInstructionSet;

#ifdef __cplusplus
extern "C" {
#endif								

#ifdef __WIN32__	
	#if (defined _M_IX86)
		//-------------------------------------------------------------------------------------------------------
		//
		//  »¡©ú:   
		//-------------------------------------------------------------------------------------------------------
		LYNXFORCEINLINE LYNXREAL ilynxSSE_Sqrt(LYNXREAL f)
		{
			LYNXREAL			Root;

			_asm
			{
				sqrtss		xmm0, f
				movss		Root, xmm0
			}
			return Root;
		}
	#endif	
#endif

//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxMath 
*  @brief ¡C
*
*	@param ¡C
*	@param ¡C  
*	@return ¡C 
*/  
//-------------------------------------------------------------------------------------------------------
LYNXFORCEINLINE LYNXCOREAPI LPCLYNXMATRIX lynxGetIdentityMatrix()
{
	return (&gIdentityM);
}
//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxMath 
*  @brief ¡C
*
*	@param ¡C
*	@param ¡C  
*	@return ¡C 
*/    
//-------------------------------------------------------------------------------------------------------
LYNXFORCEINLINE LYNXCOREAPI LYNXREAL lynxQuickSin(const LYNXREAL d)
{
	LYNXREAL D = d;
	LYNX_WRAP_DEGREE(D);
	return gSin[LYNX_REAL_ROUND_INT(D*LYNX_DEG_STEP)];			
}
//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxMath 
*  @brief ¡C
*
*	@param ¡C
*	@param ¡C  
*	@return ¡C 
*/    
//-------------------------------------------------------------------------------------------------------
LYNXFORCEINLINE LYNXCOREAPI LYNXREAL	lynxQuickCos(const LYNXREAL d)
{
	LYNXREAL D = d;
	LYNX_WRAP_DEGREE(D);	
	return gCos[LYNX_REAL_ROUND_INT(D*LYNX_DEG_STEP)];		
}
//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxMath 
*  @brief ¡C
*
*	@param ¡C
*	@param ¡C  
*	@return ¡C 
*/   
//-------------------------------------------------------------------------------------------------------
LYNXFORCEINLINE LYNXCOREAPI LYNXREAL	lynxSin(const LYNXREAL d)
{
	#ifdef __FIXED_POINT__
		return (lynxQuickSin(d));
	#else
		//LYNX_WRAP_DEGREE((*d));
		return ((LYNXREAL)sin(LYNXD2R*d));
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxMath 
*  @brief ¡C
*
*	@param ¡C
*	@param ¡C  
*	@return ¡C 
*/  
//-------------------------------------------------------------------------------------------------------
LYNXFORCEINLINE LYNXCOREAPI LYNXREAL	lynxCos(const LYNXREAL d)
{
	#ifdef __FIXED_POINT__
		return (lynxQuickCos(d));
	#else
		//LYNX_WRAP_DEGREE((*d));
		return ((LYNXREAL)cos(LYNXD2R*d));
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxMath 
*  @brief ¡C
*
*	@param ¡C
*	@param ¡C  
*	@return ¡C 
*/  
//-------------------------------------------------------------------------------------------------------
LYNXFORCEINLINE LYNXCOREAPI LYNXREAL	lynxACos(const LYNXREAL d)
{
	LYNXREAL D = LYNX_CLAMP(d, -1.0f, 1.0f);
	return (LYNXREAL)(acos(D)*LYNXR2D);	
}
//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxMath 
*  @brief ¡C
*
*	@param ¡C
*	@param ¡C  
*	@return ¡C 
*/  
//-------------------------------------------------------------------------------------------------------
LYNXFORCEINLINE LYNXCOREAPI LYNXREAL lynxSqrt(const LYNXREAL f)
{	
	#ifdef __WIN32__
		#ifdef __FIXED_POINT__			
			float				T, R;

			T = ((float)f)/(1<<16);
			R = sqrt(T);
			return (LYNX_REAL(R));
		#else			
			#if 0 //(defined __X86__ || defined __X64__)
				if ((gInstructionSet & LYNX_IS_SSE2) || (gInstructionSet & LYNX_IS_SSE) )
				{
					return ilynxSSE_Sqrt(f);		
				}
				else
				{
					return (LYNX_REAL(sqrt(f)));		
				}
			#else
				return (LYNX_REAL(sqrt(f)));		
			#endif
		#endif
	#else
		return (LYNX_REAL(sqrt(f)));		
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxMath 
*  @brief ¡C
*
*	@param ¡C
*	@param ¡C  
*	@return ¡C 
*/  
//-------------------------------------------------------------------------------------------------------
LYNXFORCEINLINE LYNXCOREAPI LYNXREAL lynxFastInverseSqrt(const LYNXREAL f)
{
	int carmack;
	LYNXREAL isx, halfx;

	halfx = 0.5f*f;
	carmack = *(int*)&f; 
	carmack = 0x5f3759df - (carmack>>1); 
	isx = *(LYNXREAL*)&carmack; 

	isx = isx*(1.5f-halfx*isx*isx);  //Newton-Rhapson step, add more for accuracy

	return isx;
}

#ifdef __cplusplus
}
#endif