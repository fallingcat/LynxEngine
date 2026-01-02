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

#ifndef __LYNXB2PHYSICSWORLD_H__
#define __LYNXB2PHYSICSWORLD_H__

#include <LynxEngineClasses.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>
#include <Box2D/Box2D.h>

namespace LynxPhysics 
{
	namespace Box2D 
	{
		class LYNXENGCLASS CPhysicsWorld : public LynxEngine::PhysicsSystem::CPhysicsWorld
		{	
			LYNX_DECLARE_CLASS(CPhysicsWorld);
		private:			
		protected:		
			b2World*							m_World;						
		public:			
			CPhysicsWorld(LynxEngine::PhysicsSystem::CPhysicsSystem* lpsys);
			virtual ~CPhysicsWorld();

			LYNXFORCEINLINE b2World*			Getb2World() {return m_World; };
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