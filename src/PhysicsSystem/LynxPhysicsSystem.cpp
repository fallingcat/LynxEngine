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

#include <LynxEngine_PCH.h>

namespace LynxEngine 
{	
	namespace PhysicsSystem 
	{
		IMPLEMENT_CLASSSCRIPT(CPhysicsSystem, CSystem)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSystem::CPhysicsSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{						
			m_UnitScale = 1.0f;
			m_UpdateRate = 30.0f;
			m_CollisionThreshold = 0.01f;
			m_bSleep = LYNX_FALSE;			

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSystem::~CPhysicsSystem(void)
		{
			for (CList<CPhysicsWorld*>::CIterator World = m_WorldList.begin(); World != m_WorldList.end(); ++World)
			{
				LYNXDEL (*World);				
			}
			m_WorldList.clear();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsSystem::vLoop(float step)
		{
			if (!m_bSleep)
			{
				for (CList<CPhysicsWorld*>::CIterator World = m_WorldList.begin(); World != m_WorldList.end(); ++World)
				{
					(*World)->vCollisionLoop();				
					(*World)->vStep(step);								
				}
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsSystem::AddWorld(CPhysicsWorld *lpw)
		{			
			m_WorldList.push_back(lpw);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsSystem::DeleteWorld(CPhysicsWorld *lpw)
		{			
			m_WorldList.remove(lpw);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsWorld* CPhysicsSystem::GetlpPhysicsWorld(const LYNXCHAR* name)
		{
			for (CList<CPhysicsWorld*>::CIterator World = m_WorldList.begin(); World != m_WorldList.end(); ++World)
			{
				if (!lynxStrCmp((*World)->GetName(), name))
				{
					return (*World);				
				}
			}
			return NULL; 
		}
	}
}