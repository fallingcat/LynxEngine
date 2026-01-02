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

#ifndef __LYNXNXPHYSICSJOINT_H__
#define __LYNXNXPHYSICSJOINT_H__

#ifdef __PHYSX__

#include <LynxEngineClasses.h>
#include <NxPhysics.h>
#include <PhysicsSystem/LynxPhysicsJoint.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class CJoint : public LynxEngine::PhysicsSystem::CPhysicsJoint
		{	
			LYNX_DECLARE_CLASS(CJoint);
		protected:			
			NxJoint*								m_lpJoint;
		public:			
			CJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw);
			virtual ~CJoint();		
			
			virtual void							vDestory();
		};	
	}
}

#endif

#endif