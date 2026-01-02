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
#include <Platform/iOS/LynxiOSAccelerometer.h>
#include <Platform/iOS/LynxiOSGameFrameworkKernel.h>
#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>

namespace LynxEngine 
{	
	namespace InputSystem 
	{
		namespace iOS
		{			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CAccelerometer::CAccelerometer(CInputSystem* lpsys)
			: LynxEngine::InputSystem::CAccelerometer(lpsys)
			{
				m_InputDeviceType = ACCELEROMETER;
				SetName(_T("System Accelerometer"));
				m_HighPassFactor = 0.1f;
                //m_MotionManager = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(m_lpInputSystem->GetlpEngine()->GetlpKernel())->GetMotionManager();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CAccelerometer::~CAccelerometer(void)
			{
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CAccelerometer::vPoll(void)
			{
                CMMotionManager* MotionManager = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(m_lpInputSystem->GetlpEngine()->GetlpKernel())->GetMotionManager();
                if (MotionManager != nil)
                    SetValue(LynxEngine::Math::CVector3(MotionManager.accelerometerData.acceleration.x, MotionManager.accelerometerData.acceleration.y, MotionManager.accelerometerData.acceleration.z));                
			}
		}
	}
}