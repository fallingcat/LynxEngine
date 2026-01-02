#include <LynxEngine_PCH.h>

#ifdef __PHYSX__

#include <LynxNxPhysicsWorld.h>
#include <LynxNxPhysicsJoint.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CJoint::CJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw)
		: LynxEngine::PhysicsSystem::CPhysicsJoint(lpw)
		{	
			m_PhysicsJointType = NULL_JOINT;	
			m_lpJoint = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CJoint::~CJoint(void)
		{
			vDestory();			
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CJoint::vDestory()
		{
			if (m_lpJoint)
			{
				((CPhysicsWorld*)m_lpWorld)->GetlpNxScene()->releaseJoint(*m_lpJoint);
				m_lpJoint = NULL;
			}
		}
	}
}

#endif