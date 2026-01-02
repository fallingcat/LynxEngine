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

#ifndef __LYNXBASEMANAGER_H__
#define __LYNXBASEMANAGER_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	template < class T >
	class LYNXENGTEMPLATE CBaseManager
	{	
	public:						
		class LYNXENGTEMPLATE CRecord
		{			
		protected:
			CList<T*>		m_List;
		public:
			CRecord() {};
			virtual ~CRecord() {m_List.clear(); };

			bool operator == (CRecord& r) { return (GetObj() == r.GetObj()); };
			bool operator == (const CString name) { return (GetObj() == name); };
			bool operator == (T& o) { return (GetObj() == o); };

			void Add(T& o)
			{
				m_List.push_back(&o);
			}

			void Remove(T& o)
			{
				m_List.remove(&o);
			}

			T& GetObj() {return *(*(m_List.begin())); };
		};
	protected:								
	public:			
		CBaseManager(void) {};
		virtual ~CBaseManager() {};
		
		virtual void vIncRef(T& p) = 0;
		virtual void vDecRef(T& p) = 0;
	};	
}

#endif