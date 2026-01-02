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

#ifndef __LYNXUNKOWN_H__
#define __LYNXUNKOWN_H__

#include <LynxType.h>
#include <LynxEngineDefines.h>
#include <LynxEngineClasses.h>

namespace LynxEngine
{
	/**
	*  @brief Base object class of the entire engine。
	*
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter.
	*
	*/
	class LYNXENGCLASS CUnknown
	{
		LYNX_DECLARE_CLASS(CUnknown);
	protected:
	public:
		CUnknown() { };
		virtual ~CUnknown() {};

	#if defined(__LYNX_DEBUG_MEM_ALLOCATOR__) || defined(__LYNX_SMALLOBJ_MEM_ALLOCATOR__)
		static void*						operator new(size_t s);
		static void*						operator new[](size_t s);
		static void*						operator new(size_t s, const char *file, const char *func, int line);
		static void*						operator new[](size_t s, const char *file, const char *func, int line);

		static void							operator delete(void* p, size_t s);
		static void							operator delete[](void* p, size_t s);
		static void							operator delete(void* p, const char *file, const char *func, int line);
		static void							operator delete[](void* p, const char *file, const char *func, int line);		
	#endif	
	};	
}

#endif