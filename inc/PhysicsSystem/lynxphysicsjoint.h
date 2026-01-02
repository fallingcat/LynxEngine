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

#ifndef __LYNXPHYSICSJOINT_H__
#define __LYNXPHYSICSJOINT_H__

#include <LynxGameObj.h>
#include <LynxEngineClasses.h>

namespace LynxEngine 
{	
	namespace PhysicsSystem 
	{
		class LYNXENGCLASS CBallJointDesc : public CCreationDesc
		{	
			LYNX_DECLARE_CLASS(CBallJointDesc);	
		public:						
			LYNXBOOL				m_bCollisionBetweenObj;

			CBallJointDesc() {m_bCollisionBetweenObj = LYNX_FALSE; };
		};	

		class LYNXENGCLASS CConeJointDesc : public CCreationDesc
		{	
			LYNX_DECLARE_CLASS(CConeJointDesc);	
		public:						
			LYNXBOOL				m_bCollisionBetweenObj;
			float					m_SwingLimit;
			float					m_TwistLimitLow;
			float					m_TwistLimitHigh;

			CConeJointDesc() {m_bCollisionBetweenObj = LYNX_FALSE; m_SwingLimit = 60.0f; m_TwistLimitLow = -30.0f; m_TwistLimitHigh = 30.0f; };
		};			

		class LYNXENGCLASS CPhysicsJoint : public CGameObj
		{	
		public:
			enum TYPE{
				NULL_JOINT = 0,
				BALL,
				CONE,
				HINGE,
				SLIDER,				
				UNIVERSAL,
				HINGE2,
				FIXED,
				ANGULAR_MOTOR,
				NUM_TYPE,	
			};
		protected:
			CPhysicsWorld*					m_lpWorld;
			TYPE							m_PhysicsJointType;
			CPhysicsObj*					m_AttchedObj[2];

		public:
			LYNX_DECLARE_CLASS(CPhysicsJoint);
			CPhysicsJoint(CPhysicsWorld *lpw);
			virtual ~CPhysicsJoint();

			LYNXFORCEINLINE TYPE			GetPhysicsJointType() const {return m_PhysicsJointType; };

			LYNXFORCEINLINE CPhysicsWorld*	GetlpWorld() const {return m_lpWorld; };

			virtual LYNXBOOL				vCreate(const CCreationDesc* desc) = 0;
			virtual void					vDestory() = 0;
			//virtual void					vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) = 0;
			//virtual void					vSetPosition(LPLYNXVECTOR3D lppos) = 0;
			//virtual void					vSetTransformMatrix(LPLYNXMATRIX lpm) = 0;

			virtual LYNXINLINE void			vAttach(CPhysicsObj* lpo1, CPhysicsObj* lpo2 = NULL) {m_AttchedObj[0] = lpo1; m_AttchedObj[1] = lpo2; };
			virtual LYNXINLINE void			vAttachObj1(CPhysicsObj* lpo) {m_AttchedObj[0] = lpo; };
			virtual LYNXINLINE void			vAttachObj2(CPhysicsObj* lpo) {m_AttchedObj[1] = lpo; };
			CPhysicsObj*					GetAttachedObj(int i) const { return m_AttchedObj[i]; };
			virtual void					vSetAnchorPosition(LynxEngine::Math::CVector3& pos) = 0;
		};	
	}
	LYNXENGCLASS CString GetEnumStr(PhysicsSystem::CPhysicsJoint::TYPE e);
}

#endif