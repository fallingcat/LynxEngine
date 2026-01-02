#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsWorld.h>
#include <LynxBtPhysicsObj.h>
#include <LynxBtBallPhysicsJoint.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBallJoint::CBallJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw)
		: CJoint(lpw)
		{	
			m_PhysicsJointType = BALL;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBallJoint::~CBallJoint(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBallJoint::vCreate(const LynxEngine::CCreationDesc* desc)
		{			
			lynxMemCpy(&m_Desc, desc, sizeof(LynxEngine::PhysicsSystem::CBallJointDesc));
			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBallJoint::Attach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2, LynxEngine::Math::CVector3& p1, LynxEngine::Math::CVector3& p2) 
		{
			CJoint::vAttach(lpo1, lpo2);
			
			vDestory();			
			btVector3 Pivot1(p1.x, p1.y, p1.z), Pivot2(p2.x, p2.y, p2.z);
			if (lpo2)
			{
				m_lpConstraint = LYNXGLOBAL_NEW btPoint2PointConstraint(*((CPhysicsObj*)lpo1)->GetbtRigidBody(), *((CPhysicsObj*)lpo2)->GetbtRigidBody(), Pivot1, Pivot2);
			}
			else
			{
				m_lpConstraint = LYNXGLOBAL_NEW btPoint2PointConstraint(*((CPhysicsObj*)lpo1)->GetbtRigidBody(), Pivot1);
			}			
			((CPhysicsWorld*)m_lpWorld)->GetDynamicsWorld()->addConstraint(m_lpConstraint, !m_Desc.m_bCollisionBetweenObj);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBallJoint::vAttach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2) 
		{
			LynxEngine::Math::CVector3 Pos1, Pos2, Center;
			LynxEngine::Math::CVector3 Pivot1, Pivot2;
			if (lpo2)
			{
				lpo1->vGetPosition(&Pos1);
				lpo2->vGetPosition(&Pos2);
				
				LynxEngine::Math::CMatrix4 TM = lpo1->vGetTransformMatrix();

				float m41 = -LYNX_DOT_PRODUCT((TM.GetRight()),	(Pos1));
				float m42 = -LYNX_DOT_PRODUCT((TM.GetUp()),		(Pos1));
				float m43 = -LYNX_DOT_PRODUCT((TM.GetLook()),	(Pos1));		

				LynxEngine::Math::CMatrix4 ParentSpaceM;		
				LYNX_MATRIX(ParentSpaceM,
		  			TM.GetRight().x, TM.GetUp().x, TM.GetLook().x, LYNX_REAL(0),
					TM.GetRight().y, TM.GetUp().y, TM.GetLook().y, LYNX_REAL(0),
					TM.GetRight().z, TM.GetUp().z, TM.GetLook().z, LYNX_REAL(0),
					m41,			 m42,		   m43,			   LYNX_REAL(1));			

				//LynxEngine::Math::InverseMatrix(ParentSpaceM, TM);
				lynxPointXMatrix(Pivot1, Pos2, ParentSpaceM);				

				Pivot2.x = 0.0f;
				Pivot2.y = 0.0f;
				Pivot2.z = 0.0f;
				
				Attach(lpo1, lpo2, Pivot1, Pivot2);
			}
			else
			{
				Pivot1.x = 0.0f;
				Pivot1.y = 1.0f;
				Pivot1.z = 0.0f;
				Attach(lpo1, lpo2, Pivot1, Pivot2);
			}		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBallJoint::vSetAnchorPosition(LynxEngine::Math::CVector3& pos)
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
			
			Attach(m_AttchedObj[0], m_AttchedObj[1], Pivot1, Pivot2);					
		}		
	}
}
