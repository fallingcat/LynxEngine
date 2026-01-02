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

#ifndef __LYNXODEBALLPHYSICSJOINT_H__
#define __LYNXODEBALLPHYSICSJOINT_H__

#include <LynxCore.h>
#include <ODE/ODE.h>
#include <LynxODEPhysicsJoint.h>

namespace LynxPhysics 
{
	namespace ODE 
	{
		class CBallJoint : public CPhysicsJoint
		{	
			DECLARE_CLASS(CBallJoint);
		protected:			
		public:			
			CBallJoint(LynxEngine::PhysicsSystem::CPhysicsWorld* lpw);
			virtual ~CBallJoint();

			virtual LYNXBOOL				vCreate();	
			virtual LYNXINLINE void			vSetAnchorPosition(LPLYNXVECTOR3D lppos) {dJointSetBallAnchor(m_Joint, lppos->s.x, lppos->s.y, lppos->s.z); };
		};	
	}
}

#endif