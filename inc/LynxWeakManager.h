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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//--------------------------------------------------------------------------
//  
//
//###########################################################################

#ifndef __LYNXMANAGER_H__
#define __LYNXMANAGER_H__

#include <LynxObj.h>
#include <LynxEngineClasses.h>
#include <LynxFactory.h>

namespace LynxEngine 
{
	template <class AbstractProduct,
		typename IdentifierType,	
		class Param = CEngine*		
	>
	class LYNXENGTEMPLATE CManager
	{	
	public:
		typedef CWeakCountedPtr<AbstractProduct>			AbstractProductPtr;
		typedef CWeakPtr<AbstractProductPtr>				AbstractProductWeakPtr;
		AbstractProductPtr									m_NULL;		
	protected:						
		CEngine*											m_lpEngine;		
		CList< AbstractProductWeakPtr >						m_ObjectList;			
		//CList< AbstractProduct* >							m_ObjectList;			
		CFactory<AbstractProduct, IdentifierType, Param>	m_Factory;
	public:			
		CManager(void) {m_lpEngine = NULL; };
		CManager(CEngine* lpe) {m_lpEngine = lpe; };
		virtual ~CManager() 
		{
			//for (CList<AbstractProductPtr>::CIterator i=m_ObjectList.begin(); i!=m_ObjectList.end(); ++i)
			//	LYNXDEL (*i);
			m_ObjectList.clear(); 
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void SetlpEngine(CEngine* e) { m_lpEngine = e; m_Factory.SetParam(e); };
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void Register(IdentifierType t, AbstractProduct* (*func)(Param)) { m_Factory.Unregister(t); m_Factory.Register(t, func); };
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		DWORD GetNumRegistries() { return m_Factory.GetNumRegistries(); };
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		IdentifierType GetRegistryID(const unsigned int i) { return m_Factory.GetRegistryID(i); };
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CreateObject(AbstractProductPtr& p, IdentifierType t)		
		{
			AbstractProduct* lpR = m_Factory.CreateObject(t);	
			p.Reset(lpR);
			Add(&p);		

			return;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void DestroyObject(AbstractProductWeakPtr& o)	
		{ 
			m_ObjectList.remove(o); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void DestroyObject(AbstractProductPtr& o)	
		{ 
			//m_ObjectList.remove(o); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void DestroyObject(const CString& name)	
		{ 
			/*CList< AbstractProductWeakPtr >::CIterator Res = LynxEngine::find(m_ObjectList.begin(), m_ObjectList.end(), name);
			if (Res != m_ObjectList.end())
				m_ObjectList.remove(*Res); 	*/		
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void Add(AbstractProductPtr* p) 
		{ 
			m_ObjectList.push_back(AbstractProductWeakPtr(p));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		CList< AbstractProductWeakPtr >& GetObjectList() 
		{ 
			return m_ObjectList; 
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void RemoveInvalidPtr() 
		{ 
			CList<AbstractProductWeakPtr>::CIterator Pos;
			Pos = m_ObjectList.begin();
			while (Pos != m_ObjectList.end())
			{	
				LYNXBOOL bRemove = LYNX_FALSE;
				if (!(*Pos).IsValid())
				{
					CList<AbstractProductWeakPtr>::CIterator Cur = Pos;		
					++Pos;		
					m_ObjectList.remove(*Cur);
				}
				else
				{
					++Pos;		
				}
			}	
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		AbstractProductPtr& Find(const CString& name)		
		{
			RemoveInvalidPtr();

			CList< AbstractProductWeakPtr >::CIterator Res = LynxEngine::find(m_ObjectList.begin(), m_ObjectList.end(), name);
			if (Res != m_ObjectList.end())
				return *((*Res).GetTargetPtr());

			return m_NULL;	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		AbstractProductPtr& Find(const LYNXCHAR *name) 
		{
			return Find(CString(name));
		}
	};	
}

#endif