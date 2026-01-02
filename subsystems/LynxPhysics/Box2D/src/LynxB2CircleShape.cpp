#include <LynxEngine_PCH.h>
#include <LynxB2CircleShape.h>

namespace LynxPhysics 
{
	namespace Box2D 
	{
		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCircleShape::CCircleShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsShape(lps)
		{	
			m_PhysicsShapeType = CIRCLE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCircleShape::~CCircleShape(void)
		{
			LYNXGLOBAL_DEL(m_lpShape);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCircleShape::vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CSphereShapeDesc* Desc = (LynxEngine::PhysicsSystem::CSphereShapeDesc*)desc;
			
			m_lpShape = LYNXGLOBAL_NEW b2CircleShape;
			((b2CircleShape*)m_lpShape)->m_p.x = Desc->m_Center.x;
			((b2CircleShape*)m_lpShape)->m_p.y = Desc->m_Center.y;
			((b2CircleShape*)m_lpShape)->m_radius = Desc->m_Radius;

			m_FixtureDef.shape = m_lpShape;
			m_FixtureDef.density = 1.0f;
			m_FixtureDef.restitution = 0.2f;
			m_FixtureDef.friction = 0.4f;			
						
			if (lpo)
				lpo->vAddShape(this);				
			else
				CreateStaticPhyObj();				

			return LYNX_TRUE;
		}	
	}
}
