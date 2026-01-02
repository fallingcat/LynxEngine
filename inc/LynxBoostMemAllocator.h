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

#ifndef __LYNXBOOSTMEMALLOCATOR_H__
#define __LYNXBOOSTMEMALLOCATOR_H__

#include <LynxMemAllocator.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CBoostMemAllocator : public CMemAllocator
	{	
		LYNX_DECLARE_CLASS(CBoostMemAllocator);
	public:		
		//std::allocator<char [256]>							m_Pool;
	public:		
		CBoostMemAllocator() {};		
		virtual ~CBoostMemAllocator() {};		

		LYNXINLINE virtual void*					vAlloc(size_t s) 
		{
			if (s < 256)
				//return 	m_Pool.allocate(s);
				return LYNX_MALLOC((DWORD)s); 
			else
				return LYNX_MALLOC((DWORD)s); 
		}
		LYNXINLINE virtual void						vFree(void* p) {return LYNX_FREE(p); };
	};
}
#endif