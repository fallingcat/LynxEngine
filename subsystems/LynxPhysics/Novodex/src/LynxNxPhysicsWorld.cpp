#include <LynxEngine_PCH.h>

#ifdef __PHYSX__

#include <LynxNxPhysicsSystem.h>
#include <LynxNxPhysicsWorld.h>
#include <LynxNxPhysicsSpace.h>
#include <LynxNxPhysicsObj.h>
#include <LynxNxConePhysicsJoint.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsWorld::CPhysicsWorld(LynxEngine::PhysicsSystem::CPhysicsSystem* lpw)
		: LynxEngine::PhysicsSystem::CPhysicsWorld(lpw)
		{				
			//m_World = 0;	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsWorld::~CPhysicsWorld(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsWorld::vCreate()		
		{
			// Create the scene
			NxSceneDesc sceneDesc;
			sceneDesc.gravity.x				= m_Gravity.x;
			sceneDesc.gravity.y				= m_Gravity.y;
			sceneDesc.gravity.z				= m_Gravity.z;
			sceneDesc.simType				= NX_SIMULATION_SW;			
    
			m_World = ((CPhysicsSystem*)m_lpPhysicsSystem)->GetNxPhySDK()->createScene(sceneDesc);
			if (!m_World)
				return LYNX_FALSE;			

			// Create the default material
			NxMaterial* defaultMaterial = m_World->getMaterialFromIndex(0); 
			defaultMaterial->setRestitution(0.5);
			defaultMaterial->setStaticFriction(0.5);
			defaultMaterial->setDynamicFriction(0.5);

			m_lpRootSpace = vCreateSpace(NULL);						

			return TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsSpace* const CPhysicsWorld::vCreateSpace(LynxEngine::PhysicsSystem::CPhysicsSpace* const parent)
		{
			LynxEngine::PhysicsSystem::CPhysicsSpace* lpSpace = LYNXNEW CPhysicsSpace(this);
			AddSpace(lpSpace);
			lpSpace->vCreate(parent);

			return lpSpace;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsWorld::vDeleteSpace(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		{
			lps->vDestory();
			DeleteSpace(lps);
			LYNXDEL lps;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsObj* const CPhysicsWorld::vCreateObj(LynxEngine::PhysicsSystem::CPhysicsObj::TYPE t, LynxEngine::PhysicsSystem::CPhysicsShape* g)
		{
			LynxEngine::PhysicsSystem::CPhysicsObj* lpObj = LYNXNEW CPhysicsObj(this);
			AddObj(lpObj);
			lpObj->vCreate(t, g);

			return lpObj;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsWorld::vDeleteObj(LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			DeleteObj(lpo);
			LYNXDEL lpo;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsJoint* const CPhysicsWorld::vCreateJoint(LynxEngine::PhysicsSystem::CPhysicsJoint::TYPE type, const LynxEngine::CCreationDesc* desc)
		{
			LynxEngine::PhysicsSystem::CPhysicsJoint *lpJoint;
			
			switch (type)
			{
				case LynxEngine::PhysicsSystem::CPhysicsJoint::CONE:
					lpJoint = LYNXNEW CConeJoint(this);					
					break;
			}						
			if (!lpJoint->vCreate(desc))
				return LYNX_FALSE;
			
			AddJoint(lpJoint);

			return lpJoint;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsWorld::vDeleteJoint(LynxEngine::PhysicsSystem::CPhysicsJoint* const lpj)
		{
			lpj->vDestory();
			DeleteJoint(lpj);

			LYNXDEL lpj;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsWorld::vCollisionLoop()				
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsWorld::vStep(float step)				
		{
			m_World->simulate(step*m_lpEngine->GetStepMultiplier());
			m_World->flushStream();			

			while (!m_World->fetchResults(NX_RIGID_BODY_FINISHED, false));
		}
	}
}

#endif