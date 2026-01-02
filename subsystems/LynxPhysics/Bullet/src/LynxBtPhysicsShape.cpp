#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsWorld.h>
#include <LynxBtPhysicsShape.h>
#include <LynxBtPhysicsObj.h>
#include <LynxBtPhysicsSpace.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		void  Matrix4ToBullet(btTransform& b, const LynxEngine::Math::CMatrix4& m);
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsShape::CPhysicsShape(LynxEngine::PhysicsSystem::CPhysicsSpace* lps)
		: LynxEngine::PhysicsSystem::CPhysicsShape(lps)
		{	
			m_lpShape = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsShape::~CPhysicsShape(void)
		{
			vDestory();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CPhysicsShape::vDestory() 
		{ 
			if (m_lpObj)
			{
				m_lpObj->vRemoveShape(this);
				m_lpObj = NULL;
			}
			if (m_lpShape)
			{
				LYNXDEL(m_lpShape);
				m_lpShape = NULL;				
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CPhysicsShape::CreateStaticPhyObj() 
		{
			m_lpObj = m_lpSpace->GetlpWorld()->vCreateObj(LynxEngine::PhysicsSystem::CPhysicsObj::STATIC, this);			
			m_lpObj->SetName(GetName() + LynxEngine::CString(_T(".PhyObj")));
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CPhysicsShape::DestoryPhyObj() 
		{
			if (m_lpObj)
			{
				m_lpObj->GetlpWorld()->vDeleteObj(m_lpObj);
				m_lpObj = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CPhysicsShape::Resize() 
		{	
			Bullet::CPhysicsObj* lpObj = dynamic_cast<Bullet::CPhysicsObj*>(m_lpObj);
			Bullet::CPhysicsWorld* lpWorld = dynamic_cast<Bullet::CPhysicsWorld*>(lpObj->GetlpWorld());

			lpObj->UpdateAllShapes();

			/*
			// remove bullet object first 			
			lpWorld->GetDynamicsWorld()->removeRigidBody(lpObj->GetbtRigidBody());

			lpObj->GetbtRigidBody()->setCollisionShape(m_lpShape);

			// recalculate mass and inertia.
			btScalar Mass;			
			if (lpObj->GetPhysicsObjType() == LynxEngine::PhysicsSystem::CPhysicsObj::DYNAMIC)
			{
				if (lpObj->GetPhysicsObjType() == LynxEngine::PhysicsSystem::CPhysicsObj::STATIC)
					Mass = 0.0f;
				else
					Mass = lpObj->GetMassValue();

				btVector3 LocalInertia(0, 0, 0);
				if (m_lpShape)
					m_lpShape->calculateLocalInertia(Mass, LocalInertia);			
				
				lpObj->GetbtRigidBody()->setMassProps(Mass, LocalInertia);			
				//m_Body->updateInertiaTensor();
				//vReset();
			}

			// add bullet object to world again
			lpWorld->GetDynamicsWorld()->addRigidBody(lpObj->GetbtRigidBody());
			lpObj->GetbtRigidBody()->setContactProcessingThreshold(lpWorld->GetlpPhysicsSystem()->GetCollisionThreshold());			
			*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CPhysicsShape::RecreatePhysicsObj() 
		{	
			LynxEngine::CString Name = m_lpObj->GetName();
			LynxEngine::PhysicsSystem::CPhysicsWorld* lpWorld = m_lpObj->GetlpWorld();
			LynxEngine::PhysicsSystem::CPhysicsObj::TYPE Type = m_lpObj->GetPhysicsObjType();
			// First, remove self from old physics obj
			m_lpObj->GetShapeList().remove(this);
			// Then destroy old physics obj
			DestoryPhyObj();

			// Now, create the new physics obj
			m_lpObj = lpWorld->vCreateObj(Type, this);
			m_lpObj->SetName(Name);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsShape::vSetTransformMatrix(const CMatrix4& m) 
		{
			if (m_lpObj)
			{
				m_lpObj->vSetTransformMatrix(m);				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMatrix4& CPhysicsShape::vGetTransformMatrix()
		{			
			if (m_lpObj)
			{
				m_M = m_lpObj->vGetTransformMatrix();
			}

			m_LocalXTransformMatrix = m_LocalM * m_M;

			return (m_LocalXTransformMatrix);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsShape::vSetLocalTransformMatrix(const CMatrix4& m) 
		{ 			
			m_LocalM = m;
			Bullet::CPhysicsObj* lpObj = dynamic_cast<Bullet::CPhysicsObj*>(m_lpObj);
			lpObj->UpdateAllShapes();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMatrix4& CPhysicsShape::vGetLocalTransformMatrix()
		{
			//NxMat34 M = m_lpShape->getLocalPose();
			//M.getColumnMajor44(m_LocalM.f);

			return m_LocalM;
		}
	}
}