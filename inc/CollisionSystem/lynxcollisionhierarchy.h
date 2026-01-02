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

#ifndef __LYNXCOLLISIONHIERARCHY_H__
#define __LYNXCOLLISIONHIERARCHY_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	namespace CollisionSystem 
	{
		class LYNXENGCLASS CCollisionHierarchy : public CUnknown
		{	
			LYNX_DECLARE_CLASS(CCollisionHierarchy);
		public:			
		protected:			
			CArray< CList<CCollisionObj*> >		m_CollisionHieList;
		public:			
			CCollisionHierarchy();
			virtual ~CCollisionHierarchy();	

			CCollisionHierarchy&				operator=(CCollisionHierarchy& rhs);						
			void								Create(int num);
			LYNXINLINE int						GetNumLevels() {return m_CollisionHieList.size(); };
			LYNXINLINE void						Add(int level, CCollisionObj* o) { m_CollisionHieList[level].push_back(o); };	
			LYNXINLINE void						Clear(int level) 
			{
				for (CList<CCollisionObj*>::CIterator C = m_CollisionHieList[level].begin(); C != m_CollisionHieList[level].end(); C++)
				{
					LYNXDEL (*C);
				}
				m_CollisionHieList[level].clear(); 
			}	
			LYNXINLINE CList<CCollisionObj*>&	GetCollisionObjList(int level) { return m_CollisionHieList[level]; };
		};
	}
}

#endif