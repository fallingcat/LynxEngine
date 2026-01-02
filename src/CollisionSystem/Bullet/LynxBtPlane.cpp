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
#include <CollisionSystem/Bullet/LynxBtCollisionSystem.h>
#include <CollisionSystem/Bullet/LynxBtOBB.h>
#include <CollisionSystem/Bullet/LynxBtCapsule.h>
#include <CollisionSystem/Bullet/LynxBtPlane.h>

#include "LinearMath/btQuaternion.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btTransformUtil.h"
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
#include "BulletCollision/NarrowPhaseCollision/btPointCollector.h"
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h"

namespace LynxEngine 
{	
	namespace CollisionSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtPlane::CBtPlane(void) :
			m_Normal(0.0f, 1.0f, 0.0f), 
			m_Distance(0.0f)
		{
			m_Type = PLANE;		
			btVector3 Normal(m_Normal.x, m_Normal.y, m_Normal.z);
			m_Shape = LYNXGLOBAL_NEW btStaticPlaneShape(Normal, m_Distance);		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtPlane::CBtPlane(const CVector3& n, const float d) :
			m_Normal(n), 
			m_Distance(d)
		{
			m_Type = PLANE;				
			btVector3 Normal(m_Normal.x, m_Normal.y, m_Normal.z);
			m_Shape = LYNXGLOBAL_NEW btStaticPlaneShape(Normal, m_Distance);		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtPlane::~CBtPlane(void)
		{
			if (m_Shape)
				LYNXGLOBAL_DEL(m_Shape);
		}								
	}
}