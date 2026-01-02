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
#include <Platform/iOS/LynxiOSGyrometer.h>
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
			CGyrometer::CGyrometer(CInputSystem* lpsys)
			: LynxEngine::InputSystem::CGyrometer(lpsys)
			{
				m_InputDeviceType = GYROMETER;
				SetName(_T("System Gyrometer"));
                //m_MotionManager = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(m_lpInputSystem->GetlpEngine()->GetlpKernel())->GetMotionManager();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CGyrometer::~CGyrometer(void)
			{
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CGyrometer::vPoll(void)
			{			
                CMMotionManager* MotionManager = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(m_lpInputSystem->GetlpEngine()->GetlpKernel())->GetMotionManager();
                if (MotionManager != nil)
                    m_Value = LynxEngine::Math::CVector3(MotionManager.gyroData.rotationRate.x, MotionManager.gyroData.rotationRate.y, MotionManager.gyroData.rotationRate.z);                
			}
		}
	}
}