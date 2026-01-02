#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsWorld.h>
#include <LynxBtPhysicsObj.h>
#include <LynxBtConePhysicsJoint.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{	
		void  Matrix4ToBullet(btTransform& b, const LynxEngine::Math::CMatrix4& m);
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CConeJoint::CConeJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw)
		: CJoint(lpw)
		{	
			m_PhysicsJointType = BALL;			
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
		void CConeJoint::Attach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2, LynxEngine::Math::CVector3& p1, LynxEngine::Math::CVector3& p2) 
		{
			CJoint::vAttach(lpo1, lpo2);
			
			vDestory();			

			btTransform FrameA, FrameB, OffsetB;

			FrameA.setIdentity();
			//Matrix4ToBullet(FrameA, lpo1->vGetTransformMatrix());			
			//FrameA.getBasis().setEulerZYX(0, 0, SIMD_PI_2);
			
			FrameB.setIdentity();
			Matrix4ToBullet(FrameB, lpo2->vGetTransformMatrix());
			FrameB.setOrigin(btVector3(0, 0, 0));

			OffsetB.setIdentity();
			OffsetB.getBasis().setEulerZYX(0, 0, btScalar(-90.0f*LYNXD2R));			
			FrameB = FrameB * OffsetB;

			btVector3 Pivot1(p1.x, p1.y, p1.z), Pivot2(p2.x, p2.y, p2.z);
			if (lpo2)
			{
				FrameA.setOrigin(Pivot1);
				FrameB.setOrigin(Pivot2);
				m_lpConstraint = LYNXGLOBAL_NEW btConeTwistConstraint(*((CPhysicsObj*)lpo1)->GetbtRigidBody(), *((CPhysicsObj*)lpo2)->GetbtRigidBody(), FrameA, FrameB);//, true);
			}
			else
			{
				FrameA.setOrigin(Pivot1);
				m_lpConstraint = LYNXGLOBAL_NEW btConeTwistConstraint(*((CPhysicsObj*)lpo1)->GetbtRigidBody(), FrameA);
			}
			//((btConeTwistConstraint*)m_lpConstraint)->setAngularOnly(true);

			((btConeTwistConstraint*)m_lpConstraint)->setLimit(btScalar(-0.001f), btScalar(0.001f), btScalar(0.0f*LYNXD2R));

			//((btGeneric6DofConstraint*)m_lpConstraint)->setAngularLowerLimit(btVector3(-SIMD_EPSILON,-SIMD_EPSILON,-SIMD_EPSILON));
			//((btGeneric6DofConstraint*)m_lpConstraint)->setAngularUpperLimit(btVector3(SIMD_EPSILON,SIMD_EPSILON,SIMD_EPSILON));
			
			((CPhysicsWorld*)m_lpWorld)->GetDynamicsWorld()->addConstraint(m_lpConstraint, true);//!m_Desc.m_bCollisionBetweenObj);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CConeJoint::vAttach(LynxEngine::PhysicsSystem::CPhysicsObj* lpo1, LynxEngine::PhysicsSystem::CPhysicsObj* lpo2) 
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
				CConeJoint::Attach(lpo1, lpo2, Pivot1, Pivot2);
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
			
			CConeJoint::Attach(m_AttchedObj[0], m_AttchedObj[1], Pivot1, Pivot2);					
		}		
	}
}
