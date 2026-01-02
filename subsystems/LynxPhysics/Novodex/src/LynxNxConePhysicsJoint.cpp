#include <LynxEngine_PCH.h>

#ifdef __PHYSX__

#include <LynxNxPhysicsWorld.h>
#include <LynxNxPhysicsObj.h>
#include <LynxNxConePhysicsJoint.h>

namespace LynxPhysics 
{
	namespace Novodex 
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CConeJoint::CConeJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw)
		: CJoint(lpw)
		{	
			m_PhysicsJointType = CONE;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CConeJoint::~CConeJoint(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CConeJoint::vCreate(const LynxEngine::CCreationDesc* desc)
		{			
			lynxMemCpy(&m_Desc, desc, sizeof(LynxEngine::PhysicsSystem::CConeJointDesc));
			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CConeJoint::Attach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2, LynxEngine::Math::CVector3& ap, LynxEngine::Math::CVector3& axis) 
		{
			CJoint::vAttach(lpo1, lpo2);
			
			vDestory();			

			NxSphericalJointDesc	fixedDesc;
			//NxFixedJointDesc		fixedDesc;

			fixedDesc.actor[0] = ((CPhysicsObj*)lpo1)->GetlpNxActor();
			fixedDesc.actor[1] = ((CPhysicsObj*)lpo2)->GetlpNxActor();
			fixedDesc.setGlobalAnchor(NxVec3(ap.x, ap.y, ap.z));
			fixedDesc.setGlobalAxis(NxVec3(axis.x, axis.y, axis.z));
			fixedDesc.jointFlags = 0;
			if (m_Desc.m_bCollisionBetweenObj)
				fixedDesc.jointFlags |= NX_JF_COLLISION_ENABLED;

		    fixedDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
			fixedDesc.swingLimit.value = m_Desc.m_SwingLimit*LYNXD2R;

			fixedDesc.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
			fixedDesc.twistLimit.low.value = m_Desc.m_TwistLimitLow*LYNXD2R;
			fixedDesc.twistLimit.high.value = m_Desc.m_TwistLimitHigh*LYNXD2R;
			
			m_lpJoint = ((CPhysicsWorld*)m_lpWorld)->GetlpNxScene()->createJoint(fixedDesc);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CConeJoint::vAttach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2) 
		{
			LynxEngine::Math::CVector3 Pos1, Pos2, Center;
			LynxEngine::Math::CVector3 Axis;
			if (lpo2)
			{
				lpo1->vGetPosition(&Pos1);
				lpo2->vGetPosition(&Pos2);
				Center = (Pos1 + Pos2) * 0.5f;
				Center = Pos2;

				lpo1->vGetTransformMatrix().GetRight(Axis);
				//Axis.x = 0.0f;
				//Axis.y = 1.0f;
				//Axis.z = 0.0f;				
				
				CConeJoint::Attach(lpo1, lpo2, Center, Axis);
			}
			else
			{
				CConeJoint::Attach(lpo1, lpo2, Center, Axis);
			}		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CConeJoint::vSetAnchorPosition(LynxEngine::Math::CVector3& pos)
		{
			LynxEngine::Math::CVector3 Pos1, Pos2, Center;
			LynxEngine::Math::CVector3 Pivot1, Pivot2;
			
			m_AttchedObj[0]->vGetPosition(&Pos1);
			m_AttchedObj[1]->vGetPosition(&Pos2);
			
			Pivot1.x = (pos.x - Pos1.x);
			Pivot1.y = (pos.y - Pos1.y);
			Pivot1.z = (pos.z - Pos1.z);
			
			Pivot2.x = (pos.x - Pos2.x);
			Pivot2.y = (pos.y - Pos2.y);
			Pivot2.z = (pos.z - Pos2.z);
			
			CConeJoint::Attach(m_AttchedObj[0], m_AttchedObj[1], pos, Pivot2);					
		}		
	}
}

#endif