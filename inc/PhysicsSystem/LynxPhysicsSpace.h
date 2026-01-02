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

#ifndef __LYNXPHYSICSSPACE_H__
#define __LYNXPHYSICSSPACE_H__

#include <LynxEngineClasses.h>
#include <LynxGameObj.h>
#include <PhysicsSystem/LynxPhysicsShape.h>

namespace LynxEngine 
{
	namespace PhysicsSystem 
	{
		class LYNXENGCLASS CPhysicsSpace : public CGameObj
		{	
			LYNX_DECLARE_CLASS(CPhysicsSpace);
		protected:
			CPhysicsWorld*						m_lpWorld;
			CList<CPhysicsShape*>				m_ShapeList;
		public:			
			CPhysicsSpace(CPhysicsWorld* const lpw);
			virtual ~CPhysicsSpace();

			virtual	LYNXBOOL					vCreate(CPhysicsSpace* const parent) = 0;
			virtual void						vDestory() = 0;

			LYNXINLINE CPhysicsWorld*			GetlpWorld() const {return m_lpWorld; };
			LYNXINLINE void						AddShape(CPhysicsShape* const lpo) {m_ShapeList.push_back(lpo); };
			LYNXINLINE void						DeleteShape(CPhysicsShape* const lpo) {m_ShapeList.remove(lpo); };
			LYNXINLINE DWORD					GetNumShapes() {return (DWORD)m_ShapeList.size(); };
			LYNXINLINE CPhysicsShape* const		GetlpShape(int i) {return m_ShapeList.get(i); };			
			virtual	CPhysicsShape* const		vCreateShape(CPhysicsShape::TYPE type, const CCreationDesc *desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL) = 0;
			virtual	void						vDeleteShape(CPhysicsShape* const lpg) = 0;
		};	
	}
}

#endif