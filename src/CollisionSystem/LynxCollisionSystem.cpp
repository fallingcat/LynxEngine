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
#include <CollisionSystem/LynxCollisionSystem.h>
#include <CollisionSystem/LynxAABB.h>
#include <CollisionSystem/LynxSphere.h>

namespace LynxEngine 
{	
	namespace CollisionSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCollisionSystem::CCollisionSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{
			m_CollisionObjList.clear();
			m_HitTestOffset = 0.0f;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCollisionSystem::~CCollisionSystem(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCollisionSystem::vCreate(void)
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCollisionObj* CCollisionSystem::vCreateCollisionObj(CCollisionObj::TYPE type)
		{			
			switch (type)
			{
				case CCollisionObj::AABB:
					return LYNXNEW CAABB();
					break;

				case CCollisionObj::SPHERE:
					return LYNXNEW CSphere();
					break;

				default:
					return NULL;
			}
		}
	}
}