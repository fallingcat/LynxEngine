//###########################################################################
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#ifndef __LYNXB2PHYSPACE_H__
#define __LYNXB2PHYSPACE_H__

#include <Box2D/Box2D.h>
#include <LynxEngineClasses.h>
#include <PhysicsSystem/LynxPhysicsSpace.h>

namespace LynxPhysics 
{
	namespace Box2D 
	{
		class LYNXENGCLASS CPhysicsSpace : public LynxEngine::PhysicsSystem::CPhysicsSpace
		{	
			LYNX_DECLARE_CLASS(CPhysicsSpace);
		protected:			
		public:			
			CPhysicsSpace(LynxEngine::PhysicsSystem::CPhysicsWorld *lpw);
			virtual ~CPhysicsSpace();			

			//LYNXINLINE dSpaceID										GetSpace() const {return m_Space; };
			virtual	LYNXBOOL											vCreate(LynxEngine::PhysicsSystem::CPhysicsSpace* const parent);
			virtual void												vDestory();

			// Geometry ----------------------------------------------------------------------------------------------------------------
			virtual	LynxEngine::PhysicsSystem::CPhysicsShape* const		vCreateShape(LynxEngine::PhysicsSystem::CPhysicsShape::TYPE type, const LynxEngine::CCreationDesc *desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);
			virtual	void												vDeleteShape(LynxEngine::PhysicsSystem::CPhysicsShape* const lpg);
		};	
	}
}

#endif