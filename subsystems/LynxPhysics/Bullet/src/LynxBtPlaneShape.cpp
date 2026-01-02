#include <LynxEngine_PCH.h>
#include <LynxNxPlaneShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPlaneShape::CPlaneShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsShape(lps)
		{	
			m_Type = PLANE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPlaneShape::~CPlaneShape(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPlaneShape::vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CPlaneShapeDesc* pDesc = (LynxEngine::PhysicsSystem::CPlaneShapeDesc*)desc;
			m_Desc.setToDefault();
			m_Desc.normal.set(pDesc->m_Normal.x, pDesc->m_Normal.y, pDesc->m_Normal.z);
			m_Desc.d = pDesc->m_Distance;						

			if (lpo)
				lpo->vAddShape(this);				
			else
				CreateStaticPhyObj();				

			return LYNX_TRUE;
		}	
	}
}
