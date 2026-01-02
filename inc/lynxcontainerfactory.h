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
//  Created by Owen Wu : 2006/06/20
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXCONTAINERFACTORY_H__
#define __LYNXCONTAINERFACTORY_H__

#include <LynxEngineClasses.h>
#include <LynxAlgorithm.h>
#include <LynxObj.h>
#include <LynxFactory.h>
#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CContainerFactory : public CFactory<CContainer, CContainer::TYPE, CEngine*>
	{	
		LYNX_DECLARE_CLASS(CContainerFactory);
	protected:		
		CList<CContainer*>				m_ContainerList;	
	public:			
		CContainerFactory() {};
		virtual ~CContainerFactory()
		{
			#ifdef __TOOL__
				m_ContainerList.clear();
			#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		CContainer* CreateObject(const CContainer::TYPE& id)
		{
			CContainer* lpC = CFactory<CContainer, CContainer::TYPE, CEngine*>::CreateObject(id);
			m_ContainerList.push_back(lpC);

			return lpC;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void DestroyObject(CContainer* const lpc)
		{
			m_ContainerList.remove(lpc);
			LYNXDEL lpc;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		CList<CContainer*>& GetContainerList()
		{
			return m_ContainerList;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		CContainer* Find(const CString& name)
		{
			CList<CContainer*>::CIterator Con = LynxEngine::find(m_ContainerList.begin(), m_ContainerList.end(), name);
			if (Con != m_ContainerList.end())
				return (*Con);

			return NULL;	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		CContainer* Find(const LYNXCHAR *name)
		{
			return Find(CString(name));
		}
	};		
}
#endif