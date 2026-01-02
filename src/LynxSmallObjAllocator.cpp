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
	CSmallObjAllocator::CPool	CSmallObjAllocator::m_Pool;
	CFixedAllocator*			CSmallObjAllocator::m_pLastAlloc;
	CFixedAllocator*			CSmallObjAllocator::m_pLastDealloc;
	size_t						CSmallObjAllocator::m_ChunkSize = 4096;
	size_t						CSmallObjAllocator::m_MaxObjectSize = 512;
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CSmallObjAllocator::CSmallObjAllocator(size_t chunkSize, size_t maxObjectSize)		
	{
		m_pLastAlloc = 0;
		m_pLastDealloc = 0;
		m_ChunkSize = chunkSize;
		m_MaxObjectSize = maxObjectSize;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CSmallObjAllocator::~CSmallObjAllocator(void)
	{			
		m_Pool.clear();
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void* CSmallObjAllocator::vAlloc(const size_t s)
	{
		if (s > m_MaxObjectSize) 
			return lynxMemAlloc(s);
    
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
	void* CSmallObjAllocator::vAlloc(size_t s, const char *file, const char *func, int line)
	{
		void* lpP = vAlloc(s);
		
		return lpP;			
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CSmallObjAllocator::vFree(void* p, size_t s)
	{
		if (s > m_MaxObjectSize) 
		{
			lynxMemFree(p);
			return;
		}
		
		if (m_pLastDealloc && m_pLastDealloc->GetBlockSize() == s)
		{
			m_pLastDealloc->Deallocate(p);			
			return;
		}
	
		CPool::CIterator i = LynxEngine::lower_bound(m_Pool.begin(), m_Pool.end(), s);		
		#if (defined(_DEBUG) || defined(DEBUG))
			LYNX_ASSERT(i != m_Pool.end());
			LYNX_ASSERT(i->GetBlockSize() == s);			
		#else
			if (i == m_Pool.end())
			{
				lynxMemFree(p);
				return;
			}
		#endif
		
		m_pLastDealloc = &(*i);
		m_pLastDealloc->Deallocate(p);		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CSmallObjAllocator::vFree(void* p, size_t s, const char *file, const char *func, int line)
	{
		vFree(p, s);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CSmallObjAllocator::vFree(void* p)
	{
		LYNX_ASSERT(LYNX_FALSE);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CSmallObjAllocator::vFree(void* p, const char *file, const char *func, int line)
	{
		LYNX_ASSERT(LYNX_FALSE);
	}
}
