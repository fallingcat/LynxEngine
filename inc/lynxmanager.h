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
#include <LynxAlgorithm.h>

namespace LynxEngine 
{
	#ifdef __USE_WEAKPTR__
		template <class AbstractProduct,
				  typename IdentifierType,	
				  class Param = CEngine*>
		class LYNXENGTEMPLATE CManager
		{	
		public:
			typedef CWeakCountedPtr<AbstractProduct>			AbstractProductPtr;
			typedef CWeakPtr<AbstractProductPtr>				AbstractProductWeakPtr;
			AbstractProductPtr									m_NULL;	
			AbstractProductWeakPtr								m_NULL_OBJ;	
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
			template <class RT>
			void CreateObject(CWeakCountedPtr<RT>& r, IdentifierType t)
			{
				RemoveInvalidPtr();
				AbstractProduct* lpR = m_Factory.CreateObject(t);	
				r.Reset((RT*)lpR);
				Add(&r);		

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
			template <class RT>
			void Add(CWeakCountedPtr<RT>* p) 
			{ 
				AbstractProductWeakPtr WeakPtr;
				WeakPtr.Reset(p);
				m_ObjectList.push_back(WeakPtr);
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
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			AbstractProductWeakPtr& FindObj(const CString& name)		
			{
				RemoveInvalidPtr();

				CList< AbstractProductWeakPtr >::CIterator Res = LynxEngine::find(m_ObjectList.begin(), m_ObjectList.end(), name);
				if (Res != m_ObjectList.end())
					return (*Res);

				return m_NULL_OBJ;	
			}
		};
	#else
		template <class AbstractProduct,
				  typename IdentifierType,	
				  class Param = CEngine*>
		class CManager
		{	
		public:
			typedef CCountedPtr<AbstractProduct>				AbstractProductPtr;
			AbstractProductPtr									m_NULL;		
		protected:						
			CEngine*											m_lpEngine;		
			CList< AbstractProductPtr >							m_ObjectList;						
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
			void ResetObjList() { m_ObjectList.clear(); };			
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
			AbstractProductPtr& CreateObject(IdentifierType t)
			{
				AbstractProduct* lpR = m_Factory.CreateObject(t);						
				Add(AbstractProductPtr(lpR));

				return m_ObjectList.back();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			template <class RT>
			void CreateObject(RT& r, IdentifierType t, LYNXBOOL brecord = LYNX_TRUE)
			{
				AbstractProduct* lpProduct = m_Factory.CreateObject(t);	
				AbstractProductPtr Ptr(lpProduct);
				//LYNX_ASSERT(lpProduct != NULL);				
				if (brecord && lpProduct)
				{					
					Add(Ptr);
				}
				//r = m_ObjectList.back();				
				r = Ptr;
			}			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void DestroyObject(AbstractProductPtr& o)	{ m_ObjectList.remove(o); };		
			//void DestroyObject(AbstractProduct* o)	{ m_ObjectList.remove(o); };		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void DestroyObject(const CString& name)	
			{ 
				AbstractProductPtr& p = Find(name);
				if (p.GetPtr() != NULL)
					m_ObjectList.remove(p); 
			}		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void Add(AbstractProductPtr& o) 
			{ 
				m_ObjectList.push_back(o); 
				/*o.Dispose();*/ 
			}
			//void Add(AbstractProduct* o) { m_ObjectList.push_back(o); /*o.Dispose();*/ };
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			CList< AbstractProductPtr >& GetObjectList() { return m_ObjectList; };		
			//CList< AbstractProduct* >& GetObjectList() { return m_ObjectList; };						
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			AbstractProductPtr& Find(const CString& name)		
			{
				typename CList< AbstractProductPtr >::CIterator Res;

				Res = LynxEngine::find(m_ObjectList.begin(), m_ObjectList.end(), name);
				if (Res != m_ObjectList.end())
					return (*Res);

				return m_NULL;	
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			AbstractProductPtr& Find(const LYNXCRC& crc)		
			{
				typename CList< AbstractProductPtr >::CIterator Res;

				Res = LynxEngine::find(m_ObjectList.begin(), m_ObjectList.end(), crc);
				if (Res != m_ObjectList.end())
					return (*Res);

				return m_NULL;	
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			AbstractProductPtr& Find(const LYNXCHAR *name) {return Find(CString(name));} ;
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void RemoveInvalidPtr() 
			{ 
				typename CList<AbstractProductPtr>::CIterator Pos;
				LYNXBOOL bContinue = LYNX_FALSE;

				Pos = m_ObjectList.begin();
				while (Pos != m_ObjectList.end())
				{
					if (!(*Pos))
					{
						int xxx = 0;
					}
					++Pos;					
				}

				do 
				{
					bContinue = LYNX_FALSE;
					Pos = m_ObjectList.begin();
					while (Pos != m_ObjectList.end())
					{	
						if (!(*Pos) || (*Pos).GetCount() == 1)
						{
							typename CList<AbstractProductPtr>::CIterator Cur = Pos;		
							++Pos;		
							m_ObjectList.erase(Cur);
							bContinue = LYNX_TRUE;
							break;
						}
						else
						{
							++Pos;		
						}
					}
				}while (bContinue);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			AbstractProductPtr& FindObj(const CString& name)		
			{
				RemoveInvalidPtr();

				typename CList< AbstractProductPtr >::CIterator Res = LynxEngine::find(m_ObjectList.begin(), m_ObjectList.end(), name);
				if (Res != m_ObjectList.end())
					return (*Res);

				return m_NULL;	
			}
		};
	#endif
}
#endif
