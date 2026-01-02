//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXPHYSICSSYSTEM_H__
#define __LYNXPHYSICSSYSTEM_H__

#include <LynxSystem.h>
#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	namespace PhysicsSystem 
	{
		class LYNXENGCLASS CPhysicsSystem : public CSystem
		{	
			LYNX_DECLARE_CLASS(CPhysicsSystem);
		public:			
			float								m_UpdateRate;
		protected:					
			float								m_UnitScale;  //Scale from physics system to rendering system 			
			float								m_CollisionThreshold;
			LYNXBOOL							m_bSleep;
			CList<CPhysicsWorld*>				m_WorldList;
		public:			
			CPhysicsSystem(CEngine* const lpengine);
			virtual ~CPhysicsSystem();

			DECLARE_CLASSSCRIPT
			
			virtual const LYNXCHAR*				vGetAPIString() = 0;
			virtual LYNXBOOL					vInit() = 0;
			virtual CPhysicsWorld* const		vCreateWorld() = 0;
			void								AddWorld(CPhysicsWorld *lpw);
			void								DeleteWorld(CPhysicsWorld *lpw);
			LYNXFORCEINLINE DWORD				GetNumWorlds() {return (DWORD)m_WorldList.size(); };
			LYNXINLINE CPhysicsWorld*			GetlpPhysicsWorld(int w) {return (m_WorldList.get(w)); };
			CPhysicsWorld*						GetlpPhysicsWorld(const LYNXCHAR* name);
			
			LYNXFORCEINLINE void				Sleep(LYNXBOOL b) {m_bSleep = b; };
			LYNXFORCEINLINE const LYNXBOOL		IsSleeping() const {return m_bSleep; };

			LYNXFORCEINLINE void				SetCollisionThreshold(float t) {m_CollisionThreshold = t; };
			LYNXFORCEINLINE const float			GetCollisionThreshold() const {return m_CollisionThreshold; };

			virtual void						vLoop(float step);			
		};	
	}
}

LynxEngine::PhysicsSystem::CPhysicsSystem* CreatePhysicsSystem(LynxEngine::CEngine* const lpengine);

#endif