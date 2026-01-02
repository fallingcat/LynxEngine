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

#ifndef __LYNXMEM_H__
#define __LYNXMEM_H__

//********************************** Lynx Mem **************************************************************
/**
 *  @defgroup LynxMem <Memory Module>
 *
 *	Memory related functions.
 *  When allocating and freeing memory, if no necessary, don't use the functions of this module.
 *	In normal usage, using LYNX_MALLOC() or LYNX_ALIGN_MALLOC() for memory allocation.
 *  Using LYNX_FREE() for freeing memory. 
 *	You can use memory management functions when using Macro functions to allocate/free memory.
 */
//**********************************************************************************************************
#include <LynxType.h>

#ifdef __WIN32__	
	#include <malloc.h>
#endif

#ifdef __ANDROID__
	#include <malloc.h>
#endif

#define LYNX_ALIGN_POINTER(lps)								(((long)lps + LYNX_CACHE_SIZE - 1) & ~(CACHE_SIZE - 1))
#ifdef __PS2__
	#define LYNX_USER_ALIGN_POINTER(lps, bound)				(lps)
#else
	#define LYNX_USER_ALIGN_POINTER(lps, bound)				(((long)lps + bound - 1) & ~(bound - 1))
#endif

#if (defined(_DEBUG) && defined (__DESKTOP__) && defined(__VC_MEM_LEAK_CHECK__))
	#define LYNX_MALLOC(size)							lynxMemAlloc_d((size), __FILE__, __FUNCTION__, __LINE__)
	#define LYNX_ALIGN_MALLOC(bound, size)				lynxAlignMemAlloc_d(bound, (size), __FILE__, __FUNCTION__, __LINE__)
	#define LYNX_REALLOC(lpp, size)						lynxMemRealloc_d(lpp, (size), __FILE__, __FUNCTION__, __LINE__)
	#define LYNX_FREE(lp)								lynxMemFree_d(lp); lp = NULL
#else
	#define LYNX_MALLOC(size)							lynxMemAlloc((size))
	#define LYNX_ALIGN_MALLOC(bound, size)				lynxAlignMemAlloc(bound, (size))
	#define LYNX_REALLOC(lpp, size)						lynxMemRealloc(lpp, (size))
	#define LYNX_FREE(lp)								lynxMemFree(lp); lp = NULL
#endif

#define	LYNX_MAX_MEM_BLOCK							196613

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN								0x10
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN									0x11
#endif

#ifdef __OS_X__
    #define LYNX_BYTE_ORDER(lpd, s)					lynxByteOrder(lpd, s)
#else
    #define LYNX_BYTE_ORDER(lpd, s)					
#endif

typedef struct LynxMemBlock{	
	BYTE											Flag;
	LPVOID											lpBase;
	DWORD											Size;
	char											File[128];
	char											Function[128];
	int												Line;
}LYNXMEMBLOCK, *LPLYNXMEMBLOCK;
typedef const LYNXMEMBLOCK* LPCLYNXMEMBLOCK;
	
typedef struct LynxMemManager{
	DWORD											TotalSize;		
	LYNXMEMBLOCK									lpMemBlock[LYNX_MAX_MEM_BLOCK];			
}LYNXMEMMANAGER, *LPLYNXMEMMANAGER;
typedef const LYNXMEMMANAGER* LPCLYNXMEMMANAGER;

typedef void* (*LPLYNXALLOCPROC)(size_t);
typedef void* (*LPLYNXALLOCPROCD)(size_t, const char *, const char *func, int);
typedef void (*LPLYNXDEALLOCPROC)(void *);
typedef void (*LPLYNXDEALLOCPROCD)(void *);

#ifdef __cplusplus
extern "C" {
#endif

LYNXCOREAPI	void			lynxSetMemFunc(LPLYNXALLOCPROC palloc, LPLYNXDEALLOCPROC pfree, LPLYNXALLOCPROCD palloc_d, LPLYNXDEALLOCPROCD pfree_d);
LYNXCOREAPI void			lynxInitMemSystem(void);

LYNXCOREAPI void			lynxRecordMemBlock(LPVOID lpp, size_t size, const char *file, const char *func, int line);
LYNXCOREAPI void			lynxReleaseMemBlock(LPVOID lp);
LYNXCOREAPI LPLYNXMEMBLOCK	lynxGetFirstMemBlock();
LYNXCOREAPI LPLYNXMEMBLOCK	lynxGetNextMemBlock();

LYNXCOREAPI int				lynxMemCmp(void *dest, const void *src, size_t size);
LYNXCOREAPI void			lynxMemCpy(void *dest, const void *src, size_t size);
LYNXCOREAPI void			lynxAlignMemCpy(void *dest, const void *src, size_t size);
LYNXCOREAPI void			lynxAlignMemSetB(void *dest, BYTE c, size_t sizet);
LYNXCOREAPI void			lynxAlignMemSetW(void *dest, WORD w, size_t sizet);
LYNXCOREAPI void			lynxMemSet(void *dest, BYTE b, size_t sizet);

LYNXCOREAPI void*			lynxAlignMemAlloc(int bound, size_t size);
LYNXCOREAPI void*			lynxAlignMemRealloc(LPVOID lpp, int bound, size_t size);
LYNXCOREAPI void*			lynxMemAlloc(size_t size);
LYNXCOREAPI void*			lynxMemRealloc(LPVOID lpp, size_t size);
LYNXCOREAPI void			lynxMemFree(void *lp);

LYNXCOREAPI void*			lynxAlignMemAlloc_d(int bound, size_t size, const char *file, const char *func, int line);
LYNXCOREAPI void*			lynxAlignMemRealloc_d(LPVOID lpp, int bound, size_t size, const char *file, int line);
LYNXCOREAPI void*			lynxMemAlloc_d(size_t size, const char *file, const char *func, int line);
LYNXCOREAPI void*			lynxMemRealloc_d(LPVOID lpp, size_t size, const char *file, const char *func, int line);
LYNXCOREAPI void			lynxMemFree_d(void *lp);

LYNXCOREAPI DWORD			lynxGetAllocMemSize(void);
LYNXCOREAPI void			lynxClearBadMemBlock(void);

LYNXCOREAPI void			lynxByteOrder(LPVOID lpd, int size);

LYNXCOREAPI LYNXLONGLONG	lynxGetAvailablePhysicalMem(void);
LYNXCOREAPI LYNXLONGLONG	lynxGetUsedPhysicalMem(void);

#ifdef __cplusplus
}
#endif
//
//#ifdef __cplusplus
//	#ifdef __TOOL__	
//		#define	LYNXNEW									new
//		#define	LYNXDEL									delete	
//		#define	LYNXDELARRAY								delete []
//	#else		
//		#ifdef _DEBUG
//			/*//LYNXINLINE void	*operator						new(size_t s) {return lynxMemAlloc_d((DWORD)s, __FILE__, __LINE__);};				
//			LYNXINLINE void	*operator							new(size_t s, const char *file, const char *func, int line) {return lynxMemAlloc_d((DWORD)s, file, func, line);};				
//			//LYNXINLINE void	operator						delete(void *p) {LYNX_FREE(p);};
//			LYNXINLINE void	operator							delete(void *p, const char *file, const char *func, int line) {LYNX_FREE(p);};			
//			//LYNXINLINE void	*operator						new[](size_t s) {return lynxMemAlloc_d((DWORD)s, __FILE__, __LINE__);};				
//			LYNXINLINE void	*operator							new[](size_t s, const char *file, const char *func, int line) {return lynxMemAlloc_d((DWORD)s, file, func, line);};				
//			//LYNXINLINE void	operator						delete[](void *p) {LYNX_FREE(p);};	
//			LYNXINLINE void	operator							delete[](void *p, const char *file, const char *func, int line) {LYNX_FREE(p);};	*/
//
//			#define	LYNXNEW								new(__FILE__, __FUNCTION__, __LINE__)		
//			#define	LYNXDEL								delete 
//			#define	LYNXDELARRAY							delete []
//		#else				
//			/*LYNXINLINE void	*operator							new(size_t s) {return LYNX_MALLOC((DWORD)s);};								
//			LYNXINLINE void	operator							delete(void *p) {LYNX_FREE(p);};				
//			LYNXINLINE void	*operator							new[](size_t s) {return LYNX_MALLOC((DWORD)s);};								
//			LYNXINLINE void	operator							delete[](void *p) {LYNX_FREE(p);};					*/
//
//			#define	LYNXNEW								new
//			#define	LYNXDEL								delete					
//			#define	LYNXDELARRAY							delete []
//		#endif		
//	#endif	
//#endif

#endif
