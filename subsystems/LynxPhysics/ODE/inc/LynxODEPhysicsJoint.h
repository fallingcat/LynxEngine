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

#ifndef __LYNXODEPHYSICSJOINT_H__
#define __LYNXODEPHYSICSJOINT_H__

#include <LynxCore.h>
#include <ODE/ODE.h>
#include <PhysicsSystem/LynxPhysicsJoint.h>
#include <PhysicsSystem/LynxPhysicsObj.h>

namespace LynxPhysics 
{
	namespace ODE 
	{
		class CPhysicsJoint : public LynxEngine::PhysicsSystem::CPhysicsJoint
		{	
			DECLARE_CLASS(CPhysicsJoint);
		protected:			
			dJointID									m_Joint;
		public:			
			CPhysicsJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw);
			virtual ~CPhysicsJoint();

			LYNXINLINE void						vDestory() { LYNX_ASSERT(m_Joint > 0); dJointDestroy (m_Joint); m_lpWorld = NULL; };
			void								vAttach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1 = NULL, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2 = NULL);			 			
			void								vAttachObj1(LynxEngine::PhysicsSystem::CPhysicsObj* lpo);
			void								vAttachObj2(LynxEngine::PhysicsSystem::CPhysicsObj* lpo);
		};	
	}
}

#endif