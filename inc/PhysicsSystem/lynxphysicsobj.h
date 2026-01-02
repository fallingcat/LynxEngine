//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXPHYSICSOBJ_H__
#define __LYNXPHYSICSOBJ_H__

#include <LynxGameObj.h>
#include <LynxEngineClasses.h>
#include <PhysicsSystem/LynxPhysicsShape.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{
	namespace PhysicsSystem 
	{
		class LYNXENGCLASS CPhysicsObj : public CObj
		{	
			LYNX_DECLARE_CLASS(CPhysicsObj);		
		public:
			enum TYPE{
				NULL_OBJ = -1,
				STATIC,
				DYNAMIC,
				KINEMATIC,
				NUM_OBJ_TYPE,	
			};
			enum MASSTYPE{
				NULL_MASS = 0,
				BOX,
				SPHERE,	
				CAPSULE,	
				CYLINDER,				
				NUM_MASS_TYPE,	
			};

			enum PHYSICS_STATUS{
				INACTIVE = 0,
				ACTIVE,
				SLEEP,	
				NUM_PHYSICS_STATUS,	
			};
		protected:
			CPhysicsWorld*						m_lpWorld;
			CRenderableObj*						m_lpRenObj;	
			CContainer*							m_lpTargetContainer;
			TYPE								m_PhysicsObjType;	
			PHYSICS_STATUS						m_PhysicsStatus;
			DECLAR_PARAM_FLOAT(m_MassValue)
			DECLAR_PARAM_FLOAT(m_Density)
			DECLAR_PARAM_BOOL(m_bKinematic)
			DECLAR_PARAM_BOOL(m_bSleepAtStart)
			DECLAR_PARAM_BOOL(m_bNeverSleep)
			MASSTYPE							m_MassType;
			LYNXREAL							m_Size[8];
			CMatrix4							m_CMM;			
			CMatrix4							m_M;

			DECLAR_PARAM_LIST(LynxEngine::PhysicsSystem::CPhysicsShape*, m_ShapeList)			
			//CList<CPhysicsShape*>				m_ShapeList;
		public:				
			CPhysicsObj(CPhysicsWorld* const lpw);			
			virtual ~CPhysicsObj();

			DECLARE_CLASSSCRIPT

			LYNXFORCEINLINE CPhysicsWorld* const			GetlpWorld() {return m_lpWorld; };

			LYNXINLINE void									SetlpRenObj(CRenderableObj* lpp) {m_lpRenObj = lpp; };		
			LYNXINLINE CRenderableObj*						GetlpRenObj() const {return m_lpRenObj;};
			LYNXINLINE void									SetlpTargetContainer(CContainer* lpc) {m_lpTargetContainer = lpc; };		
			LYNXINLINE CContainer*							GetlpTargetContainer() const {return m_lpTargetContainer;};

			LYNXFORCEINLINE CList<CPhysicsShape*>&			GetShapeList() {return m_ShapeList; };
			LYNXFORCEINLINE int								GetNumShapes() { return (int)(m_ShapeList.size()); };
			LYNXFORCEINLINE CPhysicsShape*					GetlpShape(int i) { return ((m_ShapeList.size()) ? (m_ShapeList.get(i)) : NULL); };

			virtual LYNXBOOL								vCreate(TYPE t, CPhysicsShape* g = NULL) = 0;
			virtual void									vDestory() = 0;			

			LYNXFORCEINLINE TYPE							GetPhysicsObjType() const {return m_PhysicsObjType; };
			LYNXFORCEINLINE PHYSICS_STATUS					GetPhysicsStatus() const {return m_PhysicsStatus; };

			void											UpdateProperties();

			virtual void									vSetMass(LYNXREAL mass) = 0;
			virtual void									vSetMass(MASSTYPE t, LYNXREAL density, LYNXREAL *lpdata) = 0;
			MASSTYPE										GetMassType() const {return m_MassType; };
			int												GetMassData(CReal* lpdata);
			LYNXFORCEINLINE float							GetMassValue() {return m_MassValue; };

			LYNXFORCEINLINE LYNXBOOL						IsKinematic() const {return m_bKinematic; };
			virtual void									vSetKinematic(LYNXBOOL b) {m_bKinematic = b; };

			virtual LYNXBOOL								vIsActive() const = 0; 
			virtual void									vActivate(LYNXBOOL b) = 0;

			LYNXFORCEINLINE LYNXBOOL						IsSleepingAtStart() const {return m_bSleepAtStart; };
			LYNXFORCEINLINE void							SleepAtStart(LYNXBOOL b) {m_bSleepAtStart = b; vSleep(b); };
			LYNXFORCEINLINE LYNXBOOL						IsNeverSleep() const {return m_bNeverSleep; };
			virtual void									vNeverSleep(LYNXBOOL b) {m_bNeverSleep = b; };
			virtual LYNXBOOL								vIsSleeping() const = 0;
			virtual void									vSleep(LYNXBOOL b) = 0;

			virtual void									vAddShape(CPhysicsShape* const lpg) = 0;			
			virtual void									vRemoveShape(CPhysicsShape* const lpg) = 0;
			virtual void									vRemoveShape(int i) = 0;
			virtual void									vRemoveAllShapes() = 0;
			
			//virtual LYNXBOOL								vCreatePhyGeometry(CPhysicsSpace* s, CPhysicsGeometry::TYPE, LYNXREAL *lpdata) = 0;
			//virtual void									vDestoryPhyGeometry() = 0;			

			virtual void									vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) = 0;
			virtual void									vSetPosition(LPLYNXVECTOR3D lppos) = 0;
			virtual void									vGetPosition(LPLYNXVECTOR3D lppos) = 0;
			virtual void									vSetTransformMatrix(const CMatrix4& m) = 0;
			virtual CMatrix4&								vGetTransformMatrix() = 0;

			virtual void									vSetForce(LYNXREAL x, LYNXREAL y, LYNXREAL z) = 0;
			virtual void									vSetForce(LPLYNXVECTOR3D lpforce) = 0;;
			virtual void									vAddForce(LYNXREAL x, LYNXREAL y, LYNXREAL z) = 0;
			virtual void									vAddForce(LPLYNXVECTOR3D lpforce) = 0;
			virtual void									vAddForce(LPLYNXVECTOR3D lpforce, LPLYNXVECTOR3D lppos) = 0;

			virtual void									vSetTorque(LYNXREAL x, LYNXREAL y, LYNXREAL z) = 0;
			virtual void									vSetTorque(LPLYNXVECTOR3D lpforce) = 0;
			virtual void									vAddTorque(LYNXREAL x, LYNXREAL y, LYNXREAL z) = 0;
			virtual void									vAddTorque(LPLYNXVECTOR3D lpforce) = 0;

			virtual void									vGetLinearVelocity(Math::CVector3& v) = 0;
			virtual void									vSetLinearVelocity(Math::CVector3& v) = 0;

			virtual void									vGetAngularVelocity(Math::CVector3& v) = 0;
			virtual void									vSetAngularVelocity(Math::CVector3& v) = 0;

			virtual void									vReset() = 0;

			LYNXBOOL										vSave(CStream& stream);
			LYNXBOOL										vLoad(CStream& stream, LYNXBOOL skip = LYNX_FALSE);
		};	
	}
}

#endif