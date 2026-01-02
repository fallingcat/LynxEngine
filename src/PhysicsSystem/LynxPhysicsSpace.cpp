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
#include <PhysicsSystem/LynxPhysicsSpace.h>

namespace LynxEngine 
{	
	namespace PhysicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSpace::CPhysicsSpace(CPhysicsWorld* const lpw)
		{			
			m_lpWorld = lpw;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSpace::~CPhysicsSpace(void)
		{
			for (CList<CPhysicsShape*>::CIterator Shape = m_ShapeList.begin(); Shape != m_ShapeList.end(); ++Shape)
			{
				LYNXDEL (*Shape);				
			}
			m_ShapeList.clear();
		}		
	}
}