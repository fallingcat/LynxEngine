#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsWorld.h>
#include <LynxBtPhysicsJoint.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CJoint::CJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw)
		: LynxEngine::PhysicsSystem::CPhysicsJoint(lpw)
		{	
			m_PhysicsJointType = NULL_JOINT;	
			m_lpConstraint = NULL;
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
			if (m_lpConstraint)
			{
				((CPhysicsWorld*)m_lpWorld)->GetDynamicsWorld()->removeConstraint(m_lpConstraint);
				LYNXDEL(m_lpConstraint);
				m_lpConstraint = NULL;
			}
		}
	}
}
