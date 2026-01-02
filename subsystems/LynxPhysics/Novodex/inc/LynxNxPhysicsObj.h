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

#ifndef __LYNXNXPHYSICSOBJ_H__
#define __LYNXNXPHYSICSOBJ_H__

#ifdef __PHYSX__

#include <LynxEngineClasses.h>
#include <NxPhysics.h>
#include <PhysicsSystem/LynxPhysicsObj.h>

namespace LynxPhysics 
{	
	namespace Novodex 
	{
		class CPhysicsShape;
		class LYNXENGCLASS CPhysicsObj : public LynxEngine::PhysicsSystem::CPhysicsObj
		{	
			LYNX_DECLARE_CLASS(CPhysicsObj);
		protected:
			NxActor* 							m_Body;
			NxBodyDesc							m_BodyDesc;
		public:			
			CPhysicsObj(LynxEngine::PhysicsSystem::CPhysicsWorld *lpw);
			virtual ~CPhysicsObj();


			LYNXINLINE NxActor*					GetlpNxActor() {return m_Body; };
			virtual LYNXBOOL					vCreate(LynxEngine::PhysicsSystem::CPhysicsObj::TYPE t, LynxEngine::PhysicsSystem::CPhysicsShape* g = NULL);
			LYNXBOOL							CreateStatic(CPhysicsShape* lpg);
			LYNXBOOL							CreateDynamic(CPhysicsShape* lpg);
			virtual void						vDestory();		

			virtual LYNXINLINE void				vSetMass(LYNXREAL mass) {m_MassValue = mass; m_Body->updateMassFromShapes(0.0f, m_MassValue); m_Body->saveBodyToDesc(m_BodyDesc); };			
			virtual void						vSetMass(MASSTYPE t, LYNXREAL density, LYNXREAL *lpdata);			

			LYNXFORCEINLINE LYNXBOOL			vIsActive() const {return (!m_Body->isSleeping()); };  //TODO 
			LYNXFORCEINLINE void				vActivate(LYNXBOOL b) 
			{
				if (b)
					m_Body->wakeUp();					
				else
					m_Body->putToSleep();
			}

			virtual void						vSetKinematic(LYNXBOOL b);

			LYNXFORCEINLINE LYNXBOOL			vIsSleeping() const {return (m_Body->isSleeping()); };
			LYNXFORCEINLINE void				vSleep(LYNXBOOL b) 
			{
				if (!m_bNeverSleep && b)
					m_Body->putToSleep();
			}

			virtual void						vAddShape(LynxEngine::PhysicsSystem::CPhysicsShape* const lpg);			
			virtual void						vRemoveShape(LynxEngine::PhysicsSystem::CPhysicsShape* const lpg);
			virtual void						vRemoveShape(int i);
			virtual void						vRemoveAllShapes();			

			virtual LYNXINLINE void				vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) { m_Body->setGlobalPosition(NxVec3(x, y, z)); };
			virtual LYNXINLINE void				vSetPosition(LPLYNXVECTOR3D lppos) { m_Body->setGlobalPosition(NxVec3(lppos->x, lppos->y, lppos->z)); };			
			LYNXINLINE virtual void				vGetPosition(LPLYNXVECTOR3D lppos) { NxVec3 Pos = m_Body->getGlobalPosition(); lppos->x = Pos.x; lppos->y = Pos.y; lppos->z = Pos.z;};
			virtual void						vSetTransformMatrix(const CMatrix4& m);
			virtual CMatrix4&					vGetTransformMatrix();

			virtual void						vSetForce(LYNXREAL x, LYNXREAL y, LYNXREAL z) {m_Body->addForce(NxVec3(x, y, z)); };
			virtual void						vSetForce(LPLYNXVECTOR3D lpforce) {m_Body->addForce(NxVec3(lpforce->x, lpforce->y, lpforce->z)); };
			virtual void						vAddForce(LYNXREAL x, LYNXREAL y, LYNXREAL z) {m_Body->addForce(NxVec3(x, y, z)); };
			virtual void						vAddForce(LPLYNXVECTOR3D lpforce) {m_Body->addForce(NxVec3(lpforce->x, lpforce->y, lpforce->z)); };
			virtual void						vAddForce(LPLYNXVECTOR3D lpforce, LPLYNXVECTOR3D lppos) {m_Body->addForceAtPos(NxVec3(lpforce->x, lpforce->y, lpforce->z), m_Body->getGlobalPosition() + NxVec3(lppos->x, lppos->y, lppos->z)); };

			virtual void						vSetTorque(LYNXREAL x, LYNXREAL y, LYNXREAL z) {};
			virtual void						vSetTorque(LPLYNXVECTOR3D lpforce) {};
			virtual void						vAddTorque(LYNXREAL x, LYNXREAL y, LYNXREAL z) {};
			virtual void						vAddTorque(LPLYNXVECTOR3D lpforce) {};

			LYNXFORCEINLINE virtual void		vGetLinearVelocity(LynxEngine::Math::CVector3& v) {};
			LYNXFORCEINLINE virtual void		vSetLinearVelocity(LynxEngine::Math::CVector3& v) {};

			LYNXFORCEINLINE virtual void		vGetAngularVelocity(LynxEngine::Math::CVector3& v) {};
			LYNXFORCEINLINE virtual void		vSetAngularVelocity(LynxEngine::Math::CVector3& v) {};

			virtual void						vReset();			
		};	
	}
}

#endif

#endif