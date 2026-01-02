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

#ifndef __LYNXCOUNTEDPTR_H__
#define __LYNXCOUNTEDPTR_H__

#include <LynxType.h>
#include <LynxMem.h>
#include <LynxUnknown.h>

namespace LynxEngine 
{	
#if 0
	LYNXENGCLASS class CPtrCounter : public CUnknown
	{
	public :
		LYNXDWORD					m_Value;

	public:
		CPtrCounter() { m_Value = 0; };
		CPtrCounter(const int v) { m_Value = v; };
		CPtrCounter(const LYNXDWORD x) { m_Value = x; };

		operator			const LYNXDWORD() const { return m_Value; }		
		operator			const LYNXDWORD&() const { return m_Value; }
		
		CPtrCounter operator ++() { m_Value++; return (*this);  }
		CPtrCounter operator --() { m_Value--; return (*this); }
		bool operator==(const CPtrCounter &rhs) { return (m_Value == rhs.m_Value); }
	};
#else
	#define CPtrCounter				LYNXDWORD
#endif

	/* class for counted reference semantics
	* - deletes the object to which it refers when the last CountedPtr
	*   that refers to it is destroyed
	*/
	template <class T> class CCountedPtr
	{
	private:		
		T*					m_Ptr;     // pointer to the value
		CPtrCounter*		m_Count;   // shared number of owners				
	public:
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------		
		T* GetPtr() { return m_Ptr; };		
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------		
		CPtrCounter* GetlpCount() { return m_Count; };
	private:
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------			
		void Dispose() 
		{
			LYNX_ASSERT(m_Count);
			
			--(*m_Count);
			if ((*m_Count) == 0) 
			{
				LYNXGLOBAL_DEL m_Count;
				m_Count = NULL;
				if (m_Ptr)
				{
					LYNXDEL m_Ptr;
					m_Ptr = NULL;
				}					
			}						
        }
	public:
		// initialize pointer with existing pointer
		// - requires that the pointer p is a return value of new
		explicit CCountedPtr(T* p = NULL) 
		: m_Ptr(p) 
		{
			m_Count = LYNXGLOBAL_NEW CPtrCounter(1);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// copy pointer (one more owner)
		//-------------------------------------------------------------------------------------------------------			
		CCountedPtr(const CCountedPtr<T>& p) 
		: m_Ptr(p.m_Ptr)
		, m_Count(p.m_Count) 
		{ 
			++(*m_Count); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// destructor (delete value if this was the last owner)
		//-------------------------------------------------------------------------------------------------------		
		~CCountedPtr() 
		{ 
			Dispose(); 
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		// assignment (unshare old and share new value)
		//-------------------------------------------------------------------------------------------------------			
		void Reset() 
		{
			if (m_Ptr) 
			{
				Dispose();
				m_Ptr = NULL;
				//m_Count = LYNXNEW CPtrCounter(1);
				m_Count = LYNXGLOBAL_NEW CPtrCounter(1);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// assignment (unshare old and share new value)
		//-------------------------------------------------------------------------------------------------------			
		CCountedPtr<T>& operator= (const CCountedPtr<T>& p) 
		{
			if (this != &p) 
			{
				Dispose();
				m_Ptr = p.m_Ptr;
				m_Count = p.m_Count;
				++(*m_Count);
			}
			return *this;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// assignment (unshare old and share new value)
		//-------------------------------------------------------------------------------------------------------			
		template <class RT>
		CCountedPtr<T>& operator= (CCountedPtr<RT>& p) 
		{
			if ((void*)this != (void*)&p) 
			{
				Dispose();
				//m_Ptr = static_cast<T*>(p.GetPtr());
				m_Ptr = dynamic_cast<T*>(p.GetPtr());
				m_Count = p.GetlpCount();
				++(*m_Count);		
			}
			return *this;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------			
		bool operator ==(const CCountedPtr<T>& p) const
		{
			return (m_Ptr == p.m_Ptr); 
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------			
		LYNXDWORD GetCount()
		{ 
			return (m_Count ? (LYNXDWORD)(*m_Count) : 0); 
			//return (m_Count ? (LYNXDWORD)(m_Count) : 0);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		// access the value to which the pointer refers
		//-------------------------------------------------------------------------------------------------------			
		T& operator*() const { return *m_Ptr; };
		T* operator->() const { return m_Ptr; };		
		operator T*() {return m_Ptr; };		
    };	
}

#endif