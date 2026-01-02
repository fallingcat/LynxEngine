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

#ifndef __LYNXNXCPNEPHYSICSJOINT_H__
#define __LYNXNXCONEPHYSICSJOINT_H__

#ifdef __PHYSX__

#include <LynxEngineClasses.h>
#include <LynxNxPhysicsJoint.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class CConeJoint : public CJoint
		{	
			LYNX_DECLARE_CLASS(CConeJoint);
		protected:			
			LynxEngine::PhysicsSystem::CConeJointDesc m_Desc;	
		public:			
			CConeJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw);
			virtual ~CConeJoint();

			virtual LYNXBOOL				vCreate(const LynxEngine::CCreationDesc* desc);	
			virtual void					vAttach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2 = NULL);
			virtual void					vSetAnchorPosition(LynxEngine::Math::CVector3& pos);
		protected:
			void							Attach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2, LynxEngine::Math::CVector3& p1, LynxEngine::Math::CVector3& p2);
		};	
	}
}

#endif

#endif