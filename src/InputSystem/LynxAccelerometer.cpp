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
#include <InputSystem/LynxAccelerometer.h>

namespace LynxEngine 
{	
	namespace InputSystem 
	{
        IMPLEMENT_CLASSSCRIPT(CAccelerometer, CInputDevice)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CAccelerometer::CAccelerometer(CInputSystem* lpsys)
		: CInputDevice(lpsys)
		{
			m_InputDeviceType = ACCELEROMETER;
			SetName(_T("System Accelerometer"));
			m_HighPassFactor = 0.1f;
			m_Value = Math::CVector3(0.0f, 0.0f, 0.0f);
			m_HighPassValue = Math::CVector3(0.0f, 0.0f, 0.0f);
            
            INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CAccelerometer::~CAccelerometer(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CAccelerometer::vPoll(void)
		{									
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAccelerometer::SetValue(const Math::CVector3& v)
		{
			m_Value.x = v.x;
			m_Value.y = v.y;
			m_Value.z = v.z;
			
			m_HighPassValue.x = v.x*m_HighPassFactor + m_HighPassValue.x*(1.0f-m_HighPassFactor);
			m_HighPassValue.y = v.y*m_HighPassFactor + m_HighPassValue.y*(1.0f-m_HighPassFactor);
			m_HighPassValue.z = v.z*m_HighPassFactor + m_HighPassValue.z*(1.0f-m_HighPassFactor);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CAccelerometer::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassVar(lps);
	
			REGISTER_SCRIPT_VAR(lps, "m_Value",				LynxEngine::InputSystem::CAccelerometer, m_Value)						
			REGISTER_SCRIPT_VAR(lps, "m_HighPassValue",		LynxEngine::InputSystem::CAccelerometer, m_HighPassValue)			

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CAccelerometer::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

            //REGISTER_SCRIPT_METHOD(lps, "GetNumTouches",    LynxEngine::InputSystem::CAccelerometer, int,		GetNumTouches,			())
			//REGISTER_SCRIPT_METHOD(lps, "PollTouchData",	LynxEngine::InputSystem::CAccelerometer, void,		PollTouchData,			(int))
			
			return LYNX_TRUE;
		}
	}
}
