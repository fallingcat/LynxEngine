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

#ifndef __LYNXB2PHYSICSSYSTEM_H__
#define __LYNXB2PHYSICSSYSTEM_H__

//#include <NxPhysicsSDK.h>
#include <Box2D/Box2D.h>
#include <LynxEngineClasses.h>
#include <PhysicsSystem/LynxPhysicsSystem.h>

namespace LynxPhysics 
{
	namespace Box2D 
	{
		class LYNXENGCLASS CPhysicsSystem : public LynxEngine::PhysicsSystem::CPhysicsSystem
		{	
			LYNX_DECLARE_CLASS(CPhysicsSystem);
		protected:	
		public:			
			CPhysicsSystem(LynxEngine::CEngine* const lpengine);
			~CPhysicsSystem();

			LYNXFORCEINLINE const float					GetUnitScale() const {return m_UnitScale; };
			LYNXFORCEINLINE void						SetUnitScale(const float s) {m_UnitScale = s; };
			
			LYNXFORCEINLINE const LYNXCHAR*				vGetAPIString() {return _T("Box2D"); };
			LYNXBOOL									vCreate();						
			LynxEngine::PhysicsSystem::CPhysicsWorld* const	vCreateWorld();
		
			LYNXBOOL									vInit();			
		};	
	}
}

#endif