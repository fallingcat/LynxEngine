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

#ifndef __LYNXBTBALLPHYSICSJOINT_H__
#define __LYNXBTBALLPHYSICSJOINT_H__

#include <LynxEngineClasses.h>
#include <btBulletDynamicsCommon.h>
#include <LynxBtPhysicsJoint.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		class CBallJoint : public CJoint
		{	
			LYNX_DECLARE_CLASS(CBallJoint);
		protected:			
			LynxEngine::PhysicsSystem::CBallJointDesc m_Desc;	
		public:			
			CBallJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw);
			virtual ~CBallJoint();

			virtual LYNXBOOL				vCreate(const LynxEngine::CCreationDesc* desc);	
			virtual void					vAttach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2 = NULL);
			virtual void					vSetAnchorPosition(LynxEngine::Math::CVector3& pos);
		protected:
			void							Attach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2, LynxEngine::Math::CVector3& p1, LynxEngine::Math::CVector3& p2);
		};	
	}
}

#endif