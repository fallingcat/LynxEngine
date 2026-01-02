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

#ifndef __LYNXSMALLOBJALLOCATOR_H__
#define __LYNXSMALLOBJALLOCATOR_H__

#include <LynxMemAllocator.h>
#include <LynxFixedAllocator.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CSmallObjAllocator
	{	
		LYNX_DECLARE_CLASS(CSmallObjAllocator);
	protected:		
	private:		
		typedef CArray<CFixedAllocator>	CPool;		

        static CPool					m_Pool;
		static CFixedAllocator*			m_pLastAlloc;
		static CFixedAllocator*			m_pLastDealloc;
		static size_t					m_ChunkSize;
		static size_t					m_MaxObjectSize;
	public:		
		CSmallObjAllocator(size_t chunkSize = 4096, size_t maxObjectSize = 512);		
		virtual ~CSmallObjAllocator();		

		static LYNXINLINE CPool&		GetPool() { return m_Pool; };

		static void*					vAlloc(const size_t s);
		static void*					vAlloc(const size_t s, const char *file, const char *func, int line);
		static void						vFree(void* p, size_t s);
		static void						vFree(void* p, size_t s, const char *file, const char *func, int line);
		static CList<CMemAllocator::MemoryChunk>* vGetMemoryChunkList() { return NULL; };
		static void						vFree(void* p);
		static void						vFree(void* p, const char *file, const char *func, int line);
		static void						vRemoveRecord(void* p) {};
	private:
		CSmallObjAllocator(const CSmallObjAllocator&);
		CSmallObjAllocator& operator=(const CSmallObjAllocator&);
	};
}
#endif
