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

#ifndef __LYNXDEBUGMEMALLOCATOR_H__
#define __LYNXDEBUGMEMALLOCATOR_H__

#include <LynxMemAllocator.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CDebugMemAllocator
	{	
		LYNX_DECLARE_CLASS(CDebugMemAllocator);
	protected:
		static CList<CMemAllocator::MemoryChunk>	m_MemoryChunkList;
	public:		
		CDebugMemAllocator();		
		virtual ~CDebugMemAllocator();		

		static void*					vAlloc(const size_t s);
		static void*					vAlloc(const size_t s, const char *file, const char *func, int line);
		static void						vFree(void* p);
		static void						vFree(void* p, size_t s);
		static void						vFree(void* p, const char *file, const char *func, int line);
		static void						vFree(void* p, size_t s, const char *file, const char *func, int line);
		static void						vRemoveRecord(void* p);
		static CList<CMemAllocator::MemoryChunk>* vGetMemoryChunkList() {return &m_MemoryChunkList; };
		static void						vReset();
	};
}
#endif