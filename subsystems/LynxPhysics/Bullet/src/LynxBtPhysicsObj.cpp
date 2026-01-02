#include <LynxEngine_PCH.h>
#include <LynxBtPhysicsObj.h>
#include <LynxBtPhysicsWorld.h>
#include <LynxBtPhysicsSpace.h>
#include <LynxBtPhysicsShape.h>

namespace LynxPhysics 
{
	namespace Bullet 
	{
		void BulletToMatrix4(LynxEngine::Math::CMatrix4& m, const btTransform& b)
		{
			m.m11 = (float)b.getBasis().getColumn(0).getX(); m.m12 = (float)b.getBasis().getColumn(0).getY(); m.m13 = (float)b.getBasis().getColumn(0).getZ(); m.m14 = 0.0f;
			m.m21 = (float)b.getBasis().getColumn(1).getX(); m.m22 = (float)b.getBasis().getColumn(1).getY(); m.m23 = (float)b.getBasis().getColumn(1).getZ(); m.m24 = 0.0f;
			m.m31 = (float)b.getBasis().getColumn(2).getX(); m.m32 =(float) b.getBasis().getColumn(2).getY(); m.m33 = (float)b.getBasis().getColumn(2).getZ(); m.m34 = 0.0f;
			m.m41 = (float)b.getOrigin().getX();		  m.m42 = (float)b.getOrigin().getY();			m.m43 =(float) b.getOrigin().getZ();		  m.m44 = 1.0f;
		}
		void  Matrix4ToBullet(btTransform& b, const LynxEngine::Math::CMatrix4& m)
		{
			//b.setBasis(btMatrix3x3(m.m11, m.m12, m.m13, m.m21, m.m22, m.m23, m.m31, m.m32, m.m33));
			b.setBasis(btMatrix3x3(m.m11, m.m21, m.m31, m.m12, m.m22, m.m32, m.m13, m.m23, m.m33));
			b.setOrigin(btVector3(m.m41, m.m42, m.m43));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsObj::CPhysicsObj(LynxEngine::PhysicsSystem::CPhysicsWorld *lpw)
		: LynxEngine::PhysicsSystem::CPhysicsObj(lpw)
		{			
			m_Body = NULL;
			m_lpCompoundShape = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsObj::~CPhysicsObj(void)
		{
			vDestory();
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vDestory()
		{			
			if (m_Body)
			{
				LYNXDEL(m_Body->getMotionState());		
				((CPhysicsWorld*)m_lpWorld)->GetDynamicsWorld()->removeCollisionObject(m_Body);				
				LYNXDEL(m_Body);
				m_Body = NULL;
			}			

			if (m_lpCompoundShape)
			{
				for (int i=0; i<m_lpCompoundShape->getNumChildShapes(); i++)
					m_lpCompoundShape->removeChildShapeByIndex(i);				
				LYNXDEL(m_lpCompoundShape);
				m_lpCompoundShape = NULL;
			}

			LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsShape*>::CIterator Shape;
			for (Shape = m_ShapeList.begin(); Shape != m_ShapeList.end(); ++Shape)
			{
				// Set lpObj to NULL so the destructor wouldn't remove this shape from physics object.
				(*Shape)->SetlpObj(NULL);
				m_lpWorld->GetlpSpace(0)->vDeleteShape(*Shape);
			}
			m_ShapeList.clear();	

			if (m_BodyDesc)
			{
				LYNXDEL(m_BodyDesc);
				m_BodyDesc = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsObj::vCreate(LynxEngine::PhysicsSystem::CPhysicsObj::TYPE t, LynxEngine::PhysicsSystem::CPhysicsShape* g)
		{
			m_PhysicsObjType = t;
			switch (t)
			{
				case LynxEngine::PhysicsSystem::CPhysicsObj::STATIC:
					return CreateStatic(dynamic_cast<CPhysicsShape*>(g));
					break;

				case LynxEngine::PhysicsSystem::CPhysicsObj::DYNAMIC:
					return CreateDynamic(dynamic_cast<CPhysicsShape*>(g));
					break;
			}
			return LYNX_FALSE;
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsObj::CreateStatic(CPhysicsShape* lpg)
		{
			btScalar Mass(0.0f);					
			btCollisionShape* lpShape;
			
			lpShape = NULL;
			if (lpg)
			{
				if (lpg->GetlpObj())
				{
					lpg->GetlpObj()->GetShapeList().remove(lpg);
					((CPhysicsShape*)lpg)->DestoryPhyObj();
				}
				lpShape = ((Bullet::CPhysicsShape*)lpg)->GetbtShape();
			}			
			
			btTransform TM;
			TM.setIdentity();
			TM.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* MotionState = LYNXGLOBAL_NEW btDefaultMotionState(TM);
			
			m_BodyDesc = LYNXGLOBAL_NEW btRigidBody::btRigidBodyConstructionInfo(Mass, MotionState, lpShape, btVector3(0.0f, 0.0f, 0.0f));
			m_Body = LYNXGLOBAL_NEW btRigidBody(*m_BodyDesc);
			
			//add the body to the dynamics world
			((CPhysicsWorld*)m_lpWorld)->GetDynamicsWorld()->addRigidBody(m_Body);
			m_Body->setContactProcessingThreshold(m_lpWorld->GetlpPhysicsSystem()->GetCollisionThreshold());
					
			if (lpg && lpg->GetlpObj() != this)
			{					
				m_ShapeList.push_back(lpg);
				lpg->SetlpObj(this);		
			}

			UpdateProperties();
			
			//if (lpg)
			//	vAddShape(lpg);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhysicsObj::CreateDynamic(CPhysicsShape* lpg)
		{
			btScalar Mass(m_MassValue);					
			btCollisionShape* lpShape;
			
			lpShape = NULL;			
			if (lpg)
			{
				if (lpg->GetlpObj())
				{
					lpg->GetlpObj()->GetShapeList().remove(lpg);
					((CPhysicsShape*)lpg)->DestoryPhyObj();
				}
				lpShape = ((Bullet::CPhysicsShape*)lpg)->GetbtShape();
			}
			
			btTransform TM;
			TM.setIdentity();
			TM.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
			
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* MotionState = LYNXGLOBAL_NEW btDefaultMotionState(TM);
			
			btVector3 LocalInertia(0,0,0);
			if (lpShape)
				lpShape->calculateLocalInertia(Mass, LocalInertia);
			else
			{
				btSphereShape Sphere(1.5f);
				Sphere.calculateLocalInertia(Mass, LocalInertia);				
			}
			
			m_BodyDesc = LYNXGLOBAL_NEW btRigidBody::btRigidBodyConstructionInfo(Mass, MotionState, lpShape, LocalInertia);
			m_Body = LYNXGLOBAL_NEW btRigidBody(*m_BodyDesc);
			
			//add the body to the dynamics world
			((CPhysicsWorld*)m_lpWorld)->GetDynamicsWorld()->addRigidBody(m_Body);
			m_Body->setContactProcessingThreshold(m_lpWorld->GetlpPhysicsSystem()->GetCollisionThreshold());
			
			if (lpg && lpg->GetlpObj() != this)
			{					
				m_ShapeList.push_back(lpg);
				lpg->SetlpObj(this);		
			}

			UpdateAllShapes();
					
			//UpdateProperties();

			//if (lpg)
			//	vAddShape(lpg);

			return LYNX_TRUE;
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vSetMass(LYNXREAL mass)
		{
			if (m_PhysicsObjType == LynxEngine::PhysicsSystem::CPhysicsObj::DYNAMIC)
			{
				m_MassValue = mass;

				btScalar Mass;			
				btCollisionShape* lpShape = m_Body->getCollisionShape();

				if (m_PhysicsObjType == LynxEngine::PhysicsSystem::CPhysicsObj::STATIC)
					Mass = 0.0f;
				else
					Mass = m_MassValue;

				btVector3 LocalInertia(0, 0, 0);
				//if (lpShape && m_PhysicsObjType == LynxEngine::PhysicsSystem::CPhysicsObj::DYNAMIC)
				if (m_lpCompoundShape && !m_lpCompoundShape->isConcave() && m_lpCompoundShape->getNumChildShapes())
					m_lpCompoundShape->calculateLocalInertia(Mass, LocalInertia);			
				else
				{
					btSphereShape Sphere(1.5f);
					Sphere.calculateLocalInertia(Mass, LocalInertia);				
				}

				m_Body->setMassProps(Mass, LocalInertia);			
				m_Body->updateInertiaTensor();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vSetMass(MASSTYPE t, LYNXREAL density, LYNXREAL *lpdata)
		{				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vAddShape(LynxEngine::PhysicsSystem::CPhysicsShape *lpg)
		{
			LYNXVECTOR3D Pos;

			LYNX_ASSERT(lpg->GetlpObj() != this);

			// Remove old physics obj
			if (lpg->GetlpObj())
			{
				lpg->GetlpObj()->GetShapeList().remove(lpg);
				((CPhysicsShape*)lpg)->DestoryPhyObj();
			}

			m_ShapeList.push_back(lpg);
			lpg->SetlpObj(this);			

			UpdateAllShapes();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::UpdateAllShapes()
		{			
			btCollisionShape* lpShape = NULL;			
			btTransform LocalTransform;

			if (!m_lpCompoundShape)
			{
				m_lpCompoundShape = LYNXGLOBAL_NEW btCompoundShape;			
			}
			else
			{
				for (int i=0; i<m_lpCompoundShape->getNumChildShapes(); i++)
					m_lpCompoundShape->removeChildShapeByIndex(i);
			}
			// remove bullet object first 
			//((CPhysicsWorld*)m_lpWorld)->GetDynamicsWorld()->removeRigidBody(m_Body);			
			static_cast<CPhysicsWorld*>(m_lpWorld)->GetDynamicsWorld()->removeRigidBody(m_Body);			
			
			//if (m_PhysicsObjType == LynxEngine::PhysicsSystem::CPhysicsObj::DYNAMIC)
			{
				LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsShape*>::CIterator Shape;
				for (Shape = m_ShapeList.begin(); Shape != m_ShapeList.end(); ++Shape)
				{
					lpShape = ((Bullet::CPhysicsShape*)(*Shape))->GetbtShape();

					Matrix4ToBullet(LocalTransform, (*Shape)->vGetLocalTransformMatrix());				
					m_lpCompoundShape->addChildShape(LocalTransform, lpShape);
				}			
				m_Body->setCollisionShape(m_lpCompoundShape);

				btScalar Mass;			
				if (m_PhysicsObjType == LynxEngine::PhysicsSystem::CPhysicsObj::STATIC)
					Mass = 0.0f;
				else
					Mass = m_MassValue;

				btVector3 LocalInertia(0, 0, 0);
				//if (lpShape && m_PhysicsObjType == LynxEngine::PhysicsSystem::CPhysicsObj::DYNAMIC)
				if (m_lpCompoundShape && !m_lpCompoundShape->isConcave() && m_lpCompoundShape->getNumChildShapes())
					m_lpCompoundShape->calculateLocalInertia(Mass, LocalInertia);			
				else
				{
					btSphereShape Sphere(1.5f);
					Sphere.calculateLocalInertia(Mass, LocalInertia);				
				}
				
				m_Body->setMassProps(Mass, LocalInertia);			
				//m_Body->updateInertiaTensor();
				//vReset();
			}			

			// add bullet object to world again
			((CPhysicsWorld*)m_lpWorld)->GetDynamicsWorld()->addRigidBody(m_Body);
			m_Body->setContactProcessingThreshold(m_lpWorld->GetlpPhysicsSystem()->GetCollisionThreshold());			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vRemoveShape(LynxEngine::PhysicsSystem::CPhysicsShape *lpg)
		{			
			m_ShapeList.remove(lpg);			
			UpdateAllShapes();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vRemoveShape(int i)
		{
			LynxEngine::PhysicsSystem::CPhysicsShape *lpG = GetlpShape(i);
			if (lpG)
				vRemoveShape(lpG);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vRemoveAllShapes()
		{
			LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsShape*>::CIterator Geo = m_ShapeList.begin();
			while (Geo != m_ShapeList.end())
			{
				LynxEngine::PhysicsSystem::CPhysicsShape* lpGeo = *Geo;
				++Geo;
				vRemoveShape(lpGeo);				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vSetKinematic(LYNXBOOL b)
		{
			LynxEngine::PhysicsSystem::CPhysicsObj::vSetKinematic(b);
			if (b)
			{
				m_Body->setCollisionFlags(m_Body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
				vNeverSleep(LYNX_TRUE);
			}
			else
			{
				m_Body->setCollisionFlags(m_Body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
				vNeverSleep(LYNX_FALSE);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vNeverSleep(LYNXBOOL b)
		{
			LynxEngine::PhysicsSystem::CPhysicsObj::vNeverSleep(b);
			if (b)
			{
				m_Body->setActivationState(DISABLE_DEACTIVATION);
			}
			else
			{
				m_Body->setActivationState(ACTIVE_TAG);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z)
		{
			btTransform Trans;
			Trans = m_Body->getCenterOfMassTransform();
			Trans.setOrigin(btVector3(x, y, z));
			m_Body->setCenterOfMassTransform(Trans);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vSetPosition(LPLYNXVECTOR3D lppos)
		{
			btTransform Trans;
			Trans = m_Body->getCenterOfMassTransform();
			Trans.setOrigin(btVector3(lppos->x, lppos->y, lppos->z));
			m_Body->setCenterOfMassTransform(Trans);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vGetPosition(LPLYNXVECTOR3D lppos)
		{
			const btVector3& Pos = m_Body->getCenterOfMassPosition();
			lppos->x = Pos.getX();
			lppos->y = Pos.getY();
			lppos->z = Pos.getZ();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vSetTransformMatrix(const CMatrix4& m) 
		{ 	
			btTransform Trans;
			Matrix4ToBullet(Trans, m);
			m_Body->setCenterOfMassTransform(Trans);
			//m_Body->getMotionState()->setWorldTransform(Trans);
			
			/*for (LynxEngine::CList<LynxEngine::PhysicsSystem::CPhysicsGeometry*>::CIterator Geo = m_GeometryList.begin(); Geo != m_GeometryList.end(); ++Geo)
			{				
				(*Geo)->vSetTransformMatrix(m); 				
			}*/			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMatrix4& CPhysicsObj::vGetTransformMatrix()
		{
			btTransform Trans;
			//m_Body->getMotionState()->getWorldTransform(Trans);
			Trans = m_Body->getCenterOfMassTransform();
			BulletToMatrix4(m_M, Trans);

			return m_M;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhysicsObj::vReset()
		{
			if (m_Body)
			{
				btDynamicsWorld* lpWorld = ((Bullet::CPhysicsWorld*)m_lpWorld)->GetDynamicsWorld();
				if (m_Body->getMotionState())
				{
					//btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
					//myMotionState->m_graphicsWorldTrans = myMotionState->m_startWorldTrans;
					//body->setCenterOfMassTransform( myMotionState->m_graphicsWorldTrans );
					//colObj->setInterpolationWorldTransform( myMotionState->m_startWorldTrans );
					m_Body->forceActivationState(ACTIVE_TAG);
					m_Body->activate();
					m_Body->setDeactivationTime(0);
					//colObj->setActivationState(WANTS_DEACTIVATION);
				}
				//removed cached contact points (this is not necessary if all objects have been removed from the dynamics world)
				if (lpWorld->getBroadphase()->getOverlappingPairCache())
					lpWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_Body->getBroadphaseHandle(), lpWorld->getDispatcher());

				if (m_Body)
				{
					m_Body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
					m_Body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
				}

				UpdateProperties();
			}
		}		
	}
}