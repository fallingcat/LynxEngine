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

#include <LynxEngine_PCH.h>
#include <PhysicsSystem/LynxPhysicsJoint.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(PhysicsSystem::CPhysicsJoint::TYPE e)
	{
		CString				JointType;

		switch (e)
		{
			case PhysicsSystem::CPhysicsJoint::BALL:
				JointType = _T("Ball");
				break;
			case PhysicsSystem::CPhysicsJoint::CONE:
				JointType = _T("Cone");
				break;
			case PhysicsSystem::CPhysicsJoint::HINGE:
				JointType = _T("Hing");
				break;
			case PhysicsSystem::CPhysicsJoint::SLIDER:
				JointType = _T("Slider");
				break;
			case PhysicsSystem::CPhysicsJoint::UNIVERSAL:
				JointType = _T("Universal");
				break;
			case PhysicsSystem::CPhysicsJoint::HINGE2:
				JointType = _T("Hing2");
				break;
			case PhysicsSystem::CPhysicsJoint::FIXED:
				JointType = _T("Fixed");
				break;
			case PhysicsSystem::CPhysicsJoint::ANGULAR_MOTOR:
				JointType = _T("AngularMotor");
				break;
		}
		return JointType;
	}

	namespace PhysicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsJoint::CPhysicsJoint(CPhysicsWorld* lpw)
		{				
			m_PhysicsJointType = NULL_JOINT;
			m_lpWorld = lpw;
			m_AttchedObj[0] = NULL;
			m_AttchedObj[1] = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsJoint::~CPhysicsJoint(void)
		{
		}		
	}
}