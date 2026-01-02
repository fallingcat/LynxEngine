#include <LynxODEPhysicsJoint.h>
#include <LynxODEPhysicsObj.h>

namespace LynxPhysics 
{
	namespace ODE 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsJoint::CPhysicsJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw)
		: LynxEngine::PhysicsSystem::CPhysicsJoint(lpw)
		{			
			m_Joint = 0;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsJoint::~CPhysicsJoint(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsJoint::vAttach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2)  
		{			
			if (!lpo1 && !lpo2)
				dJointAttach(m_Joint, 0, 0);
			else if (!lpo2)
				dJointAttach(m_Joint, ((CPhysicsObj*)lpo1)->GetBody(), 0);
			else if (!lpo1)
				dJointAttach(m_Joint, 0, ((CPhysicsObj*)lpo2)->GetBody());
			else
				dJointAttach(m_Joint, ((CPhysicsObj*)lpo1)->GetBody(), ((CPhysicsObj*)lpo2)->GetBody());

			LynxEngine::PhysicsSystem::CPhysicsJoint::vAttach(lpo1, lpo2); 
		};
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsJoint::vAttachObj1(LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{			
			if (!lpo)
			{
				if (m_AttchedObj[1])
					dJointAttach(m_Joint, 0, ((CPhysicsObj*)m_AttchedObj[1])->GetBody());
				else
					dJointAttach(m_Joint, 0, 0);
			}
			else
			{
				if (m_AttchedObj[1])
					dJointAttach(m_Joint, ((CPhysicsObj*)lpo)->GetBody(), ((CPhysicsObj*)m_AttchedObj[1])->GetBody());
				else
					dJointAttach(m_Joint, ((CPhysicsObj*)lpo)->GetBody(), 0);
			}

			LynxEngine::PhysicsSystem::CPhysicsJoint::vAttachObj1(lpo); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsJoint::vAttachObj2(LynxEngine::PhysicsSystem::CPhysicsObj* lpo)
		{
			if (!lpo)
			{			
				if (m_AttchedObj[0])
					dJointAttach(m_Joint, ((CPhysicsObj*)m_AttchedObj[0])->GetBody(), 0);
				else
					dJointAttach(m_Joint, 0, 0);
			}
			else
			{
				if (m_AttchedObj[0])
					dJointAttach(m_Joint, ((CPhysicsObj*)m_AttchedObj[0])->GetBody(), ((CPhysicsObj*)lpo)->GetBody());
				else
					dJointAttach(m_Joint, 0, ((CPhysicsObj*)lpo)->GetBody());
			}

			LynxEngine::PhysicsSystem::CPhysicsJoint::vAttachObj2(lpo); 
		}
	}
}