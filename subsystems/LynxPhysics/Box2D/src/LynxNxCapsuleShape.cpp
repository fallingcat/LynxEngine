#include <LynxEngine_PCH.h>
#include <LynxNxCapsuleShape.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCapsuleShape::CCapsuleShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: CPhysicsShape(lps)
		{	
			m_Type = CAPSULE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCapsuleShape::~CCapsuleShape(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCapsuleShape::vCreate(const LynxEngine::CCreationDesc* desc, LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			LynxEngine::PhysicsSystem::CCapsuleShapeDesc* pDesc = (LynxEngine::PhysicsSystem::CCapsuleShapeDesc*)desc;
			m_Desc.radius = pDesc->m_Radius;
			m_Desc.height = pDesc->m_Length;
			m_Desc.localPose.t = NxVec3(0.0f, 0.0f, 0.0f);

			if (lpo)
				lpo->vAddShape(this);				
			else
				CreateStaticPhyObj();				

			return LYNX_TRUE;
		}	
	}
}
