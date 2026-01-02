#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsObj.h>
#include <LynxBtSphereShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSphereShape::CSphereShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsShape(lps)
		{	
			m_PhysicsShapeType = SPHERE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSphereShape::~CSphereShape(void)
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
		LYNXBOOL CSphereShape::vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CSphereShapeDesc* pSphereDesc = (LynxEngine::PhysicsSystem::CSphereShapeDesc*)desc;
			
			lynxMemCpy(&m_Desc, pSphereDesc, sizeof(LynxEngine::PhysicsSystem::CSphereShapeDesc));
			
			m_lpShape = LYNXGLOBAL_NEW btSphereShape(pSphereDesc->m_Radius);
			
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
		void CSphereShape::vSetSize(LYNXREAL* lpdata) 
		{
			if (m_lpShape)
			{
				LYNXGLOBAL_DEL(m_lpShape);
				m_lpShape = NULL;				
			}			
			m_Desc.m_Radius = lpdata[0];
			m_lpShape = LYNXGLOBAL_NEW btSphereShape(m_Desc.m_Radius);

			Bullet::CPhysicsShape::Resize();			
		}
	}
}
