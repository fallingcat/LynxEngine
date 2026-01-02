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
#include <LynxDebugMemAllocator.h>
#include <LynxAlgorithm.h>

namespace LynxEngine 
{	
	CList<CMemAllocator::MemoryChunk> CDebugMemAllocator::m_MemoryChunkList;
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CDebugMemAllocator::CDebugMemAllocator()
	{
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CDebugMemAllocator::~CDebugMemAllocator(void)
	{			
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void* CDebugMemAllocator::vAlloc(const size_t s)
	{
		return lynxMemAlloc(s);

		//LYNX_ASSERT(LYNX_FALSE);
		//return NULL;

		CMemAllocator::MemoryChunk Chunk;

		Chunk.Address = lynxMemAlloc(s);
		Chunk.Size = s;
		strcpy(Chunk.File, "Unknown File");
		strcpy(Chunk.Function, "Unknown Function");
		Chunk.Line = 0;

		CList<CMemAllocator::MemoryChunk>::CIterator i = LynxEngine::lower_bound(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), Chunk.Address);		
		i = m_MemoryChunkList.insert(i, Chunk);			

		return Chunk.Address;				
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void* CDebugMemAllocator::vAlloc(const size_t s, const char *file, const char *func, int line)
	{
		if (s <= 0)
			return NULL;

		CMemAllocator::MemoryChunk Chunk;

		//Chunk.Address = lynxMemAlloc_d(s, file, func, line);		
		//Chunk.Address = ::new LYNXBYTE[s];
		Chunk.Address = lynxMemAlloc(s);
		Chunk.Size = s;
		strcpy(Chunk.File, file);
		strcpy(Chunk.Function, func);
		Chunk.Line = line;

		CList<CMemAllocator::MemoryChunk>::CIterator i = LynxEngine::lower_bound(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), Chunk.Address);		
		i = m_MemoryChunkList.insert(i, Chunk);			

		return Chunk.Address;		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugMemAllocator::vFree(void* p)
	{
		/*
		CList<CMemAllocator::MemoryChunk>::CIterator i = LynxEngine::find(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), p);
		if (i != m_MemoryChunkList.end())
			m_MemoryChunkList.erase(i);
		
		return free(p);
		*/


		CList<CMemAllocator::MemoryChunk>::CIterator i = LynxEngine::lower_bound(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), p);
		if (i != m_MemoryChunkList.end() && !(p < (*i)))
			m_MemoryChunkList.erase(i);

		return lynxMemFree(p);		

		/*


		CList<CMemAllocator::MemoryChunk>::CIterator i = LynxEngine::lower_bound(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), p);		
		if (i != m_MemoryChunkList.end() && !(p < (*i)))		
			m_MemoryChunkList.erase(i);		
		else
		{
			//void* TempP = (void*)((LYNXLONG)p - 0x4);
			void* TempP = p;
			i = LynxEngine::lower_bound(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), TempP);		
			//i = LynxEngine::find(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), TempP);		
			if (i != m_MemoryChunkList.end() && (TempP < (*i)))		
				m_MemoryChunkList.erase(i);		
		}
		//return lynxMemFree_d(p);
		return free(p);
		*/

		/*
		CList<CMemAllocator::MemoryChunk>::CIterator i = LynxEngine::lower_bound(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), p);		
		//CList<CMemAllocator::MemoryChunk>::CIterator i = LynxEngine::find(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), p);		

		if (i != m_MemoryChunkList.end())		
			m_MemoryChunkList.erase(i);		
		
		return lynxMemFree_d(p);
		*/
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugMemAllocator::vFree(void* p, size_t s)
	{
		return vFree(p);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugMemAllocator::vFree(void* p, const char *file, const char *func, int line)
	{
		vFree(p);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugMemAllocator::vFree(void* p, size_t s, const char *file, const char *func, int line)
	{
		vFree(p, s);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugMemAllocator::vRemoveRecord(void* p)
	{	
		CList<CMemAllocator::MemoryChunk>::CIterator i = LynxEngine::lower_bound(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), p);		
		//CList<CMemAllocator::MemoryChunk>::CIterator i = LynxEngine::find(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), p);		
		if (i != m_MemoryChunkList.end() && !(p < (*i)))		
			m_MemoryChunkList.erase(i);		
		else
		{
			// Issue : ::new and malloc will have different padding. ::new will align to the structure size but malloc only align to byte size.
			// Stored address will be different from the address used for free.

			//void* TempP = (void*)((LYNXLONG)p - 0x4);
			void* TempP = p;
			i = LynxEngine::lower_bound(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), TempP);		
			//i = LynxEngine::find(m_MemoryChunkList.begin(), m_MemoryChunkList.end(), TempP);		
			if (i != m_MemoryChunkList.end() && (TempP < (*i)))		
				m_MemoryChunkList.erase(i);		
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CDebugMemAllocator::vReset()
	{
		m_MemoryChunkList.clear();
	}
}