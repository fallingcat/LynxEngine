#include <LynxEngine_PCH.h>
#include <LynxBtCapsuleShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCapsuleShape::CCapsuleShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsShape(lps)
		{	
			m_PhysicsShapeType = CAPSULE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCapsuleShape::~CCapsuleShape(void)
		{
			if (m_lpShape)
			{
				LYNXGLOBAL_DEL(m_lpShape);
				m_lpShape = NULL;
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCapsuleShape::vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CCapsuleShapeDesc* pCapsuleDesc = (LynxEngine::PhysicsSystem::CCapsuleShapeDesc*)desc;
			
			lynxMemCpy(&m_Desc, pCapsuleDesc, sizeof(LynxEngine::PhysicsSystem::CCapsuleShapeDesc));
			
			m_lpShape = LYNXGLOBAL_NEW btCapsuleShape(pCapsuleDesc->m_Radius, pCapsuleDesc->m_Length);
			
			if (lpo)
				lpo->vAddShape(this);				
			else
				CreateStaticPhyObj();	
			
			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CCapsuleShape::vSetSize(LYNXREAL* lpdata) 
		{
			if (m_lpShape)
			{
				LYNXGLOBAL_DEL(m_lpShape);
				m_lpShape = NULL;				
			}			
			m_Desc.m_Radius = lpdata[0]; 
			m_Desc.m_Length = lpdata[1]; 
			m_lpShape = LYNXGLOBAL_NEW btCapsuleShape(m_Desc.m_Radius, m_Desc.m_Length);

			Bullet::CPhysicsShape::Resize();			
		}
	}
}
