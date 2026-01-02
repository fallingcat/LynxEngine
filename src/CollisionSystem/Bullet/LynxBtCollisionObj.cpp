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
#include <CollisionSystem/Bullet/LynxBtCollisionObj.h>
#include <CollisionSystem/Bullet/LynxBtCollisionSystem.h>
#include <CollisionSystem/Bullet/LynxBtOBB.h>
#include <CollisionSystem/Bullet/LynxBtCapsule.h>

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
		CBtCollisionObj::CBtCollisionObj(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtCollisionObj::~CBtCollisionObj(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBtCollisionObj::DefaultHitTest(CCollisionObj& o)
		{
			btVoronoiSimplexSolver sGjkSimplexSolver;
			btGjkEpaPenetrationDepthSolver epaSolver;
			btPointCollector gjkOutput; 
			btVector3 worldBoundsMin(-1000, -1000, -1000);
			btVector3 worldBoundsMax( 1000,  1000,  1000);			

			btGjkPairDetector convexConvex((const btConvexShape *)m_Shape, (const btConvexShape *)(((CBtCollisionObj&)o).m_Shape), &sGjkSimplexSolver, &epaSolver); 
	   
			btGjkPairDetector::ClosestPointInput Input; 

			Input.m_transformA.setIdentity();
			Input.m_transformB.setIdentity();
		
			convexConvex.getClosestPoints(Input, gjkOutput, 0); 

			if (gjkOutput.m_distance < 0)
				return LYNX_TRUE;
			else
				return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBtCollisionObj::vHitTest(CCollisionObj& o)
		{
			return HitTest((CBtCollisionObj&)o);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBtCollisionObj::HitTest(CBtCollisionObj& o)
		{
			btVoronoiSimplexSolver sGjkSimplexSolver;
			btGjkEpaPenetrationDepthSolver epaSolver;
			btPointCollector gjkOutput;

			btGjkPairDetector convexConvex(GetShape(), o.GetShape(), &sGjkSimplexSolver, &epaSolver);

			btGjkPairDetector::ClosestPointInput Input;

			CMatrix4 A, B;
			vSetupCollisionMatrix(A);
			Matrix4ToBullet(Input.m_transformA, A);

			o.vSetupCollisionMatrix(B);
			Matrix4ToBullet(Input.m_transformB, B);

			convexConvex.getClosestPoints(Input, gjkOutput, 0);

			m_CollisionNormal.x = gjkOutput.m_normalOnBInWorld.m_floats[0];
			m_CollisionNormal.y = gjkOutput.m_normalOnBInWorld.m_floats[1];
			m_CollisionNormal.z = gjkOutput.m_normalOnBInWorld.m_floats[2];
			m_CollisionDistance = -gjkOutput.m_distance;

			if (gjkOutput.m_distance <= glpKernel->GetlpEngine()->GetlpCollisionSystem()->GetHitTestOffset())
				return LYNX_TRUE;
			else
				return LYNX_FALSE;
		}
	}
}