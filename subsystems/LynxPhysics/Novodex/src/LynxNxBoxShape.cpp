#include <LynxEngine_PCH.h>
#include <LynxNxBoxShape.h>

#ifdef __PHYSX__

namespace LynxPhysics 
{
	namespace Novodex 
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
			m_Desc.setToDefault();
			m_Desc.dimensions.set(pBoxDesc->m_Extension.x, pBoxDesc->m_Extension.y, pBoxDesc->m_Extension.z);
			m_Desc.localPose.t = NxVec3(0.0f, 0.0f, 0.0f);

			if (lpo)
				lpo->vAddShape(this);				
			else
				CreateStaticPhyObj();				

			return LYNX_TRUE;
		}	
	}
}

#endif