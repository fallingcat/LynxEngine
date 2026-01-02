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

#ifndef __LYNXODEPHYSPACE_H__
#define __LYNXODEPHYSPACE_H__

#include <LynxCore.h>
#include <ODE/ODE.h>
#include <PhysicsSystem/LynxPhysicsSpace.h>

namespace LynxPhysics 
{
	namespace ODE 
	{
		class CPhysicsSpace : public LynxEngine::PhysicsSystem::CPhysicsSpace
		{	
		protected:
			dSpaceID								m_Space;
		public:
			DECLARE_CLASS(CPhysicsSpace);
			CPhysicsSpace(LynxEngine::PhysicsSystem::CPhysicsWorld* const lpw);
			virtual ~CPhysicsSpace();

			LYNXINLINE dSpaceID						GetSpace() const {return m_Space; };
			virtual	LYNXBOOL						vCreate(LynxEngine::PhysicsSystem::CPhysicsSpace* const parent);
			virtual void							vDestory();

			// Geometry ----------------------------------------------------------------------------------------------------------------
			virtual	LynxEngine::PhysicsSystem::CPhysicsGeometry* const	vCreateGeometry(LynxEngine::PhysicsSystem::CPhysicsGeometry::TYPE type, LYNXREAL *lpdata);
			virtual	void												vDeleteGeometry(LynxEngine::PhysicsSystem::CPhysicsGeometry* const lpg);
		};	
	}
}

#endif