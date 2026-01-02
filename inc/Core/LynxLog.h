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

#ifndef __LYNXLOG_H__
#define __LYNXLOG_H__

//********************************** Lynx Log **************************************************************
/**
 *  @defgroup LynxLog <Log Module>
 *
 *	Log related functions.
 */
//**********************************************************************************************************
#include <LynxType.h>
#include <LynxIO.h>

LYNX_CACHE_ALIGN typedef struct {
	LYNXCHAR						FileName[64];
	LYNXFILE						LogFile;
}LYNXLOG, *LPLYNXLOG;
typedef const LYNXLOG* LPCLYNXLOG;

#ifdef __LOG__
	#define LYNX_SET_CURRENT_LOG(l)			lynxSetCurrentLog((l))
	#define LYNX_OPEN_LOG(n)				lynxOpenLog((n))
	#define LYNX_CLOSE_LOG()				lynxCloseLog()
	#define LYNX_SET_LOG(m, l)				lynxSetLog((m), (l))	
	#define LYNX_SET_LOGF_1(m, f, a1)  		lynxSetLogf(m, f, a1)	
	#define LYNX_SET_LOGF_2(m, f, a1, a2)  		lynxSetLogf(m, f, a1, a2)	
	#define LYNX_SET_LOGF_3(m, f, a1, a2, a3)  		lynxSetLogf(m, f, a1, a2, a3)	
	#define LYNX_SET_LOGF_4(m, f, a1, a2, a3, a4)  		lynxSetLogf(m, f, a1, a2, a3, a4)	
	#define LYNX_SET_LOGF_5(m, f, a1, a2, a3, a4, a5)  		lynxSetLogf(m, f, a1, a2, a3, a4, a5)	
	#define LYNX_SET_LOGF_6(m, f, a1, a2, a3, a4, a5, a6)  		lynxSetLogf(m, f, a1, a2, a3, a4, a5, a6)	
	#define LYNX_SET_LOGF_7(m, f, a1, a2, a3, a4, a5, a6, a7)  		lynxSetLogf(m, f, a1, a2, a3, a4, a5, a6, a7)	
	#define LYNX_SET_LOGF_8(m, f, a1, a2, a3, a4, a5, a6, a7, a8)  		lynxSetLogf(m, f, a1, a2, a3, a4, a5, a6, a7, a8)	
#else
	#define LYNX_SET_CURRENT_LOG(l)
	#define LYNX_OPEN_LOG(n)				
	#define LYNX_CLOSE_LOG()				
	#define LYNX_SET_LOG(m, l)					
	#define LYNX_SET_LOGF_1(m, f, a1)  		
	#define LYNX_SET_LOGF_2(m, f, a1, a2)  		
	#define LYNX_SET_LOGF_3(m, f, a1, a2, a3)  	
	#define LYNX_SET_LOGF_4(m, f, a1, a2, a3, a4) 
	#define LYNX_SET_LOGF_5(m, f, a1, a2, a3, a4, a5) 
	#define LYNX_SET_LOGF_6(m, f, a1, a2, a3, a4, a5, a6)  
	#define LYNX_SET_LOGF_7(m, f, a1, a2, a3, a4, a5, a6, a7) 
	#define LYNX_SET_LOGF_8(m, f, a1, a2, a3, a4, a5, a6, a7, a8)
#endif

#ifdef __cplusplus
extern "C" {
#endif														

LYNXCOREAPI void			lynxInitLog(LPLYNXLOG lplog);
LYNXCOREAPI void			lynxReleaseLog(LPLYNXLOG lplog);
LYNXCOREAPI void			lynxSetCurrentLog(LPLYNXLOG lplog);
LYNXCOREAPI LYNXBOOL		lynxOpenLog(const LYNXCHAR *name);
LYNXCOREAPI void			lynxCloseLog(void);
LYNXCOREAPI void			lynxSetLog(const LYNXCHAR *mod, const LYNXCHAR *log);
LYNXCOREAPI void			lynxSetLogf(const LYNXCHAR *mod, const LYNXCHAR *fmt, ...);

#ifdef __cplusplus
}
#endif														

#endif