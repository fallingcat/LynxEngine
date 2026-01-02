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

#ifndef __LYNXPROFILE_H__
#define __LYNXPROFILE_H__

//********************************** Lynx Log **************************************************************
/**
 *  @defgroup LynxProfile <Profile Module>
 *
 *	Profile related functions.
 */
//**********************************************************************************************************
#include <LynxType.h>

/*
// Lynx Profile Data ///////////////////////////////////////////////////////////////////////
LYNX_CACHE_ALIGN typedef struct {
	DWORD					NumVertices;
	DWORD					DrawTrianglesUsedTime;
	DWORD					NumDrawedTriangles;
	DWORD					NumTriangles;
}LYNXPROFILEDATA, *LPLYNXPROFILEDATA;
typedef const LYNXPROFILEDATA* LPCLYNXPROFILEDATA;
*/

/**
 *  @ingroup LynxGraphic
 *  @brief Render structure.
 *
 *	The profie data.
 */
LYNX_CACHE_ALIGN typedef struct {	
	DWORD							SetRenderStateTimeCounter;
	DWORD							SetRenderStateCounter;	
	DWORD							DrawTimeCounter;
	DWORD							DrawCallCounter;	
	DWORD							PrimitiveCounter;
	DWORD							SetTextureCounter;
	DWORD							SetShaderCounter;
	DWORD							SetShaderConstantCounter;
}LYNXPROFILE, *LPLYNXPROFILE;
typedef const LYNXPROFILE* LPCLYNXPROFILE;

#ifdef __cplusplus
extern "C" {
#endif														

#ifdef __PROFILE__
	#define LYNX_RESET_COUNTER(c)				c = 0L;
	#define LYNX_START_COUNTER(c)				lynxSetT0(lynxOSGetPerformanceCounter());
	#define LYNX_STOP_COUNTER(c)				c += (lynxOSGetPerformanceCounter() - lynxGetT0());

	#define LYNX_RESET_SETRS_COUNTER			LYNX_RESET_COUNTER(lynxGetProfile()->SetRenderStateCounter);
	#define LYNX_INC_SETRS_COUNTER				lynxGetProfile()->SetRenderStateCounter++;

	#define LYNX_RESET_SETTEXTURE_COUNTER		LYNX_RESET_COUNTER(lynxGetProfile()->SetTextureCounter);
	#define LYNX_INC_SETTEXTURE_COUNTER			lynxGetProfile()->SetTextureCounter++;

	#define LYNX_RESET_SETSHADER_COUNTER		LYNX_RESET_COUNTER(lynxGetProfile()->SetShaderCounter);
	#define LYNX_INC_SETSHADER_COUNTER			lynxGetProfile()->SetShaderCounter++;

	#define LYNX_RESET_SETSHADERCONST_COUNTER	LYNX_RESET_COUNTER(lynxGetProfile()->SetShaderConstantCounter);
	#define LYNX_INC_SETSHADERCONST_COUNTER		lynxGetProfile()->SetShaderConstantCounter++;

	#define LYNX_RESET_PRIMITIVE_COUNTER		LYNX_RESET_COUNTER(lynxGetProfile()->PrimitiveCounter);
	#define LYNX_INC_PRIMITIVE_COUNTER(p)		lynxGetProfile()->PrimitiveCounter += p;

	#define LYNX_RESET_DRAWTIME_COUNTER			LYNX_RESET_COUNTER(lynxGetProfile()->DrawTimeCounter);
	#define LYNX_START_DRAWTIME_COUNTER			LYNX_START_COUNTER(lynxGetProfile()->DrawTimeCounter);
	#define LYNX_STOP_DRAWTIME_COUNTER			LYNX_STOP_COUNTER(lynxGetProfile()->DrawTimeCounter);	

	#define LYNX_RESET_DRAWCALL_COUNTER			lynxGetProfile()->DrawCallCounter = 0;
	#define LYNX_INC_DRAWCALL_COUNTER			lynxGetProfile()->DrawCallCounter++;

	LYNXCOREAPI LPLYNXPROFILE	lynxGetProfile();
	LYNXCOREAPI void			lynxSetT0(LYNXLONGLONG t);
	LYNXCOREAPI LYNXLONGLONG	lynxGetT0();	

	LYNXCOREAPI DWORD	lynxProfile_GetDrawCallCounter();
	LYNXCOREAPI DWORD	lynxProfile_GetDrawTimeCounter();
	LYNXCOREAPI DWORD	lynxProfile_GetPrimitiveCounter();
	LYNXCOREAPI DWORD	lynxProfile_GetSetRSCounter();
	LYNXCOREAPI DWORD	lynxProfile_GetSetTextureCounter();
	LYNXCOREAPI DWORD	lynxProfile_GetSetShaderCounter();
	LYNXCOREAPI DWORD	lynxProfile_GetSetShaderConstantCounter();
#else
	#define LYNX_RESET_COUNTER(c)
	#define LYNX_START_COUNTER(c)
	#define LYNX_STOP_COUNTER(c)

	#define LYNX_RESET_SETRS_COUNTER			
	#define LYNX_INC_SETRS_COUNTER				

	#define LYNX_RESET_SETTEXTURE_COUNTER		
	#define LYNX_INC_SETTEXTURE_COUNTER			

	#define LYNX_RESET_SETSHADER_COUNTER		
	#define LYNX_INC_SETSHADER_COUNTER		

	#define LYNX_RESET_SETSHADERCONST_COUNTER	
	#define LYNX_INC_SETSHADERCONST_COUNTER		

	#define LYNX_RESET_PRIMITIVE_COUNTER		
	#define LYNX_INC_PRIMITIVE_COUNTER(p)		

	#define LYNX_RESET_DRAWTIME_COUNTER
	#define LYNX_START_DRAWTIME_COUNTER
	#define LYNX_STOP_DRAWTIME_COUNTER

	#define LYNX_RESET_DRAWCALL_COUNTER
	#define LYNX_INC_DRAWCALL_COUNTER					

	LYNXFORCEINLINE LYNXCOREAPI LPLYNXPROFILE	lynxGetProfile() {return NULL; }
	LYNXFORCEINLINE LYNXCOREAPI void			lynxSetT0(LYNXLONGLONG t) {};
	LYNXFORCEINLINE LYNXCOREAPI LYNXLONGLONG	lynxGetT0() {return 0; };	
	LYNXFORCEINLINE LYNXCOREAPI DWORD			lynxProfile_GetDrawCallCounter() {return 0; }
	LYNXFORCEINLINE LYNXCOREAPI DWORD			lynxProfile_GetDrawTimeCounter() {return 0; }
	LYNXFORCEINLINE LYNXCOREAPI DWORD			lynxProfile_GetPrimitiveCounter() {return 0; }
	LYNXFORCEINLINE LYNXCOREAPI DWORD			lynxProfile_GetSetRSCounter() {return 0; }
	LYNXFORCEINLINE LYNXCOREAPI DWORD			lynxProfile_GetSetTextureCounter() {return 0; }
	LYNXFORCEINLINE LYNXCOREAPI DWORD			lynxProfile_GetSetShaderCounter() {return 0; }
	LYNXFORCEINLINE LYNXCOREAPI DWORD			lynxProfile_GetSetShaderConstantCounter() {return 0; }
#endif
LYNXFORCEINLINE LYNXCOREAPI void lynxProfile_ResetDrawTimeCounter() {LYNX_RESET_DRAWTIME_COUNTER; }
LYNXFORCEINLINE LYNXCOREAPI void lynxProfile_ResetDrawCallCounter() {LYNX_RESET_DRAWCALL_COUNTER; }
LYNXFORCEINLINE LYNXCOREAPI void lynxProfile_ResetPrimitiveCounter() {LYNX_RESET_PRIMITIVE_COUNTER; }
LYNXFORCEINLINE LYNXCOREAPI void lynxProfile_ResetSetRSCounter() {LYNX_RESET_SETRS_COUNTER; }
LYNXFORCEINLINE LYNXCOREAPI void lynxProfile_ResetSetTextureCounter() {LYNX_RESET_SETTEXTURE_COUNTER; }
LYNXFORCEINLINE LYNXCOREAPI void lynxProfile_ResetSetShaderCounter() {LYNX_RESET_SETSHADER_COUNTER; }
LYNXFORCEINLINE LYNXCOREAPI void lynxProfile_ResetSetShaderConstantCounter() {LYNX_RESET_SETSHADERCONST_COUNTER; }

LYNXFORCEINLINE LYNXCOREAPI void lynxProfile_ResetAllCounters() { lynxProfile_ResetDrawTimeCounter(); 
																		    lynxProfile_ResetDrawCallCounter(); 
																		    lynxProfile_ResetPrimitiveCounter();
																		    lynxProfile_ResetSetRSCounter(); 
																			lynxProfile_ResetSetTextureCounter(); 
																			lynxProfile_ResetSetShaderCounter(); 
																			lynxProfile_ResetSetShaderConstantCounter(); }
#ifdef __cplusplus
}
#endif														

#endif	
