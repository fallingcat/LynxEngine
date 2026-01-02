//##########################################################################
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

#ifndef __LYNXFIXEDALLOCATOR_H__
#define __LYNXFIXEDALLOCATOR_H__

#include <LynxEngineClasses.h>

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE 64
#endif

namespace LynxEngine
{
    class LYNXENGCLASS CFixedAllocator
    {
    public:
        class CChunk
        {           
		public:
            void							Init(size_t blockSize, unsigned char blocks);
            void*							Allocate(size_t blockSize);
            void							Deallocate(void* p, size_t blockSize);
            void							Reset(size_t blockSize, unsigned char blocks);
            void							Release();

            unsigned char*					m_pData;
            unsigned char           		m_FirstAvailableBlock, m_BlocksAvailable;

		friend class CFixedAllocator;
        };
	private:
		typedef CArray<CChunk>				Chunks;
		// Data 
        size_t								m_BlockSize;
        unsigned char						m_NumBlocks;		
        Chunks								m_Chunks;
        CChunk*								m_AllocChunk;
        CChunk*								m_DeallocChunk;
        // For ensuring proper copy semantics
        mutable const CFixedAllocator*		m_Prev;
        mutable const CFixedAllocator*		m_Next;
        
    private:
        // Internal functions        
        void								DoDeallocate(void* p);
        CChunk*								VicinityFind(void* p);                
    public:
        // Create a FixedAllocator able to manage blocks of 'blockSize' size
        explicit CFixedAllocator(size_t blockSize = 0);
        CFixedAllocator(const CFixedAllocator&);
        CFixedAllocator& operator=(const CFixedAllocator&);
        ~CFixedAllocator();        
		
		//VS 2008 fix for lower_bound()
		LYNXINLINE operator size_t() const { return GetBlockSize(); }  

		// Comparison operator for sorting 
        LYNXINLINE bool operator <(const size_t rhs) const { return GetBlockSize() < rhs; }
		LYNXINLINE bool operator ==(const size_t rhs) { return GetBlockSize() == rhs; }

        void								Swap(CFixedAllocator& rhs);        
        // Allocate a memory block
        void*								Allocate();
        // Deallocate a memory block previously allocated with Allocate()
        // (if that's not the case, the behavior is undefined)
        void								Deallocate(void* p);
        // Returns the block size with which the FixedAllocator was initialized
        LYNXINLINE size_t					GetBlockSize() const { return m_BlockSize; }        
		LYNXINLINE size_t					GetNumBlocksPerChunk() const { return m_NumBlocks; }
		LYNXINLINE int						GetNumAllocatedBlocks() 
		{ 
			int Num = 0;
			CArray<CChunk>::CIterator C;
			for (C=m_Chunks.begin(); C!=m_Chunks.end(); C++)
			{
				Num += (m_NumBlocks-(*C).m_BlocksAvailable);
			}
			return Num;
		}
		LYNXINLINE size_t					GetNumChunks() const { return m_Chunks.size(); }
    };					
}

#endif