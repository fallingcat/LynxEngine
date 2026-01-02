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
#include <CollisionSystem/LynxAABB.h>

//#define __USE_SAP__				1
#define __USE_DBVT__			1

namespace LynxEngine 
{	
	namespace CollisionSystem 
	{
		void BulletToMatrix4(LynxEngine::Math::CMatrix4& m, const btTransform& b)
		{
			m.m11 = (float)b.getBasis().getColumn(0).getX(); m.m12 = (float)b.getBasis().getColumn(0).getY(); m.m13 = (float)b.getBasis().getColumn(0).getZ(); m.m14 = 0.0f;
			m.m21 = (float)b.getBasis().getColumn(1).getX(); m.m22 = (float)b.getBasis().getColumn(1).getY(); m.m23 = (float)b.getBasis().getColumn(1).getZ(); m.m24 = 0.0f;
			m.m31 = (float)b.getBasis().getColumn(2).getX(); m.m32 =(float) b.getBasis().getColumn(2).getY(); m.m33 = (float)b.getBasis().getColumn(2).getZ(); m.m34 = 0.0f;
			m.m41 = (float)b.getOrigin().getX();		  m.m42 = (float)b.getOrigin().getY();			m.m43 =(float) b.getOrigin().getZ();		  m.m44 = 1.0f;
		}
		void Matrix4ToBullet(btTransform& b, const LynxEngine::Math::CMatrix4& m)
		{
			//b.setBasis(btMatrix3x3(m.m11, m.m12, m.m13, m.m21, m.m22, m.m23, m.m31, m.m32, m.m33));
			b.setBasis(btMatrix3x3(m.m11, m.m21, m.m31, m.m12, m.m22, m.m32, m.m13, m.m23, m.m33));
			b.setOrigin(btVector3(m.m41, m.m42, m.m43));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtCollisionSystem::CBtCollisionSystem(CEngine* const lpengine)
		: CCollisionSystem(lpengine)
		{
			m_CollisionConfiguration = NULL;
			m_Dispatcher = NULL;
			m_OverlappingPairCache = NULL;
			m_CollisionWorld = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtCollisionSystem::~CBtCollisionSystem(void)
		{
			LYNXGLOBAL_DEL(m_CollisionConfiguration);
			LYNXGLOBAL_DEL(m_Dispatcher);
			LYNXGLOBAL_DEL(m_OverlappingPairCache);
			LYNXGLOBAL_DEL(m_CollisionWorld);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBtCollisionSystem::vCreate(BROADPHASETYPE type)
		{
			//collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
			m_CollisionConfiguration = LYNXGLOBAL_NEW btDefaultCollisionConfiguration();
			
			//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
			m_Dispatcher = LYNXGLOBAL_NEW btCollisionDispatcher(m_CollisionConfiguration);
			
			switch (type)
			{
				case BP_SAP:
				{
					btVector3 worldAabbMin(-1000, -1000, -1000);
					btVector3 worldAabbMax( 1000,  1000,  1000);
					//btHashedOverlappingPairCache* pairCache = LYNXNEW btHashedOverlappingPairCache();
					//m_OverlappingPairCache = LYNXNEW btAxisSweep3(worldAabbMin, worldAabbMax, 3500, pairCache);
					m_OverlappingPairCache = LYNXGLOBAL_NEW btAxisSweep3(worldAabbMin, worldAabbMax);
					break;
				}

				case BP_DBVT:
					// Using Dbvt algorithm
					// btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
					m_OverlappingPairCache = LYNXGLOBAL_NEW btDbvtBroadphase();
					break;

				case BP_SIMPLE:
					m_OverlappingPairCache = LYNXGLOBAL_NEW btSimpleBroadphase();
					break;
			}			
			m_CollisionWorld = LYNXGLOBAL_NEW btCollisionWorld(m_Dispatcher, m_OverlappingPairCache, m_CollisionConfiguration);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCollisionObj* CBtCollisionSystem::vCreateCollisionObj(CCollisionObj::TYPE type)
		{			
			switch (type)
			{
				case CCollisionObj::OBB:
					return LYNXNEW CBtOBB();
					break;

				case CCollisionObj::CAPSULE:
					return LYNXNEW CBtCapsule();
					break;

				case CCollisionObj::SPHERE:
					return LYNXNEW CBtSphere();
					break;

				default:
					return NULL;
			}
		}
	}
}