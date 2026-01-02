#include <LynxODEPhysicsSpace.h>
#include <LynxODEPhysicsSystem.h>
#include <LynxODEPhysicsWorld.h>
#include <LynxODEPhysicsObj.h>
#include <LynxODEBallPhysicsJoint.h>

namespace LynxPhysics 
{	
	namespace ODE 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsWorld::CPhysicsWorld(LynxEngine::PhysicsSystem::CPhysicsSystem* const lpsys)
		: LynxEngine::PhysicsSystem::CPhysicsWorld(lpsys)
		{			
			m_World = 0;				
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
			m_World = dWorldCreate();
			if (m_World <= 0)
				return FALSE;

			m_lpRootSpace = vCreateSpace(NULL);			
			dWorldSetGravity(m_World, m_Gravity.s.x, m_Gravity.s.y, m_Gravity.s.z);
			//dWorldSetERP(m_World, 0.2f);			
			//dWorldSetCFM(m_World, 0.05f);

			//dWorldSetERP(m_World, 2.0f);			



			m_ContactGroup = dJointGroupCreate(0);	
			
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
		void CPhysicsWorld::vDeleteSpace(LynxEngine::PhysicsSystem::CPhysicsSpace* const lps)
		{
			lps->vDestory();
			DeleteSpace(lps);
			delete lps;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsObj* const CPhysicsWorld::vCreateObj()
		{
			LynxEngine::PhysicsSystem::CPhysicsObj* lpObj = LYNXNEW CPhysicsObj(this);
			AddObj(lpObj);
			lpObj->vCreate();

			return lpObj;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsWorld::vDeleteObj(LynxEngine::PhysicsSystem::CPhysicsObj* const lpo)
		{
			lpo->vDestory();
			DeleteObj(lpo);
			delete lpo;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsJoint* const CPhysicsWorld::vCreateJoint(LynxEngine::PhysicsSystem::CPhysicsJoint::TYPE type)
		{
			LynxEngine::PhysicsSystem::CPhysicsJoint *lpJoint;
			
			switch (type)
			{
				case LynxEngine::PhysicsSystem::CPhysicsJoint::BALL:
					lpJoint = LYNXNEW CBallJoint(this);					
					break;
			}						
			if (!lpJoint->vCreate())
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
			delete lpj;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsWorld::sNearCallback(void *data, dGeomID o1, dGeomID o2)
		{	
			dContact					Contacts[MAX_CONTACTS];	
			int							i, NumContact;	
			dJointID					C;
			CPhysicsWorld*				lpWorld;

			lpWorld = (CPhysicsWorld *)data;
			if (dGeomIsSpace (o1) || dGeomIsSpace (o2)) 
			{
				// colliding a space with something				
				dSpaceCollide2(o1, o2, data, sNearCallback);
				// collide all geoms internal to the space(s)
				if (dGeomIsSpace(o1)) 					
					dSpaceCollide((dSpaceID)o1, data, sNearCallback);
				if (dGeomIsSpace(o2)) 					
					dSpaceCollide((dSpaceID)o2, data, sNearCallback);
			}
			else 
			{
				// colliding two non-space geoms, so generate contact
				// points between o1 and o2
				NumContact = dCollide(o1, o2, 80, &Contacts[0].geom, sizeof(dContact));	
				for (i=0; i<NumContact; ++i)
				{
					Contacts[i].surface.mode = dContactBounce;
					Contacts[i].surface.mu = 250;
					Contacts[i].surface.bounce = 0.3f;
					Contacts[i].surface.bounce_vel = 0.05f;
					C = dJointCreateContact(lpWorld->GetWorld(), lpWorld->GetContactGroup(), &Contacts[i]);										
					dJointAttach(C, dGeomGetBody(Contacts[i].geom.g1), dGeomGetBody(Contacts[i].geom.g2));
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsWorld::vCollisionLoop()				
		{
			dSpaceCollide(((CPhysicsSpace *)m_lpRootSpace)->GetSpace(), this, sNearCallback);		  
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsWorld::vStep(float step)				
		{
			if (m_bFastStep)			
			{
				dWorldQuickStep(m_World, step);					
			}
			else
				dWorldStep(m_World, step);	
			dJointGroupEmpty(m_ContactGroup);
		}
	}
}