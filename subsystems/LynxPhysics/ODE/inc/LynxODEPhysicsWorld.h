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

#ifndef __LYNXODEPHYSICSWORLD_H__
#define __LYNXODEPHYSICSWORLD_H__

#include <LynxCore.h>
#include <ODE/ODE.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>

namespace LynxPhysics 
{
	namespace ODE 
	{
		class CPhysicsSystem;
		class CPhysicsWorld : public LynxEngine::PhysicsSystem::CPhysicsWorld
		{	
			DECLARE_CLASS(CPhysicsWorld);
		private:
			#define MAX_CONTACTS				80
		protected:		
			dWorldID							m_World;
			dJointGroupID						m_ContactGroup;				
		public:			
			CPhysicsWorld(LynxEngine::PhysicsSystem::CPhysicsSystem* const lpsys);
			virtual ~CPhysicsWorld();

			static void							sNearCallback(void *data, dGeomID o1, dGeomID o2);
			LYNXINLINE dWorldID					GetWorld() {return m_World; };
			LYNXINLINE dJointGroupID			GetContactGroup() {return m_ContactGroup; };
			virtual LYNXBOOL					vCreate();
			virtual void						vCollisionLoop();
			virtual void						vStep(float step);		
			virtual void 						vSetGravity(LYNXREAL x, LYNXREAL y, LYNXREAL z) { m_Gravity.s.x = x; m_Gravity.s.y = y; m_Gravity.s.z = z; dWorldSetGravity(m_World, m_Gravity.s.x, m_Gravity.s.y, m_Gravity.s.z); };
			virtual void 						vSetGravity(LPLYNXVECTOR3D lpg) { m_Gravity.s.x = lpg->s.x; m_Gravity.s.y = lpg->s.y; m_Gravity.s.z = lpg->s.z; dWorldSetGravity(m_World, m_Gravity.s.x, m_Gravity.s.y, m_Gravity.s.z); };


			virtual LynxEngine::PhysicsSystem::CPhysicsSpace* const	vCreateSpace(LynxEngine::PhysicsSystem::CPhysicsSpace* const parent);
			virtual void											vDeleteSpace(LynxEngine::PhysicsSystem::CPhysicsSpace* const lps);					

			virtual LynxEngine::PhysicsSystem::CPhysicsObj*	const 	vCreateObj();
			virtual void											vDeleteObj(LynxEngine::PhysicsSystem::CPhysicsObj* const lpo);

			virtual LynxEngine::PhysicsSystem::CPhysicsJoint* const vCreateJoint(LynxEngine::PhysicsSystem::CPhysicsJoint::TYPE type);
			virtual void											vDeleteJoint(LynxEngine::PhysicsSystem::CPhysicsJoint* const lpj);					
		};	
	}
}

#endif