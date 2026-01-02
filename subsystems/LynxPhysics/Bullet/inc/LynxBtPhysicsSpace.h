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

#ifndef __LYNXBTPHYSPACE_H__
#define __LYNXBTPHYSPACE_H__

#include <btBulletCollisionCommon.h>
#include <LynxEngineClasses.h>
#include <PhysicsSystem/LynxPhysicsSpace.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		class LYNXENGCLASS CPhysicsSpace : public LynxEngine::PhysicsSystem::CPhysicsSpace
		{	
			LYNX_DECLARE_CLASS(CPhysicsSpace);
		protected:
			btCollisionConfiguration*									m_CollisionConfiguration;
			btCollisionDispatcher*										m_Dispatcher;
			btBroadphaseInterface*										m_OverlappingPairCache;		
		public:			
			CPhysicsSpace(LynxEngine::PhysicsSystem::CPhysicsWorld *lpw);
			virtual ~CPhysicsSpace();			

			LYNXFORCEINLINE btCollisionConfiguration*					GetCollisionConfig() const {return m_CollisionConfiguration; };
			LYNXFORCEINLINE btCollisionDispatcher*						GetCollisionDispatcher() const {return m_Dispatcher; };
			LYNXFORCEINLINE btBroadphaseInterface*						GetPairCache() const {return m_OverlappingPairCache; };
			
			virtual	LYNXBOOL											vCreate(LynxEngine::PhysicsSystem::CPhysicsSpace* const parent);
			virtual void												vDestory();

			// Geometry ----------------------------------------------------------------------------------------------------------------
			virtual	LynxEngine::PhysicsSystem::CPhysicsShape* const		vCreateShape(LynxEngine::PhysicsSystem::CPhysicsShape::TYPE type, const LynxEngine::CCreationDesc *desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo = NULL);
			virtual	void												vDeleteShape(LynxEngine::PhysicsSystem::CPhysicsShape* const lpg);
		};	
	}
}

#endif