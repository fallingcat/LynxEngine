#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsSystem.h>
#include <LynxBtPhysicsWorld.h>
#include <LynxBtPhysicsSpace.h>
#include <LynxBtPhysicsObj.h>
#include <LynxBtBallPhysicsJoint.h>
#include <LynxBtConePhysicsJoint.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		LynxEngine::PhysicsSystem::CPhysicsJoint* _CreateBallJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* w)	{ return LYNXNEW CBallJoint(w); }			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsWorld::CPhysicsWorld(LynxEngine::PhysicsSystem::CPhysicsSystem* lpw)
		: LynxEngine::PhysicsSystem::CPhysicsWorld(lpw)
		{				
			m_Solver = NULL;
			m_World = NULL;	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsWorld::~CPhysicsWorld(void)
		{		
			for (LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsObj*>::CIterator Obj = m_ObjList.begin(); Obj != m_ObjList.end(); ++Obj)
			{
				LYNXGLOBAL_DEL (*Obj);				
			}
			m_ObjList.clear();

			for (LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsJoint*>::CIterator Joint = m_JointList.begin(); Joint != m_JointList.end(); ++Joint)
			{
				LYNXGLOBAL_DEL (*Joint);				
			}
			m_JointList.clear();

			for (LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsSpace*>::CIterator Space = m_SpaceList.begin(); Space != m_SpaceList.end(); ++Space)
			{
				LYNXGLOBAL_DEL (*Space);				
			}
			m_SpaceList.clear();

			LYNXGLOBAL_DEL(m_Solver);
			LYNXGLOBAL_DEL(m_World);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsWorld::vCreate()		
		{
			m_lpRootSpace = vCreateSpace(NULL);									
		
			///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
			m_Solver = LYNXGLOBAL_NEW btSequentialImpulseConstraintSolver;
			
			m_World = LYNXGLOBAL_NEW btDiscreteDynamicsWorld(((Bullet::CPhysicsSpace*)m_lpRootSpace)->GetCollisionDispatcher(), ((Bullet::CPhysicsSpace*)m_lpRootSpace)->GetPairCache(), m_Solver, ((Bullet::CPhysicsSpace*)m_lpRootSpace)->GetCollisionConfig());			
			m_World->setGravity(btVector3(m_Gravity.x, m_Gravity.y, m_Gravity.z));
			m_World->getSolverInfo().m_solverMode |= SOLVER_ENABLE_FRICTION_DIRECTION_CACHING; //don't recalculate friction values each frame
			m_World->getSolverInfo().m_numIterations = 5; //few solver iterations 

			m_JointFactory.Register(GUID_BALLJOINT, _CreateBallJoint);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsSpace* const CPhysicsWorld::vCreateSpace(LynxEngine::PhysicsSystem::CPhysicsSpace* const parent)
		{
			LynxEngine::PhysicsSystem::CPhysicsSpace* lpSpace = LYNXGLOBAL_NEW CPhysicsSpace(this);
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
			LYNXGLOBAL_DEL lps;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsObj* const CPhysicsWorld::vCreateObj(LynxEngine::PhysicsSystem::CPhysicsObj::TYPE t, LynxEngine::PhysicsSystem::CPhysicsShape* g)
		{
			LynxEngine::PhysicsSystem::CPhysicsObj* lpObj = LYNXGLOBAL_NEW Bullet::CPhysicsObj(this);
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
			LYNXGLOBAL_DEL lpo;
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
				case LynxEngine::PhysicsSystem::CPhysicsJoint::BALL:
					lpJoint = LYNXGLOBAL_NEW CBallJoint(this);					
					break;
				case LynxEngine::PhysicsSystem::CPhysicsJoint::CONE:
					lpJoint = LYNXGLOBAL_NEW CConeJoint(this);					
					break;
			}						
			if (!lpJoint->vCreate(desc))
				return NULL;
			
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

			LYNXGLOBAL_DEL lpj;
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
			#ifdef __iOS__
				m_World->stepSimulation(step*m_lpEngine->GetStepMultiplier(), 10, m_lpEngine->GetStepMultiplier()/30.0f);
			#else
				m_World->stepSimulation(step*m_lpEngine->GetStepMultiplier(), 500, m_lpEngine->GetStepMultiplier()/(m_lpPhysicsSystem->m_UpdateRate));
			#endif
		}
	}
}