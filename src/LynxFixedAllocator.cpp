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

#include <LynxEngine_PCH.h>
#include <LynxFixedAllocator.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CFixedAllocator::CChunk::Init(size_t blockSize, unsigned char blocks)
	{
		LYNX_ASSERT(blockSize > 0);
		LYNX_ASSERT(blocks > 0);
		// Overflow check
		LYNX_ASSERT((blockSize * blocks) / blockSize == blocks);
	    
		m_pData = LYNXGLOBAL_NEW unsigned char[blockSize * blocks];		
		Reset(blockSize, blocks);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CFixedAllocator::CChunk::Reset(size_t blockSize, unsigned char blocks)
	{
		LYNX_ASSERT(blockSize > 0);
		LYNX_ASSERT(blocks > 0);
		// Overflow check
		LYNX_ASSERT((blockSize * blocks) / blockSize == blocks);

		m_FirstAvailableBlock = 0;
		m_BlocksAvailable = blocks;

		unsigned char i = 0;
		unsigned char* p = m_pData;
		for (; i != blocks; p += blockSize)
		{
			*p = ++i;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CFixedAllocator::CChunk::Release()
	{
		LYNXGLOBAL_DEL_ARRAY(m_pData);		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void* CFixedAllocator::CChunk::Allocate(size_t blockSize)
	{
		if (!m_BlocksAvailable) return 0;
	    
		LYNX_ASSERT((m_FirstAvailableBlock * blockSize) / blockSize == m_FirstAvailableBlock);

		unsigned char* pResult = m_pData + (m_FirstAvailableBlock * blockSize);
		m_FirstAvailableBlock = *pResult;
		--m_BlocksAvailable;
	    
		return pResult;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CFixedAllocator::CChunk::Deallocate(void* p, size_t blockSize)
	{
		LYNX_ASSERT(p >= m_pData);

		unsigned char* toRelease = static_cast<unsigned char*>(p);
		// Alignment check
		LYNX_ASSERT((toRelease - m_pData) % blockSize == 0);

		*toRelease = m_FirstAvailableBlock;
		m_FirstAvailableBlock = static_cast<unsigned char>((toRelease - m_pData) / blockSize);
		// Truncation check
		LYNX_ASSERT(m_FirstAvailableBlock == (toRelease - m_pData) / blockSize);

		++m_BlocksAvailable;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CFixedAllocator::CFixedAllocator(size_t blockSize)
		: m_BlockSize(blockSize)
		, m_AllocChunk(0)
		, m_DeallocChunk(0)
	{
		LYNX_ASSERT(m_BlockSize > 0);
	    
		m_Prev = m_Next = this;

		size_t NumBlocks = DEFAULT_CHUNK_SIZE / blockSize;
		if (NumBlocks > UCHAR_MAX) 
			NumBlocks = UCHAR_MAX;
		else if (NumBlocks == 0) 
			NumBlocks = 8 * blockSize;
	    
		m_NumBlocks = static_cast<unsigned char>(NumBlocks);
		LYNX_ASSERT(m_NumBlocks == NumBlocks);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CFixedAllocator::CFixedAllocator(const CFixedAllocator& rhs)
		: m_BlockSize(rhs.m_BlockSize)
		, m_NumBlocks(rhs.m_NumBlocks)
		, m_Chunks(rhs.m_Chunks)
	{
		m_Prev = &rhs;
		m_Next = rhs.m_Next;
		rhs.m_Next->m_Prev = this;
		rhs.m_Next = this;
	    
		m_AllocChunk = rhs.m_AllocChunk
			? &m_Chunks.front() + (rhs.m_AllocChunk - &rhs.m_Chunks.front())
			: 0;

		m_DeallocChunk = rhs.m_DeallocChunk
			? &m_Chunks.front() + (rhs.m_DeallocChunk - &rhs.m_Chunks.front())
			: 0;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CFixedAllocator& CFixedAllocator::operator=(const CFixedAllocator& rhs)
	{
		CFixedAllocator copy(rhs);
		copy.Swap(*this);
		return *this;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CFixedAllocator::~CFixedAllocator()
	{
		if (m_Prev != this)
		{
			m_Prev->m_Next = m_Next;
			m_Next->m_Prev = m_Prev;
			return;
		}
	    
		LYNX_ASSERT(m_Prev == m_Next);
		Chunks::CIterator i = m_Chunks.begin();
		for (; i != m_Chunks.end(); ++i)
		{
		   LYNX_ASSERT(i->m_BlocksAvailable == m_NumBlocks);
		   i->Release();
		}		
		m_Chunks.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CFixedAllocator::Swap(CFixedAllocator& rhs)
	{
		using namespace std;
	    
		swap(m_BlockSize, rhs.m_BlockSize);
		swap(m_NumBlocks, rhs.m_NumBlocks);
		m_Chunks.swap(rhs.m_Chunks);
		swap(m_AllocChunk, rhs.m_AllocChunk);
		swap(m_DeallocChunk, rhs.m_DeallocChunk);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void* CFixedAllocator::Allocate()
	{
		if (m_AllocChunk == 0 || m_AllocChunk->m_BlocksAvailable == 0)
		{
			Chunks::CIterator i = m_Chunks.begin();
			for (;; ++i)
			{
				if (i == m_Chunks.end())
				{
					// Initialize
					m_Chunks.reserve(m_Chunks.size() + 1);
					CChunk NewChunk;
					NewChunk.Init(m_BlockSize, m_NumBlocks);
					m_Chunks.push_back(NewChunk);
					m_AllocChunk = &m_Chunks.back();
					m_DeallocChunk = &m_Chunks.front();
					break;
				}
				if (i->m_BlocksAvailable > 0)
				{
					m_AllocChunk = &*i;
					break;
				}
			}
		}
		LYNX_ASSERT(m_AllocChunk != 0);
		LYNX_ASSERT(m_AllocChunk->m_BlocksAvailable > 0);
	    
		return m_AllocChunk->Allocate(m_BlockSize);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CFixedAllocator::Deallocate(void* p)
	{
		LYNX_ASSERT(!m_Chunks.empty());
		LYNX_ASSERT(&m_Chunks.front() <= m_DeallocChunk);
		LYNX_ASSERT(&m_Chunks.back() >= m_DeallocChunk);
	    
		m_DeallocChunk  = VicinityFind(p);
		LYNX_ASSERT(m_DeallocChunk);

		DoDeallocate(p);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CFixedAllocator::CChunk* CFixedAllocator::VicinityFind(void* p)
	{
		LYNX_ASSERT(!m_Chunks.empty());
		LYNX_ASSERT(m_DeallocChunk);

		if (m_Chunks.size() == 1)
			return &(*m_Chunks.begin());

		const size_t chunkLength = m_NumBlocks * m_BlockSize;

		CChunk* lo = m_DeallocChunk;
		CChunk* hi = m_DeallocChunk + 1;
		CChunk* loBound = &m_Chunks.front();
		CChunk* hiBound = &m_Chunks.back() + 1;

		// Special case: m_DeallocChunk is the last in the array
		if (hi == hiBound) hi = 0;

		for (;;)
		{
			if (lo)
			{
				if (p >= lo->m_pData && p < lo->m_pData + chunkLength)
				{
					return lo;
				}
				if (lo == loBound) lo = 0;
				else --lo;
			}
	        
			if (hi)
			{
				if (p >= hi->m_pData && p < hi->m_pData + chunkLength)
				{
					return hi;
				}
				if (++hi == hiBound) hi = 0;
			}
		}
	    
		// LYNX_ASSERT(false);
		// return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CFixedAllocator::DoDeallocate(void* p)
	{
		LYNX_ASSERT(m_DeallocChunk->m_pData <= p);
		LYNX_ASSERT(m_DeallocChunk->m_pData + m_NumBlocks * m_BlockSize > p);

		// call into the chunk, will adjust the inner list but won't release memory
		m_DeallocChunk->Deallocate(p, m_BlockSize);

		if (m_DeallocChunk->m_BlocksAvailable == m_NumBlocks)
		{
			// m_DeallocChunk is completely free, should we release it? 
	        
			CChunk& lastChunk = m_Chunks.back();
	        
			if (&lastChunk == m_DeallocChunk)
			{
				// check if we have two last chunks empty
	            
				if (m_Chunks.size() > 1 && 
					m_DeallocChunk[-1].m_BlocksAvailable == m_NumBlocks)
				{
					// Two free chunks, discard the last one
					lastChunk.Release();
					m_Chunks.pop_back();
					m_AllocChunk = m_DeallocChunk = &m_Chunks.front();
				}
				return;
			}
	        
			if (lastChunk.m_BlocksAvailable == m_NumBlocks)
			{
				// Two free blocks, discard one
				lastChunk.Release();
				m_Chunks.pop_back();
				m_AllocChunk = m_DeallocChunk;
			}
			else
			{
				// move the empty chunk to the end
				std::swap(*m_DeallocChunk, lastChunk);
				m_AllocChunk = &m_Chunks.back();
			}
		}
	}
}