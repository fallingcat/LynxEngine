#include <LynxEngine_PCH.h>
#include <LynxB2BoxShape.h>

namespace LynxPhysics 
{
	namespace Box2D 
	{
		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBoxShape::CBoxShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsShape(lps)
		{	
			m_PhysicsShapeType = BOX;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBoxShape::~CBoxShape(void)
		{
			LYNXGLOBAL_DEL(m_lpShape);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBoxShape::vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CBoxShapeDesc* pBoxDesc = (LynxEngine::PhysicsSystem::CBoxShapeDesc*)desc;
			
			m_lpShape = LYNXGLOBAL_NEW b2PolygonShape;
			((b2PolygonShape*)m_lpShape)->SetAsBox(pBoxDesc->m_Extension.x, pBoxDesc->m_Extension.y, b2Vec2(pBoxDesc->m_Center.x, pBoxDesc->m_Center.y), 0.0f*LYNXD2R);
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
