//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#ifndef __LYNXRELEASEMEMALLOCATOR_H__
#define __LYNXRELEASEMEMALLOCATOR_H__

#include <LynxMemAllocator.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CReleaseMemAllocator
	{	
		LYNX_DECLARE_CLASS(CReleaseMemAllocator);
	protected:
	public:		
		CReleaseMemAllocator();		
		virtual ~CReleaseMemAllocator();		

		static void*					vAlloc(const size_t s);
		static void*					vAlloc(const size_t s, const char *file, const char *func, int line);
		static void						vFree(void* p);
		static void						vFree(void* p, size_t s);
		static void						vFree(void* p, const char *file, const char *func, int line);
		static void						vFree(void* p, size_t s, const char *file, const char *func, int line);
		static CList<CMemAllocator::MemoryChunk>* vGetMemoryChunkList() { return NULL; };
		static void						vRemoveRecord(void* p) {};
	};
}
#endif