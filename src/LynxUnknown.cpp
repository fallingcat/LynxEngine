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

#include <LynxUnknown.h>
#include <LynxMemAllocator.h>

#if 0
static LynxEngine::CList<void*>  gArrayGlobalAllocList;
static LynxEngine::CList<void*>  gNoneArrayGlobalAllocList;

static LynxEngine::CList<void*>  gArrayAllocList;
static LynxEngine::CList<void*>  gNoneArrayAllocList;

void AddArrayGlobalAlloc(void* p)
{
	gArrayGlobalAllocList.push_back(p);
}

void CheckArrayGlobalFree(void* p)
{
	LynxEngine::CList<void*>::CIterator Found = LynxEngine::find(gNoneArrayGlobalAllocList.begin(), gNoneArrayGlobalAllocList.end(), p);
	if (Found != gNoneArrayGlobalAllocList.end())
	{
		int xxxx = 0;
	}
}

void AddNoneArrayGlobalAlloc(void* p)
{
	gNoneArrayGlobalAllocList.push_back(p);
}

void CheckNoneArrayGlobalFree(void* p)
{
	LynxEngine::CList<void*>::CIterator Found = LynxEngine::find(gArrayGlobalAllocList.begin(), gArrayGlobalAllocList.end(), p);
	if (Found != gArrayGlobalAllocList.end())
	{
		int xxxx = 0;
	}
}

void AddArrayAlloc(void* p)
{
	gArrayAllocList.push_back(p);
}

void CheckArrayFree(void* p)
{
	LynxEngine::CList<void*>::CIterator Found = LynxEngine::find(gArrayAllocList.begin(), gArrayAllocList.end(), p);
	if (Found == gArrayAllocList.end())
	{
		int xxxx = 0;
	}
}

void AddNoneArrayAlloc(void* p)
{
	gNoneArrayAllocList.push_back(p);
}

void CheckNoneArrayFree(void* p)
{
	LynxEngine::CList<void*>::CIterator Found = LynxEngine::find(gNoneArrayAllocList.begin(), gNoneArrayAllocList.end(), p);
	if (Found == gNoneArrayAllocList.end())
	{
		int xxxx = 0;
	}
}
#endif


#if defined(__LYNX_DEBUG_MEM_ALLOCATOR__) || defined(__LYNX_SMALLOBJ_MEM_ALLOCATOR__)
	namespace LynxEngine 
	{	
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void* CUnknown::operator new(size_t s)
		{
			void* Ptr = LYNXMEMALLOCATOR::vAlloc(s);
			//AddNoneArrayAlloc(Ptr);
			return (Ptr);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void* CUnknown::operator new[](size_t s) 
		{
			void* Ptr = LYNXMEMALLOCATOR::vAlloc(s);
			//AddArrayAlloc(Ptr);
			return (Ptr);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void* CUnknown::operator new(size_t s, const char *file, const char *func, int line)
		{ 
			void* Ptr = LYNXMEMALLOCATOR::vAlloc(s, file, func, line);
			//AddNoneArrayAlloc(Ptr);
			return (Ptr);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void* CUnknown::operator new[](size_t s, const char *file, const char *func, int line) 
		{
			void* Ptr = LYNXMEMALLOCATOR::vAlloc(s, file, func, line);
			//AddArrayAlloc(Ptr);
			return (Ptr);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CUnknown::operator delete(void* p, size_t s) 
		{ 
			//CheckNoneArrayFree(p);
			return LYNXMEMALLOCATOR::vFree(p, s);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CUnknown::operator delete[](void* p, size_t s) 
		{
			//CheckArrayFree(p);
			return LYNXMEMALLOCATOR::vFree(p, s);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CUnknown::operator delete(void* p, const char *file, const char *func, int line) 
		{
			//CheckNoneArrayFree(p);
			return LYNXMEMALLOCATOR::vFree(p, file, func, line);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CUnknown::operator delete[](void* p, const char *file, const char *func, int line) 
		{
			//CheckArrayFree(p);
			return LYNXMEMALLOCATOR::vFree(p, file, func, line);
		}		
	}

#if 0
	/*
	void* operator new(size_t s)
	{ 
		//void* Ptr = (LynxEngine::GameFramework::CKernel::GetMemAllocator()) ? LynxEngine::GameFramework::CKernel::GetMemAllocator()->vAlloc(s) : lynxMemAlloc(s);
		void* Ptr = lynxMemAlloc(s);
		//AddNoneArrayGlobalAlloc(Ptr);
		return (Ptr);
	}
	void* operator new[](size_t s) 
	{
		//void* Ptr = (LynxEngine::GameFramework::CKernel::GetMemAllocator()) ? LynxEngine::GameFramework::CKernel::GetMemAllocator()->vAlloc(s) : lynxMemAlloc(s);
		void* Ptr = lynxMemAlloc(s);
		AddArrayGlobalAlloc(Ptr);
		return (Ptr);
	}
	*/
	void* operator new(size_t s, const char *file, const char *func, int line)
	{ 
		void* Ptr = LYNXMEMALLOCATOR::vAlloc(s, file, func, line);
		//AddNoneArrayGlobalAlloc(Ptr);
		return (Ptr);
	}
	void* operator new[](size_t s, const char *file, const char *func, int line) 
	{
		void* Ptr = LYNXMEMALLOCATOR::vAlloc(s, file, func, line);
		//AddArrayGlobalAlloc(Ptr);
		return (Ptr);
	}

	void operator delete(void* p) 
	{ 
		//CheckNoneArrayGlobalFree(p);
		return LYNXMEMALLOCATOR::vFree(p);
	}
	void operator delete[](void* p) 
	{
		//CheckArrayGlobalFree(p);
		return LYNXMEMALLOCATOR::vFree(p);
	}
	void operator delete(void* p, size_t s) { return LYNXMEMALLOCATOR::vFree(p, s); };
	void operator delete[](void* p, size_t s) {return LYNXMEMALLOCATOR::vFree(p, s); };
	void operator delete(void* p, const char *file, const char *func, int line) { return LYNXMEMALLOCATOR::vFree(p, file, func, line); };
	void operator delete[](void* p, const char *file, const char *func, int line) {return LYNXMEMALLOCATOR::vFree(p, file, func, line); };
#else	
	/*
	//void*					operator new(size_t s){ return (LynxEngine::GameFramework::CKernel::GetMemAllocator()) ? LynxEngine::GameFramework::CKernel::GetMemAllocator()->vAlloc(s) : lynxMemAlloc(s); };
	//void*					operator new[](size_t s) {return (LynxEngine::GameFramework::CKernel::GetMemAllocator()) ? LynxEngine::GameFramework::CKernel::GetMemAllocator()->vAlloc(s) : lynxMemAlloc(s); };
	void*					operator new(size_t s, const char *file, const char *func, int line){ return lynxMemAlloc(s); };
	void*					operator new[](size_t s, const char *file, const char *func, int line) {return lynxMemAlloc(s); };

	void					operator delete(void* p) { return lynxMemFree(p); };
	void					operator delete[](void* p) {return lynxMemFree(p); };
	void					operator delete(void* p, size_t s) { return lynxMemFree(p); };
	void					operator delete[](void* p, size_t s) {return lynxMemFree(p); };
	void					operator delete(void* p, const char *file, const char *func, int line) { return lynxMemFree(p); };
	void					operator delete[](void* p, const char *file, const char *func, int line) {return lynxMemFree(p); };	
	*/
#endif

#endif