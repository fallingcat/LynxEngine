#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsSpace.h>
#include <LynxBtPhysicsWorld.h>
#include <LynxBtBoxShape.h>
#include <LynxBtSphereShape.h>
#include <LynxBtCapsuleShape.h>
#include <LynxBtHullShape.h>
#include <LynxBtTriMeshShape.h>

//#define __USE_SAP__				1
#define __USE_DBVT__			1

namespace LynxPhysics 
{
	namespace Bullet 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSpace::CPhysicsSpace(LynxEngine::PhysicsSystem::CPhysicsWorld *lpw)
		: LynxEngine::PhysicsSystem::CPhysicsSpace(lpw)
		{
			m_CollisionConfiguration = NULL;
			m_Dispatcher = NULL;
			m_OverlappingPairCache = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSpace::~CPhysicsSpace(void)
		{
			for (LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsShape*>::CIterator Shape = m_ShapeList.begin(); Shape != m_ShapeList.end(); ++Shape)
			{
				LYNXDEL (*Shape);				
			}
			m_ShapeList.clear();

			LYNXGLOBAL_DEL(m_CollisionConfiguration);
			LYNXGLOBAL_DEL(m_Dispatcher);
			LYNXGLOBAL_DEL(m_OverlappingPairCache);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsSpace::vCreate(LynxEngine::PhysicsSystem::CPhysicsSpace* parent)
		{			
			//collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
			m_CollisionConfiguration = LYNXGLOBAL_NEW btDefaultCollisionConfiguration();
			
			//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
			m_Dispatcher = LYNXGLOBAL_NEW btCollisionDispatcher(m_CollisionConfiguration);
			
			// Using SAP algorithm
			#ifdef __USE_SAP__
				btVector3 worldAabbMin(-1000, -1000, -1000);
				btVector3 worldAabbMax( 1000,  1000,  1000);
				//btHashedOverlappingPairCache* pairCache = LYNXNEW btHashedOverlappingPairCache();
				//m_OverlappingPairCache = LYNXNEW btAxisSweep3(worldAabbMin, worldAabbMax, 3500, pairCache);
				m_OverlappingPairCache = LYNXNEW btAxisSweep3(worldAabbMin, worldAabbMax);
			#elif __USE_DBVT__
				// Using Dbvt algorithm
				// btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
				m_OverlappingPairCache = LYNXGLOBAL_NEW btDbvtBroadphase();
			#else
				m_OverlappingPairCache = LYNXGLOBAL_NEW btSimpleBroadphase();
			#endif
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsSpace::vDestory()
		{
			//dBodyDestroy(m_Body);			
			//m_lpWorld = NULL;
			//vDeleteAllPhyGeos();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsShape* const CPhysicsSpace::vCreateShape(LynxEngine::PhysicsSystem::CPhysicsShape::TYPE type, const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CPhysicsShape *lpGeo = NULL;			
			switch (type)
			{
				case LynxEngine::PhysicsSystem::CPhysicsShape::PLANE:
					//lpGeo = LYNXNEW CPlaneShape(this);					
					break;

				case LynxEngine::PhysicsSystem::CPhysicsShape::BOX:
					lpGeo = LYNXNEW CBoxShape(this);					
					break;

				case LynxEngine::PhysicsSystem::CPhysicsShape::SPHERE:
					lpGeo = LYNXNEW CSphereShape(this);					
					break;

				case LynxEngine::PhysicsSystem::CPhysicsShape::CAPSULE:
					lpGeo = LYNXNEW CCapsuleShape(this);					
					break;

				case LynxEngine::PhysicsSystem::CPhysicsShape::HULL:
					lpGeo = LYNXNEW CHullShape(this);					
					break;

				case LynxEngine::PhysicsSystem::CPhysicsShape::TRIMESH:
					lpGeo = LYNXNEW CTriMeshShape(this);					
					break;

				case LynxEngine::PhysicsSystem::CPhysicsShape::HEIGHTFIELD:
					//lpGeo = LYNXNEW CHeightFieldShape(this);					
					break;
			}		
			if (!lpGeo->vCreate(desc, lpo))
				return NULL;
			
			AddShape(lpGeo);
			
			return lpGeo;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsSpace::vDeleteShape(LynxEngine::PhysicsSystem::CPhysicsShape* lpg)
		{
			DeleteShape(lpg);
			LYNXDEL lpg;
		}
	}
}