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

#ifndef __LYNXMEMALLOCATOR_H__
#define __LYNXMEMALLOCATOR_H__

#include <LynxCore.h>
#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CMemAllocator
	{	
		LYNX_DECLARE_CLASS(CMemAllocator);
	public:
		typedef struct
		{
			char				File[256];
			char				Function[256];
			int					Line;			
			size_t				Size;
			void*				Address;							

			//VS 2008 fix for lower_bound()
			LYNXINLINE operator void*() const { return Address; }  

			// Comparison operator for sorting 
			LYNXINLINE bool operator <(void* rhs) const { return (LYNXLONG)Address < (LYNXLONG)rhs; }
			LYNXINLINE bool operator <=(void* rhs) const { return (LYNXLONG)Address <= (LYNXLONG)rhs; }
			LYNXINLINE bool operator >(void* rhs) const { return (LYNXLONG)Address > (LYNXLONG)rhs; }
			LYNXINLINE bool operator >=(void* rhs) const { return (LYNXLONG)Address >= (LYNXLONG)rhs; }
			LYNXINLINE bool operator ==(void* rhs) const { return (LYNXLONG)Address == (LYNXLONG)rhs; }
		}MemoryChunk;
	protected:		
	public:		
		CMemAllocator() {};		
		virtual ~CMemAllocator() {};		

		virtual void*					vAlloc(const size_t s) = 0;
		virtual void*					vAlloc(const size_t s, const char *file, const char *func, int line) = 0;
		virtual void					vFree(void* p) = 0;
		virtual void					vFree(void* p, const char *file, const char *func, int line) = 0;
		virtual void					vFree(void* p, size_t s) = 0;
		virtual void					vFree(void* p, size_t s, const char *file, const char *func, int line) = 0;
		virtual void					vRemoveRecord(void* p) = 0;
		virtual CList<MemoryChunk>*		vGetMemoryChunkList() = 0;
		virtual void					vReset() {};
	};	
	LYNXFORCEINLINE bool operator ==(const CMemAllocator::MemoryChunk lhs, CMemAllocator::MemoryChunk rhs) {return (((LYNXLONG)lhs.Address) == ((LYNXLONG)rhs.Address)); };	
}
#endif