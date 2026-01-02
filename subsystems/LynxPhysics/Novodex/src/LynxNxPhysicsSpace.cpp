#include <LynxEngine_PCH.h>

#ifdef __PHYSX__

#include <LynxNxPhysicsSpace.h>
#include <LynxNxPhysicsWorld.h>
#include <LynxNxPlaneShape.h>
#include <LynxNxBoxShape.h>
#include <LynxNxSphereShape.h>
#include <LynxNxCapsuleShape.h>
#include <LynxNxTriMeshShape.h>
#include <LynxNxHullShape.h>
#include <LynxNxHeightFieldShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSpace::CPhysicsSpace(LynxEngine::PhysicsSystem::CPhysicsWorld *lpw)
		: LynxEngine::PhysicsSystem::CPhysicsSpace(lpw)
		{
			//m_Space = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsSpace::~CPhysicsSpace(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsSpace::vCreate(LynxEngine::PhysicsSystem::CPhysicsSpace* parent)
		{			
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
					lpGeo = LYNXNEW CPlaneShape(this);					
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
					lpGeo = LYNXNEW CHeightFieldShape(this);					
					break;
			}						
			if (!lpGeo->vCreate(desc, lpo))
				return LYNX_FALSE;
			
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

#endif