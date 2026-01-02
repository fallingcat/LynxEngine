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
#include <InputSystem/LynxTouch.h>

namespace LynxEngine 
{	
	namespace InputSystem 
	{
        IMPLEMENT_CLASSSCRIPT(CTouch, CInputDevice)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CTouch::CTouch(CInputSystem* lpsys)
		: CInputDevice(lpsys)
		{
			m_InputDeviceType = TOUCH;
			SetName(_T("System Touch"));
            
            m_Value2 = m_Value = Math::CVector2(0.0f, 0.0f);
            
            INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CTouch::~CTouch(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CTouch::vPoll(void)
		{						
			if (m_lpInputSystem->IsAllInputBlocked())
			{
				m_TouchArray.clear();
				m_Gesture = TG_NULL;
				m_TouchData.TapCount = 0;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CTouch::PollTouchData(int i)
		{
			m_TouchData = GetTouch(i);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CTouch::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassVar(lps);

			REGISTER_SCRIPT_VAR(lps, "m_Gesture",		LynxEngine::InputSystem::CTouch, m_Gesture)						
			REGISTER_SCRIPT_VAR(lps, "m_TouchData",		LynxEngine::InputSystem::CTouch, m_TouchData)			

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CTouch::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

            REGISTER_SCRIPT_METHOD(lps, "GetNumTouches",    LynxEngine::InputSystem::CTouch, int,		GetNumTouches,			())
			REGISTER_SCRIPT_METHOD(lps, "PollTouchData",	LynxEngine::InputSystem::CTouch, void,		PollTouchData,			(int))
			
			return LYNX_TRUE;
		}
	}
}
