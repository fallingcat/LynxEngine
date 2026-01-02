#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsObj.h>
#include <LynxBtBoxShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
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
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBoxShape::vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CBoxShapeDesc* pBoxDesc = (LynxEngine::PhysicsSystem::CBoxShapeDesc*)desc;

			lynxMemCpy(&m_Desc, pBoxDesc, sizeof(LynxEngine::PhysicsSystem::CBoxShapeDesc));
			
			m_lpShape = LYNXGLOBAL_NEW btBoxShape(btVector3(pBoxDesc->m_Extension.x, pBoxDesc->m_Extension.y, pBoxDesc->m_Extension.z));

			if (lpo)
				lpo->vAddShape(this);				
			else
				CreateStaticPhyObj();				

			//m_lpShape->getContactBreakingThreshold(0.05f);

			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBoxShape::vSetSize(LYNXREAL* lpdata) 
		{
			if (m_lpShape)
			{
				LYNXGLOBAL_DEL(m_lpShape);
				m_lpShape = NULL;				
			}			
			m_Desc.m_Extension.x = lpdata[0]; 
			m_Desc.m_Extension.y = lpdata[1]; 
			m_Desc.m_Extension.z = lpdata[2]; 
			m_lpShape = LYNXGLOBAL_NEW btBoxShape(btVector3(m_Desc.m_Extension.x, m_Desc.m_Extension.y, m_Desc.m_Extension.z));

			Bullet::CPhysicsShape::Resize();			
		}
	}
}
