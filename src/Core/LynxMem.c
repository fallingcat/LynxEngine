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

#include <LynxLog.h>
#include <LynxMem.h>

#ifdef __WIN32__
	#include <crtdbg.h>
#endif

#ifdef __NUCLEUS__
	#ifdef __SIS_MOD__				
		extern NU_MEMORY_POOL					System_Memory;
	#endif
#endif

#ifdef __iOS__
    LYNXLONGLONG lynxiOSGetMaxPhysicalMemorySize();
#endif

#ifdef __MEMORY_MANAGER__	
	LYNXMEMMANAGER							gMemManager;		
	static int								gCurrentMemBlock = 0;
#endif

//-------------------------------------------------------------------------------------------------------
#ifdef __DOXY_ENGLISH__
/**
 *  @ingroup LynxMem 
 *  @brief Allocate memory.
 *
 *	Allocate memory block, normal version.
 *	@param size <in>The size of memory block will be allocated.
 *	@return The address of allocated memory block. 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#else
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 *
 *	記憶體配置，一般版本。
 *	@param size 配置記憶體之大小(bytes)。
 *	@return 所配置之記憶體位址。 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#endif
//-------------------------------------------------------------------------------------------------------
 void	*_lynxMemAlloc(size_t size)
{
	int				i;
	void			*lpP; 

	if (size == 0)
		return NULL;	
	
	#ifdef __WIN32__
		lpP = malloc(size);					
	#elif defined __NUCLEUS__
		NU_Allocate_Memory(&System_Memory, &lpP, size, NU_NO_SUSPEND);				
	#else
		lpP = (LPVOID)malloc(size);				 
	#endif	
	
	#ifdef _DEBUG
		if (lpP)
		{
			lynxMemSet(lpP, 0xcc, size);			
		}
		else
		{
			lynxDebugOutputPrintf(_T("Memory allocation error(size : %d)!!\n"), size);
		}
	#endif
	
	return lpP;	
}
//-------------------------------------------------------------------------------------------------------
#ifdef __DOXY_ENGLISH__
/**
 *  @ingroup LynxMem 
 *  @brief Allocate memory.
 *
 *	Allocate memory block, debug version.
 *	@param size <in>The size of memory block will be allocated.
 *	@param file <in>The file name in which this function is called.
 *	@param size <in>The line number of the file in which this function is called.
 *	@return The address of allocated memory block. 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#else
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 *
 *	記憶體配置，一般版本。
 *	@param size 配置記憶體之大小(bytes)。
 *	@return 所配置之記憶體位址。 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#endif
//-------------------------------------------------------------------------------------------------------
void *_lynxMemAlloc_d(size_t size, const char *file, const char *func, int line)
{
	int				i;
	void			*lpP; 
	size_t			Size;		

	if (size == 0)
		return NULL;	
	
	#ifdef __WIN32__				
		lpP = _malloc_dbg(size, _NORMAL_BLOCK, file, line);				
	#elif defined __NUCLEUS__
		NU_Allocate_Memory(&System_Memory, &lpP, size, NU_NO_SUSPEND);				
	#else
		lpP = (LPVOID)malloc(size);		
	#endif	
	
	#ifdef _DEBUG
		if (lpP)
		{
			lynxMemSet(lpP, 0xcc, size);			
		}
		else
		{
			lynxDebugOutputPrintf(_T("Memory allocation error(%s(%s) at line %d, size : %d)!!\n"), file, func, line, size);
		}
	#endif

	#ifdef __MEMORY_MANAGER__	
		lynxRecordMemBlock(lpP, size, file, func, line);	
	#endif	
	
	return lpP;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 *
 *	位址對齊式記憶體配置，一般版本。
 *	@param bound 對齊單位(bits)。 
 *	@param size 配置記憶體之大小(bytes)。
 *	@return 所配置之記憶體位址。 
 */
//-------------------------------------------------------------------------------------------------------
void* lynxAlignMemAlloc(int bound, size_t size)
{   
	int				i;
	void			*lpP;

	if (size == 0)
		return NULL;

	#ifdef __PS2__		
		lpP = (void *)memalign((bound>>3), size);
		#ifdef _DEBUG
			if (lp)
			{
				lynxMemSet(lp, 0xcc, size);
			}
		#endif
	#elif defined __WIN32__
		lpP = malloc(size+LYNX_CACHE_SIZE);		
	#elif defined __NUCLEUS__
		NU_Allocate_Memory(&System_Memory, &lpP, size+LYNX_CACHE_SIZE, NU_NO_SUSPEND);		
		#ifdef _DEBUG
			if (lpP)
			{
				lynxMemSet(lpP, 0xcc, size+LYNX_CACHE_SIZE);
			}
		#endif							
	#else				
		lpP = (LPVOID)malloc(size+LYNX_CACHE_SIZE);		
		#ifdef _DEBUG
			if (lpP)
			{
				lynxMemSet(lpP, 0xcc, size+LYNX_CACHE_SIZE);
			}
		#endif
	#endif		

	return lpP;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 *
 *	位址對齊式記憶體配置，一般版本。
 *	@param bound 對齊單位(bits)。 
 *	@param size 配置記憶體之大小(bytes)。
 *	@return 所配置之記憶體位址。 
 */
//-------------------------------------------------------------------------------------------------------
void* lynxAlignMemAlloc_d(int bound, size_t size, const char *file, const char *func, int line)
{   
	int				i;
	void			*lpP;

	if (size == 0)
		return NULL;

	#ifdef __PS2__		
		lpP = (void *)memalign((bound>>3), size);
		#ifdef _DEBUG
			if (lp)
			{
				lynxMemSet(lp, 0xcc, size);
			}
		#endif
	#elif defined __WIN32__
		//lpP = malloc(size+LYNX_CACHE_SIZE);
		lpP = _malloc_dbg(size+LYNX_CACHE_SIZE, _NORMAL_BLOCK, file, line);		
	#elif defined __NUCLEUS__
		NU_Allocate_Memory(&System_Memory, &lpP, size+LYNX_CACHE_SIZE, NU_NO_SUSPEND);		
		#ifdef _DEBUG
			if (lpP)
			{
				lynxMemSet(lpP, 0xcc, size+LYNX_CACHE_SIZE);
			}
		#endif							
	#else				
		lpP = (LPVOID)malloc(size+LYNX_CACHE_SIZE);		
		#ifdef _DEBUG
			if (lpP)
			{
				lynxMemSet(lpP, 0xcc, size+LYNX_CACHE_SIZE);
			}
		#endif
	#endif	

	#ifdef __MEMORY_MANAGER__	
		lynxRecordMemBlock(lpP, size+LYNX_CACHE_SIZE, file, func, line);	
	#endif

	return lpP;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 *
 *	位址對齊式記憶體配置，一般版本。
 *	@param bound 對齊單位(bits)。 
 *	@param size 配置記憶體之大小(bytes)。
 *	@return 所配置之記憶體位址。 
 */
//-------------------------------------------------------------------------------------------------------
void* lynxAlignMemAlloc32(size_t size)
{   
	return lynxAlignMemAlloc(32, size);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 *
 *	位址對齊式記憶體配置，一般版本。
 *	@param bound 對齊單位(bits)。 
 *	@param size 配置記憶體之大小(bytes)。
 *	@return 所配置之記憶體位址。 
 */
//-------------------------------------------------------------------------------------------------------
void* lynxAlignMemAlloc32_d(size_t size, const char *file, const char *func, int line)
{   
	return lynxAlignMemAlloc_d(32, size, file, func, line);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
	open 
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void _lynxMemFree(void *lp)
{	
	int				i;

	if (!lp)
		return;

	#ifdef __WIN32__        
		//HeapFree(GetProcessHeap(), 0, lp);
		free(lp);				
		//GlobalFree(lp);
		//CoTaskMemFree(lp);
	#elif defined __NUCLEUS__
		NU_Deallocate_Memory(lp);
	#else
		free(lp);				
	#endif		
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void _lynxMemFree_d(void *lp)
{	
	int				i;

	if (!lp)
		return;

	#ifdef __WIN32__        
		//free(lp);				
		_free_dbg(lp, _NORMAL_BLOCK);		
	#elif defined __NUCLEUS__
		NU_Deallocate_Memory(lp);
	#else
		free(lp);				
	#endif	

	#ifdef __MEMORY_MANAGER__	
		lynxReleaseMemBlock(lp);		
	#endif
}
//-------------------------------------------------------------------------------------------------------
static LPLYNXALLOCPROC						glpAllocFunc		= _lynxMemAlloc;
static LPLYNXALLOCPROCD						glpAllocFunc_d		= _lynxMemAlloc_d;
static LPLYNXDEALLOCPROC					glpDeallocFunc		= _lynxMemFree;
static LPLYNXDEALLOCPROCD					glpDeallocFunc_d	= _lynxMemFree_d;
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem  
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */ 
//-------------------------------------------------------------------------------------------------------
LYNXBOOL ilynxMemBlockCollideProc(const void *elem)
{
	LPLYNXMEMBLOCK		lpBlock;
	
	lpBlock = (LPLYNXMEMBLOCK)elem;

	if (lpBlock->Flag == LYNX_FREE_ENTRY)
		return LYNX_FALSE;
	else
		return LYNX_TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem  
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */ 
//-------------------------------------------------------------------------------------------------------
int ilynxMemBlockSearchCollideProc(const void *elem1, const void *elem2)
{
	long				*lpKey;
	LPLYNXMEMBLOCK		lpBlock;

	lpKey = (long *)elem1;	
	lpBlock = (LPLYNXMEMBLOCK)elem2;
	
	/*
	if (lpBlock->Flag == LYNX_FREE_ENTRY)
		return -1;
	else		
	*/
	{
		if (((long)(lpBlock->lpBase)) == (*lpKey))
			return 1;
		else
			return 0;	
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 * 
 */
//-------------------------------------------------------------------------------------------------------
void lynxRecordMemBlock(LPVOID lpp, size_t size, const char *file, const char *func, int line)
{
	int						i, j;	
	char					Buffer[256];
	
	#ifdef __MEMORY_MANAGER__	
		if (lpp)
		{
			i = lynxSimpleHash((long)lpp, gMemManager.lpMemBlock, LYNX_MAX_MEM_BLOCK, sizeof(LYNXMEMBLOCK), ilynxMemBlockCollideProc);		
			if (i >= 0)
			{
				gMemManager.TotalSize += (size+LYNX_CACHE_SIZE);
				gMemManager.lpMemBlock[i].Flag = LYNX_USED_ENTRY;
				gMemManager.lpMemBlock[i].lpBase = lpp;
				gMemManager.lpMemBlock[i].Size = (size);			
				strcpy(Buffer, file);
				for (j=(int)(strlen(Buffer)); j>=0; j--)
				{
					if (Buffer[j] == '/' || Buffer[j] == '\\')
					{
						break;
					}
				}
				strcpy(gMemManager.lpMemBlock[i].File, &Buffer[j+1]);				
				strcpy(gMemManager.lpMemBlock[i].Function, func);				
				gMemManager.lpMemBlock[i].Line = line;																				
			}
			else
			{
				lynxSetLogf(_T("Warning"), _T("Memory manager can't find any free block...\n"));			
			}
		}	
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 * 
 */
//-------------------------------------------------------------------------------------------------------
void lynxReleaseMemBlock(LPVOID lp)
{
	int						i, j;	
	
	#ifdef __MEMORY_MANAGER__	
		i = lynxSimpleHashSerach((long)lp, gMemManager.lpMemBlock, LYNX_MAX_MEM_BLOCK, sizeof(LYNXMEMBLOCK), ilynxMemBlockSearchCollideProc);		
		if (i >= 0)
		{			
			gMemManager.TotalSize -= gMemManager.lpMemBlock[i].Size;
			gMemManager.lpMemBlock[i].Flag = LYNX_FREE_ENTRY;
			gMemManager.lpMemBlock[i].lpBase = NULL;
			gMemManager.lpMemBlock[i].Size = 0;					
		}
		else
		{	
			/*
			for (i=0; i<LYNX_MAX_MEM_BLOCK; i++)
			{
				if (gMemManager.lpMemBlock[i].lpBase == lp)
				{
					gMemManager.TotalSize -= gMemManager.lpMemBlock[i].Size;
					gMemManager.lpMemBlock[i].Flag = LYNX_FREE_ENTRY;
					gMemManager.lpMemBlock[i].lpBase = NULL;
					gMemManager.lpMemBlock[i].Size = 0;		
					break;
				}
			}
			if (i == LYNX_MAX_MEM_BLOCK)
			*/
				lynxSetLogf(_T("Warning!!"), _T("Memory manager can't find allocated memory(%0x)\n"), lp);						
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxClearBadMemBlock(void)
{
	#ifdef __MEMORY_MANAGER__	
		int						i;

		if (gMemManager.TotalSize > 0)
		{
			for (i=0; i<LYNX_MAX_MEM_BLOCK; i++)
			{
				if (gMemManager.lpMemBlock[i].Flag == LYNX_USED_ENTRY)
				{
					lynxSetLogf(_T("Warning!!"), _T("Memory didn't free (%0x, size : %d, allocate by %s(%s) at line %d)\n"), gMemManager.lpMemBlock[i].lpBase, gMemManager.lpMemBlock[i].Size, gMemManager.lpMemBlock[i].File, gMemManager.lpMemBlock[i].Function, gMemManager.lpMemBlock[i].Line);					
					//free(gMemManager.lpMemBlock[i].lpBase);					
				}
			}
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LPLYNXMEMBLOCK lynxGetFirstMemBlock()
{
	#ifdef __MEMORY_MANAGER__	
		int i;

		gCurrentMemBlock = 0;
		for (i=0; i<LYNX_MAX_MEM_BLOCK; i++)
		{
			if (gMemManager.lpMemBlock[i].Flag == LYNX_USED_ENTRY)
			{
				gCurrentMemBlock = i;
				return &(gMemManager.lpMemBlock[i]);
			}				
		}
	#endif
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LPLYNXMEMBLOCK lynxGetNextMemBlock()
{
	#ifdef __MEMORY_MANAGER__	
		int i;

		for (i=gCurrentMemBlock+1; i<LYNX_MAX_MEM_BLOCK; i++)
		{
			if (gMemManager.lpMemBlock[i].Flag == LYNX_USED_ENTRY)
			{
				gCurrentMemBlock = i;
				return &(gMemManager.lpMemBlock[i]);
			}				
		}
	#endif
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSetMemFunc(LPLYNXALLOCPROC palloc, LPLYNXDEALLOCPROC pfree, LPLYNXALLOCPROCD palloc_d, LPLYNXDEALLOCPROCD pfree_d)
{
	glpAllocFunc = palloc;
	glpAllocFunc_d = palloc_d;
	glpDeallocFunc = pfree;
	glpDeallocFunc_d = pfree_d;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInitMemSystem(void)
{
    lynxGetAvailablePhysicalMem();
	#ifdef __MEMORY_MANAGER__	
		int						i;

		gMemManager.TotalSize = 0;
		for (i=0; i<LYNX_MAX_MEM_BLOCK; i++)
		{
			gMemManager.lpMemBlock[i].Flag = LYNX_FREE_ENTRY;
			gMemManager.lpMemBlock[i].lpBase = NULL;
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxAlignMemCpy(void *dest, const void *src, size_t size)
{
	while ((((long)dest) & 3) && size)
	{
		*(char *)dest = *(char *)src;
		dest = (void *)(((char *)dest) + 1);
		src = (void *)(((char *)src) + 1);
		size--;
	}
	memcpy(dest, src, size);

}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 
 *
 *	@param dest 目的記憶體位址。
 *	@param src 來源記憶體位址。
 *	@param size 複製長度(bytes)。
 *	@return 無。 
 */
//-------------------------------------------------------------------------------------------------------
int lynxMemCmp(void *dest, const void *src, size_t size)
{
	return memcmp(dest, src, size);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體複製。
 *
 *	@param dest 目的記憶體位址。
 *	@param src 來源記憶體位址。
 *	@param size 複製長度(bytes)。
 *	@return 無。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxMemCpy(void *dest, const void *src, size_t size)
{
	#ifdef __NINJA__
		njMemCopy(dest, src, size);
	#else
		memcpy(dest, src, size);
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxAlignMemSetB(void *dest, BYTE c, size_t sizet)
{
	/*
	DWORD					dwD;
	DWORD					SizeD, RSize;

	while ((((long)dest) & 3) && sizet)
	{
		*(char *)dest = c;
		dest = (void *)(((char *)dest) + 1);
		sizet--;
	}
	dwD = (c << 24) | (c << 16) | (c << 8) | c;
	SizeD = sizet >> 2;
	RSize = sizet % 4;

	_asm
	{
		mov     edi, dest
		mov		ecx, SizeD	
		mov		eax, dwD

		cld
LoopSet:
		mov		[edi], eax
		add		edi, 4
		dec		ecx
		jnz		LoopSet
		
		mov		dest, edi
	}

	while (RSize)
	{
		*(char *)dest = c;
		dest = (void *)(((char *)dest) + 1);
		RSize--;
	}*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxAlignMemSetW(void *dest, WORD w, size_t sizet)
{
	/*DWORD					dwD;
	DWORD					SizeD, RSize;

	while ((((long)dest) & 2) && sizet)
	{
		*(WORD *)dest = w;
		dest = (void *)(((WORD *)dest) + 1);
		sizet--;
	}

	dwD = (w << 16) | w;
	SizeD = sizet >> 1;
	RSize = sizet % 2;

	if (SizeD > 0)
	{
		_asm
		{
			mov     edi, dest
			mov		ecx, SizeD	
			mov		eax, dwD

			cld
	LoopSet1:
			mov		[edi], eax
			add		edi, 4
			dec		ecx
			jnz		LoopSet1

			mov		dest, edi
		}
	}

	while (RSize)
	{
		*(WORD *)dest = w;
		dest = (void *)(((WORD *)dest) + 1);
		RSize--;
	}*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxMemSet(void *dest, BYTE b, size_t sizet)
{
	memset(dest, b, sizet);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LPLYNXMEMBLOCK	lynxSearchMemBlock(void *lp)
{
	#ifdef __MEMORY_MANAGER__	
		int			Top, Mid, Bottom;

		Top = 0;
		Bottom = LYNX_MAX_MEM_BLOCK;// - 2;
		while (Top <= Bottom)
		{
			Mid = ((Top + Bottom) >> 1);		

			if (gMemManager.lpMemBlock[Mid].Flag == LYNX_USED_ENTRY && gMemManager.lpMemBlock[Mid].lpBase == lp)
			{
				return (&gMemManager.lpMemBlock[Mid]);			
			}			
		}	
	#endif
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void *lynxCalloc(size_t num, size_t size)
{
	LPVOID					lpP = NULL;

	#ifdef __NUCLEUS__
		NU_Allocate_Memory(&System_Memory, &lpP, size*num, NU_NO_SUSPEND);				
		memset(lpP, 0, size*num);
	#endif

	return lpP;
}
 //-------------------------------------------------------------------------------------------------------
#ifdef __DOXY_ENGLISH__
/**
 *  @ingroup LynxMem 
 *  @brief Allocate memory.
 *
 *	Allocate memory block, normal version.
 *	@param size <in>The size of memory block will be allocated.
 *	@return The address of allocated memory block. 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#else
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 *
 *	記憶體配置，一般版本。
 *	@param size 配置記憶體之大小(bytes)。
 *	@return 所配置之記憶體位址。 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#endif
//-------------------------------------------------------------------------------------------------------
void* lynxMemAlloc(size_t size)
{
	return glpAllocFunc(size);
}
//-------------------------------------------------------------------------------------------------------
#ifdef __DOXY_ENGLISH__
/**
 *  @ingroup LynxMem 
 *  @brief Allocate memory.
 *
 *	Allocate memory block, debug version.
 *	@param size <in>The size of memory block will be allocated.
 *	@param file <in>The file name in which this function is called.
 *	@param size <in>The line number of the file in which this function is called.
 *	@return The address of allocated memory block. 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#else
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 *
 *	記憶體配置，一般版本。
 *	@param size 配置記憶體之大小(bytes)。
 *	@return 所配置之記憶體位址。 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#endif
//-------------------------------------------------------------------------------------------------------
void* lynxMemAlloc_d(size_t size, const char *file, const char *func, int line)
{
	return glpAllocFunc_d(size, file, func, line);
}
//-------------------------------------------------------------------------------------------------------
#ifdef __DOXY_ENGLISH__
/**
 *  @ingroup LynxMem 
 *  @brief Allocate memory.
 *
 *	Allocate memory block, normal version.
 *	@param size <in>The size of memory block will be allocated.
 *	@return The address of allocated memory block. 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#else
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 *
 *	記憶體配置，一般版本。
 *	@param size 配置記憶體之大小(bytes)。
 *	@return 所配置之記憶體位址。 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#endif
//-------------------------------------------------------------------------------------------------------
 void	*lynxMemRealloc(LPVOID lpp, size_t size)
{
	int				i;
	void			*lpP; 

	if (size == 0)
		return NULL;	
	
	#ifdef __WIN32__				
		lpP = realloc(lpp, size);				
	#else
		lpP = (LPVOID)realloc(lpp, size);		
	#endif	
	
	#ifdef _DEBUG
		if (lpP)
		{
			lynxMemSet(lpP, 0xcc, size);
		}
	#endif		

	return lpP;	
}
//-------------------------------------------------------------------------------------------------------
#ifdef __DOXY_ENGLISH__
/**
 *  @ingroup LynxMem 
 *  @brief Allocate memory.
 *
 *	Allocate memory block, normal version.
 *	@param size <in>The size of memory block will be allocated.
 *	@return The address of allocated memory block. 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#else
/**
 *  @ingroup LynxMem 
 *  @brief 記憶體配置。
 *
 *	記憶體配置，一般版本。
 *	@param size 配置記憶體之大小(bytes)。
 *	@return 所配置之記憶體位址。 
 *  @see lynxMemAlloc_d(), lynxAlignMemAlloc() 
 */
#endif
//-------------------------------------------------------------------------------------------------------
 void	*lynxMemRealloc_d(LPVOID lpp, size_t size, const char *file, const char *func, int line)
{
	int				i;
	void			*lpP; 

	if (size == 0)
		return NULL;	
	
	#ifdef __WIN32__				
		//lpP = realloc(lpp, size);		
		lpP = _realloc_dbg(lpp, size, _NORMAL_BLOCK , file, line);				
	#else
		lpP = (LPVOID)realloc(lpp, size);		
	#endif	
	
	#ifdef _DEBUG
		if (lpP)
		{
			lynxMemSet(lpP, 0xcc, size);
		}
	#endif		

	return lpP;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxMemFree(void *lp)
{	
	glpDeallocFunc(lp);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxMemFree_d(void *lp)
{	
	glpDeallocFunc_d(lp);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
//  說明:   
//		Get System Memory Information
//-------------------------------------------------------------------------------------------------------
 DWORD	lynxGetMaxPhysicalMem(void)
{
	#ifdef __WIN32__
		#ifdef __WINAPP__
			return 0;
		#else
			MEMORYSTATUS				MemStatus;	
			DWORD						Size;

			MemStatus.dwLength = sizeof(MemStatus);
			GlobalMemoryStatus(&MemStatus);
			Size = (DWORD)MemStatus.dwTotalPhys;

			return Size;
		#endif
	#elif defined __PS2__
		unsigned int				FreeSize;	
		
		FreeSize = sceSifQueryMemSize();//sceSifQueryTotalFreeMemSize();
		lynxDebugOutputPrintf("Free Mem : %d \n", FreeSize);
		
		return (FreeSize);
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
//  說明:   
//		Get System Memory Information
//-------------------------------------------------------------------------------------------------------
 LYNXLONGLONG lynxGetAvailablePhysicalMem(void)
{
	#ifdef WIN32
		#ifdef __WINAPP__
			return 0;
		#else
			MEMORYSTATUS				MemStatus;	
			LYNXLONGLONG				Size;

			MemStatus.dwLength = sizeof(MemStatus);
			GlobalMemoryStatus(&MemStatus);
			Size = (LYNXLONGLONG)MemStatus.dwAvailPhys ;

			return Size;
		#endif
	#elif defined __PS2__
		unsigned int				FreeSize;	
		
		FreeSize = sceSifQueryMemSize();//sceSifQueryTotalFreeMemSize();
		lynxDebugOutputPrintf("Free Mem : %d \n", FreeSize);
		
		return (FreeSize);
	#elif defined __NUCLEUS__
		UNSIGNED					FreeSize;

		NU_Memory_Pool_Information(&System_Memory, "SYSMEM",
					NULL, NULL,
					NULL, &FreeSize,
					NULL, NULL,
					NULL);  		

		return (LYNXLONGLONG)FreeSize;
    #elif defined __iOS__
        return lynxiOSGetMaxPhysicalMemorySize();
    
        // Get memory info
        int mib[2];
        uint64_t UserMemorySize;
        size_t length;
        //mib[0] = CTL_HW;
        mib[1] = HW_USERMEM;
        //mib[1] = HW_PHYSMEM;
        length = sizeof(int64_t);
        sysctl(mib, 2, &UserMemorySize, &length, NULL, 0);
    
        struct task_basic_info info;
        mach_msg_type_number_t size = sizeof(info);
        kern_return_t kerr = task_info(mach_task_self(),
                                       TASK_BASIC_INFO,
                                       (task_info_t)&info,
                                       &size);
        return (LYNXLONGLONG)(UserMemorySize - info.resident_size);
    
    
        /*
		mach_port_t host_port;
		mach_msg_type_number_t host_size;
		vm_size_t pagesize;

		host_port = mach_host_self();
		host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
		host_page_size(host_port, &pagesize);        

		vm_statistics_data_t vm_stat;

		if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS) 
		{
			lynxDebugOutputPrintf("Failed to fetch vm statistics");
			return 0;
		}

		// Stats in bytes 
		LYNXLONGLONG mem_used = (vm_stat.active_count +
							  vm_stat.inactive_count +
							  vm_stat.wire_count) * pagesize;
		LYNXLONGLONG mem_free = vm_stat.free_count * pagesize;
		LYNXLONGLONG mem_total = mem_used + mem_free;
    
        return (LYNXLONGLONG)mem_free;
		*/

        /*
		// Get memory info
		LYNXUINT64 Free;

		int mib[2];
		LYNXLONGLONG UserMemorySize;
		LYNXLONGLONG PhysicalMemorySize;
		size_t length;
		mib[0] = CTL_HW;
    
		mib[1] = HW_MEMSIZE;
		length = sizeof(LYNXLONGLONG);
		sysctl(mib, 2, &PhysicalMemorySize, &length, NULL, 0);
    
		mib[1] = HW_USERMEM;
		length = sizeof(LYNXLONGLONG);
		sysctl(mib, 2, &UserMemorySize, &length, NULL, 0);

		Free = PhysicalMemorySize - UserMemorySize;
    
        return (LYNXLONGLONG)UserMemorySize;
         */
	#elif defined __ANDROID__
		struct mallinfo info = mallinfo();
		return (LYNXLONGLONG) info.fordblks;
	#endif
}
 //-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
//  說明:   
//		Get System Memory Information
//-------------------------------------------------------------------------------------------------------
LYNXLONGLONG lynxGetUsedPhysicalMem(void)
{
	LYNXLONGLONG Used;
	#ifdef __WIN32__
		PROCESS_MEMORY_COUNTERS Counters;
		GetProcessMemoryInfo(GetCurrentProcess(), &Counters, sizeof(Counters));
		Used = Counters.WorkingSetSize;
	#endif
	#ifdef __iOS__
        struct task_basic_info info;
        mach_msg_type_number_t size = sizeof(info);
        kern_return_t kerr = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &size);
        Used = (LYNXLONGLONG)info.resident_size;
	#endif
	#ifdef __ANDROID__
		struct mallinfo info = mallinfo();
		Used = (LYNXLONGLONG) info.uordblks;
	#endif
	return Used;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
//  說明:   
//		Get Video Memory Information
//-------------------------------------------------------------------------------------------------------
 DWORD	lynxGetMaxVideoMem(void)
{
	return 0;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
//  說明:   
//		Get Video Memory Information
//-------------------------------------------------------------------------------------------------------
 DWORD	lynxGetAvailableVideoMem(void)
{
	return 0;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 DWORD	lynxGetAllocMemSize(void)
{
	#ifdef __MEMORY_MANAGER__	
		return gMemManager.TotalSize;
	#else
		return 0;
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMem 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxByteOrder(LPVOID lpd, int size)
{
    BYTE			Data[8], *lpC;
    int				i;
    
    lynxMemCpy(Data, lpd, size);
    lpC = (BYTE *)lpd;
    for (i=0; i<size; i++)
    {
        *lpC = Data[size-1-i];
        lpC++;
    }
}
