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

#ifndef __LYNXODEPHYSICSOBJ_H__
#define __LYNXODEPHYSICSOBJ_H__

#include <LynxCore.h>
#include <ODE/ODE.h>
#include <PhysicsSystem/LynxPhysicsObj.h>
#include <LynxODEPhysicsGeometry.h>

namespace LynxPhysics 
{
	namespace ODE 
	{
		class CPhysicsObj : public  LynxEngine::PhysicsSystem::CPhysicsObj
		{	
			DECLARE_CLASS(CPhysicsObj);
		protected:
			dMass								m_Mass;
			dBodyID 							m_Body;					
		public:			
			CPhysicsObj(LynxEngine::PhysicsSystem::CPhysicsWorld* const lpw);
			virtual ~CPhysicsObj();

			LYNXINLINE dBodyID&					GetBody() {return m_Body; };
			virtual LYNXBOOL					vCreate();
			virtual void						vDestory();			
			virtual LYNXINLINE void				vSetMass(LYNXREAL mass) {m_MassValue = mass; dMassAdjust(&m_Mass, m_MassValue); dBodySetMass(m_Body, &m_Mass); };			
			virtual void						vSetMass(MASSTYPE t, LYNXREAL density, LYNXREAL *lpdata);			

			virtual void						vEnable(LYNXBOOL b) { b ? dBodyEnable(m_Body) : dBodyDisable(m_Body); };
			virtual void						vAddPhyGeo(LynxEngine::PhysicsSystem::CPhysicsGeometry* const lpg);			
			virtual void						vRemovePhyGeo(LynxEngine::PhysicsSystem::CPhysicsGeometry* const lpg);
			virtual void						vRemovePhyGeo(int i);
			virtual void						vRemoveAllPhyGeos();
			
			//virtual void						vSetGeometry(CPhysicsGeometry *lpg);
			//void								AddGeometry(CPhysicsGeometry *lpg);
			//virtual LYNXBOOL					vCreatePhyGeometry(CPhysicsSpace* s, CPhysicsGeometry::TYPE, LYNXREAL *lpdata);
			//virtual void						vDestoryPhyGeometry();

			LYNXINLINE virtual void				vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) {dBodySetPosition(m_Body, x, y, z); };
			LYNXINLINE virtual void				vSetPosition(LPLYNXVECTOR3D lppos) {dBodySetPosition(m_Body, lppos->s.x, lppos->s.y, lppos->s.z); };
			LYNXINLINE virtual void				vGetPosition(LPLYNXVECTOR3D lppos) { lynxMemCpy(lppos, dBodyGetPosition(m_Body), sizeof(LYNXVECTOR3D)); };
			virtual void						vSetTransformMatrix(const LYNXMATRIX& m);
			virtual CMatrix4&					vGetTransformMatrix();		

			LYNXINLINE virtual void				vSetForce(LYNXREAL x, LYNXREAL y, LYNXREAL z) {dBodySetForce(m_Body, x, y, z); };
			LYNXINLINE virtual void				vSetForce(LPLYNXVECTOR3D lpforce) {dBodySetForce(m_Body, lpforce->s.x, lpforce->s.y, lpforce->s.z); };
			LYNXINLINE virtual void				vAddForce(LYNXREAL x, LYNXREAL y, LYNXREAL z) {dBodyAddForce(m_Body, x, y, z); };
			LYNXINLINE virtual void				vAddForce(LPLYNXVECTOR3D lpforce) {dBodyAddForce(m_Body, lpforce->s.x, lpforce->s.y, lpforce->s.z); };

			LYNXINLINE virtual void				vSetTorque(LYNXREAL x, LYNXREAL y, LYNXREAL z) {dBodySetTorque(m_Body, x, y, z); };
			LYNXINLINE virtual void				vSetTorque(LPLYNXVECTOR3D lpforce) {dBodySetTorque(m_Body, lpforce->s.x, lpforce->s.y, lpforce->s.z); };
			LYNXINLINE virtual void				vAddTorque(LYNXREAL x, LYNXREAL y, LYNXREAL z) {dBodyAddTorque(m_Body, x, y, z); };
			LYNXINLINE virtual void				vAddTorque(LPLYNXVECTOR3D lpforce) {dBodyAddTorque(m_Body, lpforce->s.x, lpforce->s.y, lpforce->s.z); };

			LYNXINLINE virtual void				vReset() {dBodySetLinearVel(m_Body, 0.0f, 0.0f, 0.0f); dBodySetAngularVel(m_Body, 0.0f, 0.0f, 0.0f); dBodySetForce(m_Body, 0.0f, 0.0f, 0.0f); dBodySetTorque(m_Body, 0.0f, 0.0f, 0.0f); };
		};	
	}
}

#endif