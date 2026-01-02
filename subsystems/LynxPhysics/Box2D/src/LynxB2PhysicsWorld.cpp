#include <LynxEngine_PCH.h>
#include <LynxB2PhysicsSystem.h>
#include <LynxB2PhysicsWorld.h>
#include <LynxB2PhysicsSpace.h>
#include <LynxB2PhysicsObj.h>

namespace LynxPhysics 
{
	namespace Box2D 
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
			b2Vec2 Gravity(m_Gravity.x, m_Gravity.y);
			m_World = LYNXGLOBAL_NEW b2World(Gravity, true);
			if (!m_World)
				return LYNX_FALSE;			

			m_lpRootSpace = vCreateSpace(NULL);					
			
			//temp			
			b2BodyDef BD;
			b2Body* Ground = m_World->CreateBody(&BD);
			b2PolygonShape Shape;
			Shape.SetAsEdge(b2Vec2(-24.0f, 16.0f), b2Vec2(24.0f, 16.0f));	
			Ground->CreateFixture(&Shape, 0.0f);
			
			Shape.SetAsEdge(b2Vec2(-24.0f, -16.0f), b2Vec2(24.0f, -16.0f));	
			Ground->CreateFixture(&Shape, 0.0f);
			
			//Shape.SetAsEdge(b2Vec2(16.0f, -24.0f), b2Vec2(16.0f, 24.0f));	
			//Ground->CreateFixture(&Shape, 0.0f);
			
			//temp

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
			delete lps;
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
			delete lpo;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::PhysicsSystem::CPhysicsJoint* const CPhysicsWorld::vCreateJoint(LynxEngine::PhysicsSystem::CPhysicsJoint::TYPE type, const LynxEngine::CCreationDesc* desc)
		{
			LynxEngine::PhysicsSystem::CPhysicsJoint *lpJoint = NULL;
			
			/*
			switch (type)
			{
				case LynxEngine::PhysicsSystem::CPhysicsJoint::BALL:
					lpJoint = LYNXNEW CBallJoint(this);					
					break;
			}						
			if (!lpJoint->vCreate())
				return LYNX_FALSE;
			
			AddJoint(lpJoint);
			*/
			
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
		void CPhysicsWorld::vCollisionLoop()				
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsWorld::vStep(float step)				
		{
			step = 0.1f;
			int32 VelocityIterations = 6;
			int32 PositionIterations = 2;
			m_World->Step(step, VelocityIterations, PositionIterations);
			m_World->ClearForces();		
		}
	}
}