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

#ifndef	__LYNXOS_H__
#define	__LYNXOS_H__

//********************************** Lynx OS **************************************************************
/**
 *  @defgroup LynxOS <OS Module> 
 *
 *	Operation system related functions.
 */
//*********************************************************************************************************
#include <LynxType.h>
#include <uuid.h>

/**
 *  @ingroup LynxOS
 *	@struct LPLYNXTIMEDATA
 *  @brief 。
 *	@see LYNXTIMEDATA
 */
/**
 *  @ingroup LynxOS 
 *	@struct LYNXTIMEDATA
 *  @brief 。
 *	@see LPLYNXTIMEDATA
 */
LYNX_CACHE_ALIGN typedef struct {	
	WORD								Year;
	WORD								Month;
	WORD								DayOfWeek;
	WORD								Day;  
	WORD								Hour;  
	WORD								Minute;  
	WORD								Second;  
	WORD								Milliseconds;
}LYNXTIMEDATA, *LPLYNXTIMEDATA;
typedef const LYNXTIMEDATA* LPCLYNXTIMEDATA;

/**
 *  @ingroup LynxOS 
 *	@struct LPLYNXCRITICALSECTION
 *  @brief 。
 *	@see LYNXCRITICALSECTION
 */
/**
 *  @ingroup LynxOS 
 *	@struct LYNXCRITICALSECTION
 *  @brief 。
 *	@see LPLYNXCRITICALSECTION
 */
LYNX_CACHE_ALIGN typedef struct {
	#ifdef __WIN32__
		CRITICAL_SECTION				CriticalSection;
	#else
		BYTE							CriticalSection;
	#endif
}LYNXCRITICALSECTION, *LPLYNXCRITICALSECTION;
typedef const LYNXCRITICALSECTION* LPCLYNXCRITICALSECTION;

LYNX_CACHE_ALIGN typedef struct {
	#ifdef __WIN32__
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[ 8 ];			
	#else
		//unsigned char Data[16];	
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[ 8 ];			
	#endif
}LYNXGUID, *LPLYNXGUID;
typedef const LYNXGUID* LPCLYNXGUID;

#ifdef __iOS__
	//#define BEGIN_THREAD				NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	//#define END_THREAD					[pool release];
    #define BEGIN_THREAD
    #define END_THREAD					
#else
	#define BEGIN_THREAD				
	#define END_THREAD					
#endif

typedef enum {
	TP_LOWEST,
	TP_NORMAL,
	TP_HIGHEST,
	TP_TIME_CRITICAL,
}THREADPRIORITY;

typedef		VOID (* LPTHREADPROC)(LPVOID);

#ifdef __cplusplus
extern "C" {
#endif														

LYNXCOREAPI void					lynxInitOS();

LYNXCOREAPI	LYNXULONGLONG			lynxOSGetTimeStamp();
LYNXCOREAPI void					lynxOSGetTime(LPLYNXTIMEDATA lpt);
LYNXCOREAPI double					lynxOSGetTimeDifference(LPLYNXTIMEDATA lpt0, LPLYNXTIMEDATA lpt1);

LYNXCOREAPI LYNXLONGLONG			lynxOSGetPerformanceCounter();
LYNXCOREAPI LYNXLONGLONG			lynxOSGetPerformanceFrequency();
LYNXCOREAPI LYNXTHREADHANDLE		lynxOSCreateThread(LPTHREADPROC lpproc, LPVOID lpdata);
LYNXCOREAPI void					lynxOSSetThreadPriority(LYNXTHREADHANDLE handle, THREADPRIORITY pro);
LYNXCOREAPI void					lynxOSExitThread(DWORD exitcode);
LYNXCOREAPI void					lynxOSCloseThread(LYNXTHREADHANDLE th);

LYNXCOREAPI void					lynxOSCreateMutex(LYNXMUTEX* m);
LYNXCOREAPI void					lynxOSLockMutex(LYNXMUTEX* m);
LYNXCOREAPI void					lynxOSUnlockMutex(LYNXMUTEX* m);
LYNXCOREAPI void					lynxOSDestoryMutex(LYNXMUTEX* m);

LYNXCOREAPI void					lynxOSInitCriticalSection(LPLYNXCRITICALSECTION lpcs);
LYNXCOREAPI void					lynxOSReleaseCriticalSection(LPLYNXCRITICALSECTION lpcs);
LYNXCOREAPI void					lynxOSEnterCriticalSection(LPLYNXCRITICALSECTION lpcs);
LYNXCOREAPI void					lynxOSLeaveCriticalSection(LPLYNXCRITICALSECTION lpcs);

LYNXCOREAPI void					lynxOSMoveWindow(const LYNXWINDOW w, const int x, const int y, const int width, const int height, LYNXBOOL bclientsize);
LYNXCOREAPI void					lynxOSRegisterTouchWindow(const LYNXWINDOW w, const LYNXBOOL b);
LYNXCOREAPI void					lynxOSSetTitle(const LYNXWINDOW w, const LYNXCHAR *title);
LYNXCOREAPI LYNXCHAR*				lynxOSGetTitle(const LYNXWINDOW w);
LYNXCOREAPI void					lynxOSSetIcon(DWORD icon);
LYNXCOREAPI void					lynxOSSleep(DWORD t);
LYNXCOREAPI void					lynxOSClose(const LYNXWINDOW w);
LYNXCOREAPI HMODULE					lynxOSLoadLibrary(const LYNXCHAR *name);
LYNXCOREAPI FARPROC					lynxOSGetProc(HMODULE hm, const char *name);

LYNXCOREAPI void					lynxOSMakeGUID(LPLYNXGUID pguid);
LYNXCOREAPI	const LYNXCHAR*			lynxOSGUIDToString(const LYNXGUID* pguid);
LYNXCOREAPI const LYNXGUID			lynxOSStringToGUID(const LYNXCHAR* str);
LYNXCOREAPI int						lynxOSComapreGUID(const LYNXGUID* pguid1, const LYNXGUID* pguid2);

LYNXCOREAPI void					lynxOSGetCurrentWorkingDirectory(LYNXCHAR* dir, DWORD len);
LYNXCOREAPI void					lynxOSSetCurrentWorkingDirectory(const LYNXCHAR* dir);

LYNXCOREAPI void					lynxOSDebuggerBreak();

#ifdef __cplusplus
}
#endif

#endif

