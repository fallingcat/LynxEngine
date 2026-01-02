//###########################################################################
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#ifndef __LYNXBTPHYSICSOBJ_H__
#define __LYNXBTPHYSICSOBJ_H__

#include <LynxEngineClasses.h>
#include <btBulletDynamicsCommon.h>
#include <PhysicsSystem/LynxPhysicsObj.h>

namespace LynxPhysics 
{	
	namespace Bullet 
	{
		class CPhysicsShape;
		class LYNXENGCLASS CPhysicsObj : public LynxEngine::PhysicsSystem::CPhysicsObj
		{	
			LYNX_DECLARE_CLASS(CPhysicsObj);
		protected:
			btRigidBody* 						m_Body;
			btRigidBody::btRigidBodyConstructionInfo* m_BodyDesc;
			btCompoundShape*					m_lpCompoundShape;
		public:			
			CPhysicsObj(LynxEngine::PhysicsSystem::CPhysicsWorld *lpw);
			virtual ~CPhysicsObj();


			LYNXFORCEINLINE btRigidBody*		GetbtRigidBody() {return m_Body; };
			virtual LYNXBOOL					vCreate(LynxEngine::PhysicsSystem::CPhysicsObj::TYPE t, LynxEngine::PhysicsSystem::CPhysicsShape* g = NULL);
			LYNXBOOL							CreateStatic(CPhysicsShape* lpg);
			LYNXBOOL							CreateDynamic(CPhysicsShape* lpg);
			virtual void						vDestory();		

			virtual void						vSetMass(LYNXREAL mass);			
			virtual void						vSetMass(MASSTYPE t, LYNXREAL density, LYNXREAL *lpdata);			

			virtual void						vSetKinematic(LYNXBOOL b);

			virtual void						vNeverSleep(LYNXBOOL b);
			LYNXFORCEINLINE LYNXBOOL			vIsSleeping() const {return ((m_Body->getActivationState() == ISLAND_SLEEPING) ? LYNX_TRUE : LYNX_FALSE); };
			LYNXFORCEINLINE void				vSleep(LYNXBOOL b) 
			{
				if (!m_bNeverSleep && b)
					m_Body->setActivationState(ISLAND_SLEEPING);
			}

			LYNXFORCEINLINE LYNXBOOL			vIsActive() const {return m_Body->isActive(); }; 
			LYNXFORCEINLINE void				vActivate(LYNXBOOL b) 
			{
				if (b)
					m_Body->setActivationState(ACTIVE_TAG);
				else
					m_Body->setActivationState(DISABLE_SIMULATION);
			}

			virtual void						vAddShape(LynxEngine::PhysicsSystem::CPhysicsShape* const lpg);			
			virtual void						vRemoveShape(LynxEngine::PhysicsSystem::CPhysicsShape* const lpg);
			virtual void						vRemoveShape(int i);
			virtual void						vRemoveAllShapes();			
			void								UpdateAllShapes();

			virtual void						vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z);
			virtual void						vSetPosition(LPLYNXVECTOR3D lppos);			
			virtual void						vGetPosition(LPLYNXVECTOR3D lppos);
			virtual void						vSetTransformMatrix(const CMatrix4& m);
			virtual CMatrix4&					vGetTransformMatrix();

			LYNXFORCEINLINE void				vSetForce(LYNXREAL x, LYNXREAL y, LYNXREAL z) {m_Body->clearForces(); m_Body->applyCentralForce(btVector3(x, y, z)); };
			LYNXFORCEINLINE void				vSetForce(LPLYNXVECTOR3D lpforce) {m_Body->clearForces(); m_Body->applyCentralForce(btVector3(lpforce->x, lpforce->y, lpforce->z)); };
			LYNXFORCEINLINE void				vAddForce(LYNXREAL x, LYNXREAL y, LYNXREAL z) {m_Body->applyCentralForce(btVector3(x, y, z)); };
			LYNXFORCEINLINE void				vAddForce(LPLYNXVECTOR3D lpforce) {m_Body->applyCentralForce(btVector3(lpforce->x, lpforce->y, lpforce->z)); };
			LYNXFORCEINLINE void				vAddForce(LPLYNXVECTOR3D lpforce, LPLYNXVECTOR3D lppos) {m_Body->applyForce(btVector3(lpforce->x, lpforce->y, lpforce->z), btVector3(lppos->x, lppos->y, lppos->z)); };

			virtual void						vSetTorque(LYNXREAL x, LYNXREAL y, LYNXREAL z) {};
			virtual void						vSetTorque(LPLYNXVECTOR3D lpforce) {};
			virtual void						vAddTorque(LYNXREAL x, LYNXREAL y, LYNXREAL z) {};
			virtual void						vAddTorque(LPLYNXVECTOR3D lpforce) {};

			LYNXFORCEINLINE virtual void		vGetLinearVelocity(LynxEngine::Math::CVector3& v) {btVector3 Vel = m_Body->getLinearVelocity(); v.x = Vel.getX(); v.y = Vel.getY(); v.z = Vel.getZ();};
			LYNXFORCEINLINE virtual void		vSetLinearVelocity(LynxEngine::Math::CVector3& v) {m_Body->setLinearVelocity(btVector3(v.x, v.y, v.z)); };

			LYNXFORCEINLINE virtual void		vGetAngularVelocity(LynxEngine::Math::CVector3& v) {btVector3 Vel = m_Body->getAngularVelocity(); v.x = Vel.getX(); v.y = Vel.getY(); v.z = Vel.getZ();};
			LYNXFORCEINLINE virtual void		vSetAngularVelocity(LynxEngine::Math::CVector3& v) {m_Body->setAngularVelocity(btVector3(v.x, v.y, v.z)); };

			virtual void						vReset();
		protected:			
		};	
	}
}

#endif