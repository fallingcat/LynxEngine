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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxEngine.h>
#include <LynxSmallObjAllocator.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CDebugSmallObjAllocator::CDebugSmallObjAllocator(GameFramework::CKernel* lpk, size_t chunkSize, size_t maxObjectSize)
	: CMemAllocator(lpk),
	m_pLastAlloc(0), m_pLastDealloc(0), 
    m_ChunkSize(chunkSize), m_MaxObjectSize(maxObjectSize) 
	{
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CDebugSmallObjAllocator::~CDebugSmallObjAllocator(void)
	{			
		m_Pool.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void* CDebugSmallObjAllocator::Alloc(const size_t s, const char *file, const char *func, int line)
	{
		if (s > m_MaxObjectSize) 
			return lynxMemAlloc_d(s, file, func, line);
    
		if (m_pLastAlloc && m_pLastAlloc->GetBlockSize() == s)
		{
			return m_pLastAlloc->Allocate();
		}
		CPool::CIterator i = LynxEngine::lower_bound(m_Pool.begin(), m_Pool.end(), s);		
		if (i == m_Pool.end() || i->GetBlockSize() != s)
		{			
			i = m_Pool.insert(i, CFixedAllocator(s));
			m_pLastDealloc = &(*m_Pool.begin());

			LYNX_ASSERT(m_Pool.size() < m_Pool.max_size()-1);
		}
		m_pLastAlloc = &(*i);

		return m_pLastAlloc->Allocate();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void* CDebugSmallObjAllocator::vAlloc(const size_t s)
	{
		LYNX_ASSERT(LYNX_FALSE);
		return NULL;

		/*return Alloc(s);*/
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void* CDebugSmallObjAllocator::vAlloc(size_t s, const char *file, const char *func, int line)
	{
		void* lpP = Alloc(s, file, func, line);

		CMemAllocator::MemoryChunk Chunk;

		Chunk.Address = lpP;
		Chunk.Size = s;
		strcpy(Chunk.File, file);
		strcpy(Chunk.Function, func);
		Chunk.Line = line;			
		CList<CMemAllocator::MemoryChunk>::CIterator i = LynxEngine::lower_bound(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), Chunk.Address);		
		i = m_MemoryChunkList.insert(i, Chunk);	
		//m_MemoryChunkList.push_back(Chunk);

		LYNX_ASSERT(m_MemoryChunkList.size() < m_MemoryChunkList.max_size());
		
		return lpP;			
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugSmallObjAllocator::vFree(void* p, size_t s)
	{
		CList<CMemAllocator::MemoryChunk>::CIterator MC = LynxEngine::lower_bound(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), p);					
		//CList<CMemAllocator::MemoryChunk>::CIterator MC = LynxEngine::find(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), p);					
		if (MC != m_MemoryChunkList.end() && (*MC).Address == p && (*MC).Size == s)
		{			
			LYNX_ASSERT(MC != m_MemoryChunkList.end());
			LYNX_ASSERT((*MC).Size == s);
			m_MemoryChunkList.erase(MC);

			//if (MC != m_MemoryChunkList.end())
			//	m_MemoryChunkList.erase(MC);
			
			if (s > m_MaxObjectSize) 
			{
				lynxMemFree_d(p);
				return;
			}
			
			if (m_pLastDealloc && m_pLastDealloc->GetBlockSize() == s)
			{
				m_pLastDealloc->Deallocate(p);			
				return;
			}
		
			CPool::CIterator i = LynxEngine::lower_bound(m_Pool.begin(), m_Pool.end(), s);		
			LYNX_ASSERT(i != m_Pool.end());
			LYNX_ASSERT(i->GetBlockSize() == s);
			m_pLastDealloc = &(*i);
			m_pLastDealloc->Deallocate(p);
		}
		else
		{
			lynxMemFree_d(p);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugSmallObjAllocator::vFree(void* p, size_t s, const char *file, const char *func, int line)
	{
		vFree(p, s);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugSmallObjAllocator::vFree(void* p)
	{
		lynxMemFree_d(p);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugSmallObjAllocator::vFree(void* p, const char *file, const char *func, int line)
	{
		lynxMemFree_d(p);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugSmallObjAllocator::vRemoveRecord(void* p)
	{		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugSmallObjAllocator::vReset()
	{
		m_MemoryChunkList.clear();
	}
}
