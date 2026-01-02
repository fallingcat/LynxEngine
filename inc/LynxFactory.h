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
//  Created by Owen Wu : 2008/03/10
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXFACTORY_H__
#define __LYNXFACTORY_H__

#include <LynxType.h>
#include <LynxMem.h>
#include <LynxTMap.h>
#include <LynxEngineClasses.h>

namespace LynxEngine 
{	
	template <
		class AbstractProduct,
		typename IdentifierType,		
		class Param = CEngine*		
	>
	class CFactory {
	public:
		typedef AbstractProduct* (*ProductCreator)(Param);
	protected:
		typedef CMap<IdentifierType, ProductCreator> ASSOCMAP;
		ASSOCMAP		m_Associations;
		Param			m_Param;	
	public:
		CFactory(void) {};		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXINLINE void				SetParam(Param p) { m_Param = p; };
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL		Register(const IdentifierType& id, AbstractProduct* (*creator)(Param))
		{
			return m_Associations.insert(typename ASSOCMAP::value_type(id, creator)).second;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL		Unregister(const IdentifierType& id)
		{
			return m_Associations.erase(id) == 1;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXINLINE DWORD			GetNumRegistries() { return (DWORD)m_Associations.size(); };
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXINLINE IdentifierType	GetRegistryID(const unsigned int i) 
		{ 
			typename ASSOCMAP::CIterator Pos = m_Associations.begin();
			advance(Pos, i);
			return (Pos->first); 
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		AbstractProduct* CreateObject(const IdentifierType& id)
		{
			//typename ASSOCMAP::const_iterator i = m_Associations.find(id);
			//typename ASSOCMAP::CIterator i = m_Associations.find(id);
			typename ASSOCMAP::CIterator i = m_Associations.find(id);
			if (i != m_Associations.end())
			{
				return (i->second)(m_Param);
			}
			return NULL;
		}
	};
}

#endif