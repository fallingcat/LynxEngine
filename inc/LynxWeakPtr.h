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

#ifndef __LYNXWEAKPTR_H__
#define __LYNXWEAKPTR_H__

#include <LynxType.h>
#include <LynxMem.h>
#include <LynxCountedPtr.h>

namespace LynxEngine 
{
	//class CWeakPtrFlag
	//{
	//private:
	//	struct Data
	//	{
	//		LYNXBOOL		bValid;
	//		DWORD			RefCount;
	//		//-------------------------------------------------------------------------------------------------------
	//		//
	//		//-------------------------------------------------------------------------------------------------------		
	//		LYNXINLINE	Data(LYNXBOOL valid)
	//		: bValid(valid)
	//		, RefCount(1)
	//		{
	//		}
	//	};

	//	Data*			m_pData;
	//public:
	//	//-------------------------------------------------------------------------------------------------------
	//	//
	//	//-------------------------------------------------------------------------------------------------------		
	//	CWeakPtrFlag(LYNXBOOL valid = LYNX_TRUE)	
	//	: m_pData(LYNXNEW Data(valid))
	//	{
	//	}
	//	//-------------------------------------------------------------------------------------------------------
	//	//
	//	//-------------------------------------------------------------------------------------------------------		
	//	CWeakPtrFlag(CWeakPtrFlag const &rhs)
	//	: m_pData(rhs.m_pData)
	//	{
	//		IncRef();
	//	}
	//	//-------------------------------------------------------------------------------------------------------
	//	//
	//	//-------------------------------------------------------------------------------------------------------		
	//	~CWeakPtrFlag ()
	//	{
	//		DecRef();
	//	}
	//	//-------------------------------------------------------------------------------------------------------
	//	//
	//	//-------------------------------------------------------------------------------------------------------		
	//	LYNXINLINE CWeakPtrFlag& operator= (CWeakPtrFlag const &rhs)
	//	{
	//		if (m_pData != rhs.m_pData)
	//		{
	//			DecRef();
	//			m_pData = rhs.m_pData;
	//			IncRef();
	//		}
	//		return *this;			
	//	}
	//	//-------------------------------------------------------------------------------------------------------
	//	//
	//	//-------------------------------------------------------------------------------------------------------		
	//	LYNXINLINE LYNXBOOL IsValid() const
	//	{
	//		return m_pData->bValid;
	//	}
	//	//-------------------------------------------------------------------------------------------------------
	//	//
	//	//-------------------------------------------------------------------------------------------------------		
	//	LYNXINLINE void InValidate()
	//	{
	//		m_pData->bValid = LYNX_FALSE;
	//	}

	//private:
	//	//-------------------------------------------------------------------------------------------------------
	//	//
	//	//-------------------------------------------------------------------------------------------------------		
	//	LYNXINLINE void IncRef()
	//	{
	//		++(m_pData->RefCount);
	//	}
	//	//-------------------------------------------------------------------------------------------------------
	//	//
	//	//-------------------------------------------------------------------------------------------------------		
	//	LYNXINLINE void DecRef()
	//	{
	//		--(m_pData->RefCount);
	//		if (m_pData->RefCount == 0)
	//		{
	//			LYNXDEL m_pData;
	//		}
	//	}	  
	//};



	
	/* class for counted reference semantics
	* - deletes the object to which it refers when the last CountedPtr
	*   that refers to it is destroyed
	*/
	template <class T> class CWeakPtr
	{
	private:
		T*						m_TargetPtr;
		CCountedPtr<LYNXBOOL>	m_pValidityFlag;
	public:
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------		
		T* GetTargetPtr() { return m_TargetPtr; };		
	public:
		// initialize pointer with existing pointer
		// - requires that the pointer p is a return value of new
		explicit CWeakPtr(T* p = NULL) 
		: m_TargetPtr(p)		
		{
			m_pValidityFlag.Reset(LYNXNEW LYNXBOOL(LYNX_TRUE));
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		// copy pointer (one more owner)
		//-------------------------------------------------------------------------------------------------------			
		CWeakPtr(const CWeakPtr<T>& p) 
		: m_TargetPtr(p.m_TargetPtr)
		{			
			m_pValidityFlag = p.m_pValidityFlag;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// destructor (delete value if this was the last owner)
		//-------------------------------------------------------------------------------------------------------		
		~CWeakPtr() {*m_pValidityFlag = LYNX_FALSE;};	
		//-------------------------------------------------------------------------------------------------------
		//
		// assignment (unshare old and share new value)
		//-------------------------------------------------------------------------------------------------------			
		void Reset(T* p) 
		{
			m_TargetPtr = p;
			m_pValidityFlag.Reset(LYNXNEW LYNXBOOL(LYNX_TRUE));			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// assignment (unshare old and share new value)
		//-------------------------------------------------------------------------------------------------------			
		CWeakPtr<T>& operator= (const CWeakPtr<T>& rhs) 
		{
			m_TargetPtr = rhs.m_TargetPtr;
			m_pValidityFlag = rhs.m_pValidityFlag;

			return *this;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------			
		bool operator ==(const CWeakPtr<T>& p) 
		{
			return (m_TargetPtr == p.m_TargetPtr); 
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------			
		LYNXBOOL IsValid()
		{
			return (*m_pValidityFlag);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// access the value to which the pointer refers
		//-------------------------------------------------------------------------------------------------------			
		T& operator*() const { return *m_TargetPtr; };
		T* operator->() const { return m_TargetPtr; };
		operator T*() {return m_TargetPtr; };		
    };	

	
	/* class for counted reference semantics
	* - deletes the object to which it refers when the last CountedPtr
	*   that refers to it is destroyed
	*/
	template <class T> class CWeakCountedPtr
	{
	private:
		T*						m_Ptr;     // pointer to the value
		DWORD*					m_Count;   // shared number of owners	
		CCountedPtr<LYNXBOOL>	m_pValidityFlag;
	public:
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------		
		T* GetPtr() { return m_Ptr; };		
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------		
		DWORD* GetlpCount() { return m_Count; };
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------			
		void Dispose() 
		{
			LYNX_ASSERT(m_Count);

			if (--(*m_Count) == 0) 
			{
				*m_pValidityFlag = LYNX_FALSE;
				LYNXDEL(m_Count);
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
		explicit CWeakCountedPtr(T* p = NULL) 
		: m_Ptr(p)
		, m_Count(LYNXNEW DWORD(1))
		, m_pValidityFlag(LYNXNEW LYNXBOOL(LYNX_TRUE))
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// copy pointer (one more owner)
		//-------------------------------------------------------------------------------------------------------			
		CWeakCountedPtr(const CWeakCountedPtr<T>& p) 
		: m_Ptr(p.m_Ptr)
		, m_Count(p.m_Count) 
		, m_pValidityFlag(LYNXNEW LYNXBOOL(LYNX_TRUE))
		{ 
			++(*m_Count); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// destructor (delete value if this was the last owner)
		//-------------------------------------------------------------------------------------------------------		
		~CWeakCountedPtr() 
		{ 
			Dispose(); 
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		// assignment (unshare old and share new value)
		//-------------------------------------------------------------------------------------------------------			
		void Reset(T* p = NULL) 
		{
			if (m_Ptr != p) 
			{
				Dispose();
				m_Ptr = p;
				m_Count = LYNXNEW DWORD(1);
				m_pValidityFlag.Reset(LYNXNEW LYNXBOOL(LYNX_TRUE));
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// assignment (unshare old and share new value)
		//-------------------------------------------------------------------------------------------------------			
		CWeakCountedPtr<T>& operator= (const CWeakCountedPtr<T>& p) 
		{
			if (this != &p) 
			{
				Dispose();
				m_Ptr = p.m_Ptr;
				m_Count = p.m_Count;
				++(*m_Count);
				*m_pValidityFlag = LYNX_TRUE;
			}
			return *this;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// assignment (unshare old and share new value)
		//-------------------------------------------------------------------------------------------------------			
		template <class RT>
		CWeakCountedPtr<T>& operator= (CWeakCountedPtr<RT>& p) 
		{
			if ((void*)this != (void*)&p) 
			{
				Dispose();
				m_Ptr = static_cast<T*>(p.GetPtr());
				m_Count = p.GetlpCount();
				++(*m_Count);
			}
			return *this;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------			
		bool operator ==(const CWeakCountedPtr<T>& p) 
		{
			return (m_Ptr == p.m_Ptr); 
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------			
		DWORD GetCount() 
		{ 
			return (m_Count ? (*m_Count) : 0); 
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//-------------------------------------------------------------------------------------------------------			
		CCountedPtr<LYNXBOOL> GetValidityFlag() 
		{
			return m_pValidityFlag;
		}
		//-------------------------------------------------------------------------------------------------------
		//a
		// access the value to which the pointer refers
		//-------------------------------------------------------------------------------------------------------			
		T& operator*() const { return *m_Ptr; };
		T* operator->() const { return m_Ptr; };		
		operator T*() {return m_Ptr; };		
    };	
}

#endif