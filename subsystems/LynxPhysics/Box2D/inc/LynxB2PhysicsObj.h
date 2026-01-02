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

#ifndef __LYNXB2PHYSICSOBJ_H__
#define __LYNXB2PHYSICSOBJ_H__

#include <LynxEngineClasses.h>
#include <PhysicsSystem/LynxPhysicsObj.h>
#include <Box2D/Box2D.h>

namespace LynxPhysics 
{	
	namespace Box2D 
	{
		class CPhysicsShape;
		class LYNXENGCLASS CPhysicsObj : public LynxEngine::PhysicsSystem::CPhysicsObj
		{	
			LYNX_DECLARE_CLASS(CPhysicsObj);
		protected:
			b2Body* 							m_Body;
			b2BodyDef							m_BodyDef;
			b2MassData							m_MassData;
		public:			
			CPhysicsObj(LynxEngine::PhysicsSystem::CPhysicsWorld *lpw);
			virtual ~CPhysicsObj();


			LYNXFORCEINLINE b2Body*				Getlpb2Body() {return m_Body; };
			virtual LYNXBOOL					vCreate(LynxEngine::PhysicsSystem::CPhysicsObj::TYPE t, LynxEngine::PhysicsSystem::CPhysicsShape* g = NULL);
			LYNXBOOL							CreateStatic(CPhysicsShape* lpg);
			LYNXBOOL							CreateDynamic(CPhysicsShape* lpg);
			virtual void						vDestory();		

			virtual LYNXFORCEINLINE void		vUpdateMass() {};			
			virtual LYNXFORCEINLINE void		vSetMass(LYNXREAL mass) {m_MassValue = mass; m_MassData.mass = mass; m_Body->SetMassData(&m_MassData); };			
			virtual void						vSetMass(MASSTYPE t, LYNXREAL density, LYNXREAL *lpdata);			

			virtual void						vNeverSleep(LYNXBOOL b) {}; //TODO
			LYNXFORCEINLINE LYNXBOOL			vIsSleeping() const {return LYNX_FALSE; }; //TODO
			LYNXFORCEINLINE void				vSleep(LYNXBOOL b) {}; //TODO
			
			LYNXFORCEINLINE LYNXBOOL			vIsActive() const {return LYNX_TRUE; }; //TODO 
			LYNXFORCEINLINE void				vActivate(LYNXBOOL b) {}; //TODO
			
			virtual void						vAddShape(LynxEngine::PhysicsSystem::CPhysicsShape* const lpg);			
			virtual void						vRemoveShape(LynxEngine::PhysicsSystem::CPhysicsShape* const lpg);
			virtual void						vRemoveShape(int i);
			virtual void						vRemoveAllShapes();			

			virtual LYNXINLINE void				vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) {m_Body->SetTransform(b2Vec2(x, y), m_Body->GetAngle()); };
			virtual LYNXINLINE void				vSetPosition(LPLYNXVECTOR3D lppos) { m_Body->SetTransform(b2Vec2(lppos->x, lppos->y), m_Body->GetAngle()); };			
			LYNXINLINE virtual void				vGetPosition(LPLYNXVECTOR3D lppos) { b2Vec2 Pos = m_Body->GetPosition(); lppos->x = Pos.x; lppos->y = Pos.y; };
			virtual void						vSetTransformMatrix(const CMatrix4& m);
			virtual CMatrix4&					vGetTransformMatrix();

			virtual void						vSetForce(LYNXREAL x, LYNXREAL y, LYNXREAL z) {m_Body->ApplyForce(b2Vec2(x, y), b2Vec2(0.0f, 0.0f)); };
			virtual void						vSetForce(LPLYNXVECTOR3D lpforce) {m_Body->ApplyForce(b2Vec2(lpforce->x, lpforce->y), b2Vec2(0.0f, 0.0f)); };
			virtual void						vAddForce(LYNXREAL x, LYNXREAL y, LYNXREAL z) {m_Body->ApplyForce(b2Vec2(x, y), b2Vec2(0.0f, 0.0f)); };
			virtual void						vAddForce(LPLYNXVECTOR3D lpforce) {m_Body->ApplyForce(b2Vec2(lpforce->x, lpforce->y), b2Vec2(0.0f, 0.0f)); };
			virtual void						vAddForce(LPLYNXVECTOR3D lpforce, LPLYNXVECTOR3D lppos) {m_Body->ApplyForce(b2Vec2(lpforce->x, lpforce->y), b2Vec2(lppos->x, lppos->y)); };

			virtual void						vSetTorque(LYNXREAL x, LYNXREAL y, LYNXREAL z) {};
			virtual void						vSetTorque(LPLYNXVECTOR3D lpforce) {};
			virtual void						vAddTorque(LYNXREAL x, LYNXREAL y, LYNXREAL z) {};
			virtual void						vAddTorque(LPLYNXVECTOR3D lpforce) {};
            
            virtual void						vGetLinearVelocity(LynxEngine::Math::CVector3& v) {};
			virtual void						vSetLinearVelocity(LynxEngine::Math::CVector3& v) {};
            
			virtual void						vGetAngularVelocity(LynxEngine::Math::CVector3& v) {};
			virtual void						vSetAngularVelocity(LynxEngine::Math::CVector3& v) {};

			virtual void						vReset();
		};	
	}
}

#endif