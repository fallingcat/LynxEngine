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

#ifndef __LYNXNXPHYSICSWORLD_H__
#define __LYNXNXPHYSICSWORLD_H__

#ifdef __PHYSX__

#include <LynxEngineClasses.h>
#include <NxPhysics.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		class LYNXENGCLASS CPhysicsWorld : public LynxEngine::PhysicsSystem::CPhysicsWorld
		{	
			LYNX_DECLARE_CLASS(CPhysicsWorld);
		private:
			//#define MAX_CONTACTS				80
		protected:		
			NxScene*							m_World;
			//dJointGroupID						m_ContactGroup;				
		public:			
			CPhysicsWorld(LynxEngine::PhysicsSystem::CPhysicsSystem* lpsys);
			virtual ~CPhysicsWorld();

			LYNXINLINE NxScene*					GetlpNxScene() {return m_World; };
			LYNXINLINE NxScene*					GetNxScene() {return m_World; };
			virtual LYNXBOOL					vCreate();			
			virtual void						vCollisionLoop();
			virtual void						vStep(float step);		

			virtual void 						vSetGravity(LYNXREAL x, LYNXREAL y, LYNXREAL z) {};
			virtual void 						vSetGravity(LPLYNXVECTOR3D lpg) {};

			virtual LynxEngine::PhysicsSystem::CPhysicsSpace* const	vCreateSpace(LynxEngine::PhysicsSystem::CPhysicsSpace* const parent);
			virtual void											vDeleteSpace(LynxEngine::PhysicsSystem::CPhysicsSpace* const lps);					

			virtual LynxEngine::PhysicsSystem::CPhysicsObj* const	vCreateObj(LynxEngine::PhysicsSystem::CPhysicsObj::TYPE t, LynxEngine::PhysicsSystem::CPhysicsShape* g = NULL);
			virtual void											vDeleteObj(LynxEngine::PhysicsSystem::CPhysicsObj* const lpo);

			virtual LynxEngine::PhysicsSystem::CPhysicsJoint* const	vCreateJoint(LynxEngine::PhysicsSystem::CPhysicsJoint::TYPE type, const LynxEngine::CCreationDesc* desc);
			virtual void											vDeleteJoint(LynxEngine::PhysicsSystem::CPhysicsJoint* const lpj);	

		};	
	}
}

#endif

#endif