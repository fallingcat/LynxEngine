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

#ifndef __LYNXBTPHYSICSJOINT_H__
#define __LYNXBTPHYSICSJOINT_H__

#include <LynxEngineClasses.h>
#include <btBulletDynamicsCommon.h>
#include <PhysicsSystem/LynxPhysicsJoint.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		class CJoint : public LynxEngine::PhysicsSystem::CPhysicsJoint
		{	
			LYNX_DECLARE_CLASS(CJoint);
		protected:			
			btTypedConstraint*						m_lpConstraint;
		public:			
			CJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw);
			virtual ~CJoint();		
			
			virtual void							vDestory();
		};	
	}
}

#endif