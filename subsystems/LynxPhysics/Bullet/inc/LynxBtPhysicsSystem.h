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

#ifndef __LYNXBTPHYSICSSYSTEM_H__
#define __LYNXBTPHYSICSSYSTEM_H__

#include <LynxEngineClasses.h>
#include <PhysicsSystem/LynxPhysicsSystem.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{		
		class LYNXENGCLASS CPhysicsSystem : public LynxEngine::PhysicsSystem::CPhysicsSystem
		{	
			LYNX_DECLARE_CLASS(CPhysicsSystem);
		protected:		
		public:			
			CPhysicsSystem(LynxEngine::CEngine* const lpengine);
			~CPhysicsSystem();

			LYNXINLINE const LYNXCHAR*					vGetAPIString() {return _T("Bullet"); };
			LYNXBOOL									vCreate();						
			LynxEngine::PhysicsSystem::CPhysicsWorld* const	vCreateWorld();
			
			LYNXBOOL									vInit();			
		};	
	}
}

#endif