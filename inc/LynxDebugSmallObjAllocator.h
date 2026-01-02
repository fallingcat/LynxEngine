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

#ifndef __LYNXDEBUGSMALLOBJALLOCATOR_H__
#define __LYNXDEBUGSMALLOBJALLOCATOR_H__

#include <LynxMemAllocator.h>
#include <LynxFixedAllocator.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CDebugSmallObjAllocator : public CMemAllocator
	{	
		LYNX_DECLARE_CLASS(CDebugSmallObjAllocator);
	protected:
		CList<CMemAllocator::MemoryChunk>	m_MemoryChunkList;		
	private:		
		typedef CArray<CFixedAllocator>	CPool;		

        CPool						m_Pool;
        CFixedAllocator*			m_pLastAlloc;
        CFixedAllocator*			m_pLastDealloc;
        size_t						m_ChunkSize;
        size_t						m_MaxObjectSize;
	public:		
		CDebugSmallObjAllocator(GameFramework::CKernel* lpk, size_t chunkSize, size_t maxObjectSize);		
		virtual ~CDebugSmallObjAllocator();		

		LYNXINLINE CPool&			GetPool() {return m_Pool; };

		void*						Alloc(const size_t s, const char *file, const char *func, int line);
		void*						vAlloc(const size_t s);
		void*						vAlloc(const size_t s, const char *file, const char *func, int line);
		void						vFree(void* p, size_t s);
		void						vFree(void* p, size_t s, const char *file, const char *func, int line);
		CList<CMemAllocator::MemoryChunk>* vGetMemoryChunkList() {return &m_MemoryChunkList; };		
		void						vFree(void* p);
		void						vFree(const void* p);
		void						vFree(void* p, const char *file, const char *func, int line);
		void						vRemoveRecord(void* p);
		void						vReset();
	private:
        CDebugSmallObjAllocator(const CSmallObjAllocator&);
        CDebugSmallObjAllocator& operator=(const CSmallObjAllocator&);
	};
}
#endif
